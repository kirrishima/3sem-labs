#include "stdafx.h"
#include "cd.h"

using namespace std;

// = �������� ��� = ���������_���������
bool isAssignment(const std::string& expr) {
	constexpr char operators[] = "+-*/";
	for (char op : operators) {
		if (expr.find(op) != std::string::npos) {
			return false;
		}
	}
	return true;
}

std::vector<std::string> CD::CodeGeneration::parse_expression(int& index_in_lex_table)
{
	std::vector<std::string> instructions_set;

	switch (LEX_TABLE.table[index_in_lex_table].lexema[0])
	{
	case '=':
	{
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
			instructions_set.push_back("mov " + var + ", eax");// � ���� �� � ���� �����?
		}
		else
		{
			instructions_set.push_back("mov eax," + expr);
			instructions_set.push_back("mov " + var + ", eax");
		}
		instructions_set.push_back("\n");

		break;
	}
	case 'p':
	{
		int idxIT = IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 2].idxTI]);
		if (idxIT < 0)
		{
			cout << "�������� ������������ ����������� � ��������� ������� print. ��� "
				<< ID_TABLE.table[LEX_TABLE.table[index_in_lex_table + 2].idxTI].id
				<< " �� ���� ������� � ������� �������������.";

			throw "Wrong id in print's params";
		}
		auto id = ID_TABLE.table[idxIT];

		if (id.idtype == IT::IDTYPE::L)
		{
			if (id.iddatatype == IT::IDDATATYPE::INT)
			{
				instructions_set.push_back("push " + __getIDnameInDataSegment(ID_TABLE.table[idxIT]));
				instructions_set.push_back("call __PrintNumber  ; ������� ����� � �������");
			}
			else if (id.iddatatype == IT::IDDATATYPE::STR)
			{
				std::string idName = __getIDnameInDataSegment(ID_TABLE.table[idxIT]);
				instructions_set.push_back("push type " + idName);
				instructions_set.push_back("push lengthof " + idName);
				instructions_set.push_back("push offset " + idName);
				instructions_set.push_back("call __PrintArray");
			}
			else
			{
				throw "����������� ��� �������� " + to_string(id.iddatatype);
			}
		}
		else
		{
			instructions_set.push_back("push " + __getIDnameInDataSegment(ID_TABLE.table[idxIT]));
			instructions_set.push_back("call __PrintNumber  ; ������� ����� � �������");
		}
	}
	break;
	default:
		break;
	}
	return instructions_set;
}