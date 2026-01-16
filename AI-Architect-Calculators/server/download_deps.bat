@echo off
echo Downloading dependencies...

powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h' -OutFile 'httplib.h'"
if %errorlevel% neq 0 (
    echo Failed to download httplib.h
    exit /b 1
)

powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp' -OutFile 'json.hpp'"
if %errorlevel% neq 0 (
    echo Failed to download json.hpp
    exit /b 1
)

echo Dependencies downloaded successfully!
