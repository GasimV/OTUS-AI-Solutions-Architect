#pragma once

#include <optional>
#include <string>
#include <vector>

namespace adra::ai {

struct LlmConfig {
    std::string provider = "gemini";
    std::string host = "127.0.0.1";
    int port = 11434;
    std::string model = "gemma4:e2b";
    std::string api_host = "generativelanguage.googleapis.com";
    int api_port = 443;
    std::string api_key;
    int connect_timeout_sec = 3;
    int read_timeout_sec = 120;
    bool enabled = true;
};

struct LlmStatus {
    bool reachable = false;
    bool model_available = false;
    std::string active_model;
    std::vector<std::string> available_models;
    std::string error;
};

// Provider-neutral interface so the app is not tied to one LLM backend.
class LlmClient {
public:
    virtual ~LlmClient() = default;
    virtual LlmStatus check_status() = 0;
    virtual std::optional<std::string> generate(const std::string& prompt,
                                                const std::string& system_prompt = "") = 0;
    virtual const LlmConfig& config() const = 0;
    virtual void update_config(const LlmConfig& cfg) = 0;
};

}  // namespace adra::ai
