#include "services/adr_service.h"

#include <algorithm>

#include "util/util.h"

namespace adra::services {

AdrService::AdrService(persistence::AdrRepository& repo, TemplateService& templates)
    : repo_(repo), templates_(templates) {}

static void apply_template(domain::Adr& adr, const nlohmann::json& tpl) {
    if (!tpl.is_object()) return;
    if (tpl.contains("status") && tpl["status"].is_string() &&
        domain::Adr::is_valid_status(tpl["status"].get<std::string>())) {
        adr.status = tpl["status"].get<std::string>();
    }
    auto str = [&](const char* k, std::string& dst) {
        if (tpl.contains(k) && tpl[k].is_string()) dst = tpl[k].get<std::string>();
    };
    auto vec = [&](const char* k, std::vector<std::string>& dst) {
        if (tpl.contains(k) && tpl[k].is_array()) {
            dst.clear();
            for (const auto& v : tpl[k])
                if (v.is_string()) dst.push_back(v.get<std::string>());
        }
    };
    str("context", adr.context);
    str("decision", adr.decision);
    str("consequences", adr.consequences);
    vec("alternatives", adr.alternatives);
    vec("assumptions", adr.assumptions);
    vec("risks", adr.risks);
    vec("qualityAttributes", adr.quality_attributes);
    vec("requirements", adr.requirements);
    vec("stakeholders", adr.stakeholders);
    vec("tags", adr.tags);
}

domain::Adr AdrService::create_from_template(const std::string& template_name,
                                             const std::string& title,
                                             const std::string& author) {
    auto adr = domain::Adr::make_new();
    adr.number = repo_.next_number();
    adr.title = title.empty() ? "Untitled Decision" : title;
    adr.slug = util::slugify(adr.title);
    if (!template_name.empty()) {
        if (auto tpl = templates_.load_adr_template(template_name)) {
            apply_template(adr, *tpl);
        }
    }
    adr.touch(author, "created");
    repo_.save(adr);
    return adr;
}

std::optional<domain::Adr> AdrService::get(const std::string& id) {
    return repo_.find(id);
}

static bool contains_ci(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) return true;
    auto h = util::to_lower(haystack);
    auto n = util::to_lower(needle);
    return h.find(n) != std::string::npos;
}

std::vector<domain::Adr> AdrService::list(const AdrFilter& f) {
    auto all = repo_.list();
    std::vector<domain::Adr> out;
    out.reserve(all.size());
    for (const auto& a : all) {
        if (!f.status.empty() && a.status != f.status) continue;
        if (!f.tag.empty()) {
            const auto& tags = a.tags;
            if (std::find(tags.begin(), tags.end(), f.tag) == tags.end()) continue;
        }
        if (!f.quality_attribute.empty()) {
            const auto& qa = a.quality_attributes;
            if (std::find(qa.begin(), qa.end(), f.quality_attribute) == qa.end()) continue;
        }
        if (!f.text.empty()) {
            bool hit = contains_ci(a.title, f.text) || contains_ci(a.context, f.text) ||
                       contains_ci(a.decision, f.text) || contains_ci(a.consequences, f.text);
            if (!hit) continue;
        }
        out.push_back(a);
    }
    return out;
}

static void apply_patch(domain::Adr& a, const nlohmann::json& patch) {
    if (!patch.is_object()) return;
    auto str = [&](const char* k, std::string& dst) {
        if (patch.contains(k) && patch[k].is_string()) dst = patch[k].get<std::string>();
    };
    auto vec = [&](const char* k, std::vector<std::string>& dst) {
        if (patch.contains(k) && patch[k].is_array()) {
            dst.clear();
            for (const auto& v : patch[k])
                if (v.is_string()) dst.push_back(v.get<std::string>());
        }
    };
    str("title", a.title);
    if (patch.contains("status") && patch["status"].is_string()) {
        auto s = patch["status"].get<std::string>();
        if (domain::Adr::is_valid_status(s)) a.status = s;
    }
    str("date", a.date);
    str("context", a.context);
    str("decision", a.decision);
    str("consequences", a.consequences);
    vec("alternatives", a.alternatives);
    vec("assumptions", a.assumptions);
    vec("risks", a.risks);
    vec("qualityAttributes", a.quality_attributes);
    vec("requirements", a.requirements);
    vec("stakeholders", a.stakeholders);
    vec("tags", a.tags);
    if (patch.contains("links") && patch["links"].is_array()) {
        a.links.clear();
        for (const auto& l : patch["links"]) {
            domain::AdrLink link;
            domain::from_json(l, link);
            if (!link.target_id.empty()) a.links.push_back(std::move(link));
        }
    }
    a.slug = util::slugify(a.title);
}

std::optional<domain::Adr> AdrService::update(const std::string& id,
                                              const nlohmann::json& patch,
                                              const std::string& author,
                                              const std::string& note) {
    auto adr = repo_.find(id);
    if (!adr) return std::nullopt;
    apply_patch(*adr, patch);
    adr->touch(author, note.empty() ? "updated" : note);
    if (!repo_.save(*adr)) return std::nullopt;
    return adr;
}

std::optional<domain::Adr> AdrService::save_full(const domain::Adr& incoming,
                                                 const std::string& author,
                                                 const std::string& note) {
    auto existing = repo_.find(incoming.id);
    domain::Adr merged = incoming;
    if (existing) {
        merged.created_at = existing->created_at;
        merged.number = existing->number != 0 ? existing->number : incoming.number;
        merged.revisions = existing->revisions;
    } else {
        if (merged.created_at.empty()) merged.created_at = util::now_iso8601_utc();
        if (merged.number == 0) merged.number = repo_.next_number();
    }
    merged.slug = util::slugify(merged.title);
    merged.touch(author, note.empty() ? "saved" : note);
    if (!repo_.save(merged)) return std::nullopt;
    return merged;
}

bool AdrService::remove(const std::string& id) {
    return repo_.remove(id);
}

}  // namespace adra::services
