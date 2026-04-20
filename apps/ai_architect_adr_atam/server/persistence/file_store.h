#pragma once

#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "third_party/json.hpp"

namespace adra::persistence {

// Human-readable JSON file store. Each entity is one file.
// Uniform primitive used by all repositories.
class FileStore {
public:
    explicit FileStore(std::string root_dir);

    bool save_json(const std::string& relative_path, const nlohmann::json& j);
    std::optional<nlohmann::json> load_json(const std::string& relative_path);
    bool remove(const std::string& relative_path);
    std::vector<std::string> list_files(const std::string& relative_dir, const std::string& ext = ".json");

    std::string full_path(const std::string& relative_path) const;
    const std::string& root() const { return root_; }

private:
    std::string root_;
    std::mutex mutex_;
};

}  // namespace adra::persistence
