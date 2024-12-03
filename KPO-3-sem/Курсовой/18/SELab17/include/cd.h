#pragma once
#include <string>
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <format>
#include <stack>

using namespace std;

namespace CD
{
	struct CodeGeneration {
		std::string tab = "    ";

		IT::ID_Table ID_TABLE;
		LT::LexTable LEX_TABLE;
		std::ofstream OUT_ASM_FILE;

		static std::string __getIDnameInDataSegment(const IT::Entry& entry);

		CodeGeneration(const IT::ID_Table& ID_TABLE, const LT::LexTable& LEX_TABLE, const std::wstring& OUT_FILEPATH)
			: ID_TABLE(ID_TABLE), LEX_TABLE(LEX_TABLE), ifElseGeneration(*this)
		{
			this->OUT_ASM_FILE.open(OUT_FILEPATH);
		}

		std::vector<std::string> __generate_math_expressions(const std::string& expr);
		void __s_const();
		void __s_data();

		std::string lexem_to_source(LT::Entry& entry)
		{
			switch (entry.lexema[0]) {
			case 'p':
				return "print";

			case 'l':
				if (ID_TABLE.table[entry.idxTI].iddatatype == IT::IDDATATYPE::INT)
				{
					return to_string(ID_TABLE.table[entry.idxTI].value.vint);
				}
				throw "Литерал неизвестного типа данных";

			case 'i':
				return ID_TABLE.table[entry.idxTI].id;

			case 'c':
				return entry.c;

			case 'v':
			{
				std::string result = " ";
				result[0] = entry.v;
				return result;
			}

			case LEX_INTEGER:
				if (ID_TABLE.table[entry.idxTI].iddatatype == IT::IDDATATYPE::INT)
				{
					return "int";
				}
				throw "Идентификатор неизвестного типа данных";
			default: {
				std::string result = " ";
				result[0] = entry.lexema[0];
				return result;
			}
			}
		}

		std::vector<std::string> parse_expression(int& index_in_lex_table);

		void gen(const std::wstring& OUT_FILEPATH, bool);

		/// <summary>
		/// Генератор условных операторов
		/// </summary>
		struct IfElseGeneration {
			CodeGeneration& parent; // Ссылка на внешнюю структуру

			IfElseGeneration(CodeGeneration& p) : parent(p) {}

			int labelCounter = 0; // счетчик меток, используется дли уникального именования if-else меток
			int nestingLevel = 0; // уровень вложенности, используется для отступов

			std::stack<std::string> if_stack;

			void GenerateCondition(
				const std::vector<std::string>& operands, // два операнда - левый и правый 
				const std::string& comparison, // операция сравнения (>, <, ==, !=, >=, <=)
				const std::string& trueLabel, // имя метки если условие выполняется
				const std::string& falseLabel, // если не выполняется
				std::vector<std::string>& instructions // текущие инструкции
			);
			void StartIf(const std::vector<std::string>&, //2 операнда: левый и правый
				const string&,  // операция (>, <, ==, !=, >=, <=)
				std::vector<std::string>& // вектор с инструкциями, в которые будет добавлен сгенерированные новые
			);
			void StartElse(std::vector<std::string>& instructions);
			void EndIfOrElse(std::vector<std::string>& instructions);
			void EndExpression(std::vector<std::string>& instructions);

			std::vector<std::string> generateIfStatement(int& i);

			string GenerateLabel(const string& prefix, int n) {
				return prefix + "_" + to_string(n);
			}
		} ifElseGeneration;
	};




	const std::string printProcAsmCode =
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