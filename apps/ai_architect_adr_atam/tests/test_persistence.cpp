#include <filesystem>
#include <string>

#include "persistence/adr_repository.h"
#include "persistence/atam_repository.h"
#include "persistence/file_store.h"
#include "test_harness.h"
#include "util/util.h"

namespace fs = std::filesystem;
using namespace adra;

namespace {
std::string tmp_dir(const std::string& tag) {
    auto p = fs::temp_directory_path() / ("adra-test-" + tag + "-" + util::generate_uuid().substr(0, 8));
    fs::create_directories(p);
    return p.string();
}
}  // namespace

TEST(file_store_save_and_load_json) {
    auto root = tmp_dir("fs");
    persistence::FileStore s(root);
    nlohmann::json j = {{"a", 1}, {"b", "hi"}};
    CHECK(s.save_json("nested/a.json", j));
    auto back = s.load_json("nested/a.json");
    CHECK(back.has_value());
    CHECK_EQ((*back)["a"].get<int>(), 1);
    CHECK_EQ((*back)["b"].get<std::string>(), std::string("hi"));
    fs::remove_all(root);
}

TEST(adr_repository_crud) {
    auto root = tmp_dir("adr");
    persistence::FileStore s(root);
    persistence::AdrRepository repo(s);

    CHECK_EQ(repo.list().size(), (size_t)0);
    auto a = domain::Adr::make_new();
    a.number = repo.next_number();
    a.title = "Test";
    CHECK(repo.save(a));

    auto loaded = repo.find(a.id);
    CHECK(loaded.has_value());
    CHECK_EQ(loaded->title, std::string("Test"));

    CHECK_EQ(repo.list().size(), (size_t)1);
    CHECK(repo.remove(a.id));
    CHECK_EQ(repo.list().size(), (size_t)0);
    fs::remove_all(root);
}

TEST(atam_repository_crud) {
    auto root = tmp_dir("atam");
    persistence::FileStore s(root);
    persistence::AtamRepository repo(s);

    auto sess = domain::AtamSession::make_new();
    sess.title = "T";
    CHECK(repo.save(sess));
    CHECK(repo.find(sess.id).has_value());
    CHECK_EQ(repo.list().size(), (size_t)1);
    CHECK(repo.remove(sess.id));
    fs::remove_all(root);
}

TEST(adr_repository_next_number_is_monotonic) {
    auto root = tmp_dir("num");
    persistence::FileStore s(root);
    persistence::AdrRepository repo(s);
    for (int i = 1; i <= 3; ++i) {
        auto a = domain::Adr::make_new();
        a.number = repo.next_number();
        a.title = "N" + std::to_string(i);
        CHECK(repo.save(a));
        CHECK_EQ(a.number, i);
    }
    fs::remove_all(root);
}
