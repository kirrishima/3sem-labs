#include "stdafx.h"
#include "cd.h"
#include "vector"
#include "sstream"
#include "filesystem"

using namespace std;

namespace CD
{
	void CD::CodeGeneration::parse_function_body(UserDefinedFunctions& function, int start_index, int end_index)
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
				function.push_code("; конец условного");
				break;
			default:
				break;
			}
		}
	}

	void CD::CodeGeneration::parse_function(int start_index, int end_index)
	{
		UserDefinedFunctions function;
		vector<string> instrs;

		bool isMain = false;
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
				break;

			case LEX_MAIN:
				function.name = "main";
				isMain = true;
				break;

			case LEX_LEFTBRACE:
				f = true;
				break;
			default:
				break;
			}
		}

		parse_function_body(function, start_index, end_index);

		if (isMain)
		{
			function.push_code("push 0");
			function.push_code("call ExitProcess");
		}

		function.push_code(format("{} endp", function.name));

		if (isMain)
		{
			function.push_code("END main");
		}
		user_functions.push_back(function);
	}

	std::string operator*(const std::string& str, int times) {
		std::string new_str;
		for (int i = 0; i < times; i++)
		{
			new_str += str;
		}
		return new_str;
	}

	void CodeGeneration::gen(const std::wstring& OUT_FILEPATH)
	{
		/*ofstream wfile(OUT_FILEPATH);*/

		if (!OUT_ASM_FILE.is_open() || OUT_ASM_FILE.fail() || OUT_ASM_FILE.bad()) {
			std::cerr << "Не удалось открыть файл\n";
			return;
		}

		OUT_ASM_FILE << BASE;
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
			for (const auto& cl : function.code)
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
		//cout << "Compiling: \n\n";
		//system("compile_debug.bat");
#endif // _DEBUG

#ifdef _RELEASE
		system("compile_release.bat");
#endif // _RELEASE


	}
}