#pragma once

#include "ai/ai_service.h"
#include "services/adr_service.h"
#include "services/atam_service.h"
#include "services/export_service.h"
#include "services/reuse_service.h"
#include "services/template_service.h"
#include "third_party/httplib.h"

namespace adra::http {

struct ServerDeps {
    services::AdrService* adr;
    services::AtamService* atam;
    services::TemplateService* templates;
    services::ExportService* exporter;
    services::ReuseService* reuse;
    ai::AiService* ai;
};

void register_routes(httplib::Server& svr, const ServerDeps& deps);

}  // namespace adra::http
