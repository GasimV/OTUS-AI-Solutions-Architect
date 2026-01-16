#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace httplib;

// Helper function to validate number is positive
bool isPositive(double value) {
    return value > 0;
}

// Helper function to validate range
bool inRange(double value, double min, double max) {
    return value >= min && value <= max;
}

// GenAI ValueScore Calculator
json calculateValueScore(const json& input) {
    json response;

    try {
        if (!input.contains("scale") || !input.contains("frequency") || !input.contains("manualComplexity")) {
            response["error"] = "Missing required fields: scale, frequency, manualComplexity";
            return response;
        }

        double scale = input["scale"];
        double frequency = input["frequency"];
        double manualComplexity = input["manualComplexity"];

        if (!inRange(scale, 1, 10)) {
            response["error"] = "Scale must be between 1 and 10";
            return response;
        }
        if (!inRange(frequency, 1, 10)) {
            response["error"] = "Frequency must be between 1 and 10";
            return response;
        }
        if (!inRange(manualComplexity, 1, 10)) {
            response["error"] = "ManualComplexity must be between 1 and 10";
            return response;
        }

        double valueScore = (scale * frequency) / manualComplexity;

        response["success"] = true;
        response["valueScore"] = std::round(valueScore * 100.0) / 100.0;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// PERT Calculator
json calculatePERT(const json& input) {
    json response;

    try {
        if (!input.contains("optimistic") || !input.contains("mostLikely") || !input.contains("pessimistic")) {
            response["error"] = "Missing required fields: optimistic, mostLikely, pessimistic";
            return response;
        }

        double O = input["optimistic"];
        double M = input["mostLikely"];
        double P = input["pessimistic"];
        std::string unit = input.value("unit", "days");

        if (O < 0 || M < 0 || P < 0) {
            response["error"] = "All values must be non-negative";
            return response;
        }
        if (O > M || M > P) {
            response["error"] = "Values must satisfy: Optimistic <= MostLikely <= Pessimistic";
            return response;
        }

        double E = (O + 4.0 * M + P) / 6.0;
        double sigma = (P - O) / 6.0;
        double upper95 = E + 2.0 * sigma;

        response["success"] = true;
        response["expected"] = std::round(E * 100.0) / 100.0;
        response["sigma"] = std::round(sigma * 100.0) / 100.0;
        response["upper95"] = std::round(upper95 * 100.0) / 100.0;
        response["unit"] = unit;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// Risk Exposure Calculator
json calculateRisk(const json& input) {
    json response;

    try {
        if (!input.contains("probabilityPercent") || !input.contains("impact")) {
            response["error"] = "Missing required fields: probabilityPercent, impact";
            return response;
        }

        double probabilityPercent = input["probabilityPercent"];
        double impact = input["impact"];
        std::string period = input.value("period", "/month");

        if (!inRange(probabilityPercent, 0, 100)) {
            response["error"] = "Probability must be between 0 and 100";
            return response;
        }
        if (impact < 0) {
            response["error"] = "Impact must be non-negative";
            return response;
        }

        double exposure = (probabilityPercent / 100.0) * impact;

        response["success"] = true;
        response["exposure"] = std::round(exposure * 100.0) / 100.0;
        response["period"] = period;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// TCO Calculator
json calculateTCO(const json& input) {
    json response;

    try {
        if (!input.contains("capEx") || !input.contains("opExMonthly")) {
            response["error"] = "Missing required fields: capEx, opExMonthly";
            return response;
        }

        double capEx = input["capEx"];
        double opExMonthly = input["opExMonthly"];
        int months = input.value("months", 36);

        if (capEx < 0 || opExMonthly < 0) {
            response["error"] = "CapEx and OpExMonthly must be non-negative";
            return response;
        }
        if (months <= 0) {
            response["error"] = "Months must be positive";
            return response;
        }

        double tco = capEx + (opExMonthly * months);

        response["success"] = true;
        response["tco"] = std::round(tco * 100.0) / 100.0;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// Budget with Reserves Calculator
json calculateBudget(const json& input) {
    json response;

    try {
        if (!input.contains("baseBudget") || !input.contains("knownReserve")) {
            response["error"] = "Missing required fields: baseBudget, knownReserve";
            return response;
        }

        double baseBudget = input["baseBudget"];
        double knownReserve = input["knownReserve"];
        double unknownPercent = input.value("unknownPercent", 20.0);

        if (baseBudget < 0 || knownReserve < 0) {
            response["error"] = "BaseBudget and KnownReserve must be non-negative";
            return response;
        }
        if (!inRange(unknownPercent, 0, 100)) {
            response["error"] = "UnknownPercent must be between 0 and 100";
            return response;
        }

        double unknownReserve = baseBudget * (unknownPercent / 100.0);
        double totalBudget = baseBudget + knownReserve + unknownReserve;

        response["success"] = true;
        response["unknownReserve"] = std::round(unknownReserve * 100.0) / 100.0;
        response["totalBudget"] = std::round(totalBudget * 100.0) / 100.0;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

int main() {
    Server svr;

    // CORS headers for all responses
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // Handle OPTIONS requests for CORS
    svr.Options(".*", [](const Request&, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    // Serve static files from web directory
    svr.set_mount_point("/", "../../web");

    // API Endpoints
    svr.Post("/api/valueScore", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateValueScore(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/pert", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculatePERT(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/risk", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateRisk(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/tco", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateTCO(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/budget", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateBudget(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    std::cout << "AI Architect Calculators Server" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Server starting on http://localhost:8080" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl << std::endl;

    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "Failed to start server. Port 8080 may be in use." << std::endl;
        return 1;
    }

    return 0;
}
