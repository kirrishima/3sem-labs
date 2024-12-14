#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

void CD::CodeGeneration::parse_lexem_equal__(std::vector<std::string>& result_instructions, int& index_in_lex_table, int tabsize)
{
	vector<int> ids;
	string destName = get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI]);

	index_in_lex_table++;
	while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
		ids.push_back(index_in_lex_table++);

	auto p = parse_expression(ids, result_instructions, tabsize);

	if (p.isCompare)
	{
		throw "parse_lexem_equal: присвоение логических значений не допускается";
	}
	if (p.isSingleVariable && !p.isFunctionCall)
	{
		if (p.isINT)
		{
			result_instructions.push_back(format("{}mov ax, {}", tab * tabsize, p.resultStorage));
			result_instructions.push_back(format("{}mov {}, ax", tab * tabsize, destName)); // -2 от литерала/id
		}
		else if(p.isSTR)
		{
			result_instructions.push_back(format("{}mov eax, {}", tab * tabsize, p.resultStorage));
			result_instructions.push_back(format("{}mov {}, eax", tab * tabsize, destName)); // -2 от литерала/id
		}
		else if (p.isCHAR)
		{
			result_instructions.push_back(format("{}mov al, {}", tab * tabsize, p.resultStorage));
			result_instructions.push_back(format("{}mov {}, al", tab * tabsize, destName)); // -2 от литерала/id
		}
	}
	else if (p.isResultInEAX)
	{
		if (p.isSTR)
		{
			result_instructions.push_back(format("{}mov {}, eax", tab * tabsize, destName));
		}
		else
		{
			result_instructions.push_back(format("{}mov {}, ax", tab * tabsize, destName));
		}
	}
}