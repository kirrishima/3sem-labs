#include "stdafx.h"
#include "cd.h"
#include "vector"
#include "sstream"

using namespace std;

namespace CD
{
	void __s_const(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, std::ofstream& OUT_FILE);
	void __s_data(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, std::ofstream& OUT_FILE);

	void generateAssembly(const std::string& expr, std::ofstream& outFile);

	bool isAssignment(const std::string& expr) {
		return expr.find('+') == std::string::npos && expr.find('-') == std::string::npos;
	}

	void gen(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, const std::wstring& OUT_FILEPATH, bool p)
	{
		ofstream wfile(OUT_FILEPATH);

		if (!wfile.is_open() || wfile.fail() || wfile.bad()) {
			std::cerr << "Не удалось открыть файл\n";
			return;
		}
		wfile << ".586\n";
		wfile << ".model flat, stdcall\n";
		wfile << "includelib kernel32.lib\n";
		wfile << "ExitProcess PROTO : DWORD\n";
		wfile << "WriteConsoleA PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD\n";
		wfile << "GetStdHandle PROTO :DWORD\n";
		wfile << "\nSTD_OUTPUT_HANDLE EQU -11  ; Дескриптор для стандартного вывода консоли\n";
		wfile << "\n.stack 4096\n\n";

		__s_const(LEX_TABLE, ID_TABLE, wfile);
		wfile << '\n';
		__s_data(LEX_TABLE, ID_TABLE, wfile);
		wfile << "\n\n.code\n";

		if (p)
		{
			wfile << printAsmCode;
		}

		wfile << "\nmain proc\nstart:\n";
		for (size_t i = 0; i < LEX_TABLE.size; i++)
		{
			switch (LEX_TABLE.table[i].lexema[0])
			{
			case '=':
			{
				std::string var = ID_TABLE.table[LEX_TABLE.table[i].idxTI].id;
				std::string token = "";
				std::string expr = "";

				i++;
				while (i < LEX_TABLE.size && LEX_TABLE.table[i].lexema[0] != ';')
				{
					if (LEX_TABLE.table[i].lexema[0] == 'v')
					{
						expr += LEX_TABLE.table[i].v;
					}
					else
					{
						if (ID_TABLE.table[LEX_TABLE.table[i].idxTI].idtype == IT::L)
						{
							expr += "__";
						}
						expr += ID_TABLE.table[LEX_TABLE.table[i].idxTI].id;
					}
					i++;
				}

				if (!isAssignment(expr))
				{
					generateAssembly(expr, wfile);

					wfile << "\tmov [" << var << "], eax\n";
				}
				else
				{
					wfile << "\tmov eax, " << expr << '\n';
					wfile << "\tmov [" << var << "], " << "eax" << '\n';
				}
				wfile << '\n';

				break;
			}
			//case 'l':
			//{
			//	auto l = ID_TABLE.table[LEX_TABLE.table[i].idxTI];
			//	wfile << l.value.vint;
			//	break;
			//}
			case 'p':
				if (ID_TABLE.table[LEX_TABLE.table[i + 2].idxTI].idtype == IT::L)
				{
					wfile << "\tmov eax, __" << ID_TABLE.table[LEX_TABLE.table[i + 2].idxTI].id << "\n";
				}
				else
				{
					wfile << "\tmov eax, [" << ID_TABLE.table[LEX_TABLE.table[i + 2].idxTI].id << "]\n";
				}
				wfile << "\tcall ConvertToString            ; Преобразуем число в строку\n"
					<< "\tcall PrintToConsole             ; Выводим строку в консоль\n\n";
				break;
			case ';':
				//wfile << ";\n\t";
				break;
			case '{':
			case ')':
			case '(':
				break;
			case '}':
				//wfile << "\n\treturn 0;\n}";
				break;
			default:
				//wfile << LEX_TABLE.table[i].lexema[0];
				break;
			}
		}
	exit:
		wfile << "\tpush 0\n" << "\tcall ExitProcess\n" << "main ENDP" << "\nend main";
	}

	void __s_const(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, std::ofstream& OUT_FILE)
	{
		OUT_FILE << ".const\n";
		for (size_t i = 0; i < ID_TABLE.size; i++)
		{
			if (ID_TABLE.table[i].idtype == IT::L)
			{
				if (ID_TABLE.table[i].iddatatype == IT::INT)
				{
					OUT_FILE << "\t__" << ID_TABLE.table[i].id << " SDWORD " << ID_TABLE.table[i].value.vint << '\n';
				}
			}
		}
	}

	void __s_data(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, std::ofstream& OUT_FILE)
	{
		OUT_FILE << ".data\n"
			<< "buffer db \"result: \", 10 DUP(0)  ; Буфер для сообщения с результатом, включая место для числа\n"
			<< "bytesWritten DWORD 0             ; Переменная для записи количества байт\n";

		for (size_t i = 0; i < ID_TABLE.size; i++)
		{
			auto entry = &ID_TABLE.table[i];
			if (entry->idtype == IT::V)
			{
				if (entry->iddatatype == IT::INT)
				{
					OUT_FILE << "\t"
						<< entry->id
						<< " SDWORD "
						<< "0\n";
				}
			}
		}
	}

	void generateAssembly(const std::string& expr, std::ofstream& outFile)
	{
		outFile << "\t; original expression:  " << expr << "\n";

		std::string variable;
		char op = '+';
		bool isFirst = true;

		for (size_t i = 0; i < expr.size(); ++i) {
			if (isalpha(expr[i]) || expr[i] == '_' || isdigit(expr[i])) {
				// Собираем название переменной, состоящее из одного или более символов
				variable += expr[i];
			}
			else if (expr[i] == '+' || expr[i] == '-') {
				// Записываем команду для предыдущей переменной
				if (!variable.empty()) {
					bool isConst = variable.size() > 2 && variable[0] == '_' && variable[1] == '_';

					if (isFirst) {
						outFile << "\tmov eax, " << (isConst ? variable : "[" + variable + "]") << "\n";
						isFirst = false;
					}
					else {
						outFile << (op == '+' ? "\tadd eax, " : "\tsub eax, ")
							<< (isConst ? variable : "[" + variable + "]") << "\n";
					}
					variable.clear();
				}
				op = expr[i];  // Сохраняем оператор для следующей итерации
			}
		}

		// Записываем последнюю переменную
		if (!variable.empty()) {
			bool isConst = variable.size() > 2 && variable[0] == '_' && variable[1] == '_';
			outFile << (op == '+' ? "\tadd eax, " : "\tsub eax, ")
				<< (isConst ? variable : "[" + variable + "]") << "\n";
		}
	}
}
