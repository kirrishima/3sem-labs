#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;


void CD::CodeGeneration::parse_print_lexem__(std::vector<std::string>& result_instructions, int& index_in_lex_table, int tabsize)
{
	if (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_PRINT)
	{
		throw "__parse_print_expression_: передан индекс лексемы, которая не равна LEX_PRINT";
	}
	vector<int> lexems;

	index_in_lex_table += 2;

	while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
	{
		lexems.push_back(index_in_lex_table++);
	};
	lexems.pop_back();

	auto p = parse_expression(lexems, result_instructions, tabsize);

	if (p.isResultInDefaultBool)
	{
		result_instructions.push_back(format("{}movzx eax, {}", tab * tabsize, reservedBoolName));
		result_instructions.push_back(tab * tabsize + "push eax");
		result_instructions.push_back(tab * tabsize + "call __PrintBool");
	}
	else if (p.isMath)
	{
		if (p.isResultInEAX)
		{
			result_instructions.push_back(tab * tabsize + "push eax");
			result_instructions.push_back(tab * tabsize + "call __PrintNumber");
		}
		else
		{
			result_instructions.push_back(format("{}push {}", tab * tabsize, p.resultStorage));
			result_instructions.push_back(tab * tabsize + "call __PrintNumber");
		}
	}
	else if (p.isSTR)
	{
		result_instructions.push_back(format("{}push {}", tab * tabsize, p.resultStorage));
		result_instructions.push_back(tab * tabsize + "call __Print");
	}
	else if (p.isINT)
	{
		result_instructions.push_back(format("{}push {}", tab * tabsize, p.resultStorage));
		result_instructions.push_back(tab * tabsize + "call __PrintNumber");
	}
}
