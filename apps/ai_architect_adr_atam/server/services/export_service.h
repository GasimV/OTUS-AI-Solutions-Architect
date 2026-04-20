#pragma once

#include <string>

#include "domain/adr.h"
#include "domain/atam.h"

namespace adra::services {

class ExportService {
public:
    explicit ExportService(std::string exports_dir);

    // Render-only
    std::string adr_to_markdown(const domain::Adr& adr) const;
    std::string atam_to_markdown(const domain::AtamSession& session) const;

    // Render + write into exports_dir. Returns on-disk path, empty on failure.
    std::string export_adr_markdown(const domain::Adr& adr) const;
    std::string export_adr_json(const domain::Adr& adr) const;
    std::string export_atam_markdown(const domain::AtamSession& s) const;
    std::string export_atam_json(const domain::AtamSession& s) const;

    const std::string& exports_dir() const { return dir_; }

private:
    std::string dir_;
};

}  // namespace adra::services
