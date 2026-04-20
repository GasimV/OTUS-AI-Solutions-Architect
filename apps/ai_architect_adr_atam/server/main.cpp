#include <csignal>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

#include "ai/ai_service.h"
#include "ai/provider_llm_client.h"
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
    std::string ai_provider = "ollama";
    std::string ollama_host = "127.0.0.1";
    int ollama_port = 11434;
    std::string ollama_model = "gemma4:e2b";
    std::string gemini_api_host = "generativelanguage.googleapis.com";
    int gemini_api_port = 443;
    std::string gemini_model = "gemma-4-26b-a4b-it";
    std::string gemini_api_key;
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

std::string normalize_provider(std::string provider) {
    std::transform(provider.begin(), provider.end(), provider.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (provider == "gemini") return provider;
    return "ollama";
}

void set_env_if_absent(const std::string& name, const std::string& value) {
    if (name.empty() || std::getenv(name.c_str())) return;
#ifdef _WIN32
    _putenv_s(name.c_str(), value.c_str());
#else
    setenv(name.c_str(), value.c_str(), 0);
#endif
}

std::string unquote_env_value(std::string value) {
    value = adra::util::trim(value);
    if (value.size() >= 2) {
        const char first = value.front();
        const char last = value.back();
        if ((first == '"' && last == '"') || (first == '\'' && last == '\'')) {
            return value.substr(1, value.size() - 2);
        }
    }
    return value;
}

void load_env_file(const fs::path& path) {
    std::ifstream in(path);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        line = adra::util::trim(line);
        if (line.empty() || line[0] == '#') continue;
        if (line.rfind("export ", 0) == 0) line = adra::util::trim(line.substr(7));
        const auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        auto key = adra::util::trim(line.substr(0, eq));
        auto value = unquote_env_value(line.substr(eq + 1));
        set_env_if_absent(key, value);
    }
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

fs::path root_arg_from_cli(int argc, char** argv) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (std::string(argv[i]) == "--root") return argv[i + 1];
    }
    return {};
}

