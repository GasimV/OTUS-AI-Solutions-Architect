#include "persistence/file_store.h"

#include <filesystem>

#include "util/util.h"

namespace fs = std::filesystem;

namespace adra::persistence {

FileStore::FileStore(std::string root_dir) : root_(std::move(root_dir)) {
    util::ensure_directory(root_);
}

std::string FileStore::full_path(const std::string& relative_path) const {
    fs::path p(root_);
    p /= relative_path;
    return p.string();
}

bool FileStore::save_json(const std::string& relative_path, const nlohmann::json& j) {
    std::lock_guard<std::mutex> lk(mutex_);
    const auto path = full_path(relative_path);
    return util::write_file(path, j.dump(2));
}

std::optional<nlohmann::json> FileStore::load_json(const std::string& relative_path) {
    std::lock_guard<std::mutex> lk(mutex_);
    const auto path = full_path(relative_path);
    auto data = util::read_file(path);
    if (!data) return std::nullopt;
    try {
        return nlohmann::json::parse(*data);
    } catch (...) {
        return std::nullopt;
    }
}

bool FileStore::remove(const std::string& relative_path) {
    std::lock_guard<std::mutex> lk(mutex_);
    return util::remove_file(full_path(relative_path));
}

std::vector<std::string> FileStore::list_files(const std::string& relative_dir,
                                               const std::string& ext) {
    std::lock_guard<std::mutex> lk(mutex_);
    return util::list_files(full_path(relative_dir), ext);
}

}  // namespace adra::persistence
