#include "services/atam_service.h"

#include <algorithm>
#include <functional>

#include "util/util.h"

namespace adra::services {

AtamService::AtamService(persistence::AtamRepository& repo, TemplateService& templates)
    : repo_(repo), templates_(templates) {}

static void apply_template(domain::AtamSession& s, const nlohmann::json& tpl) {
    if (!tpl.is_object()) return;
    try {
        domain::AtamSession parsed;
        domain::from_json(tpl, parsed);
        // Preserve identity and timestamps, seed fields from template
        if (!parsed.business_drivers.empty()) s.business_drivers = parsed.business_drivers;
        if (!parsed.constraints.empty()) s.constraints = parsed.constraints;
        if (!parsed.assumptions.empty()) s.assumptions = parsed.assumptions;
        if (!parsed.approaches.empty()) s.approaches = parsed.approaches;
        if (!parsed.scenarios.empty()) s.scenarios = parsed.scenarios;
        if (!parsed.utility_tree.empty()) s.utility_tree = parsed.utility_tree;
        if (!parsed.findings.empty()) s.findings = parsed.findings;
        if (!parsed.tags.empty()) s.tags = parsed.tags;
        if (s.system_context.empty() && !parsed.system_context.empty())
            s.system_context = parsed.system_context;
    } catch (...) {
    }
}

static void assign_missing_ids(domain::AtamSession& s) {
    for (auto& a : s.approaches)
        if (a.id.empty()) a.id = util::generate_uuid();
    for (auto& sc : s.scenarios)
        if (sc.id.empty()) sc.id = util::generate_uuid();
    for (auto& f : s.findings)
        if (f.id.empty()) f.id = util::generate_uuid();
    std::function<void(domain::AtamQualityAttributeNode&)> walk =
        [&](domain::AtamQualityAttributeNode& n) {
            if (n.id.empty()) n.id = util::generate_uuid();
            for (auto& c : n.children) walk(c);
        };
    for (auto& n : s.utility_tree) walk(n);
}

domain::AtamSession AtamService::create_from_template(const std::string& template_name,
                                                      const std::string& title,
                                                      const std::string& author) {
    auto s = domain::AtamSession::make_new();
    s.title = title.empty() ? "Untitled ATAM session" : title;
    if (!template_name.empty()) {
        if (auto tpl = templates_.load_atam_template(template_name)) {
            apply_template(s, *tpl);
        }
    }
    assign_missing_ids(s);
    s.touch(author, "created");
    repo_.save(s);
    return s;
}

std::optional<domain::AtamSession> AtamService::get(const std::string& id) {
    return repo_.find(id);
}

static bool contains_ci(const std::string& h, const std::string& n) {
    if (n.empty()) return true;
    return util::to_lower(h).find(util::to_lower(n)) != std::string::npos;
}

std::vector<domain::AtamSession> AtamService::list(const AtamFilter& f) {
    auto all = repo_.list();
    std::vector<domain::AtamSession> out;
    out.reserve(all.size());
    for (const auto& s : all) {
        if (!f.status.empty() && s.status != f.status) continue;
        if (!f.tag.empty()) {
            const auto& tags = s.tags;
            if (std::find(tags.begin(), tags.end(), f.tag) == tags.end()) continue;
        }
        if (!f.text.empty()) {
            bool hit = contains_ci(s.title, f.text) || contains_ci(s.system_context, f.text);
            if (!hit) {
                for (const auto& sc : s.scenarios) {
                    if (contains_ci(sc.stimulus, f.text) || contains_ci(sc.response, f.text) ||
                        contains_ci(sc.quality_attribute, f.text)) {
                        hit = true;
                        break;
                    }
                }
            }
            if (!hit) continue;
        }
        out.push_back(s);
    }
    return out;
}

std::optional<domain::AtamSession> AtamService::save_full(const domain::AtamSession& incoming,
                                                          const std::string& author,
                                                          const std::string& note) {
    auto existing = repo_.find(incoming.id);
    domain::AtamSession merged = incoming;
    if (existing) {
        merged.created_at = existing->created_at;
        merged.revisions = existing->revisions;
    } else if (merged.created_at.empty()) {
        merged.created_at = util::now_iso8601_utc();
    }
    assign_missing_ids(merged);
    merged.touch(author, note.empty() ? "saved" : note);
    if (!repo_.save(merged)) return std::nullopt;
    return merged;
}

bool AtamService::remove(const std::string& id) {
    return repo_.remove(id);
}

}  // namespace adra::services
