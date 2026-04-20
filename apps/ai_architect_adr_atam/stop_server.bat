@echo off
setlocal
REM Stops any running adr_atam_server.exe. Primary stop mechanism is Ctrl+C
REM in the window running start_server.bat; this script is a fallback for
REM backgrounded or orphaned processes.
taskkill /IM adr_atam_server.exe /F >nul 2>&1
if %errorlevel% equ 0 (
    echo Stopped adr_atam_server.exe.
) else (
    echo No running adr_atam_server.exe found.
)
endlocal
