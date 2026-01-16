@echo off
title AI Architect Calculators Server

if not exist "build\Release\calculator_server.exe" (
    echo ERROR: Server not built yet!
    echo.
    echo Please build the project first:
    echo   1. cd server
    echo   2. download_deps.bat
    echo   3. cd ..
    echo   4. mkdir build
    echo   5. cd build
    echo   6. cmake ..
    echo   7. cmake --build . --config Release
    echo.
    echo See QUICKSTART.md for details.
    pause
    exit /b 1
)

cd build\Release
echo Starting AI Architect Calculators Server...
echo.
echo Once the server starts, open your browser to:
echo   http://localhost:8080
echo.
echo Press Ctrl+C to stop the server
echo.
calculator_server.exe
