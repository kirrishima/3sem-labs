@echo off
setlocal enabledelayedexpansion
chcp 1251
:: Создание временного файла для вывода программы
program.exe > output.txt


:: Сравнение файлов
fc output.txt expected_output.txt > nul
if %errorlevel% equ 0 (
    echo Тексты совпадают.
) else (
    echo Тексты не совпадают.
)

endlocal
