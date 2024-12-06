@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

ml /c /nologo /Fo program.obj input.txt.asm
if errorlevel 1 exit /b 1

link /nologo /subsystem:console program.obj GMSStandardLib.lib libucrt.lib libcmt.lib libvcruntime.lib kernel32.lib /NODEFAULTLIB:libcmtd /NODEFAULTLIB:MSVCRTD
if errorlevel 1 exit /b 1
