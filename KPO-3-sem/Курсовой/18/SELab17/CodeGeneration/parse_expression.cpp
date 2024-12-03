#include "stdafx.h"
#include "cd.h"

using namespace std;

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

std::vector<std::string> CD::CodeGeneration::parse_expression(int& index_in_lex_table)
{
	std::vector<std::string> instructions_set;

	switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
	{
	case '=':
	{
		if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].iddatatype == IT::IDDATATYPE::STR
			/*&& ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->len > 0*/)
		{
			if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::V)
			{
				if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].idxfirstLE =
						ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idxfirstLE;
				}
				else
				{
					throw "Присвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
						to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
				}
			}
			else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::L)
			{
				ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].idxfirstLE = index_in_lex_table + 1;
			}
			else
			{
				throw "Присвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
					to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
			}
			//char* destIdValue = ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str;
			//char* srcIdValue = ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].value.vstr->str;

			//if (strcmp(destIdValue, srcIdValue) != 0)
			//{
			//	strcpy(destIdValue, srcIdValue);
			//}
			break;
		}
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
	{
		int idxIT = IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 2].idxTI]);
		if (idxIT < 0)
		{
			cout << "Встречен неопознанный индификатор в праметрах функции print. Имя "
				<< ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 2].idxTI].id
				<< " не было найдено в таблице индификаторов.";

			throw "Wrong id in print's params";
		}
		IT::Entry* id = &ID_TABLE.table[idxIT];

		//if (id->idtype == IT::IDTYPE::L)
		//{
		if (id->iddatatype == IT::IDDATATYPE::INT)
		{
			instructions_set.push_back("push " + __getIDnameInDataSegment(ID_TABLE.table[idxIT]));
			instructions_set.push_back("call __PrintNumber  ; Выводим число в консоль");
		}
		else if (id->iddatatype == IT::IDDATATYPE::STR)
		{
			std::string idName;
			if (id->idtype == IT::IDTYPE::L)
			{
				idName = __getIDnameInDataSegment(ID_TABLE.table[idxIT]);
			}
			else
			{
				idName = __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[id->idxfirstLE].idxTI]);
			}
			instructions_set.push_back("PrintArrayMACRO " + idName);
		}
		else
		{
			throw "Неожиданный тип литерала: " + to_string(id->iddatatype);
		}

	}

	break;
	default:
		break;
	}
	return instructions_set;
}