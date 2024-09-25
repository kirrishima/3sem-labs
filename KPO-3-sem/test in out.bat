@echo off
setlocal

set "OUT_DIR=test in out"
set "INPUT_FILE=input.txt"
set "EXE=SE_Lab14.exe"

if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"
copy "%~dp0%INPUT_FILE%" "%OUT_DIR%\%INPUT_FILE%"

cls
%EXE% -in:"%OUT_DIR%\%INPUT_FILE%" -out:"%OUT_DIR%\output.txt"

pause
endlocal
