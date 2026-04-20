#include "services/reuse_service.h"

#include <algorithm>
#include <cctype>
#include <set>
#include <unordered_set>

#include "util/util.h"

namespace adra::services {

namespace {

std::unordered_set<std::string> tokenize(const std::string& s) {
    static const std::unordered_set<std::string> kStop = {
        "the", "a",   "an",  "and", "or",   "of",    "to",   "in",    "for",  "with",
        "is",  "are", "be",  "on",  "by",   "this",  "that", "these", "those", "it",
        "as",  "at",  "from","we",  "our",  "their","its",  "will",  "can",   "may",
        "so",  "but","if",   "not", "no",   "we'll","we're","data",  "system","service"};
    std::unordered_set<std::string> out;
    std::string cur;
    for (char c : s) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            cur.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        } else {
            if (cur.size() >= 3 && !kStop.count(cur)) out.insert(cur);
            cur.clear();
        }
    }
    if (cur.size() >= 3 && !kStop.count(cur)) out.insert(cur);
    return out;
}

double jaccard(const std::unordered_set<std::string>& a,
               const std::unordered_set<std::string>& b) {
    if (a.empty() || b.empty()) return 0.0;
    std::size_t inter = 0;
    const auto& s = a.size() < b.size() ? a : b;
    const auto& l = a.size() < b.size() ? b : a;
    for (const auto& t : s)
        if (l.count(t)) ++inter;
    const auto uni = a.size() + b.size() - inter;
    if (uni == 0) return 0.0;
    return static_cast<double>(inter) / static_cast<double>(uni);
}

}  // namespace

ReuseService::ReuseService(persistence::AdrRepository& adr_repo,
                           persistence::AtamRepository& atam_repo)
    : adr_repo_(adr_repo), atam_repo_(atam_repo) {}

std::vector<SimilarAdr> ReuseService::find_similar_adrs(
    const std::string& title, const std::string& context,
    const std::vector<std::string>& tags,
    const std::vector<std::string>& quality_attrs, std::size_t limit) const {

    auto query_tokens = tokenize(title + " " + context);
    std::set<std::string> query_tags(tags.begin(), tags.end());
    std::set<std::string> query_qas(quality_attrs.begin(), quality_attrs.end());

    std::vector<SimilarAdr> scored;
    auto adrs = const_cast<persistence::AdrRepository&>(adr_repo_).list();
    for (const auto& a : adrs) {
        auto doc = tokenize(a.title + " " + a.context + " " + a.decision + " " + a.consequences);
        double tok_score = jaccard(query_tokens, doc);

        std::vector<std::string> matching_tags;
        for (const auto& t : a.tags)
            if (query_tags.count(t)) matching_tags.push_back(t);
        double tag_boost = matching_tags.empty() ? 0.0 : 0.1 * static_cast<double>(matching_tags.size());

        double qa_boost = 0.0;
        for (const auto& q : a.quality_attributes)
            if (query_qas.count(q)) qa_boost += 0.05;

        double total = tok_score + tag_boost + qa_boost;
        if (total < 0.02) continue;

        SimilarAdr hit;
        hit.id = a.id;
        hit.number = a.number;
        hit.title = a.title;
        hit.status = a.status;
        hit.score = total;
        hit.matching_tags = std::move(matching_tags);
        scored.push_back(std::move(hit));
    }
    std::sort(scored.begin(), scored.end(),
              [](const SimilarAdr& x, const SimilarAdr& y) { return x.score > y.score; });
    if (scored.size() > limit) scored.resize(limit);
    return scored;
}

std::vector<SimilarScenario> ReuseService::find_similar_scenarios(
    const std::string& quality_attribute, const std::string& stimulus,
    const std::string& response, std::size_t limit) const {

    auto q_tokens = tokenize(stimulus + " " + response);
    auto target_qa = util::to_lower(util::trim(quality_attribute));

    std::vector<SimilarScenario> scored;
    auto sessions = const_cast<persistence::AtamRepository&>(atam_repo_).list();
    for (const auto& s : sessions) {
        for (const auto& sc : s.scenarios) {
            auto doc = tokenize(sc.stimulus + " " + sc.response + " " + sc.response_measure);
            double tok = jaccard(q_tokens, doc);
            double qa_boost = 0.0;
            if (!target_qa.empty() && util::to_lower(sc.quality_attribute) == target_qa)
                qa_boost = 0.2;
            double total = tok + qa_boost;
            if (total < 0.05) continue;
            SimilarScenario hit;
            hit.session_id = s.id;
            hit.session_title = s.title;
            hit.scenario_id = sc.id;
            hit.quality_attribute = sc.quality_attribute;
            hit.stimulus = sc.stimulus;
            hit.response = sc.response;
            hit.score = total;
            scored.push_back(std::move(hit));
        }
    }
    std::sort(scored.begin(), scored.end(),
              [](const SimilarScenario& x, const SimilarScenario& y) { return x.score > y.score; });
    if (scored.size() > limit) scored.resize(limit);
    return scored;
}

}  // namespace adra::services
