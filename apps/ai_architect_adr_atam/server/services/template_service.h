#pragma once

#include <optional>
#include <string>
#include <vector>

#include "third_party/json.hpp"

namespace adra::services {

struct TemplateInfo {
    std::string name;
    std::string path;
    std::string description;
};

class TemplateService {
public:
    TemplateService(std::string adr_templates_dir, std::string atam_templates_dir);

    std::vector<TemplateInfo> list_adr_templates() const;
    std::vector<TemplateInfo> list_atam_templates() const;

    std::optional<nlohmann::json> load_adr_template(const std::string& name) const;
    std::optional<nlohmann::json> load_atam_template(const std::string& name) const;

private:
    static std::vector<TemplateInfo> list_dir(const std::string& dir);
    static std::optional<nlohmann::json> load_from(const std::string& dir, const std::string& name);

    std::string adr_dir_;
    std::string atam_dir_;
};

}  // namespace adra::services
