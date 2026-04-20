#pragma once

#include <string>
#include <vector>

#include "third_party/json.hpp"

namespace adra::domain {

struct AtamQualityAttributeNode {
    std::string id;
    std::string name;
    std::string description;
    std::vector<AtamQualityAttributeNode> children;
    std::vector<std::string> scenarios;  // scenario ids
};

struct AtamScenario {
    std::string id;
    std::string quality_attribute;   // Performance, Security, Availability, ...
    std::string stimulus_source;
    std::string stimulus;
    std::string environment;
    std::string artifact;
    std::string response;
    std::string response_measure;
    std::string importance;   // H/M/L — business value
    std::string difficulty;   // H/M/L — architectural difficulty
};

struct AtamApproach {
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::string> tactics;           // free text tactics
    std::vector<std::string> quality_attributes;
};

struct AtamFinding {
    std::string id;
    std::string kind;                  // risk, non-risk, sensitivity-point, tradeoff-point
    std::string description;
    std::vector<std::string> related_scenarios;
    std::vector<std::string> related_approaches;
    std::vector<std::string> related_quality_attributes;
    std::string severity;              // H/M/L
    std::string linked_adr_id;         // optional ADR crosslink
};

struct AtamRevision {
    std::string timestamp;
    std::string author;
    std::string note;
};

struct AtamSession {
    std::string id;
    std::string title;
    std::string date;
    std::string status;  // draft, in-progress, final
    std::string system_context;
    std::vector<std::string> business_drivers;
    std::vector<std::string> constraints;
    std::vector<std::string> assumptions;
    std::vector<AtamApproach> approaches;
    std::vector<AtamScenario> scenarios;
    std::vector<AtamQualityAttributeNode> utility_tree;
    std::vector<AtamFinding> findings;
    std::vector<std::string> tags;
    std::vector<AtamRevision> revisions;
    std::string created_at;
    std::string updated_at;

    static AtamSession make_new();
    void touch(const std::string& author, const std::string& note);
};

void to_json(nlohmann::json& j, const AtamQualityAttributeNode& n);
void from_json(const nlohmann::json& j, AtamQualityAttributeNode& n);
void to_json(nlohmann::json& j, const AtamScenario& s);
void from_json(const nlohmann::json& j, AtamScenario& s);
void to_json(nlohmann::json& j, const AtamApproach& a);
void from_json(const nlohmann::json& j, AtamApproach& a);
void to_json(nlohmann::json& j, const AtamFinding& f);
void from_json(const nlohmann::json& j, AtamFinding& f);
void to_json(nlohmann::json& j, const AtamRevision& r);
void from_json(const nlohmann::json& j, AtamRevision& r);
void to_json(nlohmann::json& j, const AtamSession& s);
void from_json(const nlohmann::json& j, AtamSession& s);

}  // namespace adra::domain
