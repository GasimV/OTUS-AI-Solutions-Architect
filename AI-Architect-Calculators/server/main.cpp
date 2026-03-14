#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <vector>
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

double roundTo2(double value) {
    return std::round(value * 100.0) / 100.0;
}

std::string getPressureStatus(double pressure) {
    if (pressure > 1.0) {
        return "Critical";
    }
    if (pressure > 0.8) {
        return "Warning";
    }
    return "OK";
}

json makeRecommendation(const std::string& area, const std::string& priority, const std::string& message) {
    return json{
        {"area", area},
        {"priority", priority},
        {"message", message}
    };
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

// Resource Calculation (Sizing) Calculator
json calculateResourceSizing(const json& input) {
    json response;

    try {
        auto requireNumber = [&](const std::string& field, double& value) -> bool {
            if (!input.contains(field) || !input[field].is_number()) {
                response["error"] = "Missing required field: " + field;
                return false;
            }
            value = input[field].get<double>();
            return true;
        };

        auto requireNonNegative = [&](const std::string& field, double& value) -> bool {
            if (!requireNumber(field, value)) {
                return false;
            }
            if (value < 0) {
                response["error"] = field + " must be non-negative";
                return false;
            }
            return true;
        };

        auto requirePositive = [&](const std::string& field, double& value) -> bool {
            if (!requireNumber(field, value)) {
                return false;
            }
            if (value <= 0) {
                response["error"] = field + " must be positive";
                return false;
            }
            return true;
        };

        auto getOptionalNumber = [&](const std::string& field, double defaultValue, double& value) -> bool {
            if (!input.contains(field) || input[field].is_null()) {
                value = defaultValue;
                return true;
            }
            if (!input[field].is_number()) {
                response["error"] = "Field must be numeric: " + field;
                return false;
            }
            value = input[field].get<double>();
            return true;
        };

        auto validateRatio = [&](const std::string& field, double value) -> bool {
            if (!inRange(value, 0.0, 10.0)) {
                response["error"] = field + " must be between 0 and 10";
                return false;
            }
            return true;
        };

        auto validateFraction = [&](const std::string& field, double value) -> bool {
            if (!inRange(value, 0.0, 1.0)) {
                response["error"] = field + " must be between 0 and 1";
                return false;
            }
            return true;
        };

        if (!input.contains("componentType") || !input["componentType"].is_string()) {
            response["error"] = "Missing required field: componentType";
            return response;
        }

        std::string componentType = input["componentType"];
        if (componentType != "stateless" &&
            componentType != "sql" &&
            componentType != "nosql" &&
            componentType != "vector_db" &&
            componentType != "cache" &&
            componentType != "logging" &&
            componentType != "llm_vram") {
            response["error"] = "componentType must be one of: stateless, sql, nosql, vector_db, cache, logging, llm_vram";
            return response;
        }

        double peakRps = 0.0;
        double avgLatencyMs = 0.0;
        double avgRequestSizeKB = 0.0;
        double avgResponseSizeKB = 0.0;
        double replicas = 0.0;
        double safetyFactor = 1.3;
        double peakFactor = 1.5;

        response["componentType"] = componentType;
        double concurrency = 0.0;
        double networkMBps = 0.0;
        double networkGbps = 0.0;

        if (!getOptionalNumber("safetyFactor", 1.3, safetyFactor)) {
            return response;
        }
        if (safetyFactor <= 0) {
            response["error"] = "safetyFactor must be positive";
            return response;
        }

        if (componentType != "llm_vram") {
            if (!requirePositive("peakRps", peakRps) ||
                !requirePositive("avgLatencyMs", avgLatencyMs) ||
                !requireNonNegative("avgRequestSizeKB", avgRequestSizeKB) ||
                !requireNonNegative("avgResponseSizeKB", avgResponseSizeKB) ||
                !requirePositive("replicas", replicas) ||
                !getOptionalNumber("peakFactor", 1.5, peakFactor)) {
                return response;
            }

            if (peakFactor <= 0) {
                response["error"] = "peakFactor must be positive";
                return response;
            }

            concurrency = peakRps * (avgLatencyMs / 1000.0);
            networkMBps = peakRps * (avgRequestSizeKB + avgResponseSizeKB) / 1024.0;
            networkGbps = (networkMBps * 8.0) / 1024.0;

            response["concurrency"] = roundTo2(concurrency);
            response["networkMBps"] = roundTo2(networkMBps);
            response["networkGbps"] = roundTo2(networkGbps);
        }

        if (componentType == "stateless") {
            double cpuSecondsPerRequest = 0.0;
            double baseRamGB = 0.0;
            double memPerRequestMB = 0.0;
            double vcpuPerNode = 0.0;
            double ramPerNodeGB = 0.0;

            if (!requireNonNegative("cpuSecondsPerRequest", cpuSecondsPerRequest) ||
                !requireNonNegative("baseRamGB", baseRamGB) ||
                !requireNonNegative("memPerRequestMB", memPerRequestMB) ||
                !requirePositive("vcpuPerNode", vcpuPerNode) ||
                !requirePositive("ramPerNodeGB", ramPerNodeGB)) {
                return response;
            }

            double requiredVcpu = (peakRps * cpuSecondsPerRequest * peakFactor * safetyFactor) / 0.65;
            double requiredRamGB =
                ((baseRamGB + (concurrency * memPerRequestMB / 1024.0)) * peakFactor * safetyFactor) / 0.75;
            double nodesByCpu = vcpuPerNode > 0 ? requiredVcpu / vcpuPerNode : 0.0;
            double nodesByRam = ramPerNodeGB > 0 ? requiredRamGB / ramPerNodeGB : 0.0;
            long long requiredNodes = static_cast<long long>(std::ceil(std::max(nodesByCpu, nodesByRam)));

            response["requiredVcpu"] = roundTo2(requiredVcpu);
            response["requiredRamGB"] = roundTo2(requiredRamGB);
            response["requiredNodes"] = requiredNodes;
        } else if (componentType == "sql") {
            double dataGB = 0.0;
            double hotDataGB = 0.0;
            double readRps = 0.0;
            double writeRps = 0.0;
            double cpuReadSec = 0.0;
            double cpuWriteSec = 0.0;
            double indexRatio = 0.5;
            double walGB = 0.0;
            double tempGB = 0.0;
            double queryWorkspaceGB = 0.0;
            double osReserveGB = 0.0;
            double cacheCoverageRatio = 0.7;

            if (!requirePositive("dataGB", dataGB) ||
                !requireNonNegative("hotDataGB", hotDataGB) ||
                !requireNonNegative("readRps", readRps) ||
                !requireNonNegative("writeRps", writeRps) ||
                !requireNonNegative("cpuReadSec", cpuReadSec) ||
                !requireNonNegative("cpuWriteSec", cpuWriteSec) ||
                !getOptionalNumber("indexRatio", 0.5, indexRatio) ||
                !requireNonNegative("walGB", walGB) ||
                !requireNonNegative("tempGB", tempGB) ||
                !requireNonNegative("queryWorkspaceGB", queryWorkspaceGB) ||
                !requireNonNegative("osReserveGB", osReserveGB) ||
                !getOptionalNumber("cacheCoverageRatio", 0.7, cacheCoverageRatio)) {
                return response;
            }

            if (!validateRatio("indexRatio", indexRatio) ||
                !validateFraction("cacheCoverageRatio", cacheCoverageRatio)) {
                return response;
            }
            if (hotDataGB > dataGB) {
                response["error"] = "hotDataGB cannot exceed dataGB";
                return response;
            }

            double sqlDiskGB =
                ((dataGB + dataGB * indexRatio + walGB + tempGB) * replicas * safetyFactor) / 0.75;
            double sqlRamGB =
                ((hotDataGB * cacheCoverageRatio + queryWorkspaceGB + osReserveGB) * safetyFactor) / 0.75;
            double sqlVcpu =
                (((readRps * cpuReadSec) + (writeRps * cpuWriteSec)) * peakFactor * safetyFactor) / 0.55;

            response["sqlVcpu"] = roundTo2(sqlVcpu);
            response["sqlRamGB"] = roundTo2(sqlRamGB);
            response["sqlDiskGB"] = roundTo2(sqlDiskGB);
        } else if (componentType == "nosql") {
            double dataGB = 0.0;
            double hotDataGB = 0.0;
            double readRps = 0.0;
            double writeRps = 0.0;
            double rpsPerShard = 0.0;
            double dataGBPerShard = 0.0;
            double cpuReadSec = 0.0;
            double cpuWriteSec = 0.0;
            double indexRatio = 0.2;
            double compactionRatio = 0.3;
            double cacheRatio = 0.3;
            double memtableGB = 0.0;
            double osReserveGB = 0.0;

            if (!requirePositive("dataGB", dataGB) ||
                !requireNonNegative("hotDataGB", hotDataGB) ||
                !requireNonNegative("readRps", readRps) ||
                !requireNonNegative("writeRps", writeRps) ||
                !requirePositive("rpsPerShard", rpsPerShard) ||
                !requirePositive("dataGBPerShard", dataGBPerShard) ||
                !requireNonNegative("cpuReadSec", cpuReadSec) ||
                !requireNonNegative("cpuWriteSec", cpuWriteSec) ||
                !getOptionalNumber("indexRatio", 0.2, indexRatio) ||
                !getOptionalNumber("compactionRatio", 0.3, compactionRatio) ||
                !getOptionalNumber("cacheRatio", 0.3, cacheRatio) ||
                !requireNonNegative("memtableGB", memtableGB) ||
                !requireNonNegative("osReserveGB", osReserveGB)) {
                return response;
            }

            if (!validateRatio("indexRatio", indexRatio) ||
                !validateRatio("compactionRatio", compactionRatio) ||
                !validateFraction("cacheRatio", cacheRatio)) {
                return response;
            }
            if (hotDataGB > dataGB) {
                response["error"] = "hotDataGB cannot exceed dataGB";
                return response;
            }

            long long shards = std::max(
                static_cast<long long>(std::ceil(peakRps / rpsPerShard)),
                static_cast<long long>(std::ceil(dataGB / dataGBPerShard))
            );
            long long nosqlNodes = static_cast<long long>(std::ceil(shards * replicas));
            double nosqlDiskGB =
                (dataGB * (1.0 + indexRatio + compactionRatio) * replicas * safetyFactor) / 0.75;
            double nosqlRamGB =
                ((hotDataGB * cacheRatio + memtableGB + osReserveGB) * safetyFactor) / 0.75;
            double nosqlVcpu =
                (((readRps * cpuReadSec) + (writeRps * cpuWriteSec)) * peakFactor * safetyFactor) / 0.60;

            response["shards"] = shards;
            response["nosqlNodes"] = nosqlNodes;
            response["nosqlVcpu"] = roundTo2(nosqlVcpu);
            response["nosqlRamGB"] = roundTo2(nosqlRamGB);
            response["nosqlDiskGB"] = roundTo2(nosqlDiskGB);
        } else if (componentType == "vector_db") {
            double numVectors = 0.0;
            double dimension = 0.0;
            double metadataBytesPerVector = 0.0;
            double indexRatio = 0.0;
            double residentFraction = 0.0;
            double indexResidentFraction = 0.0;
            double peakQueryQps = 0.0;
            double candidatesScanned = 0.0;
            double vectorOpsPerCorePerSecond = 0.0;
            double queryBufferGB = 0.0;
            double walGB = 0.0;
            double snapshotGB = 0.0;
            double osReserveGB = 0.0;

            if (!requirePositive("numVectors", numVectors) ||
                !requirePositive("dimension", dimension) ||
                !requireNonNegative("metadataBytesPerVector", metadataBytesPerVector) ||
                !requireNonNegative("peakQueryQps", peakQueryQps) ||
                !requirePositive("candidatesScanned", candidatesScanned) ||
                !requirePositive("vectorOpsPerCorePerSecond", vectorOpsPerCorePerSecond) ||
                !requireNonNegative("queryBufferGB", queryBufferGB) ||
                !requireNonNegative("walGB", walGB) ||
                !requireNonNegative("snapshotGB", snapshotGB) ||
                !requireNonNegative("osReserveGB", osReserveGB) ||
                !getOptionalNumber("indexRatio", 0.0, indexRatio) ||
                !getOptionalNumber("residentFraction", 0.0, residentFraction) ||
                !getOptionalNumber("indexResidentFraction", 0.0, indexResidentFraction)) {
                return response;
            }

            if (!input.contains("precision") || !input["precision"].is_string()) {
                response["error"] = "Missing required field: precision";
                return response;
            }

            std::string precision = input["precision"];
            double bytesPerElement = 0.0;
            if (precision == "fp32") {
                bytesPerElement = 4.0;
            } else if (precision == "fp16") {
                bytesPerElement = 2.0;
            } else if (precision == "int8") {
                bytesPerElement = 1.0;
            } else if (precision == "int4") {
                bytesPerElement = 0.5;
            } else {
                response["error"] = "precision must be one of: fp32, fp16, int8, int4";
                return response;
            }

            if (!validateRatio("indexRatio", indexRatio) ||
                !validateFraction("residentFraction", residentFraction) ||
                !validateFraction("indexResidentFraction", indexResidentFraction)) {
                return response;
            }

            const long double gib = 1024.0L * 1024.0L * 1024.0L;
            long double rawVectorGB = (static_cast<long double>(numVectors) * dimension * bytesPerElement) / gib;
            long double metadataGB = (static_cast<long double>(numVectors) * metadataBytesPerVector) / gib;
            long double indexGB = rawVectorGB * indexRatio;
            long double vectorRamGB =
                (((rawVectorGB * residentFraction) +
                  (indexGB * indexResidentFraction) +
                  queryBufferGB +
                  osReserveGB) * safetyFactor) / 0.70;
            long double vectorDiskGB =
                ((rawVectorGB + metadataGB + indexGB + walGB + snapshotGB) * replicas * safetyFactor) / 0.75;
            long double vectorCpuSecPerQuery =
                (static_cast<long double>(candidatesScanned) * dimension) / vectorOpsPerCorePerSecond;
            long double vectorVcpu =
                (peakQueryQps * vectorCpuSecPerQuery * peakFactor * safetyFactor) / 0.50;

            response["rawVectorGB"] = roundTo2(static_cast<double>(rawVectorGB));
            response["metadataGB"] = roundTo2(static_cast<double>(metadataGB));
            response["indexGB"] = roundTo2(static_cast<double>(indexGB));
            response["vectorVcpu"] = roundTo2(static_cast<double>(vectorVcpu));
            response["vectorRamGB"] = roundTo2(static_cast<double>(vectorRamGB));
            response["vectorDiskGB"] = roundTo2(static_cast<double>(vectorDiskGB));
        } else if (componentType == "cache") {
            double hotCacheDataGB = 0.0;
            double serializationOverhead = 1.2;
            double evictionHeadroom = 1.15;
            double rpsPerCore = 0.0;

            if (!requirePositive("hotCacheDataGB", hotCacheDataGB) ||
                !getOptionalNumber("serializationOverhead", 1.2, serializationOverhead) ||
                !getOptionalNumber("evictionHeadroom", 1.15, evictionHeadroom) ||
                !requirePositive("rpsPerCore", rpsPerCore)) {
                return response;
            }

            if (serializationOverhead < 0 || evictionHeadroom < 0) {
                response["error"] = "Cache overhead values must be non-negative";
                return response;
            }

            double cacheRamGB =
                (hotCacheDataGB * serializationOverhead * evictionHeadroom * safetyFactor) / 0.75;
            double cacheVcpu = (peakRps * peakFactor * safetyFactor) / (rpsPerCore * 0.65);

            response["cacheVcpu"] = roundTo2(cacheVcpu);
            response["cacheRamGB"] = roundTo2(cacheRamGB);
        } else if (componentType == "logging") {
            double eventsPerSec = 0.0;
            double avgLogKB = 0.0;
            double retentionDays = 0.0;
            double replicationFactor = 0.0;
            double overheadRatio = 0.0;

            if (!requirePositive("eventsPerSec", eventsPerSec) ||
                !requirePositive("avgLogKB", avgLogKB) ||
                !requirePositive("retentionDays", retentionDays) ||
                !requirePositive("replicationFactor", replicationFactor) ||
                !requireNonNegative("overheadRatio", overheadRatio)) {
                return response;
            }

            if (!validateRatio("overheadRatio", overheadRatio)) {
                return response;
            }

            double dailyLogGB = (eventsPerSec * avgLogKB * 86400.0) / (1024.0 * 1024.0);
            double loggingDiskGB =
                (dailyLogGB * retentionDays * replicationFactor * (1.0 + overheadRatio) * safetyFactor) / 0.75;

            response["dailyLogGB"] = roundTo2(dailyLogGB);
            response["loggingDiskGB"] = roundTo2(loggingDiskGB);
        } else if (componentType == "llm_vram") {
            double modelParameters = 0.0;
            double batchSize = 0.0;
            double contextLength = 0.0;
            double hiddenSize = 0.0;
            double numLayers = 0.0;

            if (!requirePositive("modelParameters", modelParameters) ||
                !requirePositive("batchSize", batchSize) ||
                !requirePositive("contextLength", contextLength) ||
                !requirePositive("hiddenSize", hiddenSize) ||
                !requirePositive("numLayers", numLayers)) {
                return response;
            }
            if (!input.contains("precisionType") || !input["precisionType"].is_string()) {
                response["error"] = "Missing required field: precisionType";
                return response;
            }
            if (!input.contains("architectureType") || !input["architectureType"].is_string()) {
                response["error"] = "Missing required field: architectureType";
                return response;
            }

            std::string precisionType = input["precisionType"];
            std::string architectureType = input["architectureType"];
            if (architectureType != "decoder_only" &&
                architectureType != "encoder_decoder" &&
                architectureType != "other") {
                response["error"] = "architectureType must be one of: decoder_only, encoder_decoder, other";
                return response;
            }

            double bytesPerParam = 0.0;
            if (precisionType == "fp32") {
                bytesPerParam = 4.0;
            } else if (precisionType == "fp16") {
                bytesPerParam = 2.0;
            } else if (precisionType == "int8") {
                bytesPerParam = 1.0;
            } else if (precisionType == "int4") {
                bytesPerParam = 0.5;
            } else {
                response["error"] = "precisionType must be one of: fp32, fp16, int8, int4";
                return response;
            }

            bool hasAttentionHeads = input.contains("numAttentionHeads") && input["numAttentionHeads"].is_number();
            bool hasKeyValueHeads = input.contains("numKeyValueHeads") && input["numKeyValueHeads"].is_number();
            double numAttentionHeads = 0.0;
            double numKeyValueHeads = 0.0;

            if (hasAttentionHeads) {
                numAttentionHeads = input["numAttentionHeads"].get<double>();
                if (numAttentionHeads <= 0) {
                    response["error"] = "numAttentionHeads must be positive when provided";
                    return response;
                }
            }

            if (hasKeyValueHeads) {
                numKeyValueHeads = input["numKeyValueHeads"].get<double>();
                if (numKeyValueHeads <= 0) {
                    response["error"] = "numKeyValueHeads must be positive when provided";
                    return response;
                }
            }

            const long double gib = 1024.0L * 1024.0L * 1024.0L;
            long double weightMemoryGB = (static_cast<long double>(modelParameters) * bytesPerParam) / gib;
            long double kvCacheGB =
                (static_cast<long double>(batchSize) * contextLength * hiddenSize * 2.0L * bytesPerParam * numLayers) / gib;
            long double activationGB = 0.0L;
            long double prefillActivationGB = 0.0L;
            long double decodeWorkspaceGB = 0.0L;
            long double fallbackActivationMultiplier = 3.0L;
            bool usedFallbackHeuristic = architectureType != "decoder_only" || !hasAttentionHeads;
            long double numAttentionHeadsUsed = 0.0L;
            long double numKeyValueHeadsUsed = 0.0L;
            long double gqaRatio = 1.0L;

            if (!usedFallbackHeuristic) {
                numAttentionHeadsUsed = numAttentionHeads;
                numKeyValueHeadsUsed = hasKeyValueHeads ? numKeyValueHeads : numAttentionHeads;

                if (numKeyValueHeadsUsed > numAttentionHeadsUsed) {
                    response["error"] = "numKeyValueHeads cannot exceed numAttentionHeads";
                    return response;
                }

                gqaRatio = numKeyValueHeadsUsed / numAttentionHeadsUsed;
                prefillActivationGB =
                    (static_cast<long double>(batchSize) * contextLength * hiddenSize * numLayers * bytesPerParam * (2.0L + gqaRatio)) / gib;
                decodeWorkspaceGB =
                    (static_cast<long double>(batchSize) * hiddenSize * numLayers * bytesPerParam * (6.0L + (2.0L * gqaRatio))) / gib;
                activationGB = prefillActivationGB + decodeWorkspaceGB;
            } else {
                activationGB =
                    (static_cast<long double>(batchSize) * contextLength * hiddenSize * numLayers * bytesPerParam * fallbackActivationMultiplier) / gib;
            }

            long double totalVramGB =
                (weightMemoryGB + kvCacheGB + activationGB) * safetyFactor;

            response["architectureType"] = architectureType;
            response["numLayers"] = static_cast<long long>(std::llround(numLayers));
            response["weightMemoryGB"] = roundTo2(static_cast<double>(weightMemoryGB));
            response["kvCacheGB"] = roundTo2(static_cast<double>(kvCacheGB));
            response["activationGB"] = roundTo2(static_cast<double>(activationGB));
            response["totalVramGB"] = roundTo2(static_cast<double>(totalVramGB));
            response["usedFallbackHeuristic"] = usedFallbackHeuristic;

            if (!usedFallbackHeuristic) {
                response["numAttentionHeadsUsed"] = static_cast<long long>(std::llround(numAttentionHeadsUsed));
                response["numKeyValueHeadsUsed"] = static_cast<long long>(std::llround(numKeyValueHeadsUsed));
                response["gqaRatio"] = roundTo2(static_cast<double>(gqaRatio));
                response["prefillActivationGB"] = roundTo2(static_cast<double>(prefillActivationGB));
                response["decodeWorkspaceGB"] = roundTo2(static_cast<double>(decodeWorkspaceGB));
            } else {
                response["fallbackActivationMultiplier"] = roundTo2(static_cast<double>(fallbackActivationMultiplier));
            }
        }

        bool includeTco = input.value("includeTco", false);
        if (includeTco) {
            double hardwareCapex = 0.0;
            double storageCapex = 0.0;
            double networkCapex = 0.0;
            double annualPowerCost = 0.0;
            double annualCoolingCost = 0.0;
            double annualSupportCost = 0.0;
            double annualLicensingCost = 0.0;
            double annualStaffCost = 0.0;
            double annualDowntimeRiskCost = 0.0;
            double years = 0.0;

            double monthlyComputeCost = 0.0;
            double monthlyStorageCost = 0.0;
            double monthlyBackupCost = 0.0;
            double monthlyEgressCost = 0.0;
            double monthlyManagedServicesCost = 0.0;
            double monthlySupportPlanCost = 0.0;
            double monthlyStaffCost = 0.0;
            double monthlyDowntimeRiskCost = 0.0;
            double months = 0.0;

            if (!requireNonNegative("hardwareCapex", hardwareCapex) ||
                !requireNonNegative("storageCapex", storageCapex) ||
                !requireNonNegative("networkCapex", networkCapex) ||
                !requireNonNegative("annualPowerCost", annualPowerCost) ||
                !requireNonNegative("annualCoolingCost", annualCoolingCost) ||
                !requireNonNegative("annualSupportCost", annualSupportCost) ||
                !requireNonNegative("annualLicensingCost", annualLicensingCost) ||
                !requireNonNegative("annualStaffCost", annualStaffCost) ||
                !requireNonNegative("annualDowntimeRiskCost", annualDowntimeRiskCost) ||
                !requirePositive("years", years) ||
                !requireNonNegative("monthlyComputeCost", monthlyComputeCost) ||
                !requireNonNegative("monthlyStorageCost", monthlyStorageCost) ||
                !requireNonNegative("monthlyBackupCost", monthlyBackupCost) ||
                !requireNonNegative("monthlyEgressCost", monthlyEgressCost) ||
                !requireNonNegative("monthlyManagedServicesCost", monthlyManagedServicesCost) ||
                !requireNonNegative("monthlySupportPlanCost", monthlySupportPlanCost) ||
                !requireNonNegative("monthlyStaffCost", monthlyStaffCost) ||
                !requireNonNegative("monthlyDowntimeRiskCost", monthlyDowntimeRiskCost) ||
                !requirePositive("months", months)) {
                return response;
            }

            double tcoOnPrem =
                hardwareCapex +
                storageCapex +
                networkCapex +
                years * (
                    annualPowerCost +
                    annualCoolingCost +
                    annualSupportCost +
                    annualLicensingCost +
                    annualStaffCost +
                    annualDowntimeRiskCost
                );

            double tcoCloud =
                months * (
                    monthlyComputeCost +
                    monthlyStorageCost +
                    monthlyBackupCost +
                    monthlyEgressCost +
                    monthlyManagedServicesCost +
                    monthlySupportPlanCost +
                    monthlyStaffCost +
                    monthlyDowntimeRiskCost
                );

            double upfrontOnPremCapex = hardwareCapex + storageCapex + networkCapex;
            double monthlyOnPremEquivalent =
                (annualPowerCost +
                 annualCoolingCost +
                 annualSupportCost +
                 annualLicensingCost +
                 annualStaffCost +
                 annualDowntimeRiskCost) / 12.0;
            double monthlyCloud =
                monthlyComputeCost +
                monthlyStorageCost +
                monthlyBackupCost +
                monthlyEgressCost +
                monthlyManagedServicesCost +
                monthlySupportPlanCost +
                monthlyStaffCost +
                monthlyDowntimeRiskCost;

            response["tcoOnPrem"] = roundTo2(tcoOnPrem);
            response["tcoCloud"] = roundTo2(tcoCloud);

            if (monthlyCloud > monthlyOnPremEquivalent) {
                double breakEvenMonths = upfrontOnPremCapex / (monthlyCloud - monthlyOnPremEquivalent);
                response["breakEvenMonths"] = roundTo2(breakEvenMonths);
            }
        }

        response["success"] = true;
    } catch (const std::exception& e) {
        response["error"] = std::string("Calculation error: ") + e.what();
    }

    return response;
}

json calculateLoadAnalysis(const json& input) {
    json response;

    try {
        auto requireNumber = [&](const std::string& field, double& value) -> bool {
            if (!input.contains(field) || !input[field].is_number()) {
                response["error"] = "Missing required field: " + field;
                return false;
            }
            value = input[field].get<double>();
            return true;
        };

        auto requirePositive = [&](const std::string& field, double& value) -> bool {
            if (!requireNumber(field, value)) {
                return false;
            }
            if (value <= 0) {
                response["error"] = field + " must be positive";
                return false;
            }
            return true;
        };

        auto requireNonNegative = [&](const std::string& field, double& value) -> bool {
            if (!requireNumber(field, value)) {
                return false;
            }
            if (value < 0) {
                response["error"] = field + " must be non-negative";
                return false;
            }
            return true;
        };

        double expectedRps = 0.0;
        double actualRps = 0.0;
        double cpuUsagePercent = 0.0;
        double cpuThresholdPercent = 0.0;
        double ramUsageGB = 0.0;
        double ramThresholdGB = 0.0;
        double diskIops = 0.0;
        double diskIopsThreshold = 0.0;
        double latencyMs = 0.0;
        double latencySlaMs = 0.0;
        double errorRatePercent = 0.0;
        double errorBudgetPercent = 0.0;

        if (!requirePositive("expectedRps", expectedRps) ||
            !requireNonNegative("actualRps", actualRps) ||
            !requireNonNegative("cpuUsagePercent", cpuUsagePercent) ||
            !requirePositive("cpuThresholdPercent", cpuThresholdPercent) ||
            !requireNonNegative("ramUsageGB", ramUsageGB) ||
            !requirePositive("ramThresholdGB", ramThresholdGB) ||
            !requireNonNegative("diskIops", diskIops) ||
            !requirePositive("diskIopsThreshold", diskIopsThreshold) ||
            !requireNonNegative("latencyMs", latencyMs) ||
            !requirePositive("latencySlaMs", latencySlaMs) ||
            !requireNonNegative("errorRatePercent", errorRatePercent) ||
            !requirePositive("errorBudgetPercent", errorBudgetPercent)) {
            return response;
        }

        if (!input.contains("serviceType") || !input["serviceType"].is_string()) {
            response["error"] = "Missing required field: serviceType";
            return response;
        }

        std::string serviceType = input["serviceType"];
        if (serviceType != "stateless" && serviceType != "stateful") {
            response["error"] = "serviceType must be one of: stateless, stateful";
            return response;
        }

        double trafficPressure = actualRps / expectedRps;
        double cpuPressure = cpuUsagePercent / cpuThresholdPercent;
        double ramPressure = ramUsageGB / ramThresholdGB;
        double diskPressure = diskIops / diskIopsThreshold;
        double latencyPressure = latencyMs / latencySlaMs;
        double errorPressure = errorRatePercent / errorBudgetPercent;

        std::vector<double> pressures = {
            trafficPressure,
            cpuPressure,
            ramPressure,
            diskPressure,
            latencyPressure,
            errorPressure
        };

        std::string overallStatus = "OK";
        bool hasCritical = std::any_of(pressures.begin(), pressures.end(), [](double value) {
            return value > 1.0;
        });
        bool hasWarning = std::any_of(pressures.begin(), pressures.end(), [](double value) {
            return value > 0.8;
        });

        if (hasCritical) {
            overallStatus = "Critical";
        } else if (hasWarning) {
            overallStatus = "Warning";
        }

        json recommendations = json::array();
        if (cpuPressure > 1.0) {
            recommendations.push_back(makeRecommendation("CPU", "High", "Increase backend/service instances or optimize CPU-heavy processing."));
        } else if (cpuPressure > 0.8) {
            recommendations.push_back(makeRecommendation("CPU", "Medium", "CPU usage is near the limit; monitor closely and prepare to scale."));
        }

        if (ramPressure > 1.0) {
            recommendations.push_back(makeRecommendation("RAM", "High", "Increase RAM or reduce memory footprint; consider caching and memory profiling."));
        } else if (ramPressure > 0.8) {
            recommendations.push_back(makeRecommendation("RAM", "Medium", "Memory usage is near the limit; review heap/cache usage and prepare to increase RAM."));
        }

        if (diskPressure > 1.0) {
            recommendations.push_back(makeRecommendation("Disk", "High", "Upgrade storage performance (SSD/NVMe), increase IOPS, or optimize read/write patterns."));
        } else if (diskPressure > 0.8) {
            recommendations.push_back(makeRecommendation("Disk", "Medium", "Disk subsystem is nearing saturation; monitor IOPS and storage latency."));
        }

        if (latencyPressure > 1.0) {
            recommendations.push_back(makeRecommendation("Latency", "High", "Reduce latency by optimizing queries, scaling services, or adding caching."));
        }

        if (errorPressure > 1.0) {
            recommendations.push_back(makeRecommendation("Errors", "High", "Investigate application errors, improve resilience, and review SLA/error-budget violations."));
        }

        if (trafficPressure > 1.0) {
            recommendations.push_back(makeRecommendation("Traffic", "High", "Current traffic exceeds planned capacity; scale horizontally or revise sizing assumptions."));
        }

        if (recommendations.empty()) {
            recommendations.push_back(makeRecommendation("Overall", "Info", "System load is within healthy limits based on current thresholds."));
        }

        response["success"] = true;
        response["serviceType"] = serviceType;
        response["trafficPressure"] = roundTo2(trafficPressure);
        response["cpuPressure"] = roundTo2(cpuPressure);
        response["ramPressure"] = roundTo2(ramPressure);
        response["diskPressure"] = roundTo2(diskPressure);
        response["latencyPressure"] = roundTo2(latencyPressure);
        response["errorPressure"] = roundTo2(errorPressure);
        response["trafficStatus"] = getPressureStatus(trafficPressure);
        response["cpuStatus"] = getPressureStatus(cpuPressure);
        response["ramStatus"] = getPressureStatus(ramPressure);
        response["diskStatus"] = getPressureStatus(diskPressure);
        response["latencyStatus"] = getPressureStatus(latencyPressure);
        response["errorStatus"] = getPressureStatus(errorPressure);
        response["overallStatus"] = overallStatus;
        response["recommendations"] = recommendations;
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

    svr.Post("/api/resource-sizing", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateResourceSizing(input);
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            json error;
            error["error"] = std::string("Invalid JSON: ") + e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });

    svr.Post("/api/load-analysis", [](const Request& req, Response& res) {
        try {
            json input = json::parse(req.body);
            json result = calculateLoadAnalysis(input);
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
