#include "stdafx.h"
#include "cd.h"
#include "vector"
#include "sstream"
#include "filesystem"

using namespace std;

namespace CD
{
	void CodeGeneration::gen(const std::wstring& OUT_FILEPATH)
	{
		/*ofstream wfile(OUT_FILEPATH);*/

		if (!OUT_ASM_FILE.is_open() || OUT_ASM_FILE.fail() || OUT_ASM_FILE.bad()) {
			std::cerr << "Не удалось открыть файл\n";
			return;
		}

		OUT_ASM_FILE << BASE;
		OUT_ASM_FILE << '\n' << ".stack " << parm.stackSize;
		OUT_ASM_FILE << "\n\n";
		__s_const();
		OUT_ASM_FILE << '\n';
		__s_data();
		OUT_ASM_FILE << "\n\n.code";

		int count = 0;

		//OUT_ASM_FILE << "\nmain proc\nstart:\n";
		int start = 0;
		int end = LEX_TABLE.size;

		vector<pair<int, int>> functions;

		for (size_t i = 0; i < end; i++)
		{
			if (LEX_TABLE.table[i].lexema[0] == LEX_LEFTBRACE)
			{
				count++;
			}
			else if (LEX_TABLE.table[i].lexema[0] == LEX_BRACELET)
			{
				if (--count == 0)
				{

					functions.push_back({ start, i });
					start = i + 1;
				}
			}
		}

		for (const auto& p : functions)
		{
			parse_function(p.first, p.second);
		}

		int padding = 1;
		for (const auto& function : user_functions)
		{
			OUT_ASM_FILE << '\n';
			for (const auto& cl : function.second.code)
			{
				if (isMASMLabel(cl) || isFunctionEnd(cl) || isFunctionStart(cl))
				{
					OUT_ASM_FILE << cl << '\n';
					continue;
				}
				OUT_ASM_FILE << tab << cl << '\n';
			}
		}

	exit:
		OUT_ASM_FILE.flush();
		OUT_ASM_FILE.close();

#ifdef _DEBUG
		cout << "Compiling: \n\n";
		system("compile_debug.bat");
		system("program.exe");
#endif // _DEBUG

#ifdef _RELEASE
		system("compile_release.bat");
#endif // _RELEASE


	}
}