#pragma once

#include <mutex>

#include "ai/llm_client.h"

namespace adra::ai {

class ProviderLlmClient : public LlmClient {
public:
    explicit ProviderLlmClient(LlmConfig cfg);

    LlmStatus check_status() override;
    std::optional<std::string> generate(const std::string& prompt,
                                        const std::string& system_prompt = "") override;

    const LlmConfig& config() const override { return cfg_; }
    void update_config(const LlmConfig& cfg) override;

private:
    LlmConfig snapshot() const;

    LlmConfig cfg_;
    mutable std::mutex cfg_mutex_;
};

}  // namespace adra::ai
