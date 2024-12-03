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

		//system("compile.bat");
	}
}