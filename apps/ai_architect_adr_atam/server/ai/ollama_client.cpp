#include "ai/ollama_client.h"

#include "third_party/httplib.h"
#include "third_party/json.hpp"

namespace adra::ai {

using json = nlohmann::json;

OllamaClient::OllamaClient(LlmConfig cfg) : cfg_(std::move(cfg)) {}

void OllamaClient::update_config(const LlmConfig& cfg) {
    std::lock_guard<std::mutex> lk(cfg_mutex_);
    cfg_ = cfg;
}

static httplib::Client make_client(const LlmConfig& cfg) {
    httplib::Client cli(cfg.host, cfg.port);
    cli.set_connection_timeout(cfg.connect_timeout_sec, 0);
    cli.set_read_timeout(cfg.read_timeout_sec, 0);
    cli.set_write_timeout(cfg.read_timeout_sec, 0);
    return cli;
}

LlmStatus OllamaClient::check_status() {
    LlmConfig cfg;
    {
        std::lock_guard<std::mutex> lk(cfg_mutex_);
        cfg = cfg_;
    }
    LlmStatus st;
    st.active_model = cfg.model;
    if (!cfg.enabled) {
        st.error = "AI features disabled by configuration";
        return st;
    }
    try {
        auto cli = make_client(cfg);
        auto res = cli.Get("/api/tags");
        if (!res) {
            st.error = "Ollama not reachable at " + cfg.host + ":" + std::to_string(cfg.port);
            return st;
        }
        if (res->status != 200) {
            st.error = "Ollama returned HTTP " + std::to_string(res->status);
            return st;
        }
        st.reachable = true;
        auto body = json::parse(res->body, nullptr, false);
        if (body.is_discarded()) {
            st.error = "Malformed /api/tags response";
            return st;
        }
        if (body.contains("models") && body["models"].is_array()) {
            for (const auto& m : body["models"]) {
                if (m.contains("name") && m["name"].is_string()) {
                    auto name = m["name"].get<std::string>();
                    st.available_models.push_back(name);
                    if (name == cfg.model) st.model_available = true;
                }
            }
        }
        if (!st.model_available) {
            st.error = "Model '" + cfg.model + "' not found in Ollama";
        }
    } catch (const std::exception& e) {
        st.error = std::string("Exception: ") + e.what();
    }
    return st;
}

std::optional<std::string> OllamaClient::generate(const std::string& prompt,
                                                  const std::string& system_prompt) {
    LlmConfig cfg;
    {
        std::lock_guard<std::mutex> lk(cfg_mutex_);
        cfg = cfg_;
    }
    if (!cfg.enabled) return std::nullopt;
    try {
        auto cli = make_client(cfg);
        json req = {
            {"model", cfg.model},
            {"prompt", prompt},
            {"stream", false},
        };
        if (!system_prompt.empty()) req["system"] = system_prompt;
        auto res = cli.Post("/api/generate", req.dump(), "application/json");
        if (!res || res->status != 200) return std::nullopt;
        auto body = json::parse(res->body, nullptr, false);
        if (body.is_discarded()) return std::nullopt;
        if (body.contains("response") && body["response"].is_string()) {
            return body["response"].get<std::string>();
        }
        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

}  // namespace adra::ai
