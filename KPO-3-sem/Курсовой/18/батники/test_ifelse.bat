@echo off
setlocal enabledelayedexpansion
chcp 1251
:: �������� ���������� ����� ��� ������ ���������
program.exe > output.txt


:: ��������� ������
fc output.txt expected_output.txt > nul
if %errorlevel% equ 0 (
    echo ������ ���������.
) else (
    echo ������ �� ���������.
)

endlocal
