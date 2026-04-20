#include "http/api_routes.h"

#include <functional>
#include <string>

#include "third_party/json.hpp"
#include "util/util.h"

namespace adra::http {

using json = nlohmann::json;

namespace {

void send_json(httplib::Response& res, int status, const json& body) {
    res.status = status;
    res.set_content(body.dump(2), "application/json");
}

void send_error(httplib::Response& res, int status, const std::string& message) {
    send_json(res, status, json{{"error", message}});
}

template <typename Handler>
void with_json_body(const httplib::Request& req, httplib::Response& res, Handler handler) {
    try {
        json body = req.body.empty() ? json::object() : json::parse(req.body);
        handler(body);
    } catch (const std::exception& e) {
        send_error(res, 400, std::string("Invalid JSON: ") + e.what());
    }
}

std::string query_param(const httplib::Request& req, const char* key,
                        const std::string& def = "") {
    return req.has_param(key) ? req.get_param_value(key) : def;
}

json ai_status_json(ai::AiService* ai) {
    if (!ai) return json{{"enabled", false}, {"reachable", false}};
    auto st = ai->client().check_status();
    const auto& cfg = ai->client().config();
    return json{
        {"enabled", cfg.enabled},
        {"reachable", st.reachable},
        {"modelAvailable", st.model_available},
        {"activeModel", st.active_model},
        {"availableModels", st.available_models},
        {"error", st.error},
        {"host", cfg.host},
        {"port", cfg.port},
    };
}

}  // namespace

void register_routes(httplib::Server& svr, const ServerDeps& deps) {
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"},
    });
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        res.status = 204;
    });

    svr.Get("/api/status", [deps](const httplib::Request&, httplib::Response& res) {
        send_json(res, 200, json{
            {"app", "AI-Architect ADR/ATAM"},
            {"version", "0.1.0"},
            {"ai", ai_status_json(deps.ai)},
        });
    });

    svr.Get("/api/ai/status", [deps](const httplib::Request&, httplib::Response& res) {
        send_json(res, 200, ai_status_json(deps.ai));
    });

    svr.Post("/api/ai/config", [deps](const httplib::Request& req, httplib::Response& res) {
        if (!deps.ai) return send_error(res, 503, "AI not configured");
        with_json_body(req, res, [&](const json& body) {
            auto cfg = deps.ai->client().config();
            if (body.contains("host") && body["host"].is_string()) cfg.host = body["host"].get<std::string>();
            if (body.contains("port") && body["port"].is_number_integer()) cfg.port = body["port"].get<int>();
            if (body.contains("model") && body["model"].is_string()) cfg.model = body["model"].get<std::string>();
            if (body.contains("enabled") && body["enabled"].is_boolean()) cfg.enabled = body["enabled"].get<bool>();
            if (body.contains("readTimeoutSec") && body["readTimeoutSec"].is_number_integer())
                cfg.read_timeout_sec = body["readTimeoutSec"].get<int>();
            deps.ai->client().update_config(cfg);
            send_json(res, 200, ai_status_json(deps.ai));
        });
    });

    // --- ADR templates -------------------------------------------------------
    svr.Get("/api/adr/templates", [deps](const httplib::Request&, httplib::Response& res) {
        json arr = json::array();
        for (const auto& t : deps.templates->list_adr_templates()) {
            arr.push_back(json{{"name", t.name}, {"description", t.description}});
        }
        send_json(res, 200, arr);
    });

    // --- ADR CRUD ------------------------------------------------------------
    svr.Get("/api/adr", [deps](const httplib::Request& req, httplib::Response& res) {
        services::AdrFilter f;
        f.text = query_param(req, "q");
        f.status = query_param(req, "status");
        f.tag = query_param(req, "tag");
        f.quality_attribute = query_param(req, "qa");
        json arr = json::array();
        for (const auto& a : deps.adr->list(f)) {
            json j = a;
            arr.push_back(j);
        }
        send_json(res, 200, arr);
    });

    svr.Get(R"(/api/adr/([0-9a-fA-F-]+))", [deps](const httplib::Request& req, httplib::Response& res) {
        auto id = req.matches[1].str();
        auto a = deps.adr->get(id);
        if (!a) return send_error(res, 404, "ADR not found");
        send_json(res, 200, *a);
    });

    svr.Post("/api/adr", [deps](const httplib::Request& req, httplib::Response& res) {
        with_json_body(req, res, [&](const json& body) {
            std::string title = body.value("title", "");
            std::string tpl = body.value("template", "");
            std::string author = body.value("author", "architect");
            auto a = deps.adr->create_from_template(tpl, title, author);
            send_json(res, 201, a);
        });
    });

    svr.Put(R"(/api/adr/([0-9a-fA-F-]+))", [deps](const httplib::Request& req, httplib::Response& res) {
        auto id = req.matches[1].str();
        with_json_body(req, res, [&](const json& body) {
            std::string author = body.value("author", "architect");
            std::string note = body.value("note", "");
            domain::Adr incoming;
            try {
                domain::from_json(body, incoming);
            } catch (...) {
                return send_error(res, 400, "Invalid ADR payload");
            }
            incoming.id = id;
            auto saved = deps.adr->save_full(incoming, author, note);
            if (!saved) return send_error(res, 500, "Failed to save ADR");
            send_json(res, 200, *saved);
        });
    });

    svr.Delete(R"(/api/adr/([0-9a-fA-F-]+))",
               [deps](const httplib::Request& req, httplib::Response& res) {
                   auto id = req.matches[1].str();
                   if (!deps.adr->remove(id)) return send_error(res, 404, "Not found");
                   send_json(res, 200, json{{"ok", true}});
               });

    // --- ADR export ----------------------------------------------------------
    svr.Get(R"(/api/adr/([0-9a-fA-F-]+)/export)",
            [deps](const httplib::Request& req, httplib::Response& res) {
                auto id = req.matches[1].str();
                auto fmt = query_param(req, "format", "md");
                bool save = query_param(req, "save", "false") == "true";
                auto a = deps.adr->get(id);
                if (!a) return send_error(res, 404, "ADR not found");
                if (fmt == "json") {
                    json body = *a;
                    if (save) deps.exporter->export_adr_json(*a);
                    res.set_content(body.dump(2), "application/json");
                } else {
                    auto md = deps.exporter->adr_to_markdown(*a);
                    if (save) deps.exporter->export_adr_markdown(*a);
                    res.set_content(md, "text/markdown; charset=utf-8");
                }
            });

    // --- ATAM templates ------------------------------------------------------
    svr.Get("/api/atam/templates", [deps](const httplib::Request&, httplib::Response& res) {
        json arr = json::array();
        for (const auto& t : deps.templates->list_atam_templates()) {
            arr.push_back(json{{"name", t.name}, {"description", t.description}});
        }
        send_json(res, 200, arr);
    });

    // --- ATAM CRUD -----------------------------------------------------------
    svr.Get("/api/atam", [deps](const httplib::Request& req, httplib::Response& res) {
        services::AtamFilter f;
        f.text = query_param(req, "q");
        f.status = query_param(req, "status");
        f.tag = query_param(req, "tag");
        json arr = json::array();
        for (const auto& s : deps.atam->list(f)) {
            json j = s;
            arr.push_back(j);
        }
        send_json(res, 200, arr);
    });

    svr.Get(R"(/api/atam/([0-9a-fA-F-]+))",
            [deps](const httplib::Request& req, httplib::Response& res) {
                auto id = req.matches[1].str();
                auto s = deps.atam->get(id);
                if (!s) return send_error(res, 404, "ATAM session not found");
                send_json(res, 200, *s);
            });

    svr.Post("/api/atam", [deps](const httplib::Request& req, httplib::Response& res) {
        with_json_body(req, res, [&](const json& body) {
            std::string title = body.value("title", "");
            std::string tpl = body.value("template", "");
            std::string author = body.value("author", "architect");
            auto s = deps.atam->create_from_template(tpl, title, author);
            send_json(res, 201, s);
        });
    });

    svr.Put(R"(/api/atam/([0-9a-fA-F-]+))",
            [deps](const httplib::Request& req, httplib::Response& res) {
                auto id = req.matches[1].str();
                with_json_body(req, res, [&](const json& body) {
                    std::string author = body.value("author", "architect");
                    std::string note = body.value("note", "");
                    domain::AtamSession incoming;
                    try {
                        domain::from_json(body, incoming);
                    } catch (...) {
                        return send_error(res, 400, "Invalid ATAM payload");
                    }
                    incoming.id = id;
                    auto saved = deps.atam->save_full(incoming, author, note);
                    if (!saved) return send_error(res, 500, "Failed to save ATAM");
                    send_json(res, 200, *saved);
                });
            });

    svr.Delete(R"(/api/atam/([0-9a-fA-F-]+))",
               [deps](const httplib::Request& req, httplib::Response& res) {
                   auto id = req.matches[1].str();
                   if (!deps.atam->remove(id)) return send_error(res, 404, "Not found");
                   send_json(res, 200, json{{"ok", true}});
               });

    svr.Get(R"(/api/atam/([0-9a-fA-F-]+)/export)",
            [deps](const httplib::Request& req, httplib::Response& res) {
                auto id = req.matches[1].str();
                auto fmt = query_param(req, "format", "md");
                bool save = query_param(req, "save", "false") == "true";
                auto s = deps.atam->get(id);
                if (!s) return send_error(res, 404, "Session not found");
                if (fmt == "json") {
                    json body = *s;
                    if (save) deps.exporter->export_atam_json(*s);
                    res.set_content(body.dump(2), "application/json");
                } else {
                    auto md = deps.exporter->atam_to_markdown(*s);
                    if (save) deps.exporter->export_atam_markdown(*s);
                    res.set_content(md, "text/markdown; charset=utf-8");
                }
            });

    // --- Reuse ---------------------------------------------------------------
    svr.Post("/api/reuse/adrs", [deps](const httplib::Request& req, httplib::Response& res) {
        with_json_body(req, res, [&](const json& body) {
            std::string title = body.value("title", "");
            std::string context = body.value("context", "");
            std::vector<std::string> tags, qas;
            if (body.contains("tags") && body["tags"].is_array())
                for (const auto& t : body["tags"])
                    if (t.is_string()) tags.push_back(t.get<std::string>());
            if (body.contains("qualityAttributes") && body["qualityAttributes"].is_array())
                for (const auto& q : body["qualityAttributes"])
                    if (q.is_string()) qas.push_back(q.get<std::string>());
            auto hits = deps.reuse->find_similar_adrs(title, context, tags, qas, 5);
            json arr = json::array();
            for (const auto& h : hits) {
                arr.push_back(json{
                    {"id", h.id},
                    {"number", h.number},
                    {"title", h.title},
                    {"status", h.status},
                    {"score", h.score},
                    {"matchingTags", h.matching_tags},
                });
            }
            send_json(res, 200, arr);
        });
    });

    svr.Post("/api/reuse/scenarios",
             [deps](const httplib::Request& req, httplib::Response& res) {
                 with_json_body(req, res, [&](const json& body) {
                     auto qa = body.value("qualityAttribute", "");
                     auto st = body.value("stimulus", "");
                     auto rp = body.value("response", "");
                     auto hits = deps.reuse->find_similar_scenarios(qa, st, rp, 5);
                     json arr = json::array();
                     for (const auto& h : hits) {
                         arr.push_back(json{
                             {"sessionId", h.session_id},
                             {"sessionTitle", h.session_title},
                             {"scenarioId", h.scenario_id},
                             {"qualityAttribute", h.quality_attribute},
                             {"stimulus", h.stimulus},
                             {"response", h.response},
                             {"score", h.score},
                         });
                     }
                     send_json(res, 200, arr);
                 });
             });

    // --- AI assist ----------------------------------------------------------
    auto ai_required = [&](httplib::Response& res) -> bool {
        if (!deps.ai) {
            send_error(res, 503, "AI service not available");
            return false;
        }
        return true;
    };

    auto to_ai_response = [](httplib::Response& res, const ai::AiResult& r) {
        if (!r.ok) {
            send_json(res, 503, json{{"ok", false}, {"error", r.error}});
            return;
        }
        send_json(res, 200, json{{"ok", true}, {"text", r.text}});
    };

    svr.Post("/api/ai/draft-adr", [deps, ai_required, to_ai_response](
                                      const httplib::Request& req, httplib::Response& res) {
        if (!ai_required(res)) return;
        with_json_body(req, res, [&](const json& body) {
            ai::AiDraftAdrRequest r;
            r.title = body.value("title", "");
            r.notes = body.value("notes", "");
            if (body.contains("qualityAttributes") && body["qualityAttributes"].is_array())
                for (const auto& q : body["qualityAttributes"])
                    if (q.is_string()) r.quality_attributes.push_back(q.get<std::string>());
            to_ai_response(res, deps.ai->draft_adr(r));
        });
    });

    svr.Post("/api/ai/improve-section", [deps, ai_required, to_ai_response](
                                            const httplib::Request& req, httplib::Response& res) {
        if (!ai_required(res)) return;
        with_json_body(req, res, [&](const json& body) {
            std::string section = body.value("section", "");
            std::string text = body.value("text", "");
            to_ai_response(res, deps.ai->improve_section(section, text));
        });
    });

    svr.Post("/api/ai/summarize-context", [deps, ai_required, to_ai_response](
                                              const httplib::Request& req, httplib::Response& res) {
        if (!ai_required(res)) return;
        with_json_body(req, res, [&](const json& body) {
            to_ai_response(res, deps.ai->summarize_context(body.value("context", "")));
        });
    });

    svr.Post("/api/ai/suggest-alternatives",
             [deps, ai_required, to_ai_response](const httplib::Request& req, httplib::Response& res) {
                 if (!ai_required(res)) return;
                 with_json_body(req, res, [&](const json& body) {
                     to_ai_response(res, deps.ai->suggest_alternatives(body.value("title", ""),
                                                                        body.value("context", ""),
                                                                        body.value("decision", "")));
                 });
             });

    svr.Post("/api/ai/suggest-consequences",
             [deps, ai_required, to_ai_response](const httplib::Request& req, httplib::Response& res) {
                 if (!ai_required(res)) return;
                 with_json_body(req, res, [&](const json& body) {
                     to_ai_response(res, deps.ai->suggest_consequences(body.value("title", ""),
                                                                        body.value("decision", "")));
                 });
             });

    svr.Post("/api/ai/suggest-risks",
             [deps, ai_required, to_ai_response](const httplib::Request& req, httplib::Response& res) {
                 if (!ai_required(res)) return;
                 with_json_body(req, res, [&](const json& body) {
                     to_ai_response(res, deps.ai->suggest_risks(body.value("title", ""),
                                                                 body.value("decision", "")));
                 });
             });

    svr.Post("/api/ai/suggest-scenarios",
             [deps, ai_required, to_ai_response](const httplib::Request& req, httplib::Response& res) {
                 if (!ai_required(res)) return;
                 with_json_body(req, res, [&](const json& body) {
                     to_ai_response(res, deps.ai->suggest_scenarios(body.value("qualityAttribute", ""),
                                                                     body.value("systemContext", "")));
                 });
             });

    svr.Post(R"(/api/ai/atam/([0-9a-fA-F-]+)/infer-tradeoffs)",
             [deps, ai_required, to_ai_response](const httplib::Request& req, httplib::Response& res) {
                 if (!ai_required(res)) return;
                 auto id = req.matches[1].str();
                 auto s = deps.atam->get(id);
                 if (!s) return send_error(res, 404, "Session not found");
                 to_ai_response(res, deps.ai->infer_sensitivity_tradeoffs(*s));
             });

    svr.Post(R"(/api/ai/atam/([0-9a-fA-F-]+)/summary)",
             [deps, ai_required, to_ai_response](const httplib::Request& req, httplib::Response& res) {
                 if (!ai_required(res)) return;
                 auto id = req.matches[1].str();
                 auto s = deps.atam->get(id);
                 if (!s) return send_error(res, 404, "Session not found");
                 to_ai_response(res, deps.ai->summarize_atam(*s));
             });

    svr.Post(R"(/api/ai/atam/([0-9a-fA-F-]+)/adr-candidates)",
             [deps, ai_required, to_ai_response](const httplib::Request& req, httplib::Response& res) {
                 if (!ai_required(res)) return;
                 auto id = req.matches[1].str();
                 auto s = deps.atam->get(id);
                 if (!s) return send_error(res, 404, "Session not found");
                 to_ai_response(res, deps.ai->atam_to_adr_candidates(*s));
             });
}

}  // namespace adra::http
