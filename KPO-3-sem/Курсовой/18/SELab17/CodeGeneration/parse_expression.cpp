#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

vector<string> CD::CodeGeneration::parse_lexem(int& index_in_lex_table)
{
	vector<string> instructions_set;

	switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
	{
	case '=':
	{
		auto v = parse_lexem_equal__(index_in_lex_table);
		instructions_set.insert(instructions_set.end(), v.begin(), v.end());
		break;
	}
	case 'p':
	{
		auto print_insts = parse_print_lexem__(index_in_lex_table);
		instructions_set.insert(instructions_set.end(), print_insts.begin(), print_insts.end());
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

			while (index_in_lex_table <LEX_TABLE.size)
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

			for (auto& func : user_functions)
			{
				if (func.name == name)
				{
					auto v = parse_function_call(func, ids[0], ids.back());
					instructions_set.insert(instructions_set.end(), v.begin(), v.end());
					break;
				}
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

		auto p = parse_expression(ids, instructions_set);

		if (p.isResultInDefaultBool)
		{
			instructions_set.push_back(format("mov eax, {}", reservedBoolName));
		}
		else if (p.isResultInEAX)
		{
			break;
		}
		else if (p.isResultInSTACK)
		{
			instructions_set.push_back("pop eax");
		}
		else if (p.isSingleVariable)
		{
			instructions_set.push_back(format("mov eax, {}", p.resultStorage));
		}
		break;
	}
	default:
		break;
	}
	return instructions_set;
}

vector<string> CD::CodeGeneration::parse_print_lexem__(int& index_in_lex_table)
{
	if (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_PRINT)
	{
		throw "__parse_print_expression_: передан индекс лексемы, которая не равна LEX_PRINT";
	}
	vector<string> instructions_set;
	vector<int> lexems;

	index_in_lex_table += 2;

	while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
	{
		lexems.push_back(index_in_lex_table++);
	};
	lexems.pop_back();

	auto p = parse_expression(lexems, instructions_set);

	if (p.isResultInDefaultBool)
	{
		instructions_set.push_back(format("movzx eax, {}", reservedBoolName));
		instructions_set.push_back("push eax");
		instructions_set.push_back("call __PrintBool");
	}
	else if (p.isMath)
	{
		if (p.isResultInEAX)
		{
			instructions_set.push_back("push eax");
			instructions_set.push_back("call __PrintNumber");
		}
		else
		{
			instructions_set.push_back(format("push {}", p.resultStorage));
			instructions_set.push_back("call __PrintNumber");
		}
	}
	else if (p.isSTR)
	{
		instructions_set.push_back(format("push {}", p.resultStorage));
		instructions_set.push_back("call __Print");
	}
	else if (p.isINT)
	{
		instructions_set.push_back(format("push {}", p.resultStorage));
		instructions_set.push_back("call __PrintNumber");
	}
	return instructions_set;
}
