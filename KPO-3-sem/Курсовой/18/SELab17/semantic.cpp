#include "stdafx.h"
#include "semantic.h"

using namespace std;

int errors = 0;

#define ID_ENTRY_BY_LEX_ID(id) ID_Table.table[LEX_Table.table[id].idxTI]

std::string iddatatype_to_str(IT::IDDATATYPE t)
{
	switch (t)
	{
	case IT::INT:
		return "int";
	case IT::STR:
		return "str";
	case IT::CHAR:
		return "char";
	default:
		return "undefined";
	}
}

std::string get_id_in_source(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, int i) 
{ return ID_Table.table[LEX_Table.table[i].idxTI].id; };


struct Expression
{
	IT::IDDATATYPE iddatatype;
	IT::IDTYPE idtype;
	bool isVarInitialiation = false;
	bool isFunctionCallArg = false;
	bool isMath = false;
	bool isCompare = false;
	bool isReturn = false;
	std::vector<int> ids;
	int firstLine;
	int destId;
	bool isInitialization;
};

void checkBraces(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table);
void check_equal(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, Expression &expr);

int semantic::check(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table)
{
	checkBraces(ID_Table, LEX_Table);
	
	
	for (size_t i = 0; i < LEX_Table.size; i++)
	{
		switch (LEX_Table.table[i].lexema[0])
		{
		case LEX_EQUAL:
		{

			Expression ex;
			ex.destId = i - 1;
			ex.firstLine = LEX_Table.table[i].sn;

			if (LEX_Table.table[i - 2].lexema[0] == LEX_TYPE)
			{
				ex.isInitialization = true;
			}

			i++;
			ex.iddatatype = ID_ENTRY_BY_LEX_ID(i - 2).iddatatype;
			while (LEX_Table.table[i].lexema[0] != LEX_SEMICOLON)
			{
				switch (LEX_Table.table[i].lexema[0])
				{
				case LEX_COMPARE:
					ex.isCompare = true;
					break;
				case LEX_MATH:
					ex.isMath = true;

				}
				ex.ids.push_back(i++);
			}
			check_equal(ID_Table, LEX_Table, ex);
		}
		}
	}
	return errors;
}

void checkBraces(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table)
{
	stack<char> braces;

	for (size_t i = 0; i < LEX_Table.size; i++)
	{
		switch (LEX_Table.table[i].lexema[0])
		{
		case LEX_LEFTTHESIS:
		case LEX_LEFTBRACE:
			braces.push(LEX_Table.table[i].lexema[0]);
			break;

		case LEX_BRACELET:
			if (braces.top() != LEX_LEFTBRACE)
			{
				cout << format("—трока {}, встречена неверна€ лексема {}, хот€ последней открывающей была {}",
					LEX_Table.table[i].sn, LEX_Table.table[i].lexema[0], braces.top());
				throw ERROR_THROW(700);
			}
			braces.pop();
			break;

		case LEX_RIGHTTHESIS:
			if (braces.top() != LEX_LEFTTHESIS)
			{
				cout << format("—трока {}, встречена неверна€ лексема {}, хот€ последней открывающей была {}",
					LEX_Table.table[i].sn, LEX_Table.table[i].lexema[0], braces.top());
				throw ERROR_THROW(700);
			}
			braces.pop();
		}
	}

	if (!braces.empty())
	{
		throw ERROR_THROW(701);
	}
}

void check_equal(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, Expression& expr)
{
	if (expr.ids.size() == 1)
	{
		if (expr.iddatatype != ID_ENTRY_BY_LEX_ID(expr.ids[0]).iddatatype)
		{
			cout << format("—трока {}: тип {} идентификатора {} не совместим с {}",
				LEX_Table.table[expr.ids[0]].sn, iddatatype_to_str(expr.iddatatype), get_id_in_source(ID_Table, LEX_Table, expr.ids[0]-2),
				iddatatype_to_str(ID_Table.table[LEX_Table.table[expr.ids[0]].idxTI].iddatatype)) << endl;
			errors++;
		}
		return;
	}

	unordered_map< IT::IDDATATYPE, int> types = { 
		{IT::CHAR, 0},
		{IT::INT, 0},
		{IT::STR, 0}
	};

	for (const int id : expr.ids)
	{
		switch (LEX_Table.table[id].lexema[0])
		{
		case LEX_LITERAL:
		case LEX_ID:
			types[ID_ENTRY_BY_LEX_ID(id).iddatatype]++;
			if (&ID_ENTRY_BY_LEX_ID(expr.destId) == &ID_ENTRY_BY_LEX_ID(id) && expr.isInitialization)
			{
				cout << format("—трока {}: переменна€ {} не может быть использована в выражении, так она неинициализированна.",
					LEX_Table.table[id].sn, ID_ENTRY_BY_LEX_ID(expr.destId).id) << endl;
				errors++;
			}
			break;
		case LEX_MATH:
			if (types[IT::STR] != 0 || types[IT::CHAR] != 0)
			{
				cout << format("—трока {}: выражение содержит тип(ы), которые не поддерживают операцию {}", 
					LEX_Table.table[id].sn, LEX_Table.table[id].v) << endl;
				errors++;
			}
			break;

		case LEX_COMPARE:
			cout << format("—трока {}: нельз€ сохранить результат сравнени€.", LEX_Table.table[id].sn)<<endl;
			errors++;
			break;

		default:
			break;
		}
	}

	for (const auto& t: types)
	{
		if (t.first != expr.iddatatype && t.second >0)
		{
			cout << format("—трока {}, выражение содержит ошибочный тип {}, когда ожидалс€ {}\n",
				expr.firstLine, iddatatype_to_str(t.first), iddatatype_to_str(expr.iddatatype));
		}
	}
}