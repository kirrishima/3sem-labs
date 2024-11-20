#include "stdafx.h"
#include "cd.h"
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include <sstream>
#include <format>

using namespace std;

namespace CD
{
	// Определение приоритетов операций
	int getPrecedence(char op) {
		if (op == '+' || op == '-') return 1;
		if (op == '*' || op == '/') return 2;
		if (op == '(') return 0; // Приоритет для открывающей скобки
		return -1; // Неизвестный оператор
	}

	// Проверка, является ли символ оператором
	bool isOperator(char ch) {
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	}

	// Проверка, является ли строка идентификатором
	bool isIdentifier(const string& token) {
		if (token.empty() || isdigit(token[0])) return false;
		for (char ch : token) {
			if (!isalnum(ch) && ch != '_' && token.front() != '[' && token.back() != '[') return false;
		}
		return true;
	}

	// Проверка, является ли строка литералом
	bool isLiteral(const string& token) {
		return token.size() > 2 && token[0] == '_' && token[1] == '_';
	}

	// Функция для разбора выражения с учетом приоритета операций
	vector<string> parseExpression(const string& expression) {
		stack<char> operators;         // Стек для операторов
		vector<string> output;         // Выходная очередь (в конечном итоге RPN)
		string token;                  // Для накопления идентификаторов и операторов

		for (char ch : expression) {
			if (isspace(ch)) continue; // Пропуск пробелов

			if (isalnum(ch) || ch == '_') {
				token += ch; // Накопление идентификатора или литерала
			}
			else {
				if (!token.empty()) {
					if (token.size() > 3 && token[0] == '_' && token[1] == '_')
					{
						output.push_back(token);
					}
					else
					{
						output.push_back(format("[{}]", token));
					}
					// Добавляем идентификатор или литерал
					token.clear();
				}

				if (isOperator(ch)) {
					while (!operators.empty() &&
						getPrecedence(operators.top()) >= getPrecedence(ch)) {
						output.push_back(string(1, operators.top()));
						operators.pop();
					}
					operators.push(ch);
				}
				else if (ch == '(') {
					operators.push(ch); // Открывающая скобка
				}
				else if (ch == ')') {
					while (!operators.empty() && operators.top() != '(') {
						output.push_back(string(1, operators.top()));
						operators.pop();
					}
					if (!operators.empty()) operators.pop(); // Удалить '(' из стека
				}
			}
		}

		if (!token.empty()) {
			output.push_back(token); // Последний токен
		}

		// Перенос оставшихся операторов в выход
		while (!operators.empty()) {
			output.push_back(string(1, operators.top()));
			operators.pop();
		}

		return output;
	}

	string generateMASM(const vector<string>& rpn) {
		stringstream masmCode;

		for (const string& token : rpn) {
			if (isIdentifier(token) || isLiteral(token)) {
				// Если токен — идентификатор или литерал, генерируем `PUSH`
				masmCode << "\tPUSH " << token << "\n";
			}
			else if (isOperator(token[0])) {
				// Если токен — оператор, извлекаем два операнда из стека
				masmCode << "\tPOP EBX\n"; // Второй операнд
				masmCode << "\tPOP EAX\n"; // Первый операнд

				// Генерируем код для операции
				if (token == "+") {
					masmCode << "\tADD EAX, EBX\n";
				}
				else if (token == "-") {
					masmCode << "\tSUB EAX, EBX\n";
				}
				else if (token == "*") {
					masmCode << "\tIMUL EBX\n";
				}
				else if (token == "/") {
					masmCode << "\tCDQ\n";    // Расширение EAX для деления
					masmCode << "\tIDIV EBX\n";
				}

				// Помещаем результат обратно в стек
				masmCode << "\tPUSH EAX\n";
			}
			else {
				throw runtime_error("Неизвестный токен: " + token);
			}
		}

		// Извлекаем результат из стека и сохраняем его в `RESULT`
		masmCode << "\tPOP EAX\n";

		return masmCode.str();
	}

	void __generate_math_expressions(const std::string& expr, std::ofstream& outFile)
	{
		vector<string> result = parseExpression(expr);

		outFile << generateMASM(result);
	}
}