#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

// = значение или = результат_выражени€
bool isAssignment(const std::string& expr) {
	constexpr char operators[] = "+-*/";
	for (char op : operators) {
		if (expr.find(op) != std::string::npos) {
			return false;
		}
	}
	return true;
}

std::vector<std::string> CD::CodeGeneration::parse_lexem(int& index_in_lex_table)
{
	std::vector<std::string> instructions_set;

	switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
	{
	case '=':
	{
		if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].iddatatype == IT::IDDATATYPE::STR
			/*&& ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->len > 0*/)
		{
			if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::V)
			{
				if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].idxfirstLE =
						ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idxfirstLE;
				}
				else
				{
					throw "ѕрисвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
						to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
				}
			}
			else if (ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].idtype == IT::IDTYPE::L)
			{
				ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].idxfirstLE = index_in_lex_table + 1;
			}
			else
			{
				throw "ѕрисвоение неверного типа данных: " + ("dest: " + to_string(IT::IDDATATYPE::STR) + ", src: " +
					to_string(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].iddatatype));
			}
			//char* destIdValue = ID_TABLE.table[LEX_TABLE.table[index_in_lex_table - 1].idxTI].value.vstr->str;
			//char* srcIdValue = ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 1].idxTI].value.vstr->str;

			//if (strcmp(destIdValue, srcIdValue) != 0)
			//{
			//	strcpy(destIdValue, srcIdValue);
			//}
			break;
		}
		std::string var = __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table].idxTI]);
		std::string token = "";
		std::string expr = "";

		index_in_lex_table++;
		while (index_in_lex_table < LEX_TABLE.size && LEX_TABLE.table[index_in_lex_table].lexema[0] != ';')
		{
			if (LEX_TABLE.table[index_in_lex_table].lexema[0] == '(' || LEX_TABLE.table[index_in_lex_table].lexema[0] == ')')
			{
				expr += LEX_TABLE.table[index_in_lex_table].lexema[0];
			}
			else if (LEX_TABLE.table[index_in_lex_table].lexema[0] == 'v')
			{
				expr += LEX_TABLE.table[index_in_lex_table].v;
			}
			else
			{
				expr += __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[index_in_lex_table].idxTI]);
			}
			index_in_lex_table++;
		}

		if (!isAssignment(expr))
		{
			auto a = __generate_math_expressions(expr);
			for (const std::string& str : a)
			{
				instructions_set.push_back(str);
			}
			instructions_set.push_back("mov " + var + ", eax");// а есть ли в этом смысл?
		}
		else
		{
			instructions_set.push_back("mov eax, " + expr);
			instructions_set.push_back("mov " + var + ", eax");
		}
		instructions_set.push_back("\n");

		break;
	}
	case 'p':
	{
		auto print_insts = __parse_print_expression_(index_in_lex_table);
		instructions_set.insert(instructions_set.end(), print_insts.begin(), print_insts.end());
		break;
	}
	default:
		break;
	}
	return instructions_set;
}

std::vector<std::string> CD::CodeGeneration::parse_expression(vector<int> ids)
{
	bool isCompare = false;
	bool isMath = false;
	bool isINT = false;
	bool isSTR = false;

	// что нам дали?
	for (int id : ids)
	{
		LT::Entry& lt_entry = LEX_TABLE.table[id];
		switch (LEX_TABLE.table[id].lexema[0])
		{
		case LEX_ID:
			switch (ID_TABLE.table[lt_entry.idxTI].iddatatype)
			{
			case IT::IDDATATYPE::INT:
				isINT = true;
				break;
			case IT::IDDATATYPE::STR:
				isSTR = true;
				break;
			}
			break;
		case LEX_COMPARE:
			isCompare = true;
			break;
		case LEX_MATH:
			isMath = true;
			break;
		default:
			break;
		}
	}

	std::vector<std::string> instructions;

	// выражение сравнени€ (что_то операци€_сравнени€ что_то_еще)
	if (isCompare)
	{
		vector<vector<int>> operands;
		int pos = 0;
		int operationID = 0;

		for (int id : ids) // слева и справа от оператора сравнени€
		{
			if (LEX_TABLE.table[id].lexema[0] == LEX_COMPARE)
			{
				pos++;
				continue;
			}
			operands[pos].push_back(id);
		}

		if (isSTR)
		{
			if (operands[0].size() != 1 || operands[1].size() != 1)
				throw "parse_expression: установлен флаг isSTR, но число операндов не равно по 1 дл€ каждой стороны";

			ifElseGeneration.CompareStrings(instructions, __get_string_value(operands[0][0]), __get_string_value(operands[1][0]));
		}
		else if (isINT)
		{
			vector<string> str_operands(2, "");
			str_operands[0] = __lexemVectorIDStoString(operands[0]);
			str_operands[1] = __lexemVectorIDStoString(operands[1]);

			ifElseGeneration.CompareInts(instructions, str_operands);
			//ifElseGeneration.CompareInts(instructions, operands)
		}
	}
}

