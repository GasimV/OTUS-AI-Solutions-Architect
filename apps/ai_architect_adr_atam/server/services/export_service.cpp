#include "services/export_service.h"

#include <filesystem>
#include <iomanip>
#include <sstream>

#include "third_party/json.hpp"
#include "util/util.h"

namespace fs = std::filesystem;

namespace adra::services {

ExportService::ExportService(std::string dir) : dir_(std::move(dir)) {
    util::ensure_directory(dir_);
}

namespace {

std::string list_section(const std::string& heading, const std::vector<std::string>& items) {
    if (items.empty()) return {};
    std::ostringstream os;
    os << "## " << heading << "\n\n";
    for (const auto& it : items) {
        if (it.empty()) continue;
        os << "- " << it << "\n";
    }
    os << "\n";
    return os.str();
}

std::string text_section(const std::string& heading, const std::string& body) {
    if (body.empty()) return {};
    std::ostringstream os;
    os << "## " << heading << "\n\n" << body << "\n\n";
    return os.str();
}

std::string render_utility_tree(const std::vector<domain::AtamQualityAttributeNode>& nodes,
                                int depth) {
    std::ostringstream os;
    std::string indent(static_cast<size_t>(depth) * 2, ' ');
    for (const auto& n : nodes) {
        os << indent << "- **" << (n.name.empty() ? "(unnamed)" : n.name) << "**";
        if (!n.description.empty()) os << " — " << n.description;
        os << "\n";
        if (!n.scenarios.empty()) {
            os << indent << "  - scenarios: " << util::join(n.scenarios, ", ") << "\n";
        }
        if (!n.children.empty()) {
            os << render_utility_tree(n.children, depth + 1);
        }
    }
    return os.str();
}

}  // namespace

std::string ExportService::adr_to_markdown(const domain::Adr& adr) const {
    std::ostringstream os;
    os << "# ADR-" << adr.number << ": "
       << (adr.title.empty() ? std::string("Untitled") : adr.title) << "\n\n";
    os << "- **Status:** " << (adr.status.empty() ? "proposed" : adr.status) << "\n";
    os << "- **Date:** " << (adr.date.empty() ? util::today_iso_date() : adr.date) << "\n";
    if (!adr.stakeholders.empty())
        os << "- **Stakeholders:** " << util::join(adr.stakeholders, ", ") << "\n";
    if (!adr.tags.empty())
        os << "- **Tags:** " << util::join(adr.tags, ", ") << "\n";
    if (!adr.quality_attributes.empty())
        os << "- **Quality Attributes:** " << util::join(adr.quality_attributes, ", ") << "\n";
    os << "\n";

    os << text_section("Context", adr.context);
    os << text_section("Decision", adr.decision);
    os << text_section("Consequences", adr.consequences);
    os << list_section("Alternatives Considered", adr.alternatives);
    os << list_section("Assumptions", adr.assumptions);
    os << list_section("Risks", adr.risks);
    os << list_section("Related Requirements", adr.requirements);

    if (!adr.links.empty()) {
        os << "## Related Decisions\n\n";
        for (const auto& l : adr.links) {
            os << "- **" << (l.relation.empty() ? "related" : l.relation) << "**: "
               << l.target_id << "\n";
        }
        os << "\n";
    }
    if (!adr.revisions.empty()) {
        os << "## History\n\n";
        for (const auto& r : adr.revisions) {
            os << "- " << r.timestamp << " — " << r.author;
            if (!r.note.empty()) os << " — " << r.note;
            os << "\n";
        }
        os << "\n";
    }
    return os.str();
}

std::string ExportService::atam_to_markdown(const domain::AtamSession& s) const {
    std::ostringstream os;
    os << "# ATAM Session: "
       << (s.title.empty() ? std::string("Untitled") : s.title) << "\n\n";
    os << "- **Status:** " << s.status << "\n";
    os << "- **Date:** " << s.date << "\n";
    if (!s.tags.empty()) os << "- **Tags:** " << util::join(s.tags, ", ") << "\n";
    os << "\n";

    os << text_section("System Context", s.system_context);
    os << list_section("Business Drivers", s.business_drivers);
    os << list_section("Constraints", s.constraints);
    os << list_section("Assumptions", s.assumptions);

    if (!s.approaches.empty()) {
        os << "## Architectural Approaches\n\n";
        for (const auto& a : s.approaches) {
            os << "### " << (a.name.empty() ? std::string("(unnamed)") : a.name) << "\n\n";
            if (!a.description.empty()) os << a.description << "\n\n";
            if (!a.quality_attributes.empty())
                os << "- **Quality Attributes:** "
                   << util::join(a.quality_attributes, ", ") << "\n";
            if (!a.tactics.empty()) {
                os << "- **Tactics:**\n";
                for (const auto& t : a.tactics) os << "    - " << t << "\n";
            }
            os << "\n";
        }
    }

    if (!s.utility_tree.empty()) {
        os << "## Utility Tree\n\n" << render_utility_tree(s.utility_tree, 0) << "\n";
    }

    if (!s.scenarios.empty()) {
        os << "## Quality Attribute Scenarios\n\n";
        os << "| QA | Stimulus | Environment | Response | Measure | Imp | Diff |\n";
        os << "|---|---|---|---|---|---|---|\n";
        for (const auto& sc : s.scenarios) {
            auto esc = [](std::string t) {
                t = util::replace_all(std::move(t), "|", "\\|");
                t = util::replace_all(std::move(t), "\n", " ");
                return t;
            };
            os << "| " << esc(sc.quality_attribute)
               << " | " << esc(sc.stimulus)
               << " | " << esc(sc.environment)
               << " | " << esc(sc.response)
               << " | " << esc(sc.response_measure)
               << " | " << esc(sc.importance)
               << " | " << esc(sc.difficulty) << " |\n";
        }
        os << "\n";
    }

    auto findings_by_kind = [&](const std::string& kind, const std::string& heading) {
        std::vector<const domain::AtamFinding*> picked;
        for (const auto& f : s.findings)
            if (f.kind == kind) picked.push_back(&f);
        if (picked.empty()) return std::string{};
        std::ostringstream fs;
        fs << "## " << heading << "\n\n";
        for (const auto* f : picked) {
            fs << "- " << f->description;
            if (!f->severity.empty()) fs << " _(severity: " << f->severity << ")_";
            if (!f->linked_adr_id.empty()) fs << " [ADR: " << f->linked_adr_id << "]";
            fs << "\n";
        }
        fs << "\n";
        return fs.str();
    };
    os << findings_by_kind("risk", "Risks");
    os << findings_by_kind("non-risk", "Non-Risks");
    os << findings_by_kind("sensitivity-point", "Sensitivity Points");
    os << findings_by_kind("tradeoff-point", "Tradeoff Points");
    return os.str();
}

std::string ExportService::export_adr_markdown(const domain::Adr& adr) const {
    auto md = adr_to_markdown(adr);
    std::ostringstream name;
    name << "adr-" << std::setw(4) << std::setfill('0') << adr.number << "-"
         << (adr.slug.empty() ? util::slugify(adr.title) : adr.slug) << ".md";
    fs::path out = fs::path(dir_) / name.str();
    if (!util::write_file(out.string(), md)) return {};
    return out.string();
}

std::string ExportService::export_adr_json(const domain::Adr& adr) const {
    nlohmann::json j = adr;
    std::ostringstream name;
    name << "adr-" << std::setw(4) << std::setfill('0') << adr.number << "-"
         << (adr.slug.empty() ? util::slugify(adr.title) : adr.slug) << ".json";
    fs::path out = fs::path(dir_) / name.str();
    if (!util::write_file(out.string(), j.dump(2))) return {};
    return out.string();
}

std::string ExportService::export_atam_markdown(const domain::AtamSession& s) const {
    auto md = atam_to_markdown(s);
    auto slug = util::slugify(s.title.empty() ? s.id : s.title);
    fs::path out = fs::path(dir_) / ("atam-" + slug + ".md");
    if (!util::write_file(out.string(), md)) return {};
    return out.string();
}

std::string ExportService::export_atam_json(const domain::AtamSession& s) const {
    nlohmann::json j = s;
    auto slug = util::slugify(s.title.empty() ? s.id : s.title);
    fs::path out = fs::path(dir_) / ("atam-" + slug + ".json");
    if (!util::write_file(out.string(), j.dump(2))) return {};
    return out.string();
}

}  // namespace adra::services
