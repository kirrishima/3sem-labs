#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

void CD::CodeGeneration::parse_lexem_equal__(std::vector<std::string>& result_instructions, int& index_in_lex_table)
{
	vector<int> ids;
	string destName = get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI]);

	index_in_lex_table++;
	while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
		ids.push_back(index_in_lex_table++);

	auto p = parse_expression(ids, result_instructions);

	if (p.isCompare)
	{
		throw "parse_lexem_equal: присвоение логических значений не допускаетс€";
	}
	if (p.isSingleVariable && !p.isFunctionCall)
	{
		result_instructions.push_back(format("mov eax, {}", p.resultStorage));
		result_instructions.push_back(format("mov {}, eax", destName)); // -2 от литерала/id
	}
	else if (p.isResultInEAX)
	{
		result_instructions.push_back(format("mov {}, eax", destName));
	}
	//else if()
	//{

	//}

	//int idIndex = LEX_TABLE.table[index_in_lex_table - 1].idxTI;

	//if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].iddatatype == IT::IDDATATYPE::STR
	//	&& ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype != IT::IDTYPE::F
	//	/*&& ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->len > 0*/)
	//{
	//	if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::V) // если присваем значение переменной
	//	{
	//		if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
	//		{
	//			/*strcpy(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str,
	//				ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].value.vstr->str);*/
	//			result_instructions.push_back("mov eax, "
	//				+ get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI]));
	//			result_instructions.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
	//		}
	//		else
	//		{
	//			throw "ѕрисвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
	//				to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
	//		}
	//	}
	//	else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::L) // если присваем значеним литерала
	//	{
	//		//strcpy(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str, "offset ");
	//		//strcpy((ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str + 7),
	//		//	get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI]).c_str());
	//		result_instructions.push_back("lea eax, "
	//			+ get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI]));
	//		result_instructions.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
	//	}
	//	//else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::F)
	//	//{

	//	//}
	//	else
	//	{
	//		throw "ѕрисвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
	//			to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
	//	}
	//	return result_instructions;
	//}
	//else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::F)
	//{
	//	std::string funcName = ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].id;
	//	
	//	stack<char> skobki;
	//	skobki.push(LEX_LEFTTHESIS);
	//	int start = index_in_lex_table + 3;
	//	int end = start;

	//	bool f = false;
	//	while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON && !f)
	//	{
	//		switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
	//		{
	//		case LEX_RIGHTTHESIS:
	//			skobki.pop();
	//			if ( skobki.size() == 0)
	//			{
	//				f = true;
	//			}
	//			break;
	//		case LEX_LEFTTHESIS:
	//			skobki.push(LEX_LEFTTHESIS);
	//		default:
	//			end = index_in_lex_table++;
	//			break;
	//		}
	//	}

	//	auto function = find_if(user_functions.begin(), user_functions.end(),
	//		[&](const UserDefinedFunctions& func) {return func.name == funcName; });

	//	if (function == user_functions.end())
	//	{
	//		throw "parse_lexem_equal__: ѕопытка вызвать несуществующую функцию";
	//	}
	//	auto v = parse_function_call(*function, start, end);
	//	result_instructions.insert(result_instructions.end(), v.begin(), v.end());
	//	result_instructions.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
	//	return result_instructions;
	//}

	//vector<int> lexems;

	//index_in_lex_table++;
	//while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
	//	lexems.push_back(index_in_lex_table++);

	//auto p = parse_expression(lexems, result_instructions);

	//if (p.isCompare)
	//{
	//	throw "parse_lexem_equal: присвоение логических значений не допускаетс€";
	//}
	//if (p.isSingleVariable)
	//{
	//	result_instructions.push_back(format("mov eax, {}", p.resultStorage));
	//	result_instructions.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[lexems[0] - 2].idxTI]))); // -2 от литерала/id
	//}
	//else if (p.isMath)
	//{
	//	result_instructions.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[idIndex])));
	//}

	//return result_instructions;
}