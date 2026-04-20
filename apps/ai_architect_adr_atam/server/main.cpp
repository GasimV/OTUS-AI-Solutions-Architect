#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "ai/ai_service.h"
#include "ai/ollama_client.h"
#include "http/api_routes.h"
#include "persistence/adr_repository.h"
#include "persistence/atam_repository.h"
#include "persistence/file_store.h"
#include "services/adr_service.h"
#include "services/atam_service.h"
#include "services/export_service.h"
#include "services/reuse_service.h"
#include "services/template_service.h"
#include "third_party/httplib.h"
#include "util/util.h"

namespace fs = std::filesystem;

namespace {

struct AppConfig {
    std::string host = "127.0.0.1";
    int port = 8090;
    fs::path app_root;  // resolved at runtime
    std::string ollama_host = "127.0.0.1";
    int ollama_port = 11434;
    std::string ollama_model = "gemma4:e2b";
    bool ai_enabled = true;
};

std::string env_or(const char* name, const std::string& def) {
    if (const char* v = std::getenv(name); v && *v) return v;
    return def;
}
int env_int_or(const char* name, int def) {
    if (const char* v = std::getenv(name); v && *v) {
        try { return std::stoi(v); } catch (...) {}
    }
    return def;
}

fs::path resolve_app_root(const fs::path& exe_dir) {
    // Expected: <app_root>/{web,templates,data,exports,server}
    // The binary may live in <app_root>/build/ or equivalent.
    fs::path candidates[] = {
        exe_dir / "..",
        exe_dir,
        exe_dir / ".." / "..",
    };
    for (const auto& c : candidates) {
        std::error_code ec;
        fs::path cn = fs::canonical(c, ec);
        if (ec) continue;
        if (fs::exists(cn / "web") && fs::exists(cn / "templates")) return cn;
    }
    return fs::current_path();
}

AppConfig parse_config(int argc, char** argv, const fs::path& exe_dir) {
    AppConfig cfg;
    cfg.host = env_or("ADRA_HOST", cfg.host);
    cfg.port = env_int_or("ADRA_PORT", cfg.port);
    cfg.ollama_host = env_or("OLLAMA_HOST", cfg.ollama_host);
    cfg.ollama_port = env_int_or("OLLAMA_PORT", cfg.ollama_port);
    cfg.ollama_model = env_or("OLLAMA_MODEL", cfg.ollama_model);
    cfg.ai_enabled = env_or("ADRA_AI_ENABLED", "true") != "false";

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto next = [&](const std::string& name) -> std::string {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for " << name << "\n";
                std::exit(2);
            }
            return argv[++i];
        };
        if (a == "--host") cfg.host = next(a);
        else if (a == "--port") cfg.port = std::stoi(next(a));
        else if (a == "--root") cfg.app_root = next(a);
        else if (a == "--ollama-host") cfg.ollama_host = next(a);
        else if (a == "--ollama-port") cfg.ollama_port = std::stoi(next(a));
        else if (a == "--ollama-model") cfg.ollama_model = next(a);
        else if (a == "--no-ai") cfg.ai_enabled = false;
    }

    if (cfg.app_root.empty()) cfg.app_root = resolve_app_root(exe_dir);
    return cfg;
}

httplib::Server* g_server = nullptr;

void signal_handler(int) {
    if (g_server) g_server->stop();
}

}  // namespace

int main(int argc, char** argv) {
    fs::path exe_dir = fs::absolute(fs::path(argv[0])).parent_path();
    AppConfig cfg = parse_config(argc, argv, exe_dir);

    const auto data_dir = cfg.app_root / "data";
    const auto templates_adr = (cfg.app_root / "templates" / "adr").string();
    const auto templates_atam = (cfg.app_root / "templates" / "atam").string();
    const auto exports_dir = (cfg.app_root / "exports").string();
    const auto web_dir = (cfg.app_root / "web").string();

    adra::util::ensure_directory(data_dir.string());
    adra::util::ensure_directory(exports_dir);

    adra::persistence::FileStore store(data_dir.string());
    adra::persistence::AdrRepository adr_repo(store);
    adra::persistence::AtamRepository atam_repo(store);

    adra::services::TemplateService templates(templates_adr, templates_atam);
    adra::services::AdrService adr_service(adr_repo, templates);
    adra::services::AtamService atam_service(atam_repo, templates);
    adra::services::ExportService exporter(exports_dir);
    adra::services::ReuseService reuse(adr_repo, atam_repo);

    adra::ai::LlmConfig llm_cfg;
    llm_cfg.host = cfg.ollama_host;
    llm_cfg.port = cfg.ollama_port;
    llm_cfg.model = cfg.ollama_model;
    llm_cfg.enabled = cfg.ai_enabled;
    adra::ai::OllamaClient ollama(llm_cfg);
    adra::ai::AiService ai(ollama);

    httplib::Server svr;
    g_server = &svr;
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    adra::http::ServerDeps deps{&adr_service, &atam_service, &templates, &exporter, &reuse, &ai};
    adra::http::register_routes(svr, deps);

    svr.set_mount_point("/", web_dir.c_str());

    std::cout << "AI-Architect ADR/ATAM\n"
              << "---------------------\n"
              << "app root:     " << cfg.app_root.string() << "\n"
              << "web dir:      " << web_dir << "\n"
              << "data dir:     " << data_dir.string() << "\n"
              << "templates:    " << templates_adr << " | " << templates_atam << "\n"
              << "exports dir:  " << exports_dir << "\n"
              << "ollama:       http://" << cfg.ollama_host << ":" << cfg.ollama_port
              << " model=" << cfg.ollama_model << (cfg.ai_enabled ? "" : " (disabled)") << "\n"
              << "listening on: http://" << cfg.host << ":" << cfg.port << "\n"
              << "Open http://" << cfg.host << ":" << cfg.port << "/ in your browser.\n"
              << "Press Ctrl+C to stop.\n" << std::endl;

    if (!svr.listen(cfg.host.c_str(), cfg.port)) {
        std::cerr << "Failed to bind " << cfg.host << ":" << cfg.port
                  << ". Port may be in use.\n";
        return 1;
    }
    return 0;
}
