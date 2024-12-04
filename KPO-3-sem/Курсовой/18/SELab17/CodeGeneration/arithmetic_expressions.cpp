#include "stdafx.h"
#include "cd.h"
#include <cctype>
#include <format>
#include <sstream>

using namespace std;

namespace CD
{
	std::vector<std::string> masmCode;

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
						output.push_back(token);
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

	void generateOperation(const std::string& operation)
	{
		if (operation == "+") {
			masmCode.push_back("add eax, ebx");
		}
		else if (operation == "-") {
			masmCode.push_back("sub eax, ebx");
		}
		else if (operation == "*") {
			masmCode.push_back("imul ebx");
		}
		else if (operation == "/") {
			masmCode.push_back("cdq");    // Расширение eax для деления
			masmCode.push_back("idiv ebx");
		}
		else {
			throw runtime_error("Неизвестный оператор: " + operation);
		}
	}

	void generateMASM(const vector<string>& rpn) {
		bool printedNLafterpush = false;

		if (rpn.size() == 1)
		{
			masmCode.push_back("push " + rpn[0]);
			return;
		}

		if (rpn.size() == 3 &&
			(isIdentifier(rpn[0]) || isLiteral(rpn[0])) &&
			(isIdentifier(rpn[1]) || isLiteral(rpn[1])) &&
			isOperator(rpn[2][0]))
		{
			masmCode.push_back("mov eax, " + rpn[0]); // Первый операнд в eax
			masmCode.push_back("mov ebx, " + rpn[1]); // Второй операнд в ebx

			generateOperation(rpn[2]);
			// Результат остается в eax, добавлять в стек не требуется, если это не нужно
			return;
		}

		for (const string& token : rpn) {
			if (isIdentifier(token) || isLiteral(token)) {
				if (!printedNLafterpush)
				{
					printedNLafterpush = true;
				}
				// Если токен — идентификатор или литерал, генерируем `push`
				masmCode.push_back("push " + token);
			}
			else if (isOperator(token[0])) {
				printedNLafterpush = false;
				// Если токен — оператор, извлекаем два операнда из стека
				masmCode.push_back("pop ebx"); // Второй операнд
				masmCode.push_back("pop eax"); // Первый операнд

				generateOperation(token);

				// Помещаем результат обратно в стек
				masmCode.push_back("push eax");
			}
			else {
				throw runtime_error("Неизвестный токен: " + token);
			}
		}

		if (masmCode.back() == "push eax")
		{
			masmCode.pop_back(); // оставляем результат в eax, в стек не помещаем
		}
	}

	std::vector<std::string> CD::CodeGeneration::__generate_math_expressions(const std::string& expr)
	{
		masmCode.clear();
		vector<string> result = parseExpression(expr);

		generateMASM(result);
		//for (const auto e : result)
		//{
		//	cout << e << " ";
		//}

		return masmCode;
	}
}