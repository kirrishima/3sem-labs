#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

CD::CodeGeneration::ParseExpressionReturnParms CD::CodeGeneration::parse_expression(vector<int> ids, vector<string>& instructions)
{
	ParseExpressionReturnParms params;
	params.stringRepresentation = lexems_vector_to_string(ids);
	// что нам дали?
	for (int id : ids)
	{
		LT::Entry& lt_entry = LEX_TABLE.table[id];
		switch (LEX_TABLE.table[id].lexema[0])
		{
		case LEX_LITERAL:
		case LEX_ID:
			switch (ID_TABLE.table[lt_entry.idxTI].iddatatype)
			{
			case IT::IDDATATYPE::INT:
				params.isINT = true;
				break;
			case IT::IDDATATYPE::STR:
				params.isSTR = true;
				break;
			}
			if (ID_TABLE.table[lt_entry.idxTI].idtype == IT::IDTYPE::F)
				params.isFunctionCall = true;
			break;
		case LEX_COMPARE:
			params.isCompare = true;
			params.isSingleVariable = false;
			break;
		case LEX_MATH:
			params.isMath = true;
			params.isSingleVariable = false;
			break;
		default:
			break;
		}
	}

	// выражение сравнения (что_то операция_сравнения что_то_еще)
	if (params.isCompare)
	{
		vector<vector<int>> operands(2, vector<int>());
		int pos = 0;
		int operationID = 0;

		for (int id : ids) // слева и справа от оператора сравнения
		{
			if (LEX_TABLE.table[id].lexema[0] == LEX_COMPARE)
			{
				operationID = id;
				pos++;
				continue;
			}
			operands[pos].push_back(id);
		}

		if (params.isSTR)
		{
			params.isStringCompare = true;
			if (operands[0].size() != 1 || operands[1].size() != 1)
				throw "parse_expression: установлен флаг isSTR, но число операндов не равно по 1 для каждой стороны";

			ifElseGen.compare_strings(instructions, get_string_value(operands[0][0]), get_string_value(operands[1][0]));
			instructions.push_back("cmp eax, 0");
		}
		else if (params.isINT)
		{
			params.isIntCompare = true;
			//vector<string> str_operands(2, "");
			//str_operands[0] = lexems_vector_to_string(operands[0]);
			//str_operands[1] = lexems_vector_to_string(operands[1]);

			ifElseGen.compare_ints(instructions, operands);
			//ifElseGen.compare_ints(instructions, operands)
			instructions.push_back("cmp eax, ebx");
		}
		else
		{
			throw "Неизвестный тип данных в сравнении";
		}

		instructions.push_back(format("mov {}, 0", reservedBoolName));
		instructions.push_back(format("{} @true{}", ifElseGen.cmp_op_to_jmp(LEX_TABLE.table[operationID].c), trueLabelsCount));
		instructions.push_back(format("jmp @false{}", trueLabelsCount));
		instructions.push_back(format("@true{}:", trueLabelsCount));
		instructions.push_back(format("mov {}, 1", reservedBoolName));
		instructions.push_back(format("@false{}:", trueLabelsCount++));

		params.isResultInDefaultBool = true;
		params.isResultComputed = true;
		params.isSingleVariable = false;
		params.resultStorage = reservedBoolName;
	}
	else if (params.isMath)
	{
		auto v = generate_math_expressions(ids);
		instructions.insert(instructions.end(), v.begin(), v.end());
		params.isSingleVariable = false;
		params.isResultComputed = true;
		params.isResultInEAX = true;
		params.resultStorage = "eax";
	}
	else if (params.isSingleVariable && params.isFunctionCall)
	{
		std::string functionName = get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[ids[0]].idxTI]);

		parse_lexem(instructions, ids.front());
		params.isResultInEAX = true;
		params.resultStorage = "eax";
	}
	else if (params.isSingleVariable)
	{
		if (params.isINT)
		{
			params.resultStorage = params.stringRepresentation;
		}
		else if (params.isSTR)
		{
			params.resultStorage = get_string_value(ids[0]);
		}
	}

	return params;
}