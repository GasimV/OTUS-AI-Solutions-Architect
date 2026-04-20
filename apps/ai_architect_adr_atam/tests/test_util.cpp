#include <set>
#include <string>

#include "test_harness.h"
#include "util/util.h"

using namespace adra;

TEST(util_generate_uuid_is_v4_and_unique) {
    auto a = util::generate_uuid();
    auto b = util::generate_uuid();
    CHECK(a.size() == 36);
    CHECK(a != b);
    CHECK_EQ(a[14], '4');  // v4 marker
}

TEST(util_slugify_basic) {
    CHECK_EQ(util::slugify("Hello World!!"), std::string("hello-world"));
    CHECK_EQ(util::slugify("ADR: Adopt Postgres"), std::string("adr-adopt-postgres"));
    CHECK_EQ(util::slugify(""), std::string("untitled"));
}

TEST(util_split_join_round_trip) {
    auto parts = util::split("a,b,c", ',');
    CHECK_EQ(parts.size(), (size_t)3);
    CHECK_EQ(util::join(parts, "|"), std::string("a|b|c"));
}

TEST(util_trim_and_lower) {
    CHECK_EQ(util::trim("   hello \t\n"), std::string("hello"));
    CHECK_EQ(util::to_lower("MiXeD"), std::string("mixed"));
}

TEST(util_replace_all) {
    CHECK_EQ(util::replace_all("one two one", "one", "1"), std::string("1 two 1"));
}

TEST(util_today_and_now_are_nonempty) {
    CHECK(!util::today_iso_date().empty());
    auto now = util::now_iso8601_utc();
    CHECK(now.size() >= 20);
    CHECK(now.back() == 'Z');
}
