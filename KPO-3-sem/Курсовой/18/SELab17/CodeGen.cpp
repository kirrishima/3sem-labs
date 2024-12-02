#include "stdafx.h"
#include "cd.h"
#include "vector"
#include "sstream"
#include "filesystem"

using namespace std;

namespace CD
{
	// = значение или = результат_выражения
	bool isAssignment(const std::string& expr) {
		constexpr char operators[] = "+-*/";
		for (char op : operators) {
			if (expr.find(op) != std::string::npos) {
				return false;
			}
		}
		return true;
	}

	std::vector<std::string> CodeGeneration::parse_expression(int& index_in_lex_table)
	{
		std::vector<std::string> instructions_set;

		switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
		{
		case '=':
		{
			std::string var = __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table].idxTI]);
			std::string token = "";
			std::string expr = "";

			index_in_lex_table++;
			while (index_in_lex_table < LEX_TABLE.size && LEX_TABLE.table[index_in_lex_table].lexema[0] != ';')
			{
				if (LEX_TABLE.table[index_in_lex_table].lexema[0] == '(' || LEX_TABLE.table[index_in_lex_table].lexema[0] == ')')
				{
					expr += LEX_TABLE.table[index_in_lex_table].lexema[0];
				}
				else if (LEX_TABLE.table[index_in_lex_table].lexema[0] == 'v')
				{
					expr += LEX_TABLE.table[index_in_lex_table].v;
				}
				else
				{
					expr += __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table].idxTI]);
				}
				index_in_lex_table++;
			}

			if (!isAssignment(expr))
			{
				auto a = __generate_math_expressions(expr);
				for (const std::string& str : a)
				{
					instructions_set.push_back(str);
				}
				instructions_set.push_back("mov " + var + ", eax");// а есть ли в этом смысл?
			}
			else
			{
				instructions_set.push_back("mov eax," + expr);
				instructions_set.push_back("mov " + var + ", eax");
			}
			instructions_set.push_back("\n");


			break;
		}
		case 'p':

			if (IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 2].idxTI]) < 0)
			{
				cout << "Встречен неопознанный индификатор п праметрах функции print. Имя "
					<< ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 2].idxTI].id
					<< " не было найдено в таблице индификаторов.";

				throw "Wrong id in print's params";
			}
			instructions_set.push_back("mov eax, " + __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 2].idxTI]));

			instructions_set.push_back("call ConvertToString            ; Преобразуем число в строку");
			instructions_set.push_back("call PrintToConsole             ; Выводим строку в консоль");
			break;
		default:
			break;
		}
		return instructions_set;
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
		OUT_ASM_FILE << "includelib kernel32.lib\n";
		OUT_ASM_FILE << "ExitProcess PROTO : DWORD\n";
		OUT_ASM_FILE << "WriteConsoleA PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD\n";
		OUT_ASM_FILE << "GetStdHandle PROTO :DWORD\n";
		OUT_ASM_FILE << "\nSTD_OUTPUT_HANDLE EQU -11  ; Дескриптор для стандартного вывода консоли\n";
		OUT_ASM_FILE << "\n.stack 4096\n\n";

		__s_const();
		OUT_ASM_FILE << '\n';
		__s_data();
		OUT_ASM_FILE << "\n\n.code\n";

		if (p)
		{
			cout << "Создается код для print...\n";
			OUT_ASM_FILE << printProcAsmCode;
		}

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
				generateIfStatement(i);
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