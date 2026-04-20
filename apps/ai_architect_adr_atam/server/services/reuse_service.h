#pragma once

#include <string>
#include <vector>

#include "domain/adr.h"
#include "domain/atam.h"
#include "persistence/adr_repository.h"
#include "persistence/atam_repository.h"

namespace adra::services {

struct SimilarAdr {
    std::string id;
    int number = 0;
    std::string title;
    std::string status;
    double score = 0.0;
    std::vector<std::string> matching_tags;
};

struct SimilarScenario {
    std::string session_id;
    std::string session_title;
    std::string scenario_id;
    std::string quality_attribute;
    std::string stimulus;
    std::string response;
    double score = 0.0;
};

// Lightweight similarity over stored artifacts (token overlap + tag/QA boosts).
// Runs fully offline; no LLM required.
class ReuseService {
public:
    ReuseService(persistence::AdrRepository& adr_repo,
                 persistence::AtamRepository& atam_repo);

    std::vector<SimilarAdr> find_similar_adrs(const std::string& title,
                                              const std::string& context,
                                              const std::vector<std::string>& tags,
                                              const std::vector<std::string>& quality_attrs,
                                              std::size_t limit = 5) const;

    std::vector<SimilarScenario> find_similar_scenarios(const std::string& quality_attribute,
                                                        const std::string& stimulus,
                                                        const std::string& response,
                                                        std::size_t limit = 5) const;

private:
    persistence::AdrRepository& adr_repo_;
    persistence::AtamRepository& atam_repo_;
};

}  // namespace adra::services
