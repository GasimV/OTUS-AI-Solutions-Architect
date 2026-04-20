@echo off
setlocal
pushd "%~dp0"
if exist "C:\msys64\ucrt64\bin" (
    set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"
)
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
