#include "ai/provider_llm_client.h"

#include <algorithm>
#include <cctype>
#include <utility>

#include "ai/gemini_client.h"
#include "ai/ollama_client.h"

namespace adra::ai {

namespace {

std::string normalize_provider(std::string provider) {
    std::transform(provider.begin(), provider.end(), provider.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (provider == "gemini") return provider;
    return "ollama";
}

}  // namespace

ProviderLlmClient::ProviderLlmClient(LlmConfig cfg) : cfg_(std::move(cfg)) {
    cfg_.provider = normalize_provider(cfg_.provider);
}

LlmConfig ProviderLlmClient::snapshot() const {
    std::lock_guard<std::mutex> lk(cfg_mutex_);
    return cfg_;
}

void ProviderLlmClient::update_config(const LlmConfig& cfg) {
    std::lock_guard<std::mutex> lk(cfg_mutex_);
    cfg_ = cfg;
    cfg_.provider = normalize_provider(cfg_.provider);
}

LlmStatus ProviderLlmClient::check_status() {
    auto cfg = snapshot();
    if (cfg.provider == "gemini") {
        GeminiClient gemini(cfg);
        return gemini.check_status();
    }
    OllamaClient ollama(cfg);
    return ollama.check_status();
}

std::optional<std::string> ProviderLlmClient::generate(const std::string& prompt,
                                                       const std::string& system_prompt) {
    auto cfg = snapshot();
    if (cfg.provider == "gemini") {
        GeminiClient gemini(cfg);
        return gemini.generate(prompt, system_prompt);
    }
    OllamaClient ollama(cfg);
    return ollama.generate(prompt, system_prompt);
}

}  // namespace adra::ai
