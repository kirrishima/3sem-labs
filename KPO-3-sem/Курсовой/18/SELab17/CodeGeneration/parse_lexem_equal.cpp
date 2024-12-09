#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

vector<string> CD::CodeGeneration::parse_lexem_equal__(int& index_in_lex_table)
{
	std::vector<std::string> instructions_set;
	int idIndex = LEX_TABLE.table[index_in_lex_table - 1].idxTI;

	if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].iddatatype == IT::IDDATATYPE::STR
		&& ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype != IT::IDTYPE::F
		/*&& ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->len > 0*/)
	{
		if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::V) // если присваем значение переменной
		{
			if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
			{
				/*strcpy(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str,
					ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].value.vstr->str);*/
				instructions_set.push_back("mov eax, "
					+ get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI]));
				instructions_set.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
			}
			else
			{
				throw "ѕрисвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
					to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
			}
		}
		else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::L) // если присваем значеним литерала
		{
			//strcpy(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str, "offset ");
			//strcpy((ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str + 7),
			//	get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI]).c_str());
			instructions_set.push_back("lea eax, "
				+ get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI]));
			instructions_set.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
		}
		//else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::F)
		//{

		//}
		else
		{
			throw "ѕрисвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
				to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
		}
		return instructions_set;
	}
	else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::F)
	{
		std::string funcName = ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].id;
		int start = index_in_lex_table + 3;
		int end = start;

		while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
		{
			end = index_in_lex_table++;
		}

		auto function = find_if(user_functions.begin(), user_functions.end(),
			[&](const UserDefinedFunctions& func) {return func.name == funcName; });

		if (function == user_functions.end())
		{
			throw "parse_lexem_equal__: ѕопытка вызвать несуществующую функцию";
		}
		auto v = parse_function_call(*function, start, end);
		instructions_set.insert(instructions_set.end(), v.begin(), v.end());
		instructions_set.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
		return instructions_set;
	}

	vector<int> lexems;

	index_in_lex_table++;
	while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
		lexems.push_back(index_in_lex_table++);

	auto p = parse_expression(lexems, instructions_set);

	if (p.isCompare)
	{
		throw "parse_lexem_equal: присвоение логических значений не допускаетс€";
	}
	if (p.isSingleVariable)
	{
		instructions_set.push_back(format("mov eax, {}", p.resultStorage));
		instructions_set.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[lexems[0] - 2].idxTI]))); // -2 от литерала/id
	}
	else if (p.isMath)
	{
		instructions_set.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
	}

	return instructions_set;
}