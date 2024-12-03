#include "stdafx.h"
#include "cd.h"
#include "vector"
#include "sstream"
#include "filesystem"

using namespace std;

namespace CD
{
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
		OUT_ASM_FILE << "__PrintArray PROTO :SDWORD, :SDWORD, :SDWORD\n";
		OUT_ASM_FILE << "\n.stack 4096\n\n";

		OUT_ASM_FILE << R"(PrintArrayMACRO MACRO arrName
    LOCAL arrType, arrLength, arrOffset
    push type arrName     ; Тип элементов массива
    push lengthof arrName ; Длина массива
    push offset arrName   ; Смещение массива
    call __PrintArray     ; Вызов процедуры __PrintArray
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


		OUT_ASM_FILE << "\nmain proc\nstart:\n";
		for (int i = 0; i < LEX_TABLE.size; i++)
		{
			switch (LEX_TABLE.table[i].lexema[0])
			{
			case 'p':
			case '=':
			{
				auto res = parse_expression(i);
				for (const std::string& s : res)
				{
					OUT_ASM_FILE << '\t' << s << '\n';
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
				for (const std::string& str : ifElseGeneration.generateIfStatement(i))
				{
					OUT_ASM_FILE << str << '\n';
				}
				OUT_ASM_FILE << "\n; закончились условки\n";
				break;
			default:
				break;
			}
		}
	exit:
		OUT_ASM_FILE << "\tpush 0\n" << "\tcall ExitProcess\n" << "main ENDP" << endl;
		OUT_ASM_FILE << "END main";
		OUT_ASM_FILE.flush();
		OUT_ASM_FILE.close();

		cout << "Compiling: \n\n";

#ifdef _DEBUG
		system("compile_debug.bat");
#endif // _DEBUG

#ifdef _RELEASE
		system("compile_release.bat");
#endif // _RELEASE


	}
}