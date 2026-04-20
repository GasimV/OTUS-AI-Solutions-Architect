#pragma once

#include <optional>
#include <string>
#include <vector>

#include "domain/atam.h"
#include "persistence/atam_repository.h"
#include "services/template_service.h"

namespace adra::services {

struct AtamFilter {
    std::string text;
    std::string status;
    std::string tag;
};

class AtamService {
public:
    AtamService(persistence::AtamRepository& repo, TemplateService& templates);

    domain::AtamSession create_from_template(const std::string& template_name,
                                             const std::string& title,
                                             const std::string& author);

    std::optional<domain::AtamSession> get(const std::string& id);
    std::vector<domain::AtamSession> list(const AtamFilter& filter = {});

    std::optional<domain::AtamSession> save_full(const domain::AtamSession& session,
                                                 const std::string& author,
                                                 const std::string& note);

    bool remove(const std::string& id);

private:
    persistence::AtamRepository& repo_;
    TemplateService& templates_;
};

}  // namespace adra::services
