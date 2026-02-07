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

// Little's Law (Concurrency) Calculator
json calculateLittlesLaw(const json& input) {
    json response;

    try {
        if (!input.contains("rps") || !input.contains("latency")) {
            response["error"] = "Missing required fields: rps, latency";
            return response;
        }

        double rps = input["rps"];
        double latency = input["latency"];

        if (!isPositive(rps)) {
            response["error"] = "RPS must be positive";
            return response;
        }
        if (!isPositive(latency)) {
            response["error"] = "Latency must be positive";
            return response;
        }

        double concurrency = rps * latency;

        response["success"] = true;
        response["concurrency"] = std::round(concurrency * 100.0) / 100.0;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// LLM Latency Calculator
json calculateLLMLatency(const json& input) {
    json response;

    try {
        if (!input.contains("tokensCount") || !input.contains("timePerToken")) {
            response["error"] = "Missing required fields: tokensCount, timePerToken";
            return response;
        }

        int tokensCount = input["tokensCount"];
        double timePerToken = input["timePerToken"];
        double prefillTime = input.value("prefillTime", 0.0);

        if (tokensCount <= 0) {
            response["error"] = "Tokens count must be positive";
            return response;
        }
        if (timePerToken <= 0) {
            response["error"] = "Time per token must be positive";
            return response;
        }
        if (prefillTime < 0) {
            response["error"] = "Prefill time must be non-negative";
            return response;
        }

        double generationTimeSec = (tokensCount * timePerToken) / 1000.0;
        double totalTimeSec = (prefillTime / 1000.0) + generationTimeSec;

        response["success"] = true;
        response["generationTimeSec"] = std::round(generationTimeSec * 1000.0) / 1000.0;
        response["totalTimeSec"] = std::round(totalTimeSec * 1000.0) / 1000.0;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// Vector DB Storage Sizing Calculator
json calculateVectorStorage(const json& input) {
    json response;

    try {
        if (!input.contains("pages") || !input.contains("chunksPerPage") || !input.contains("vectorDim")) {
            response["error"] = "Missing required fields: pages, chunksPerPage, vectorDim";
            return response;
        }

        int pages = input["pages"];
        int chunksPerPage = input["chunksPerPage"];
        int vectorDim = input["vectorDim"];
        int bytesPerFloat = input.value("bytesPerFloat", 4);

        if (pages <= 0) {
            response["error"] = "Pages must be positive";
            return response;
        }
        if (chunksPerPage <= 0) {
            response["error"] = "Chunks per page must be positive";
            return response;
        }
        if (vectorDim <= 0) {
            response["error"] = "Vector dimension must be positive";
            return response;
        }
        if (bytesPerFloat != 2 && bytesPerFloat != 4 && bytesPerFloat != 8) {
            response["error"] = "Bytes per float must be 2 (float16), 4 (float32), or 8 (float64)";
            return response;
        }

        long long totalVectors = (long long)pages * chunksPerPage;
        double vectorSizeKB = (vectorDim * bytesPerFloat) / 1024.0;
        double totalSizeMB = (totalVectors * vectorSizeKB) / 1024.0;

        response["success"] = true;
        response["totalVectors"] = totalVectors;
        response["vectorSizeKB"] = std::round(vectorSizeKB * 100.0) / 100.0;
        response["totalSizeMB"] = std::round(totalSizeMB * 100.0) / 100.0;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// Log Storage (Observability) Calculator
json calculateLogStorage(const json& input) {
    json response;

    try {
        if (!input.contains("rps") || !input.contains("logSizeKB")) {
            response["error"] = "Missing required fields: rps, logSizeKB";
            return response;
        }

        double rps = input["rps"];
        double logSizeKB = input["logSizeKB"];
        int retentionDays = input.value("retentionDays", 30);

        if (!isPositive(rps)) {
            response["error"] = "RPS must be positive";
            return response;
        }
        if (!isPositive(logSizeKB)) {
            response["error"] = "Log size must be positive";
            return response;
        }
        if (retentionDays <= 0) {
            response["error"] = "Retention days must be positive";
            return response;
        }

        double eventsPerDay = rps * 86400.0;
        double dailySizeGB = (eventsPerDay * logSizeKB) / (1024.0 * 1024.0);
        double totalSizeGB = dailySizeGB * retentionDays;

        response["success"] = true;
        response["eventsPerDay"] = std::round(eventsPerDay);
        response["dailySizeGB"] = std::round(dailySizeGB * 100.0) / 100.0;
        response["totalSizeGB"] = std::round(totalSizeGB * 100.0) / 100.0;

    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

// Scalability Check (RAM) Calculator
json calculateScalability(const json& input) {
    json response;

    try {
        if (!input.contains("rps") || !input.contains("latency") || !input.contains("ramPerWorkerMB") || !input.contains("availableRAM_GB")) {
            response["error"] = "Missing required fields: rps, latency, ramPerWorkerMB, availableRAM_GB";
            return response;
        }

        double rps = input["rps"];
        double latency = input["latency"];
        double ramPerWorkerMB = input["ramPerWorkerMB"];
        double availableRAM_GB = input["availableRAM_GB"];

        if (!isPositive(rps)) {
            response["error"] = "RPS must be positive";
            return response;
        }
        if (!isPositive(latency)) {
            response["error"] = "Latency must be positive";
            return response;
        }
        if (!isPositive(ramPerWorkerMB)) {
            response["error"] = "RAM per worker must be positive";
            return response;
        }
        if (!isPositive(availableRAM_GB)) {
            response["error"] = "Available RAM must be positive";
            return response;
        }

        double concurrency = rps * latency;
        double requiredRAM_GB = (concurrency * ramPerWorkerMB) / 1024.0;
        bool willCrash = requiredRAM_GB > availableRAM_GB;

        response["success"] = true;
        response["concurrency"] = std::round(concurrency * 100.0) / 100.0;
        response["requiredRAM_GB"] = std::round(requiredRAM_GB * 100.0) / 100.0;
        response["availableRAM_GB"] = availableRAM_GB;
        response["willCrash"] = willCrash;

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

    svr.Post("/api/littlesLaw", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateLittlesLaw(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/llmLatency", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateLLMLatency(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/vectorStorage", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateVectorStorage(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/logStorage", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateLogStorage(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/scalability", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateScalability(input);
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
