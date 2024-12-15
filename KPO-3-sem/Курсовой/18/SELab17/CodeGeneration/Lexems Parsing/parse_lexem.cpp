#include "stdafx.h"
#include "code_gen.h"
#include <vector>

using namespace std;

void CD::CodeGeneration::parse_lexem(std::vector<std::string>& result_instructions, int& index_in_lex_table, int tabsize)
{
	switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
	{
	case LEX_EQUAL:
	{
		parse_lexem_equal__(result_instructions, index_in_lex_table, tabsize);
		break;
	}
	case LEX_PRINT:
	case LEX_WRITE:
	{
		parse_print_lexem__(result_instructions, index_in_lex_table, tabsize);
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
			auto v = parse_function_call(user_functions[name], ids.size() ? ids.front() : -1, ids.size() ? ids.back() : -1);
			for (const std::string& str : v)
			{
				result_instructions.push_back(tab * tabsize + str);
			}
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

		auto p = parse_expression(ids, result_instructions, tabsize);

		if (p.isResultInDefaultBool)
		{
			result_instructions.push_back(format("{}mov ax, {}", tab * tabsize, reservedBoolName));
		}
		else if (p.isResultInEAX)
		{
			break;
		}
		else if (p.isResultInSTACK)
		{
			result_instructions.push_back(format("{}pop eax", tab * tabsize));
		}
		else if (p.isSingleVariable)
		{
			if (p.isSTR)
			{
				result_instructions.push_back(format("{}mov eax, {}", tab * tabsize, p.resultStorage));
			}
			else if (p.isCHAR)
			{
				result_instructions.push_back(format("{}mov eax, offset {}", tab * tabsize, p.resultStorage));
			}
			else
			{
				result_instructions.push_back(format("{}movzx eax, {}", tab * tabsize, p.resultStorage));
			}
		}
		if (currentFunction->name == "main")
		{
			result_instructions.push_back(format("{}push eax", tab * tabsize));
		}
		result_instructions.push_back(format("{}jmp {}", tab * tabsize, currentFunction->endLabel));
		break;
	}
	case '?':
		for (const std::string& str : ifElseGen.generate_if_statement(index_in_lex_table))
		{
			result_instructions.push_back(tab * tabsize + str);
		}
		break;
	default:
		break;
	}
}
