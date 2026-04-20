#pragma once

#include <optional>
#include <string>
#include <vector>

#include "ai/llm_client.h"
#include "domain/adr.h"
#include "domain/atam.h"

namespace adra::ai {

struct AiResult {
    bool ok = false;
    std::string text;
    std::string error;
};

struct AiDraftAdrRequest {
    std::string title;
    std::string notes;
    std::vector<std::string> quality_attributes;
};

// High-level, architect-oriented wrappers around LlmClient.
// Fails gracefully (returns ok=false) when the LLM is unavailable.
class AiService {
public:
    explicit AiService(LlmClient& client);

    AiResult draft_adr(const AiDraftAdrRequest& req);
    AiResult improve_section(const std::string& section, const std::string& text);
    AiResult summarize_context(const std::string& context);
    AiResult suggest_alternatives(const std::string& title, const std::string& context,
                                  const std::string& decision);
    AiResult suggest_consequences(const std::string& title, const std::string& decision);
    AiResult suggest_risks(const std::string& title, const std::string& decision);

    AiResult suggest_scenarios(const std::string& quality_attribute,
                               const std::string& system_context);
    AiResult infer_sensitivity_tradeoffs(const domain::AtamSession& session);
    AiResult summarize_atam(const domain::AtamSession& session);
    AiResult atam_to_adr_candidates(const domain::AtamSession& session);

    LlmClient& client() { return client_; }

private:
    LlmClient& client_;
};

}  // namespace adra::ai
