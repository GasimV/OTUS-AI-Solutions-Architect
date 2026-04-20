#include "persistence/adr_repository.h"

#include <algorithm>
#include <filesystem>

#include "util/util.h"

namespace fs = std::filesystem;

namespace adra::persistence {

namespace {
constexpr const char* kDir = "adr";
}

AdrRepository::AdrRepository(FileStore& store) : store_(store) {
    util::ensure_directory(store_.full_path(kDir));
}

std::string AdrRepository::filename_for_id(const std::string& id) const {
    // File name uses id for stability; the file itself is the source of truth.
    return std::string(kDir) + "/" + id + ".json";
}

std::string AdrRepository::filename_for(const domain::Adr& a) const {
    return filename_for_id(a.id);
}

bool AdrRepository::save(const domain::Adr& adr) {
    nlohmann::json j = adr;
    return store_.save_json(filename_for(adr), j);
}

std::optional<domain::Adr> AdrRepository::find(const std::string& id) {
    auto j = store_.load_json(filename_for_id(id));
    if (!j) return std::nullopt;
    try {
        return j->get<domain::Adr>();
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<domain::Adr> AdrRepository::list() {
    std::vector<domain::Adr> out;
    for (const auto& path : store_.list_files(kDir, ".json")) {
        auto data = util::read_file(path);
        if (!data) continue;
        try {
            auto j = nlohmann::json::parse(*data);
            out.push_back(j.get<domain::Adr>());
        } catch (...) {
        }
    }
    std::sort(out.begin(), out.end(), [](const domain::Adr& a, const domain::Adr& b) {
        if (a.number != b.number) return a.number > b.number;
        return a.updated_at > b.updated_at;
    });
    return out;
}

bool AdrRepository::remove(const std::string& id) {
    return store_.remove(filename_for_id(id));
}

int AdrRepository::next_number() {
    int max_n = 0;
    for (const auto& a : list()) {
        if (a.number > max_n) max_n = a.number;
    }
    return max_n + 1;
}

}  // namespace adra::persistence
