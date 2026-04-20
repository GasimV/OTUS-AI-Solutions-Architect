#include "persistence/atam_repository.h"

#include <algorithm>

#include "util/util.h"

namespace adra::persistence {

namespace {
constexpr const char* kDir = "atam";
}

AtamRepository::AtamRepository(FileStore& store) : store_(store) {
    util::ensure_directory(store_.full_path(kDir));
}

std::string AtamRepository::filename_for_id(const std::string& id) const {
    return std::string(kDir) + "/" + id + ".json";
}

bool AtamRepository::save(const domain::AtamSession& session) {
    nlohmann::json j = session;
    return store_.save_json(filename_for_id(session.id), j);
}

std::optional<domain::AtamSession> AtamRepository::find(const std::string& id) {
    auto j = store_.load_json(filename_for_id(id));
    if (!j) return std::nullopt;
    try {
        return j->get<domain::AtamSession>();
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<domain::AtamSession> AtamRepository::list() {
    std::vector<domain::AtamSession> out;
    for (const auto& path : store_.list_files(kDir, ".json")) {
        auto data = util::read_file(path);
        if (!data) continue;
        try {
            auto j = nlohmann::json::parse(*data);
            out.push_back(j.get<domain::AtamSession>());
        } catch (...) {
        }
    }
    std::sort(out.begin(), out.end(),
              [](const domain::AtamSession& a, const domain::AtamSession& b) {
                  return a.updated_at > b.updated_at;
              });
    return out;
}

bool AtamRepository::remove(const std::string& id) {
    return store_.remove(filename_for_id(id));
}

}  // namespace adra::persistence
