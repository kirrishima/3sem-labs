@echo off
setlocal

set "LOG_DIR=test in log"
set "INPUT_FILE=input.txt"
set "EXE=SELab17.exe"

if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
copy "%~dp0%INPUT_FILE%" "%LOG_DIR%\%INPUT_FILE%"

cls
%EXE% -in:"%LOG_DIR%\%INPUT_FILE%" -log:"%LOG_DIR%\logfile.log"

pause
endlocal
