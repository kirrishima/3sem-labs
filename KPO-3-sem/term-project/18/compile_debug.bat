@echo off
:: Установка окружения Visual Studio
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

:: Компиляция с генерацией отладочных символов
ml /c /nologo /Zi /Fo program.obj input.txt.asm
if errorlevel 1 (
    echo Compilation failed. Check your ASM code for errors.
    exit /b 1
)

:: Линковка с генерацией PDB (отладочный файл)
link /nologo /DEBUG /subsystem:console program.obj GMSStandardLib.lib libucrt.lib libcmt.lib libvcruntime.lib kernel32.lib /NODEFAULTLIB:libcmtd /NODEFAULTLIB:MSVCRTD /PDB:program.pdb
if errorlevel 1 (
    echo Linking failed. Check library paths and compatibility.
    exit /b 1
)

exit /b 0
