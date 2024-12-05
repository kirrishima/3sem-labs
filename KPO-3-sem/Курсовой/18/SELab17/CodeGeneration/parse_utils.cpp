#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

std::string CD::CodeGeneration::lexems_vector_to_string(const vector<int>& ids)
{
	string result = "";
	for (size_t i = 0; i < ids.size(); i++)
	{
		int lexId = ids[i];

		switch (LEX_TABLE.table[lexId].lexema[0])
		{
		case LEX_ID:
		case LEX_LITERAL:
			result += get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[lexId].idxTI]);
			break;
		case LEX_MATH:
			result += LEX_TABLE.table[lexId].v;
			break;
		case LEX_COMPARE:
			result += LEX_TABLE.table[lexId].c;
			break;
		default:
			result += LEX_TABLE.table[lexId].lexema[0];
			break;
		}
	}
	return result;
}

std::string CD::CodeGeneration::get_string_value(const int lex_id)
{
	int idxIT = IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[lex_id].idxTI]);
	IT::Entry* id = &ID_TABLE.table[idxIT];

	if (id->iddatatype == IT::IDDATATYPE::STR)
	{
		std::string idName;
		if (id->idtype == IT::IDTYPE::L)
		{
			idName = get_id_name_in_data_segment(ID_TABLE.table[idxIT]);
		}
		else
		{
			idName = get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[id->idxfirstLE].idxTI]);
		}
		return idName;
	}
	else
	{
		throw "Ќеожиданный тип при определении значени€ строковой лексемы <"
			+ to_string(lex_id) + ">: " + to_string(id->iddatatype);
	}
}

// = значение или = результат_выражени€
bool CD::is_assignment(const std::string& expr) {
	constexpr char operators[] = "+-*/";
	for (char op : operators) {
		if (expr.find(op) != std::string::npos) {
			return false;
		}
	}
	return true;
}