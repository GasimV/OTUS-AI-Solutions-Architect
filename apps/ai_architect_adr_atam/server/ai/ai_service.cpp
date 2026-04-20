#include "ai/ai_service.h"

#include <sstream>

#include "util/util.h"

namespace adra::ai {

namespace {

constexpr const char* kSystemAdr =
    "You are an experienced software architect assisting with Architecture Decision Records (ADR).\n"
    "Be concise, technical, and concrete. Prefer bullet points where appropriate.\n"
    "Do not hallucinate facts that were not given. If information is missing, state reasonable assumptions.";

constexpr const char* kSystemAtam =
    "You are an experienced software architect assisting with ATAM "
    "(Architecture Tradeoff Analysis Method). Be structured and concrete. "
    "Use the standard ATAM vocabulary: business drivers, quality attributes, "
    "scenarios (stimulus/response/measure), risks, non-risks, sensitivity points, tradeoff points.";

std::string as_list(const std::vector<std::string>& xs) {
    if (xs.empty()) return "(none)";
    std::ostringstream os;
    for (size_t i = 0; i < xs.size(); ++i) {
        if (i) os << ", ";
        os << xs[i];
    }
    return os.str();
}

AiResult wrap(std::optional<std::string> maybe, const char* default_error) {
    AiResult r;
    if (maybe && !maybe->empty()) {
        r.ok = true;
        r.text = util::trim(*maybe);
    } else {
        r.error = default_error;
    }
    return r;
}

}  // namespace

AiService::AiService(LlmClient& client) : client_(client) {}

AiResult AiService::draft_adr(const AiDraftAdrRequest& req) {
    std::ostringstream p;
    p << "Draft an Architecture Decision Record in Markdown.\n\n"
      << "Title: " << (req.title.empty() ? "(generate one)" : req.title) << "\n"
      << "Quality attributes: " << as_list(req.quality_attributes) << "\n"
      << "Architect notes:\n" << (req.notes.empty() ? "(no notes)" : req.notes) << "\n\n"
      << "Return sections: Context, Decision, Consequences, Alternatives Considered, "
      << "Assumptions, Risks. Keep each section tight and actionable.";
    return wrap(client_.generate(p.str(), kSystemAdr), "LLM unavailable or returned no content");
}

AiResult AiService::draft_atam(const AiDraftAtamRequest& req) {
    std::ostringstream p;
    p << "Draft initial ATAM work products from the architect notes.\n\n"
      << "Return ONLY compact JSON. Do not wrap it in Markdown fences. Use exactly this shape:\n"
      << "{\n"
      << "  \"systemContext\": \"...\",\n"
      << "  \"businessDrivers\": [\"...\"],\n"
      << "  \"constraints\": [\"...\"],\n"
      << "  \"assumptions\": [\"...\"],\n"
      << "  \"approaches\": [\n"
      << "    {\"name\":\"...\",\"description\":\"...\",\"qualityAttributes\":[\"...\"],\"tactics\":[\"...\"]}\n"
      << "  ],\n"
      << "  \"scenarios\": [\n"
      << "    {\"qualityAttribute\":\"...\",\"stimulusSource\":\"...\",\"stimulus\":\"...\","
         "\"environment\":\"...\",\"artifact\":\"...\",\"response\":\"...\","
         "\"responseMeasure\":\"...\",\"importance\":\"H|M|L\",\"difficulty\":\"H|M|L\"}\n"
      << "  ],\n"
      << "  \"findings\": [\n"
      << "    {\"kind\":\"risk|non-risk|sensitivity-point|tradeoff-point\","
         "\"description\":\"...\",\"severity\":\"H|M|L\"}\n"
      << "  ]\n"
      << "}\n\n"
      << "Title: " << (req.title.empty() ? "(not provided)" : req.title) << "\n"
      << "Quality attributes to emphasize: " << as_list(req.quality_attributes) << "\n"
      << "Existing system context:\n"
      << (req.system_context.empty() ? "(not provided)" : req.system_context) << "\n\n"
      << "Architect notes:\n" << (req.notes.empty() ? "(no notes)" : req.notes) << "\n\n"
      << "Keep content concrete and useful for an architecture review. "
      << "If information is missing, make conservative assumptions and list them in assumptions. "
      << "Generate 2-4 business drivers, 2-4 constraints, 2-4 assumptions, "
      << "2-4 approaches, 3-5 quality attribute scenarios, and 3-6 findings.";
    return wrap(client_.generate(p.str(), kSystemAtam), "LLM unavailable or returned no content");
}

AiResult AiService::improve_section(const std::string& section, const std::string& text) {
    std::ostringstream p;
    p << "Rewrite the following ADR '" << section
      << "' section to be clearer, more concise, and more technically precise. "
      << "Keep the meaning intact. Output only the revised text.\n\n---\n"
      << text << "\n---";
    return wrap(client_.generate(p.str(), kSystemAdr), "LLM unavailable");
}

AiResult AiService::summarize_context(const std::string& context) {
    std::ostringstream p;
    p << "Summarize the following architectural context into 3 bullet points that highlight the "
      << "core forces and constraints:\n\n" << context;
    return wrap(client_.generate(p.str(), kSystemAdr), "LLM unavailable");
}

AiResult AiService::suggest_alternatives(const std::string& title, const std::string& context,
                                         const std::string& decision) {
    std::ostringstream p;
    p << "Given this ADR, propose 3 concrete alternatives that were not chosen, each as a short "
      << "bullet with one line of rationale.\n\n"
      << "Title: " << title << "\nContext:\n" << context << "\nCurrent decision:\n" << decision;
    return wrap(client_.generate(p.str(), kSystemAdr), "LLM unavailable");
}

AiResult AiService::suggest_consequences(const std::string& title,
                                         const std::string& decision) {
    std::ostringstream p;
    p << "List likely positive and negative consequences of this decision, as two bullet lists "
      << "(Positive, Negative).\n\nTitle: " << title << "\nDecision:\n" << decision;
    return wrap(client_.generate(p.str(), kSystemAdr), "LLM unavailable");
}

AiResult AiService::suggest_risks(const std::string& title, const std::string& decision) {
    std::ostringstream p;
    p << "Identify up to 5 meaningful risks that follow from this decision. "
      << "For each, give a 1-line risk statement. No preamble.\n\n"
      << "Title: " << title << "\nDecision:\n" << decision;
    return wrap(client_.generate(p.str(), kSystemAdr), "LLM unavailable");
}

AiResult AiService::suggest_scenarios(const std::string& qa, const std::string& system_context) {
    std::ostringstream p;
    p << "Propose 3 concrete quality attribute scenarios for '" << qa << "' using the ATAM "
      << "six-part form (source, stimulus, environment, artifact, response, response measure). "
      << "Return each scenario as a short bulleted block.\n\nSystem context:\n"
      << (system_context.empty() ? "(not provided)" : system_context);
    return wrap(client_.generate(p.str(), kSystemAtam), "LLM unavailable");
}

static std::string atam_session_digest(const domain::AtamSession& s) {
    std::ostringstream os;
    os << "Title: " << s.title << "\n";
    if (!s.system_context.empty()) os << "System context:\n" << s.system_context << "\n";
    if (!s.business_drivers.empty())
        os << "Business drivers: " << as_list(s.business_drivers) << "\n";
    if (!s.approaches.empty()) {
        os << "Approaches:\n";
        for (const auto& a : s.approaches) {
            os << "- " << a.name;
            if (!a.description.empty()) os << ": " << a.description;
            os << "\n";
        }
    }
    if (!s.scenarios.empty()) {
        os << "Scenarios:\n";
        for (const auto& sc : s.scenarios) {
            os << "- [" << sc.quality_attribute << "] stimulus='" << sc.stimulus
               << "' response='" << sc.response << "' measure='" << sc.response_measure << "'\n";
        }
    }
    if (!s.findings.empty()) {
        os << "Existing findings:\n";
        for (const auto& f : s.findings) {
            os << "- [" << f.kind << "] " << f.description << "\n";
        }
    }
    return os.str();
}

AiResult AiService::infer_sensitivity_tradeoffs(const domain::AtamSession& session) {
    std::ostringstream p;
    p << "Given this ATAM session, propose likely sensitivity points and tradeoff points.\n"
      << "Return two sections: 'Sensitivity points' and 'Tradeoff points', each as bullets.\n\n"
      << atam_session_digest(session);
    return wrap(client_.generate(p.str(), kSystemAtam), "LLM unavailable");
}

AiResult AiService::summarize_atam(const domain::AtamSession& session) {
    std::ostringstream p;
    p << "Produce a final ATAM report summary in Markdown. Include: Executive summary, "
      << "Key quality attributes, Top risks, Non-risks, Sensitivity & tradeoff points, "
      << "Recommendations. Keep it crisp.\n\n"
      << atam_session_digest(session);
    return wrap(client_.generate(p.str(), kSystemAtam), "LLM unavailable");
}

AiResult AiService::atam_to_adr_candidates(const domain::AtamSession& session) {
    std::ostringstream p;
    p << "From this ATAM session, suggest up to 5 ADR candidates worth writing. "
      << "For each, give: proposed title, 1-line context, 1-line decision direction, "
      << "quality attributes touched. Use bullets.\n\n"
      << atam_session_digest(session);
    return wrap(client_.generate(p.str(), kSystemAtam), "LLM unavailable");
}

}  // namespace adra::ai
