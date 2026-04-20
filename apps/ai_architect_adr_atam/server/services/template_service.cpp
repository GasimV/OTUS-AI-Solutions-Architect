#include "services/template_service.h"

#include <filesystem>

#include "util/util.h"

namespace fs = std::filesystem;

namespace adra::services {

TemplateService::TemplateService(std::string adr_dir, std::string atam_dir)
    : adr_dir_(std::move(adr_dir)), atam_dir_(std::move(atam_dir)) {
    util::ensure_directory(adr_dir_);
    util::ensure_directory(atam_dir_);
}

std::vector<TemplateInfo> TemplateService::list_dir(const std::string& dir) {
    std::vector<TemplateInfo> out;
    for (const auto& path : util::list_files(dir, ".json")) {
        auto data = util::read_file(path);
        if (!data) continue;
        TemplateInfo info;
        info.path = path;
        info.name = fs::path(path).stem().string();
        try {
            auto j = nlohmann::json::parse(*data);
            if (j.contains("description") && j["description"].is_string()) {
                info.description = j["description"].get<std::string>();
            }
        } catch (...) {
        }
        out.push_back(std::move(info));
    }
    return out;
}

std::vector<TemplateInfo> TemplateService::list_adr_templates() const {
    return list_dir(adr_dir_);
}

std::vector<TemplateInfo> TemplateService::list_atam_templates() const {
    return list_dir(atam_dir_);
}

std::optional<nlohmann::json> TemplateService::load_from(const std::string& dir,
                                                        const std::string& name) {
    fs::path path = fs::path(dir) / (name + ".json");
    auto data = util::read_file(path.string());
    if (!data) return std::nullopt;
    try {
        return nlohmann::json::parse(*data);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<nlohmann::json> TemplateService::load_adr_template(const std::string& name) const {
    return load_from(adr_dir_, name);
}
std::optional<nlohmann::json> TemplateService::load_atam_template(const std::string& name) const {
    return load_from(atam_dir_, name);
}

}  // namespace adra::services
