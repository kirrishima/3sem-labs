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
			case '=':
			{
				std::string var = __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[i].idxTI]);
				std::string token = "";
				std::string expr = "";

				i++;
				while (i < LEX_TABLE.size && LEX_TABLE.table[i].lexema[0] != ';')
				{
					if (LEX_TABLE.table[i].lexema[0] == '(' || LEX_TABLE.table[i].lexema[0] == ')')
					{
						expr += LEX_TABLE.table[i].lexema[0];
					}
					else if (LEX_TABLE.table[i].lexema[0] == 'v')
					{
						expr += LEX_TABLE.table[i].v;
					}
					else
					{
						expr += __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[i].idxTI]);
					}
					i++;
				}

				if (!isAssignment(expr))
				{
					__generate_math_expressions(expr);
					OUT_ASM_FILE << "\tmov " << var << ", eax\n";// а есть ли в этом смысл?
				}
				else
				{
					OUT_ASM_FILE << "\tmov " << var << std::format(", {}\n", expr);
				}
				OUT_ASM_FILE << '\n';


				break;
			}
			case 'p':

				if (IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[i + 2].idxTI]) < 0)
				{
					cout << "Встречен неопознанный индификатор п праметрах функции print. Имя "
						<< ID_TABLE.table[LEX_TABLE.table[i + 2].idxTI].id
						<< " не было найдено в таблице индификаторов.";

					throw "Wrong id in print's params";
				}
				OUT_ASM_FILE << "\tmov eax, " << __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[i + 2].idxTI]) << '\n';

				OUT_ASM_FILE << "\tcall ConvertToString            ; Преобразуем число в строку\n"
					<< "\tcall PrintToConsole             ; Выводим строку в консоль\n\n";
				break;
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
				OUT_ASM_FILE.close();
				exit(0);
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