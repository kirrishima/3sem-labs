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
				throw "������� ������������ ���� ������";

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
				throw "������������� ������������ ���� ������";
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
		/// ��������� �������� ����������
		/// </summary>
		struct IfElseGeneration {
			CodeGeneration& parent; // ������ �� ������� ���������

			IfElseGeneration(CodeGeneration& p) : parent(p) {}

			int labelCounter = 0; // ������� �����, ������������ ��� ����������� ���������� if-else �����
			int nestingLevel = 0; // ������� �����������, ������������ ��� ��������

			std::stack<std::string> if_stack;

			void GenerateCondition(
				const std::vector<std::string>& operands, // ��� �������� - ����� � ������ 
				const std::string& comparison, // �������� ��������� (>, <, ==, !=, >=, <=)
				const std::string& trueLabel, // ��� ����� ���� ������� �����������
				const std::string& falseLabel, // ���� �� �����������
				std::vector<std::string>& instructions // ������� ����������
			);
			void StartIf(const std::vector<std::string>&, //2 ��������: ����� � ������
				const string&,  // �������� (>, <, ==, !=, >=, <=)
				std::vector<std::string>& // ������ � ������������, � ������� ����� �������� ��������������� �����
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
		"; ��������� ��� �������������� ����� � ������ � ������ � buffer\n"
		"ConvertToString proc\n"
		"    mov ecx, 10                     ; ��������� ������� ��������� (���������� �������)\n"
		"    mov edi, OFFSET buffer + 16     ; ��������� ������� ��� ������ ����� ����� \"result: \"\n"
		"convertLoop:\n"
		"    xor edx, edx                    ; �������� edx ����� ��������\n"
		"    div ecx                         ; ����� eax �� 10, ��������� ������� � eax, ������� � edx\n"
		"    add dl, '0'                     ; ����������� ������� � ASCII\n"
		"    dec edi                         ; ��������� � ���������� ������� ������\n"
		"    mov [edi], dl                   ; ��������� ������ � ������\n"
		"    test eax, eax                   ; ���������, �������� �� ��� �����\n"
		"    jnz convertLoop                 ; ���� ��������, ���������� ����\n\n"
		"    ; ��������� ������ ����� ������ � ������ ����� ������ ����� �����\n"
		"    mov byte ptr [OFFSET buffer + 17], 0Ah  ; ��������� ������ ����� ������ '\\n'\n"
		"    mov byte ptr [OFFSET buffer + 18], 0    ; ��������� NULL � ����� ������\n"
		"    ret\n"
		"ConvertToString endp\n\n"
		"; ��������� ��� ������ ������ � �������\n"
		"PrintToConsole proc\n"
		"    ; �������� ���������� �������\n"
		"    push STD_OUTPUT_HANDLE\n"
		"    call GetStdHandle\n"
		"    mov ebx, eax                    ; ��������� ���������� � ebx ��� ������������� � WriteConsoleA\n\n"
		"    ; ��������� ����� ���� ������\n"
		"    mov eax, OFFSET buffer + 18     ; ����� ������ ����� ����� ������ � NULL\n"
		"    sub eax, OFFSET buffer          ; ��������� ����� ����� ������ �� ������ \"result: \" �� ����� �����\n\n"
		"    ; ������� ������ � �������\n"
		"    push 0                          ; lpOverlapped (NULL)\n"
		"    push OFFSET bytesWritten        ; lpNumberOfBytesWritten\n"
		"    push eax                        ; nNumberOfBytesToWrite\n"
		"    push OFFSET buffer              ; lpBuffer (������ ���� ������ \"result: \" + �����)\n"
		"    push ebx                        ; hConsoleOutput (���������� �������)\n"
		"    call WriteConsoleA              ; �������� WriteConsoleA ��� ������ ������\n"
		"    ret\n"
		"PrintToConsole endp\n";
}