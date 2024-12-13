#include "stdafx.h"
#include "semantic.h"
#include <set>

using namespace std;

int errors = 0;

#define ID_ENTRY_BY_LEX_ID(id) ID_Table.table[LEX_Table.table[id].idxTI]

unordered_map <IT::Entry*, std::vector<IT::IDDATATYPE>> protos;
unordered_map <IT::Entry*, std::vector<std::pair<IT::Entry*, int>>> references;
IT::Entry* currentFunction;

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
	case IT::ANY:
		return "any";
	default:
		return "undefined";
	}
}

std::string get_id_in_source(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, int i)
{
	return ID_Table.table[LEX_Table.table[i].idxTI].id;
};

struct Expression
{
	IT::IDDATATYPE iddatatype;
	bool isFunctionCallArg = false;
	bool isCompare = false;
	bool isReturn = false;
	std::vector<int> ids;
	int firstLine;
	int dest_IT_index;
	bool isInitialization;
};

static std::vector<std::vector<int>> get_function_params(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, int& start);
void parse_functions(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table);
void check_expression(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, Expression& expr);
void check_function_call(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, int funcID, std::vector<std::vector<int>> params);
void detect_cycles_in_references(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table);

int semantic::check(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table)
{
	parse_functions(ID_Table, LEX_Table);
	detect_cycles_in_references(ID_Table, LEX_Table);

	auto handle_expression = [&](int& i, int dest_IT_index, bool isReturn, bool isCompare, IT::IDDATATYPE iddatatype) {
		Expression ex;
		ex.dest_IT_index = dest_IT_index;
		ex.firstLine = LEX_Table.table[i].sn;
		ex.isReturn = isReturn;
		ex.isCompare = isCompare;
		ex.iddatatype = iddatatype;

		while (LEX_Table.table[i].lexema[0] != LEX_SEMICOLON && LEX_Table.table[i].lexema[0] != LEX_LEFTBRACE) {
			if (LEX_Table.table[i].lexema[0] == LEX_ID) {
				ex.ids.push_back(i);
				if (ID_ENTRY_BY_LEX_ID(i).idtype == IT::F) {
					if (LEX_Table.table[i + 1].lexema[0] != LEX_LEFTTHESIS) {
						cout << format("Строка {}: Присваивание значения функции непосредственно переменной недопустимо",
							LEX_Table.table[i].sn) << endl;
						errors++;
					}
					else {
						int x = i;
						i += 2;
						check_function_call(ID_Table, LEX_Table, x, get_function_params(ID_Table, LEX_Table, i));
					}
				}
			}
			else {
				ex.ids.push_back(i);
			}
			i++;
		}

		if (!isCompare && !isReturn || dest_IT_index == -1) {
			ex.ids.pop_back();  // Убираем лишний элемент, если это не сравнение или возврат
		}

		check_expression(ID_Table, LEX_Table, ex);
		};

	for (int i = 0; i < LEX_Table.size; i++) {
		switch (LEX_Table.table[i].lexema[0]) {
		case LEX_ID:
			if (LEX_Table.table[i - 1].lexema[0] == LEX_TYPE && ID_Table.table[LEX_Table.table[i].idxTI].idtype == IT::F) {
				currentFunction = &ID_Table.table[LEX_Table.table[i].idxTI];
			}
			else if (LEX_Table.table[i + 1].lexema[0] == LEX_LEFTTHESIS && LEX_Table.table[i - 1].lexema[0] != LEX_TYPE) {
				int x = i;
				i += 2;
				check_function_call(ID_Table, LEX_Table, x, get_function_params(ID_Table, LEX_Table, i));
			}
			break;

		case LEX_EQUAL:
			handle_expression(i, LEX_Table.table[i - 1].idxTI, false, false, ID_ENTRY_BY_LEX_ID(i - 1).iddatatype);
			break;

		case LEX_IF:
		case LEX_PRINT:
			i += 2;
			handle_expression(i, -1, false, true, IT::ANY);
			break;

		case LEX_RETURN:
			handle_expression(i, -1, true, false, currentFunction->iddatatype);
			break;
		}
	}

	return errors;
}