vector<string> CD::CodeGeneration::__parse_print_expression_(int& index_in_lex_table)
{

	if (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_PRINT)
	{
		throw "__parse_print_expression_: передан индекс лексемы, котора€ не равна LEX_PRINT";
	}
	vector<string> instructions_set;
	vector<int> lexems;

	index_in_lex_table += 2;

	while (LEX_TABLE.table[index_in_lex_table].lexema[0] != LEX_SEMICOLON)
	{
		cout << LEX_TABLE.table[index_in_lex_table].lexema[0];
		lexems.push_back(index_in_lex_table++);
	};
	lexems.pop_back();
	cout << endl;

	if (lexems.size() == 1)
	{
		int idxIT = IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[lexems[0]].idxTI]); // (x
		if (idxIT < 0)
		{
			cout << "¬стречен неопознанный индификатор в праметрах функции print. »м€ "
				<< ID_TABLE.table[LEX_TABLE.table[lexems[0]].idxTI].id
				<< " не было найдено в таблице индификаторов.";

			throw "Wrong id in print's params";
		}
		IT::Entry* id = &ID_TABLE.table[idxIT];

		if (id->iddatatype == IT::IDDATATYPE::INT)
		{
			instructions_set.push_back("push " + __getIDnameInDataSegment(ID_TABLE.table[idxIT]));
			instructions_set.push_back("call __PrintNumber  ; ¬ыводим число в консоль");
		}
		else if (id->iddatatype == IT::IDDATATYPE::STR)
		{
			std::string idName;
			if (id->idtype == IT::IDTYPE::L)
			{
				idName = __getIDnameInDataSegment(ID_TABLE.table[idxIT]);
			}
			else
			{
				idName = __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[id->idxfirstLE].idxTI]);
			}
			instructions_set.push_back("PrintArrayMACRO " + idName);
		}
		else
		{
			throw "Ќеожиданный тип литерала: " + to_string(id->iddatatype);
		}
	}

	if (lexems.size() > 2)
	{

	}
	return instructions_set;
}

/// <summary>
/// ѕреобразует входной вектор индексов лексем в строку, представл€ющую выражение с именами на асм.
/// »спользуетс€ дл€ генерации польской натации по выражению
/// </summary>
/// <param name="ids">индексы лесем</param>
/// <returns></returns>
std::string CD::CodeGeneration::__lexemVectorIDStoString(const vector<int>& ids)
{
	string result = "";
	for (int i = 0; i < ids.size(); i++)
	{
		int lexId = ids[i];

		switch (LEX_TABLE.table[lexId].lexema[0])
		{
		case LEX_ID:
		case LEX_LITERAL:
			result += __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[lexId].idxTI]);
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

std::string CD::CodeGeneration::__get_string_value(const int lex_id)
{
	int idxIT = IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[lex_id].idxTI]);
	IT::Entry* id = &ID_TABLE.table[idxIT];

	if (id->iddatatype == IT::IDDATATYPE::STR)
	{
		std::string idName;
		if (id->idtype == IT::IDTYPE::L)
		{
			idName = __getIDnameInDataSegment(ID_TABLE.table[idxIT]);
		}
		else
		{
			idName = __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[id->idxfirstLE].idxTI]);
		}
		return idName;
	}
	else
	{
		throw "Ќеожиданный тип при определении значени€ строковой лексемы <"
			+ to_string(lex_id) + ">: " + to_string(id->iddatatype);
	}
}