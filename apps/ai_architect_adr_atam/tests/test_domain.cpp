#include "domain/adr.h"
#include "domain/atam.h"
#include "test_harness.h"

using namespace adra;

TEST(adr_status_validation) {
    CHECK(domain::Adr::is_valid_status("accepted"));
    CHECK(domain::Adr::is_valid_status("superseded"));
    CHECK(!domain::Adr::is_valid_status("bogus"));
}

TEST(adr_make_new_sets_defaults) {
    auto a = domain::Adr::make_new();
    CHECK(!a.id.empty());
    CHECK_EQ(a.status, std::string("proposed"));
    CHECK(!a.date.empty());
    CHECK(!a.created_at.empty());
    CHECK_EQ(a.created_at, a.updated_at);
}

TEST(adr_round_trip_json) {
    auto a = domain::Adr::make_new();
    a.number = 7;
    a.title = "Round-trip";
    a.tags = {"x", "y"};
    a.alternatives = {"alt-a", "alt-b"};
    domain::AdrLink l{"target-id", "supersedes"};
    a.links = {l};
    nlohmann::json j = a;
    domain::Adr back;
    domain::from_json(j, back);
    CHECK_EQ(back.id, a.id);
    CHECK_EQ(back.number, 7);
    CHECK_EQ(back.title, std::string("Round-trip"));
    CHECK_EQ(back.tags.size(), (size_t)2);
    CHECK_EQ(back.alternatives.size(), (size_t)2);
    CHECK_EQ(back.links.size(), (size_t)1);
    CHECK_EQ(back.links[0].relation, std::string("supersedes"));
}

TEST(atam_round_trip_json) {
    auto s = domain::AtamSession::make_new();
    s.title = "x";
    s.business_drivers = {"d1", "d2"};

    domain::AtamScenario sc;
    sc.id = "s1";
    sc.quality_attribute = "Performance";
    sc.stimulus = "submits query";
    sc.response = "returns";
    sc.response_measure = "p95 < 300ms";
    s.scenarios.push_back(sc);

    domain::AtamQualityAttributeNode n;
    n.name = "Performance";
    n.children.push_back({});
    n.children[0].name = "Latency";
    s.utility_tree.push_back(n);

    nlohmann::json j = s;
    domain::AtamSession back;
    domain::from_json(j, back);
    CHECK_EQ(back.id, s.id);
    CHECK_EQ(back.scenarios.size(), (size_t)1);
    CHECK_EQ(back.scenarios[0].quality_attribute, std::string("Performance"));
    CHECK_EQ(back.utility_tree.size(), (size_t)1);
    CHECK_EQ(back.utility_tree[0].children.size(), (size_t)1);
}
