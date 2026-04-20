@echo off
setlocal
pushd "%~dp0"
if not exist "build\adr_atam_server.exe" (
    echo Binary not found. Build first:
    echo   cmake -S . -B build
    echo   cmake --build build --config Release
    popd
    exit /b 1
)
"build\adr_atam_server.exe" %*
popd
endlocal
