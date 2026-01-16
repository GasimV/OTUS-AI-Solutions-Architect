# AI Architect Calculators

A lightweight local web application for AI Solution Architects featuring essential calculators with a C++ backend and HTML/CSS/JS frontend.

## Features

### 1. GenAI ValueScore Calculator
Calculates the value score of GenAI initiatives based on scale, frequency, and manual complexity.

**Formula:** `ValueScore = (Scale × Frequency) / ManualComplexity`

### 2. PERT Estimation Calculator
Performs Program Evaluation and Review Technique (PERT) analysis for project time estimation.

**Formulas:**
- `E = (O + 4M + P) / 6` (Expected time)
- `σ = (P - O) / 6` (Standard deviation)
- `Upper95% = E + 2σ` (95% confidence interval)

### 3. Risk Exposure Calculator
Calculates financial exposure from identified risks.

**Formula:** `Exposure = (Probability% / 100) × Impact`

### 4. Total Cost of Ownership (TCO) Calculator
Computes total cost including capital and operational expenses over time.

**Formula:** `TCO = CapEx + (OpExMonthly × Months)`

### 5. Budget with Reserves Calculator
Calculates total budget including known and unknown reserves.

**Formulas:**
- `UnknownReserve = BaseBudget × (Unknown% / 100)`
- `TotalBudget = BaseBudget + KnownReserve + UnknownReserve`

## Prerequisites

- **Windows:** Visual Studio 2019 or later (with C++ support), or MinGW-w64
- **Linux/Mac:** GCC 8+ or Clang 10+
- **CMake:** Version 3.15 or higher
- **Internet connection:** Required once to download dependencies

## Building the Application

### Step 1: Download Dependencies

The project uses two header-only libraries:
- [cpp-httplib](https://github.com/yhirose/cpp-httplib) - HTTP server
- [nlohmann/json](https://github.com/nlohmann/json) - JSON parsing

**Windows:**
```cmd
cd AI-Architect-Calculators\server
download_deps.bat
```

**Linux/Mac:**
```bash
cd AI-Architect-Calculators/server
chmod +x download_deps.sh
./download_deps.sh
```

### Step 2: Build the Server

**Windows (Visual Studio):**
```cmd
cd AI-Architect-Calculators
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

**Windows (MinGW):**
```cmd
cd AI-Architect-Calculators
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

**Linux/Mac:**
```bash
cd AI-Architect-Calculators
mkdir build
cd build
cmake ..
make
```

## Running the Application

### Step 1: Start the Server

**Windows:**
```cmd
cd AI-Architect-Calculators\build
calculator_server.exe
```

**Linux/Mac:**
```bash
cd AI-Architect-Calculators/build
./calculator_server
```

You should see:
```
AI Architect Calculators Server
================================
Server starting on http://localhost:8080
Press Ctrl+C to stop
```

### Step 2: Open the Application

Open your web browser and navigate to:
```
http://localhost:8080
```

The application will load with all five calculators ready to use.

## Usage

Each calculator has:
- **Input fields** with validation (no negatives, sensible ranges)
- **Calculate button** to compute results
- **Output section** displaying formatted results
- **Enter key support** for quick calculations

### Example: GenAI ValueScore

1. Enter Scale (1-10): `8`
2. Enter Frequency (1-10): `9`
3. Enter ManualComplexity (1-10): `6`
4. Click "Calculate"
5. Result: `ValueScore: 12.00`

### Example: PERT Estimation

1. Enter Optimistic: `5`
2. Enter Most Likely: `10`
3. Enter Pessimistic: `20`
4. Select Unit: `days`
5. Click "Calculate"
6. Results:
   - Expected (E): `10.83 days`
   - Sigma (σ): `2.50 days`
   - Upper 95%: `15.83 days`


## Configuration

**Default Port:** 8080

To change the port, edit `main.cpp:321`:
```cpp
if (!svr.listen("0.0.0.0", 8080)) {
```

## Troubleshooting

### Port Already in Use
If port 8080 is occupied:
1. Close other applications using port 8080
2. Or change the port in `main.cpp` and rebuild

### Dependencies Download Failed
- Check internet connection
- Manually download files:
  - [httplib.h](https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h)
  - [json.hpp](https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp)
- Place them in the `server/` directory

## License

This project is provided as-is for educational and professional use.

## Author

Gasym A. Valiyev

Created for AI Solutions Architects, 2026
