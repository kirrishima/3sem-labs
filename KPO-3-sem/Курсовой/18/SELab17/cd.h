#pragma once
#include <string>
#include "LT.h"
#include "IT.h"

namespace CD
{
	void gen(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, const std::wstring& OUT_FILEPATH, bool);

	void __generate_math_expressions(const std::string& expr, std::ofstream& outFile);

	const std::string printAsmCode =
		"; Процедура для преобразования числа в строку и записи в buffer\n"
		"ConvertToString proc\n"
		"    mov ecx, 10                     ; Основание системы счисления (десятичная система)\n"
		"    mov edi, OFFSET buffer + 16     ; Указываем позицию для записи числа после \"result: \"\n"
		"convertLoop:\n"
		"    xor edx, edx                    ; Обнуляем edx перед делением\n"
		"    div ecx                         ; Делим eax на 10, результат деления в eax, остаток в edx\n"
		"    add dl, '0'                     ; Преобразуем остаток в ASCII\n"
		"    dec edi                         ; Переходим к предыдущей позиции буфера\n"
		"    mov [edi], dl                   ; Сохраняем символ в буфере\n"
		"    test eax, eax                   ; Проверяем, остались ли ещё цифры\n"
		"    jnz convertLoop                 ; Если остались, продолжаем цикл\n\n"
		"    ; Добавляем символ новой строки и символ конца строки после числа\n"
		"    mov byte ptr [OFFSET buffer + 17], 0Ah  ; Добавляем символ новой строки '\\n'\n"
		"    mov byte ptr [OFFSET buffer + 18], 0    ; Добавляем NULL в конец строки\n"
		"    ret\n"
		"ConvertToString endp\n\n"
		"; Процедура для вывода строки в консоль\n"
		"PrintToConsole proc\n"
		"    ; Получаем дескриптор консоли\n"
		"    push STD_OUTPUT_HANDLE\n"
		"    call GetStdHandle\n"
		"    mov ebx, eax                    ; Сохраняем дескриптор в ebx для использования в WriteConsoleA\n\n"
		"    ; Вычисляем длину всей строки\n"
		"    mov eax, OFFSET buffer + 18     ; Конец строки после новой строки и NULL\n"
		"    sub eax, OFFSET buffer          ; Вычисляем общую длину строки от начала \"result: \" до конца числа\n\n"
		"    ; Выводим строку в консоль\n"
		"    push 0                          ; lpOverlapped (NULL)\n"
		"    push OFFSET bytesWritten        ; lpNumberOfBytesWritten\n"
		"    push eax                        ; nNumberOfBytesToWrite\n"
		"    push OFFSET buffer              ; lpBuffer (начало всей строки \"result: \" + число)\n"
		"    push ebx                        ; hConsoleOutput (дескриптор консоли)\n"
		"    call WriteConsoleA              ; Вызываем WriteConsoleA для вывода строки\n"
		"    ret\n"
		"PrintToConsole endp\n";
}