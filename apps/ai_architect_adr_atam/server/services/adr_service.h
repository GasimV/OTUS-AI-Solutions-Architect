#pragma once

#include <optional>
#include <string>
#include <vector>

#include "domain/adr.h"
#include "persistence/adr_repository.h"
#include "services/template_service.h"

namespace adra::services {

struct AdrFilter {
    std::string text;
    std::string status;
    std::string tag;
    std::string quality_attribute;
};

class AdrService {
public:
    AdrService(persistence::AdrRepository& repo, TemplateService& templates);

    domain::Adr create_from_template(const std::string& template_name,
                                     const std::string& title,
                                     const std::string& author);

    std::optional<domain::Adr> get(const std::string& id);
    std::vector<domain::Adr> list(const AdrFilter& filter = {});

    std::optional<domain::Adr> update(const std::string& id,
                                      const nlohmann::json& patch,
                                      const std::string& author,
                                      const std::string& note);

    bool remove(const std::string& id);

    // Overwrite-save entire ADR payload, preserving id and created_at.
    std::optional<domain::Adr> save_full(const domain::Adr& incoming,
                                         const std::string& author,
                                         const std::string& note);

private:
    persistence::AdrRepository& repo_;
    TemplateService& templates_;
};

}  // namespace adra::services
