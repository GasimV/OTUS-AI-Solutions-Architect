#include "domain/atam.h"

#include "util/util.h"

namespace adra::domain {

static std::vector<std::string> as_string_vec(const nlohmann::json& j, const char* key) {
    if (!j.contains(key) || !j[key].is_array()) return {};
    std::vector<std::string> out;
    for (const auto& v : j[key]) {
        if (v.is_string()) out.push_back(v.get<std::string>());
    }
    return out;
}

AtamSession AtamSession::make_new() {
    AtamSession s;
    s.id = util::generate_uuid();
    s.status = "draft";
    s.date = util::today_iso_date();
    s.created_at = util::now_iso8601_utc();
    s.updated_at = s.created_at;
    return s;
}

void AtamSession::touch(const std::string& author, const std::string& note) {
    updated_at = util::now_iso8601_utc();
    AtamRevision r;
    r.timestamp = updated_at;
    r.author = author.empty() ? "architect" : author;
    r.note = note;
    revisions.push_back(std::move(r));
}

void to_json(nlohmann::json& j, const AtamRevision& r) {
    j = nlohmann::json{{"timestamp", r.timestamp}, {"author", r.author}, {"note", r.note}};
}
void from_json(const nlohmann::json& j, AtamRevision& r) {
    r.timestamp = j.value("timestamp", "");
    r.author = j.value("author", "");
    r.note = j.value("note", "");
}

void to_json(nlohmann::json& j, const AtamScenario& s) {
    j = nlohmann::json{
        {"id", s.id},
        {"qualityAttribute", s.quality_attribute},
        {"stimulusSource", s.stimulus_source},
        {"stimulus", s.stimulus},
        {"environment", s.environment},
        {"artifact", s.artifact},
        {"response", s.response},
        {"responseMeasure", s.response_measure},
        {"importance", s.importance},
        {"difficulty", s.difficulty},
    };
}
void from_json(const nlohmann::json& j, AtamScenario& s) {
    s.id = j.value("id", "");
    s.quality_attribute = j.value("qualityAttribute", "");
    s.stimulus_source = j.value("stimulusSource", "");
    s.stimulus = j.value("stimulus", "");
    s.environment = j.value("environment", "");
    s.artifact = j.value("artifact", "");
    s.response = j.value("response", "");
    s.response_measure = j.value("responseMeasure", "");
    s.importance = j.value("importance", "");
    s.difficulty = j.value("difficulty", "");
}

void to_json(nlohmann::json& j, const AtamApproach& a) {
    j = nlohmann::json{
        {"id", a.id},
        {"name", a.name},
        {"description", a.description},
        {"tactics", a.tactics},
        {"qualityAttributes", a.quality_attributes},
    };
}
void from_json(const nlohmann::json& j, AtamApproach& a) {
    a.id = j.value("id", "");
    a.name = j.value("name", "");
    a.description = j.value("description", "");
    a.tactics = as_string_vec(j, "tactics");
    a.quality_attributes = as_string_vec(j, "qualityAttributes");
}

void to_json(nlohmann::json& j, const AtamFinding& f) {
    j = nlohmann::json{
        {"id", f.id},
        {"kind", f.kind},
        {"description", f.description},
        {"relatedScenarios", f.related_scenarios},
        {"relatedApproaches", f.related_approaches},
        {"relatedQualityAttributes", f.related_quality_attributes},
        {"severity", f.severity},
        {"linkedAdrId", f.linked_adr_id},
    };
}
void from_json(const nlohmann::json& j, AtamFinding& f) {
    f.id = j.value("id", "");
    f.kind = j.value("kind", "risk");
    f.description = j.value("description", "");
    f.related_scenarios = as_string_vec(j, "relatedScenarios");
    f.related_approaches = as_string_vec(j, "relatedApproaches");
    f.related_quality_attributes = as_string_vec(j, "relatedQualityAttributes");
    f.severity = j.value("severity", "");
    f.linked_adr_id = j.value("linkedAdrId", "");
}

void to_json(nlohmann::json& j, const AtamQualityAttributeNode& n) {
    j = nlohmann::json{
        {"id", n.id},
        {"name", n.name},
        {"description", n.description},
        {"children", n.children},
        {"scenarios", n.scenarios},
    };
}
void from_json(const nlohmann::json& j, AtamQualityAttributeNode& n) {
    n.id = j.value("id", "");
    n.name = j.value("name", "");
    n.description = j.value("description", "");
    n.children.clear();
    if (j.contains("children") && j["children"].is_array()) {
        for (const auto& c : j["children"]) {
            AtamQualityAttributeNode child;
            from_json(c, child);
            n.children.push_back(std::move(child));
        }
    }
    n.scenarios = as_string_vec(j, "scenarios");
}

void to_json(nlohmann::json& j, const AtamSession& s) {
    j = nlohmann::json{
        {"id", s.id},
        {"title", s.title},
        {"date", s.date},
        {"status", s.status},
        {"systemContext", s.system_context},
        {"businessDrivers", s.business_drivers},
        {"constraints", s.constraints},
        {"assumptions", s.assumptions},
        {"approaches", s.approaches},
        {"scenarios", s.scenarios},
        {"utilityTree", s.utility_tree},
        {"findings", s.findings},
        {"tags", s.tags},
        {"revisions", s.revisions},
        {"createdAt", s.created_at},
        {"updatedAt", s.updated_at},
    };
}
void from_json(const nlohmann::json& j, AtamSession& s) {
    s.id = j.value("id", "");
    s.title = j.value("title", "");
    s.date = j.value("date", "");
    s.status = j.value("status", "draft");
    s.system_context = j.value("systemContext", "");
    s.business_drivers = as_string_vec(j, "businessDrivers");
    s.constraints = as_string_vec(j, "constraints");
    s.assumptions = as_string_vec(j, "assumptions");
    s.tags = as_string_vec(j, "tags");
    s.approaches.clear();
    if (j.contains("approaches") && j["approaches"].is_array()) {
        for (const auto& a : j["approaches"]) {
            AtamApproach ap;
            from_json(a, ap);
            s.approaches.push_back(std::move(ap));
        }
    }
    s.scenarios.clear();
    if (j.contains("scenarios") && j["scenarios"].is_array()) {
        for (const auto& sc : j["scenarios"]) {
            AtamScenario scen;
            from_json(sc, scen);
            s.scenarios.push_back(std::move(scen));
        }
    }
    s.utility_tree.clear();
    if (j.contains("utilityTree") && j["utilityTree"].is_array()) {
        for (const auto& n : j["utilityTree"]) {
            AtamQualityAttributeNode node;
            from_json(n, node);
            s.utility_tree.push_back(std::move(node));
        }
    }
    s.findings.clear();
    if (j.contains("findings") && j["findings"].is_array()) {
        for (const auto& f : j["findings"]) {
            AtamFinding finding;
            from_json(f, finding);
            s.findings.push_back(std::move(finding));
        }
    }
    s.revisions.clear();
    if (j.contains("revisions") && j["revisions"].is_array()) {
        for (const auto& r : j["revisions"]) {
            AtamRevision rev;
            from_json(r, rev);
            s.revisions.push_back(std::move(rev));
        }
    }
    s.created_at = j.value("createdAt", "");
    s.updated_at = j.value("updatedAt", "");
}

}  // namespace adra::domain
