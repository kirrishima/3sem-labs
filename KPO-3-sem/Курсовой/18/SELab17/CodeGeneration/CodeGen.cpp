#include "stdafx.h"
#include "cd.h"
#include "vector"
#include "sstream"
#include "filesystem"

using namespace std;

namespace CD
{
	vector<string> CD::CodeGeneration::parse_function_body(UserDefinedFunctions& function, int start_index, int end_index)
	{
		function.push_code(format("{} proc", function.name));
		function.push_code("start:");
		for (int i = start_index; i < end_index; i++)
		{
			switch (LEX_TABLE.table[i].lexema[0])
			{
			case 'p':
			case '=':
			{
				auto res = parse_lexem(i);
				for (const std::string& s : res)
				{
					function.push_code(s);
				}
				break;
			}
			case ';':
				break;
			case '{':
			case ')':
			case '(':
				break;
			case '}':
				break;
			case '?':
				for (const std::string& str : ifElseGen.generate_if_statement(i))
				{
					function.push_code(str);
				}
				function.push_code( "; конец условного");
				break;
			default:
				break;
			}
		}
		function.push_code(format("{} endp", function.name));
	}

	vector<string> CD::CodeGeneration::parse_function(int start_index, int end_index)
	{
		UserDefinedFunctions function;
		vector<string> instrs;

		bool f = false;
		for (start_index; start_index < end_index && !f; start_index++)
		{
			switch (LEX_TABLE.table[start_index].lexema[0]) {
			case LEX_ID:
				switch (ID_TABLE.table[LEX_TABLE.table[start_index].idxTI].idtype)
				{
				case IT::IDTYPE::F:
					function.name = string(ID_TABLE.table[LEX_TABLE.table[start_index].idxTI].id);
					break;

				case IT::IDTYPE::P:
					function.push_params(ID_TABLE.table[LEX_TABLE.table[start_index].idxTI].iddatatype);
					break;
				default:
					break;
				}
			case LEX_LEFTBRACE:
				f = true;
				break;
			default:
				break;
			}
		}

		parse_function_body(function, start_index, end_index);
	}

	void CodeGeneration::gen(const std::wstring& OUT_FILEPATH, bool p)
	{
		/*ofstream wfile(OUT_FILEPATH);*/

		if (!OUT_ASM_FILE.is_open() || OUT_ASM_FILE.fail() || OUT_ASM_FILE.bad()) {
			std::cerr << "Не удалось открыть файл\n";
			return;
		}

		OUT_ASM_FILE << ".586\n";
		OUT_ASM_FILE << ".model flat, stdcall\n";
		OUT_ASM_FILE << "ExitProcess PROTO : DWORD\n";
		OUT_ASM_FILE << "__PrintNumber PROTO :SDWORD\n";
		OUT_ASM_FILE << "__PrintBool PROTO :BYTE\n";
		OUT_ASM_FILE << "__PrintArray PROTO :DWORD, :DWORD, :DWORD\n";
		OUT_ASM_FILE << "__StrCmp PROTO :DWORD, :DWORD\n";
		OUT_ASM_FILE << "\n.stack 4096\n\n";

		OUT_ASM_FILE << R"(PrintArrayMACRO MACRO arrName
    LOCAL arrType, arrLength, arrOffset
    push type arrName     ; Тип элементов массива
    push lengthof arrName ; Длина массива
    push offset arrName   ; Смещение массива
    call __PrintArray     ; Вызов процедуры __PrintArray
ENDM)" << "\n\n";

		OUT_ASM_FILE << R"(StrCmpCallMACRO MACRO str1, str2
    push OFFSET str2   ; Адрес второй строки в стек
    push OFFSET str1   ; Адрес первой строки в стек
    call __StrCmp      ; Вызов функции __StrCmp
ENDM)" << "\n\n";

		__s_const();
		OUT_ASM_FILE << '\n';
		__s_data();
		OUT_ASM_FILE << "\n\n.code\n";

#ifdef _LEGACY_CODE
		if (p)
		{
			cout << "Создается код для print...\n";
			OUT_ASM_FILE << printProcAsmCode;
		}
#endif // _LEGACY_CODE

		int count = 0;

		OUT_ASM_FILE << "\nmain proc\nstart:\n";
		int start = 0;
		int end = LEX_TABLE.size;

		for (size_t i = start; i < end; i++)
		{
			if (LEX_TABLE.table[i].lexema[0]==LEX_LEFTBRACE)
			{
				count++;
			}
			else if(LEX_TABLE.table[i].lexema[0] == LEX_BRACELET)
			{
				if (--count == 0)
				{
					break;
				}
			}
		}

		
	exit:
		OUT_ASM_FILE << "\tpush 0\n" << "\tcall ExitProcess\n" << "main ENDP" << endl;
		OUT_ASM_FILE << "END main";
		OUT_ASM_FILE.flush();
		OUT_ASM_FILE.close();

#ifdef _DEBUG
		cout << "Compiling: \n\n";
		system("compile_debug.bat");
#endif // _DEBUG

#ifdef _RELEASE
		system("compile_release.bat");
#endif // _RELEASE


	}
}