#pragma once

#include <string>
#include <vector>

#include "third_party/json.hpp"

namespace adra::domain {

struct AdrRevision {
    std::string timestamp;
    std::string author;
    std::string note;
};

struct AdrLink {
    std::string target_id;
    std::string relation;  // supersedes, related, depends-on, refines, conflicts-with
};

// An Architecture Decision Record (Nygard + extensions).
struct Adr {
    std::string id;
    std::string slug;
    int number = 0;
    std::string title;
    std::string status;  // proposed, accepted, rejected, deprecated, superseded
    std::string date;
    std::string context;
    std::string decision;
    std::string consequences;
    std::vector<std::string> alternatives;
    std::vector<std::string> assumptions;
    std::vector<std::string> risks;
    std::vector<std::string> quality_attributes;
    std::vector<std::string> requirements;
    std::vector<std::string> stakeholders;
    std::vector<std::string> tags;
    std::vector<AdrLink> links;
    std::vector<AdrRevision> revisions;
    std::string created_at;
    std::string updated_at;

    static Adr make_new();
    void touch(const std::string& author, const std::string& note);
    static const std::vector<std::string>& allowed_statuses();
    static bool is_valid_status(const std::string& s);
};

void to_json(nlohmann::json& j, const AdrRevision& r);
void from_json(const nlohmann::json& j, AdrRevision& r);
void to_json(nlohmann::json& j, const AdrLink& l);
void from_json(const nlohmann::json& j, AdrLink& l);
void to_json(nlohmann::json& j, const Adr& a);
void from_json(const nlohmann::json& j, Adr& a);

}  // namespace adra::domain
