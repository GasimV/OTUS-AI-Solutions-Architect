#include <filesystem>

#include "persistence/adr_repository.h"
#include "persistence/atam_repository.h"
#include "persistence/file_store.h"
#include "services/adr_service.h"
#include "services/atam_service.h"
#include "services/export_service.h"
#include "services/reuse_service.h"
#include "services/template_service.h"
#include "test_harness.h"
#include "util/util.h"

namespace fs = std::filesystem;
using namespace adra;

namespace {
std::string tmp_dir(const std::string& tag) {
    auto p = fs::temp_directory_path() / ("adra-svc-" + tag + "-" + util::generate_uuid().substr(0, 8));
    fs::create_directories(p);
    return p.string();
}

void write_template(const std::string& dir, const std::string& name, const nlohmann::json& j) {
    fs::create_directories(dir);
    util::write_file((fs::path(dir) / (name + ".json")).string(), j.dump());
}
}  // namespace

TEST(adr_service_create_and_update) {
    auto root = tmp_dir("adr");
    persistence::FileStore store(root);
    persistence::AdrRepository repo(store);
    services::TemplateService tpl(root + "/tpl-adr", root + "/tpl-atam");
    services::AdrService svc(repo, tpl);

    auto a = svc.create_from_template("", "My Decision", "author");
    CHECK_EQ(a.number, 1);
    CHECK_EQ(a.title, std::string("My Decision"));

    nlohmann::json patch = {{"status", "accepted"}, {"decision", "Do the thing."}};
    auto updated = svc.update(a.id, patch, "author", "accepted it");
    CHECK(updated.has_value());
    CHECK_EQ(updated->status, std::string("accepted"));
    CHECK_EQ(updated->decision, std::string("Do the thing."));
    CHECK(updated->revisions.size() >= 2);
    fs::remove_all(root);
}

TEST(adr_service_template_applied) {
    auto root = tmp_dir("tpl");
    persistence::FileStore store(root);
    persistence::AdrRepository repo(store);
    auto adr_tpl_dir = root + "/tpl-adr";
    write_template(adr_tpl_dir, "default",
                   {{"context", "tmpl context"},
                    {"tags", {"a", "b"}},
                    {"qualityAttributes", {"Performance"}}});
    services::TemplateService tpl(adr_tpl_dir, root + "/tpl-atam");
    services::AdrService svc(repo, tpl);

    auto a = svc.create_from_template("default", "With Template", "me");
    CHECK_EQ(a.context, std::string("tmpl context"));
    CHECK_EQ(a.tags.size(), (size_t)2);
    CHECK_EQ(a.quality_attributes[0], std::string("Performance"));
    fs::remove_all(root);
}

TEST(export_service_markdown_contains_sections) {
    auto root = tmp_dir("exp");
    services::ExportService exp(root);
    auto a = domain::Adr::make_new();
    a.number = 42;
    a.title = "Sample";
    a.context = "because";
    a.decision = "do X";
    a.consequences = "Y happens";
    a.alternatives = {"do nothing"};
    auto md = exp.adr_to_markdown(a);
    CHECK(md.find("# ADR-42: Sample") != std::string::npos);
    CHECK(md.find("## Context") != std::string::npos);
    CHECK(md.find("## Decision") != std::string::npos);
    CHECK(md.find("- do nothing") != std::string::npos);
    fs::remove_all(root);
}

TEST(export_service_atam_markdown_table) {
    auto root = tmp_dir("exp2");
    services::ExportService exp(root);
    auto s = domain::AtamSession::make_new();
    s.title = "T";
    domain::AtamScenario sc;
    sc.quality_attribute = "Performance";
    sc.stimulus = "query";
    sc.response = "result";
    sc.response_measure = "p95 < 300ms";
    s.scenarios.push_back(sc);
    auto md = exp.atam_to_markdown(s);
    CHECK(md.find("Quality Attribute Scenarios") != std::string::npos);
    CHECK(md.find("Performance") != std::string::npos);
    fs::remove_all(root);
}

TEST(reuse_service_finds_similar_adr) {
    auto root = tmp_dir("reuse");
    persistence::FileStore store(root);
    persistence::AdrRepository adr_repo(store);
    persistence::AtamRepository atam_repo(store);

    auto a = domain::Adr::make_new();
    a.number = 1;
    a.title = "Use Postgres for order service";
    a.context = "Need relational storage with partitioning";
    a.tags = {"storage"};
    a.quality_attributes = {"Modifiability"};
    adr_repo.save(a);

    services::ReuseService reuse(adr_repo, atam_repo);
    auto hits = reuse.find_similar_adrs(
        "Postgres for payments", "Relational storage needed with partitioning",
        {"storage"}, {"Modifiability"}, 3);
    CHECK(hits.size() >= 1);
    CHECK_EQ(hits[0].id, a.id);
    fs::remove_all(root);
}