AppConfig parse_config(int argc, char** argv, const fs::path& exe_dir) {
    AppConfig cfg;
    cfg.host = env_or("ADRA_HOST", cfg.host);
    cfg.port = env_int_or("ADRA_PORT", cfg.port);
    cfg.ai_provider = normalize_provider(env_or("ADRA_LLM_PROVIDER", cfg.ai_provider));
    cfg.ollama_host = env_or("OLLAMA_HOST", cfg.ollama_host);
    cfg.ollama_port = env_int_or("OLLAMA_PORT", cfg.ollama_port);
    cfg.ollama_model = env_or("OLLAMA_MODEL", cfg.ollama_model);
    cfg.gemini_api_host = env_or("GEMINI_API_HOST", cfg.gemini_api_host);
    cfg.gemini_api_port = env_int_or("GEMINI_API_PORT", cfg.gemini_api_port);
    cfg.gemini_model = env_or("GEMINI_MODEL", cfg.gemini_model);
    cfg.gemini_api_key = env_or("GEMINI_API_KEY", cfg.gemini_api_key);
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
        else if (a == "--ai-provider" || a == "--provider") cfg.ai_provider = normalize_provider(next(a));
        else if (a == "--ollama-host") cfg.ollama_host = next(a);
        else if (a == "--ollama-port") cfg.ollama_port = std::stoi(next(a));
        else if (a == "--ollama-model") cfg.ollama_model = next(a);
        else if (a == "--gemini-api-host") cfg.gemini_api_host = next(a);
        else if (a == "--gemini-api-port") cfg.gemini_api_port = std::stoi(next(a));
        else if (a == "--gemini-model") cfg.gemini_model = next(a);
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
    const auto resolved_root = resolve_app_root(exe_dir);
    load_env_file(resolved_root / ".env");
    load_env_file(fs::current_path() / ".env");
    const auto cli_root = root_arg_from_cli(argc, argv);
    if (!cli_root.empty()) load_env_file(cli_root / ".env");

    AppConfig cfg = parse_config(argc, argv, exe_dir);

    const auto data_dir = cfg.app_root / "data";
    const auto templates_adr = (cfg.app_root / "templates" / "adr").string();
    const auto templates_atam = (cfg.app_root / "templates" / "atam").string();
    const auto exports_dir = (cfg.app_root / "exports").string();
    const auto logs_dir = (cfg.app_root / "logs").string();
    const auto web_dir = (cfg.app_root / "web").string();

    adra::util::ensure_directory(data_dir.string());
    adra::util::ensure_directory(exports_dir);
    adra::util::ensure_directory(logs_dir);
    std::ofstream log_file(logs_dir + "/server.log", std::ios::app);
    std::mutex log_mutex;
    auto write_log = [&](const std::string& msg) {
        std::lock_guard<std::mutex> lk(log_mutex);
        const auto line = adra::util::now_iso8601_utc() + " " + msg;
        std::cout << line << "\n";
        if (log_file) {
            log_file << line << "\n";
            log_file.flush();
        }
    };

    adra::persistence::FileStore store(data_dir.string());
    adra::persistence::AdrRepository adr_repo(store);
    adra::persistence::AtamRepository atam_repo(store);

    adra::services::TemplateService templates(templates_adr, templates_atam);
    adra::services::AdrService adr_service(adr_repo, templates);
    adra::services::AtamService atam_service(atam_repo, templates);
    adra::services::ExportService exporter(exports_dir);
    adra::services::ReuseService reuse(adr_repo, atam_repo);

    adra::ai::LlmConfig llm_cfg;
    llm_cfg.provider = cfg.ai_provider;
    llm_cfg.host = cfg.ollama_host;
    llm_cfg.port = cfg.ollama_port;
    llm_cfg.model = cfg.ai_provider == "gemini" ? cfg.gemini_model : cfg.ollama_model;
    llm_cfg.api_host = cfg.gemini_api_host;
    llm_cfg.api_port = cfg.gemini_api_port;
    llm_cfg.api_key = cfg.gemini_api_key;
    llm_cfg.enabled = cfg.ai_enabled;
    adra::ai::ProviderLlmClient llm(llm_cfg);
    adra::ai::AiService ai(llm);

    httplib::Server svr;
    g_server = &svr;
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    adra::http::ServerDeps deps{&adr_service, &atam_service, &templates, &exporter, &reuse, &ai};
    adra::http::register_routes(svr, deps);
    svr.set_logger([&](const httplib::Request& req, const httplib::Response& res) {
        write_log(req.method + " " + req.path + " -> " + std::to_string(res.status));
    });
    svr.set_error_logger([&](const httplib::Error& err, const httplib::Request* req) {
        std::string target = req ? (req->method + " " + req->path) : "(no request)";
        write_log("HTTP error " + std::to_string(static_cast<int>(err)) + " " + target);
    });

    svr.set_mount_point("/", web_dir.c_str());

    std::cout << "AI-Architect ADR/ATAM\n"
              << "---------------------\n"
              << "app root:     " << cfg.app_root.string() << "\n"
              << "web dir:      " << web_dir << "\n"
              << "data dir:     " << data_dir.string() << "\n"
              << "templates:    " << templates_adr << " | " << templates_atam << "\n"
              << "exports dir:  " << exports_dir << "\n"
              << "logs dir:     " << logs_dir << "\n"
              << "ai provider:  " << cfg.ai_provider << " model=" << llm_cfg.model
              << (cfg.ai_enabled ? "" : " (disabled)") << "\n"
              << "ollama:       http://" << cfg.ollama_host << ":" << cfg.ollama_port
              << " model=" << cfg.ollama_model << "\n"
              << "gemini:       https://" << cfg.gemini_api_host << ":" << cfg.gemini_api_port
              << " model=" << cfg.gemini_model
              << " key=" << (cfg.gemini_api_key.empty() ? "missing" : "configured") << "\n"
              << "listening on: http://" << cfg.host << ":" << cfg.port << "\n"
              << "Open http://" << cfg.host << ":" << cfg.port << "/ in your browser.\n"
              << "Press Ctrl+C to stop.\n" << std::endl;
    write_log("server starting " + cfg.host + ":" + std::to_string(cfg.port) +
              " provider=" + cfg.ai_provider + " model=" + llm_cfg.model);

    if (!svr.listen(cfg.host.c_str(), cfg.port)) {
        std::cerr << "Failed to bind " << cfg.host << ":" << cfg.port
                  << ". Port may be in use.\n";
        return 1;
    }
    return 0;
}