void parse_functions(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table) {
	// Вспомогательная функция для извлечения параметров функции
	auto extract_parameters = [&](size_t& index) -> std::vector<IT::IDDATATYPE> {
		std::vector<IT::IDDATATYPE> parameters;
		int counter = 1;
		bool exit = false;

		while (!exit) {
			switch (LEX_Table.table[index].lexema[0]) {
			case LEX_RIGHTTHESIS:
				if (--counter == 0) {
					exit = true;
				}
				break;
			case LEX_ID:
			case LEX_LITERAL:
				parameters.push_back(ID_ENTRY_BY_LEX_ID(index).iddatatype);
				break;
			}
			index++;
		}
		return parameters;
		};

	// Вспомогательная функция для извлечения ссылок внутри функции
	auto extract_references = [&](size_t& index) -> std::vector<std::pair<IT::Entry*, int>> {
		std::vector<std::pair<IT::Entry*, int>> refs;
		int braces = 1;

		while (LEX_Table.table[index].lexema[0] != LEX_LEFTBRACE) {
			index++;
		}
		index++;

		while (braces > 0) {
			switch (LEX_Table.table[index].lexema[0]) {
			case LEX_ID:
				if (ID_Table.table[LEX_Table.table[index].idxTI].idtype == IT::F) {
					refs.push_back({ &ID_Table.table[LEX_Table.table[index].idxTI], LEX_Table.table[index].sn });
				}
				break;

			case LEX_LEFTBRACE:
				braces++;
				break;

			case LEX_BRACELET:
				braces--;
				break;
			}
			index++;
		}
		return refs;
		};

	int start = 0;

	// Обработка прототипов функций
	for (size_t i = 0; i < LEX_Table.size; i++) {
		if (LEX_Table.table[i].lexema[0] == LEX_ID
			&& LEX_Table.table[i - 1].lexema[0] == LEX_TYPE
			&& ID_ENTRY_BY_LEX_ID(i).idtype == IT::F) {
			int tmpI = i;
			i += 2;
			protos[&ID_Table.table[LEX_Table.table[tmpI].idxTI]] = extract_parameters(i);
		}
	}

	// Обработка вызовов функций внутри других функций
	for (size_t i = 0; i < LEX_Table.size; i++) {
		if (LEX_Table.table[i].lexema[0] == LEX_ID
			&& LEX_Table.table[i - 1].lexema[0] == LEX_TYPE
			&& ID_ENTRY_BY_LEX_ID(i).idtype == IT::F) {

			int tmpI = i;
			i += 2;
			references[&ID_Table.table[LEX_Table.table[tmpI].idxTI]] = extract_references(i);
		}
	}
}

std::vector<std::vector<int>> get_function_params(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, int& start)
{
	std::vector<std::vector<int>> params;
	std::vector<int> tmp;

	int counter = 1;
	bool exit = false;

	while (!exit)
	{
		switch (LEX_Table.table[start].lexema[0])
		{
		case LEX_ID:
			if (ID_ENTRY_BY_LEX_ID(start).idtype == IT::F)
			{
				tmp.push_back(start);
				start += 2;
				get_function_params(ID_Table, LEX_Table, start);
			}
			break;

		case LEX_SEMICOLON:
			exit = true;
			break;

		case LEX_COMMA:
			params.push_back(tmp);
			tmp.clear();
			break;

		case LEX_RIGHTTHESIS:
			if (--counter == 0)
			{
				exit = true;
				break;
			}
		default:
			tmp.push_back(start);
			break;
		}
		start++;
	}
	start--;
	params.push_back(tmp);
	return params;
}

