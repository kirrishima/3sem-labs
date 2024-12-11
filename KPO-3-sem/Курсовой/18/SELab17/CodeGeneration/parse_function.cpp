#include "stdafx.h"
#include "cd.h"

std::vector<std::string> CD::CodeGeneration::parse_function_call(UserDefinedFunctions* function, int params_start_index, int params_end_index)
{
	std::vector<std::string> params;
	int params_pos = function->params.size() - 1;
	vector<int> param;
	for (int i = params_end_index; i >= params_start_index; i--)
	{
		LT::Entry* lex = &LEX_TABLE.table[i];
		if (lex->lexema[0] == LEX_COMMA || params_start_index > i - 1)
		{
			if (params_start_index == i)
			{
				param.push_back(i);
			}
			std::reverse(param.begin(), param.end());
			auto p = parse_expression(param, params);

			if (p.isINT && !(function->params[params_pos] == IT::IDDATATYPE::INT) ||
				p.isSTR && !(function->params[params_pos] == IT::IDDATATYPE::STR))
			{
				throw "parse_function_call: неверный параметр в вызове функции";
			}
			params_pos--;
			if (p.isSingleVariable)
			{
				params.push_back(format("push {}", p.resultStorage));
			}
			else
			{
				if (p.isResultInEAX)
				{
					params.push_back("push eax");
				}
				else if (p.isResultInEBX)
				{
					params.push_back("push ebx");
				}
				else
				{
					params.push_back(p.resultStorage);
				}
			}
			param.clear();
		}
		else param.push_back(i);
	}
	params.push_back(format("call {}", function->name));
	return params;
}

void CD::CodeGeneration::parse_function_body(UserDefinedFunctions* function, int start_index, int end_index)
{
	for (int i = start_index; i < end_index; i++)
	{
		parse_lexem(function->code, i);
	}
}

void CD::CodeGeneration::parse_function(int start_index, int end_index)
{
	UserDefinedFunctions* function = new UserDefinedFunctions();
	currentFunction = function;
	vector<string> instrs;

	std::vector<std::string> params_names;
	bool isMain = false;
	bool f = false;
	int a = 0;
	for (start_index; start_index < end_index && !f; start_index++)
	{
		switch (LEX_TABLE.table[start_index].lexema[0]) {
		case LEX_ID:
			switch (ID_TABLE.table[LEX_TABLE.table[start_index].idxTI].idtype)
			{
			case IT::IDTYPE::F:
				function->name = string(ID_TABLE.table[LEX_TABLE.table[start_index].idxTI].id);
				break;

			case IT::IDTYPE::P:
				function->push_params(ID_TABLE.table[LEX_TABLE.table[start_index].idxTI].iddatatype);
				params_names.push_back(format("mov eax, [esp + {}]", a += 4));
				params_names.push_back(format("mov {}, eax", get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[start_index].idxTI])));
				break;

			default:
				break;
			}
			break;

		case LEX_MAIN:
			function->name = "main";
			isMain = true;
			break;

		case LEX_LEFTBRACE:
			f = true;
			break;
		default:
			break;
		}
	}
	function->endLabel = format("{}_END", function->name);
	function->push_code(format("{} proc", function->name));
	function->push_code("start:");
	function->code.insert(function->code.end(), params_names.begin(), params_names.end());
	parse_function_body(function, start_index, end_index);

	function->push_code(format("{}:", function->endLabel));

	if (isMain)
	{
		function->push_code("push 0");
		function->push_code("call ExitProcess");
	}
	else
	{
		function->push_code(format("ret {}", function->params.size() * 4));
	}

	function->push_code(format("{} endp", function->name));

	if (isMain)
	{
		function->push_code("END main");
	}
	user_functions[function->name] = function;
	__user_functions.push_back(function);
}