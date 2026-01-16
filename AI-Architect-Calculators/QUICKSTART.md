# Quick Start Guide

## First Time Setup (5 minutes)

### 1. Download Dependencies
```cmd
cd server
download_deps.bat
```

Wait for the two libraries to download (~1.5 MB total).

### 2. Build the Server
```cmd
cd ..
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

Build takes about 30 seconds.

## Running the Application

### Start the Server
```cmd
cd build\Release
calculator_server.exe
```

You should see:
```
AI Architect Calculators Server
================================
Server starting on http://localhost:8080
Press Ctrl+C to stop
```

### Open in Browser
Open your browser and go to:
```
http://localhost:8080
```

That's it! All five calculators are ready to use.

## Quick Test

Try the **GenAI ValueScore** calculator:
1. Scale: `8`
2. Frequency: `9`
3. ManualComplexity: `6`
4. Click "Calculate"
5. Result: `ValueScore: 12.00`

## Stopping the Server

Press `Ctrl+C` in the terminal where the server is running.

## Need Help?

See [README.md](README.md) for full documentation.
