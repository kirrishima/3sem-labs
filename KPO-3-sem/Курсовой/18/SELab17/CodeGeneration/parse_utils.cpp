#include "stdafx.h"
#include "cd.h"
#include <vector>

using namespace std;

std::string trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");
	return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Удаляет комментарии, начиная с символа ';'
std::string removeComment(const std::string& str) {
	size_t commentPos = str.find(';');
	return (commentPos == std::string::npos) ? str : str.substr(0, commentPos);
}

// Проверяет, является ли строка меткой в MASM
bool isMASMLabel(const std::string& line) {
	// Убираем комментарии и обрезаем пробелы
	std::string cleanLine = trim(removeComment(line));

	// Проверяем, что строка не пуста и заканчивается двоеточием
	if (cleanLine.empty() || cleanLine.back() != ':') {
		return false;
	}

	// Убираем двоеточие для проверки идентификатора
	std::string identifier = cleanLine.substr(0, cleanLine.size() - 1);

	// Проверяем, что идентификатор не пуст и состоит из допустимых символов
	if (identifier.empty()) {
		return false;
	}

	// Первый символ должен быть буквой или подчеркиванием
	if (!std::isalpha(identifier[0]) && identifier[0] != '_') {
		return false;
	}

	// Остальные символы могут быть буквами, цифрами или подчеркиваниями
	for (size_t i = 1; i < identifier.size(); ++i) {
		if (!std::isalnum(identifier[i]) && identifier[i] != '_') {
			return false;
		}
	}

	return true;
}

// Проверяет, является ли идентификатор допустимым именем функции в MASM
bool isValidIdentifier(const std::string& identifier) {
	if (identifier.empty()) {
		return false;
	}

	// Первый символ должен быть буквой или подчеркиванием
	if (!std::isalpha(identifier[0]) && identifier[0] != '_') {
		return false;
	}

	// Остальные символы могут быть буквами, цифрами или подчеркиваниями
	for (size_t i = 1; i < identifier.size(); ++i) {
		if (!std::isalnum(identifier[i]) && identifier[i] != '_') {
			return false;
		}
	}

	return true;
}

// Проверяет, является ли строка началом функции (оканчивается на PROC)
bool isFunctionStart(const std::string& line) {
	// Убираем комментарии и обрезаем пробелы
	std::string cleanLine = trim(removeComment(line));

	// Проверяем, что строка оканчивается на "PROC"
	if (cleanLine.size() < 4 || cleanLine.substr(cleanLine.size() - 4) != "proc") {
		return false;
	}

	// Убираем "PROC" для проверки идентификатора
	std::string identifier = trim(cleanLine.substr(0, cleanLine.size() - 4));

	return isValidIdentifier(identifier);
}

// Проверяет, является ли строка завершением функции (оканчивается на ENDP)
bool isFunctionEnd(const std::string& line) {
	// Убираем комментарии и обрезаем пробелы
	std::string cleanLine = trim(removeComment(line));

	// Проверяем, что строка оканчивается на "ENDP"
	if (cleanLine.size() < 4 || cleanLine.substr(cleanLine.size() - 4) != "endp") {
		return false;
	}

	// Убираем "ENDP" для проверки идентификатора
	std::string identifier = trim(cleanLine.substr(0, cleanLine.size() - 4));

	return isValidIdentifier(identifier);
}

std::string CD::CodeGeneration::lexems_vector_to_string(const vector<int>& ids)
{
	string result = "";
	for (size_t i = 0; i < ids.size(); i++)
	{
		int lexId = ids[i];

		switch (LEX_TABLE.table[lexId].lexema[0])
		{
		case LEX_ID:
		case LEX_LITERAL:
			result += get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[lexId].idxTI]);
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

std::string CD::CodeGeneration::get_string_value(const int lex_id)
{
	int idxIT = IT::search(ID_TABLE, ID_TABLE.table[LEX_TABLE.table[lex_id].idxTI]);
	IT::Entry* id = &ID_TABLE.table[idxIT];

	if (id->iddatatype == IT::IDDATATYPE::STR)
	{
		std::string idName;
		if (id->idtype == IT::IDTYPE::L)
		{
			idName = get_id_name_in_data_segment(ID_TABLE.table[idxIT]);
		}
		else
		{
			idName = get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[id->idxfirstLE].idxTI]);
		}
		return idName;
	}
	else
	{
		throw "Неожиданный тип при определении значения строковой лексемы <"
			+ to_string(lex_id) + ">: " + to_string(id->iddatatype);
	}
}

// = значение или = результат_выражения
bool CD::is_assignment(const std::string& expr) {
	constexpr char operators[] = "+-*/";
	for (char op : operators) {
		if (expr.find(op) != std::string::npos) {
			return false;
		}
	}
	return true;
}