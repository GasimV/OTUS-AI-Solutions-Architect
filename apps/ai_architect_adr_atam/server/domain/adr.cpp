#include "domain/adr.h"

#include <algorithm>

#include "util/util.h"

namespace adra::domain {

const std::vector<std::string>& Adr::allowed_statuses() {
    static const std::vector<std::string> v = {
        "proposed", "accepted", "rejected", "deprecated", "superseded"};
    return v;
}

bool Adr::is_valid_status(const std::string& s) {
    const auto& v = allowed_statuses();
    return std::find(v.begin(), v.end(), s) != v.end();
}

Adr Adr::make_new() {
    Adr a;
    a.id = util::generate_uuid();
    a.status = "proposed";
    a.date = util::today_iso_date();
    a.created_at = util::now_iso8601_utc();
    a.updated_at = a.created_at;
    return a;
}

void Adr::touch(const std::string& author, const std::string& note) {
    updated_at = util::now_iso8601_utc();
    AdrRevision r;
    r.timestamp = updated_at;
    r.author = author.empty() ? "architect" : author;
    r.note = note;
    revisions.push_back(std::move(r));
}

void to_json(nlohmann::json& j, const AdrRevision& r) {
    j = nlohmann::json{{"timestamp", r.timestamp}, {"author", r.author}, {"note", r.note}};
}

void from_json(const nlohmann::json& j, AdrRevision& r) {
    r.timestamp = j.value("timestamp", "");
    r.author = j.value("author", "");
    r.note = j.value("note", "");
}

void to_json(nlohmann::json& j, const AdrLink& l) {
    j = nlohmann::json{{"targetId", l.target_id}, {"relation", l.relation}};
}

void from_json(const nlohmann::json& j, AdrLink& l) {
    l.target_id = j.value("targetId", "");
    l.relation = j.value("relation", "related");
}

void to_json(nlohmann::json& j, const Adr& a) {
    j = nlohmann::json{
        {"id", a.id},
        {"slug", a.slug},
        {"number", a.number},
        {"title", a.title},
        {"status", a.status},
        {"date", a.date},
        {"context", a.context},
        {"decision", a.decision},
        {"consequences", a.consequences},
        {"alternatives", a.alternatives},
        {"assumptions", a.assumptions},
        {"risks", a.risks},
        {"qualityAttributes", a.quality_attributes},
        {"requirements", a.requirements},
        {"stakeholders", a.stakeholders},
        {"tags", a.tags},
        {"links", a.links},
        {"revisions", a.revisions},
        {"createdAt", a.created_at},
        {"updatedAt", a.updated_at},
    };
}

static std::vector<std::string> as_string_vec(const nlohmann::json& j, const char* key) {
    if (!j.contains(key) || !j[key].is_array()) return {};
    std::vector<std::string> out;
    for (const auto& v : j[key]) {
        if (v.is_string()) out.push_back(v.get<std::string>());
    }
    return out;
}

void from_json(const nlohmann::json& j, Adr& a) {
    a.id = j.value("id", "");
    a.slug = j.value("slug", "");
    a.number = j.value("number", 0);
    a.title = j.value("title", "");
    a.status = j.value("status", "proposed");
    a.date = j.value("date", "");
    a.context = j.value("context", "");
    a.decision = j.value("decision", "");
    a.consequences = j.value("consequences", "");
    a.alternatives = as_string_vec(j, "alternatives");
    a.assumptions = as_string_vec(j, "assumptions");
    a.risks = as_string_vec(j, "risks");
    a.quality_attributes = as_string_vec(j, "qualityAttributes");
    a.requirements = as_string_vec(j, "requirements");
    a.stakeholders = as_string_vec(j, "stakeholders");
    a.tags = as_string_vec(j, "tags");
    a.links.clear();
    if (j.contains("links") && j["links"].is_array()) {
        for (const auto& l : j["links"]) {
            AdrLink link;
            from_json(l, link);
            a.links.push_back(std::move(link));
        }
    }
    a.revisions.clear();
    if (j.contains("revisions") && j["revisions"].is_array()) {
        for (const auto& r : j["revisions"]) {
            AdrRevision rev;
            from_json(r, rev);
            a.revisions.push_back(std::move(rev));
        }
    }
    a.created_at = j.value("createdAt", "");
    a.updated_at = j.value("updatedAt", "");
}

}  // namespace adra::domain
