#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

void CD::CodeGeneration::parse_lexem(std::vector<std::string>& result_instructions, int& index_in_lex_table)
{
	switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
	{
	case '=':
	{
		parse_lexem_equal__(result_instructions, index_in_lex_table);
		break;
	}
	case 'p':
	{
		parse_print_lexem__(result_instructions, index_in_lex_table);
		break;
	}

	case LEX_ID:
		if (LEX_TABLE.table[index_in_lex_table + 1].lexema[0] == LEX_LEFTTHESIS)
		{
			std::string name = ID_TABLE.table[LEX_TABLE.table[index_in_lex_table].idxTI].id;
			index_in_lex_table += 2;
			std::vector<int> ids;

			stack<char> parenthesis;
			parenthesis.push(LEX_LEFTTHESIS);

			while (index_in_lex_table < LEX_TABLE.size)
			{
				if (LEX_TABLE.table[index_in_lex_table].lexema[0] == LEX_RIGHTTHESIS)
				{
					parenthesis.pop();
					if (parenthesis.size() == 0)
					{
						break;
					}
				}
				if (LEX_TABLE.table[index_in_lex_table].lexema[0] == LEX_LEFTTHESIS)
				{
					parenthesis.push(index_in_lex_table);
				}

				ids.push_back(index_in_lex_table++);
			}
			auto v = parse_function_call(user_functions[name], ids[0], ids.back());
			result_instructions.insert(result_instructions.end(), v.begin(), v.end());
		}
		break;
	case LEX_RETURN:
	{
		std::vector<int> ids;
		index_in_lex_table++;
		while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
		{
			ids.push_back(index_in_lex_table++);
		}

		auto p = parse_expression(ids, result_instructions);

		if (p.isResultInDefaultBool)
		{
			result_instructions.push_back(format("mov eax, {}", reservedBoolName));
		}
		else if (p.isResultInEAX)
		{
			break;
		}
		else if (p.isResultInSTACK)
		{
			result_instructions.push_back("pop eax");
		}
		else if (p.isSingleVariable)
		{
			result_instructions.push_back(format("mov eax, {}", p.resultStorage));
		}
		result_instructions.push_back(format("jmp {}", currentFunction->endLabel));
		break;
	}
	default:
		break;
	}
}
