#include "ai/gemini_client.h"

#include <cctype>
#include <iomanip>
#include <memory>
#include <sstream>
#include <utility>

#include "third_party/httplib.h"
#include "third_party/json.hpp"

namespace adra::ai {

using json = nlohmann::json;

namespace {

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
std::string normalize_model_name(const std::string& model) {
    const std::string prefix = "models/";
    if (model.rfind(prefix, 0) == 0) return model.substr(prefix.size());
    return model;
}

std::string url_encode(const std::string& value) {
    std::ostringstream escaped;
    escaped << std::hex << std::uppercase;
    for (unsigned char c : value) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << std::setfill('0') << static_cast<int>(c);
            escaped << std::setfill(' ');
        }
    }
    return escaped.str();
}

std::string model_resource_name(const std::string& model) {
    return "models/" + normalize_model_name(model);
}

void add_model_name(LlmStatus& st, const std::string& name, const std::string& requested_model) {
    if (name.empty()) return;
    st.available_models.push_back(name);
    const auto normalized = normalize_model_name(name);
    const auto requested = normalize_model_name(requested_model);
    if (name == requested_model || name == model_resource_name(requested_model) || normalized == requested) {
        st.model_available = true;
    }
}

std::unique_ptr<httplib::SSLClient> make_client(const LlmConfig& cfg) {
    auto cli = std::make_unique<httplib::SSLClient>(cfg.api_host, cfg.api_port);
    cli->set_connection_timeout(cfg.connect_timeout_sec, 0);
    cli->set_read_timeout(cfg.read_timeout_sec, 0);
    cli->set_write_timeout(cfg.read_timeout_sec, 0);
    cli->enable_server_certificate_verification(true);
    return cli;
}
#endif

}  // namespace

GeminiClient::GeminiClient(LlmConfig cfg) : cfg_(std::move(cfg)) {}

void GeminiClient::update_config(const LlmConfig& cfg) {
    std::lock_guard<std::mutex> lk(cfg_mutex_);
    cfg_ = cfg;
}

LlmStatus GeminiClient::check_status() {
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
    if (cfg.api_key.empty()) {
        st.error = "GEMINI_API_KEY is not set";
        return st;
    }

#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
    st.error = "Gemini provider requires OpenSSL support in cpp-httplib";
    return st;
#else
    try {
        auto cli = make_client(cfg);
        const auto path = "/v1beta/models?key=" + url_encode(cfg.api_key);
        auto res = cli->Get(path.c_str());
        if (!res) {
            st.error = "Gemini API not reachable at https://" + cfg.api_host + ":" +
                       std::to_string(cfg.api_port);
            return st;
        }
        if (res->status != 200) {
            st.error = "Gemini API returned HTTP " + std::to_string(res->status);
            return st;
        }
        st.reachable = true;
        auto body = json::parse(res->body, nullptr, false);
        if (body.is_discarded()) {
            st.error = "Malformed Gemini models response";
            return st;
        }
        if (body.contains("models") && body["models"].is_array()) {
            for (const auto& m : body["models"]) {
                if (m.contains("name") && m["name"].is_string()) {
                    add_model_name(st, m["name"].get<std::string>(), cfg.model);
                } else if (m.contains("baseModelId") && m["baseModelId"].is_string()) {
                    add_model_name(st, m["baseModelId"].get<std::string>(), cfg.model);
                }
            }
        }
        if (!st.model_available) {
            st.error = "Model '" + cfg.model + "' not found in Gemini API model list";
        }
    } catch (const std::exception& e) {
        st.error = std::string("Exception: ") + e.what();
    }
    return st;
#endif
}

std::optional<std::string> GeminiClient::generate(const std::string& prompt,
                                                  const std::string& system_prompt) {
    LlmConfig cfg;
    {
        std::lock_guard<std::mutex> lk(cfg_mutex_);
        cfg = cfg_;
    }
    if (!cfg.enabled || cfg.api_key.empty()) return std::nullopt;

#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
    (void)prompt;
    (void)system_prompt;
    return std::nullopt;
#else
    try {
        auto cli = make_client(cfg);
        json req = {
            {"contents", json::array({
                {
                    {"role", "user"},
                    {"parts", json::array({{{"text", prompt}}})},
                },
            })},
        };
        if (!system_prompt.empty()) {
            req["systemInstruction"] = {
                {"parts", json::array({{{"text", system_prompt}}})},
            };
        }

        const auto path = "/v1beta/" + model_resource_name(cfg.model) +
                          ":generateContent?key=" + url_encode(cfg.api_key);
        auto res = cli->Post(path.c_str(), req.dump(), "application/json");
        if (!res || res->status != 200) return std::nullopt;

        auto body = json::parse(res->body, nullptr, false);
        if (body.is_discarded()) return std::nullopt;
        if (!body.contains("candidates") || !body["candidates"].is_array() ||
            body["candidates"].empty()) {
            return std::nullopt;
        }

        std::ostringstream out;
        const auto& candidate = body["candidates"][0];
        if (candidate.contains("content") && candidate["content"].contains("parts") &&
            candidate["content"]["parts"].is_array()) {
            for (const auto& part : candidate["content"]["parts"]) {
                if (part.contains("text") && part["text"].is_string()) {
                    out << part["text"].get<std::string>();
                }
            }
        }
        auto text = out.str();
        if (text.empty()) return std::nullopt;
        return text;
    } catch (...) {
        return std::nullopt;
    }
#endif
}

}  // namespace adra::ai