void check_expression(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, Expression& expr)
{
	if (expr.ids.empty()) return;
	bool insideFunction = false;
	int countBraces = 0;
	bool metCompare = false;
	bool metMath = false;
	// Проверка на один идентификатор
	if (expr.ids.size() == 1) {
		const auto& id_entry = ID_ENTRY_BY_LEX_ID(expr.ids[0]);
		const int line_number = LEX_Table.table[expr.ids[0]].sn;
		const std::string id_in_source = get_id_in_source(ID_Table, LEX_Table, expr.ids[0]);

		if (expr.iddatatype != id_entry.iddatatype && expr.iddatatype != IT::ANY) {
			if (expr.isFunctionCallArg) {
				cout << format(
					"Строка {}: аргумент функции {} имеет тип {}, но ожидается {}",
					line_number,
					ID_ENTRY_BY_LEX_ID(expr.dest_IT_index).id,
					iddatatype_to_str(id_entry.iddatatype),
					iddatatype_to_str(expr.iddatatype)) << endl;
			}
			else {
				cout << format(
					"Строка {}: тип {} не совместим с типом {}",
					line_number,
					id_entry.idtype == IT::L ? std::string("литерала") : "идентификатора",
					iddatatype_to_str(expr.iddatatype)) << endl;
			}
			errors++;
		}
		return;
	}

	unordered_map<IT::IDDATATYPE, int> types = {
		{IT::CHAR, 0},
		{IT::INT, 0},
		{IT::STR, 0}
	};

	for (int id = expr.ids.front(); id <= expr.ids.back(); id++) {
		const auto& lex_entry = LEX_Table.table[id];
		switch (lex_entry.lexema[0]) {
		case LEX_LITERAL:
		case LEX_ID:
			if (!insideFunction)
			{
				types[ID_ENTRY_BY_LEX_ID(id).iddatatype]++;
			}
			if (ID_ENTRY_BY_LEX_ID(id).idtype == IT::F)
			{
				insideFunction = true;
			}
			if (expr.isInitialization && expr.dest_IT_index == LEX_Table.table[id].idxTI) {
				cout << format("Строка {}: переменная {} не может быть использована в выражении, так как она неинициализирована.",
					lex_entry.sn, ID_Table.table[expr.dest_IT_index].id) << endl;

				errors++;
			}

			break;

		case LEX_MATH:
			if (types[IT::STR] != 0 || types[IT::CHAR] != 0 || (id != expr.ids.back() && ID_ENTRY_BY_LEX_ID(id + 1).iddatatype != IT::INT)) {
				cout << format("Строка {}: выражение содержит тип(ы), которые не поддерживают операцию {}",
					lex_entry.sn, lex_entry.v) << endl;
				errors++;
			}
			break;

		case LEX_COMPARE:
			if (!expr.isCompare) {
				cout << format("Строка {}: результат сравнения используется вне контекста сравнения.",
					lex_entry.sn) << endl;
				errors++;
			}
			else if (types[IT::STR] != 0 || types[IT::CHAR] != 0 || (id != expr.ids.back() && ID_ENTRY_BY_LEX_ID(id + 1).iddatatype != IT::INT))
			{

			}
			break;

		case LEX_PRINT:
			cout << format("Строка {}: функция print не может учавствовать в выражениях.",
				lex_entry.sn) << endl;
			errors++;

		case LEX_LEFTTHESIS:
			if (insideFunction)
			{
				countBraces++;
			}
			break;
		case LEX_RIGHTTHESIS:
			if (insideFunction && --countBraces == 0)
			{
				insideFunction = false;
			}
			break;
		default:
			break;
		}
	}

	int non_zero_count = 0;
	for (const auto& [type, count] : types) {
		if (count > 0) {
			if (++non_zero_count > 1) {
				if (expr.isCompare && metCompare)
				{
					cout << format("Строка {}: операции сравнения не могут быть применены к данным разных типов.",
						LEX_Table.table[expr.ids[0]].sn) << endl;
					errors++;
				}
				if (metMath)
				{
					cout << format("Строка {}: арифметические операции не могут быть применены к данным разных типов или сравнениям.",
						LEX_Table.table[expr.ids[0]].sn) << endl;
					errors++;
				}
				break;
			}
		}
	}
}

void check_function_call(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table, int funcID, std::vector<std::vector<int>> params)
{
	const auto& entry = &ID_Table.table[LEX_Table.table[funcID].idxTI];

	if (params.size() != protos[entry].size())
	{
		cout << format("Строка {}, число передаваемых аргументов в функцию {} не соответствует ее прототипу\n",
			LEX_Table.table[funcID].sn, entry->id);
		errors++;
	}

	int size = params.size() > protos[entry].size() ? protos[entry].size() : params.size();

	for (size_t i = 0; i < size; i++)
	{
		Expression ex;
		ex.iddatatype = protos[entry][i];
		ex.dest_IT_index = funcID;
		ex.isFunctionCallArg = true;
		ex.ids = params[i];

		check_expression(ID_Table, LEX_Table, ex);
	}
}

void detect_cycles_in_references(const IT::ID_Table& ID_Table, const LT::LexTable& LEX_Table) {
	std::set<IT::Entry*> visited;
	std::set<IT::Entry*> recursion_stack;

	// Лямбда для поиска цикла через DFS
	auto has_cycle = [&](IT::Entry* node, auto& has_cycle_ref) -> bool {
		recursion_stack.insert(node);
		if (references.count(node)) {
			for (const auto& callee : references.at(node)) {
				if (recursion_stack.count(callee.first) || !visited.count(callee.first) && has_cycle_ref(callee.first, has_cycle_ref)) {
					std::cout << format(
						"Строка {}: Функция {} ссылается на {} и образует цикл.",
						callee.second,
						node->id,
						callee.first->id) << std::endl;
					errors++;
					return true;
				}
			}
		}
		recursion_stack.erase(node);
		visited.insert(node);
		return false;
		};

	for (const auto& [func, calls] : references) {
		if (!visited.count(func)) {
			has_cycle(func, has_cycle);
		}
	}
}
