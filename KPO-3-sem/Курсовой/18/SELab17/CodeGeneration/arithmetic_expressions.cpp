#include "stdafx.h"
#include "cd.h"
#include <cctype>
#include <format>
#include <sstream>

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

	string generateMASM(const vector<string>& rpn) {
		stringstream masmCode;
		bool printedNLafterpush = false;

		if (rpn.size() == 1)
		{
			masmCode << "\tpush " << rpn[0] << '\n';
			return masmCode.str();
		}

		for (const string& token : rpn) {
			if (isIdentifier(token) || isLiteral(token)) {
				if (!printedNLafterpush)
				{
					printedNLafterpush = true;
					masmCode << '\n';
				}
				// Если токен — идентификатор или литерал, генерируем `push`
				masmCode << "\tpush " << token << "\n";
			}
			else if (isOperator(token[0])) {
				printedNLafterpush = false;
				// Если токен — оператор, извлекаем два операнда из стека
				masmCode << "\n\tpop ebx\n"; // Второй операнд
				masmCode << "\tpop eax\n"; // Первый операнд

				// Генерируем код для операции
				if (token == "+") {
					masmCode << "\tadd eax, ebx\n";
				}
				else if (token == "-") {
					masmCode << "\tsub eax, ebx\n";
				}
				else if (token == "*") {
					masmCode << "\timul ebx\n";
				}
				else if (token == "/") {
					masmCode << "\tcdq\n";    // Расширение eax для деления
					masmCode << "\tidiv ebx\n";
				}

				// Помещаем результат обратно в стек
				masmCode << "\tpush eax\n";
			}
			else {
				throw runtime_error("Неизвестный токен: " + token);
			}
		}

		// Извлекаем результат из стека и сохраняем его в `RESULT`
		masmCode << "\n\tpop eax\n";

		return masmCode.str();
	}

	void CD::CodeGeneration::__generate_math_expressions(const std::string& expr)
	{
		vector<string> result = parseExpression(expr);

		//for (const auto e : result)
		//{
		//	cout << e << " ";
		//}

		OUT_ASM_FILE << generateMASM(result);
	}
}