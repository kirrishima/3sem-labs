#include "stdafx.h"
#include "code_gen.h"
#include <cctype>
#include <format>
#include <sstream>

using namespace std;

namespace CD
{
	unordered_map <std::string, std::vector<std::string>> used_functions;
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
	//bool isIdentifier(const string& token) {
	//	//if (token.empty() || isdigit(token[0])) return false;
	//	//for (char ch : token) {
	//	//	if (!isalnum(ch) && ch != '_' && token.front() != '[' && token.back() != '[') return false;
	//	//}
	//	//return true;
	//	/*return !isOperator()*/
	//}


	// Функция для разбора выражения с учетом приоритета операций
	vector<string> CD::CodeGeneration::__parse_math_expression(const std::vector<int>& expression) {
		stack<char> operators;         // Стек для операторов
		vector<string> output;         // Выходная очередь (в конечном итоге RPN)

		for (int id = expression.front(); id <= expression.back(); id++) {
			if (LEX_TABLE.table[id].lexema[0] == LEX_ID && ID_TABLE.table[LEX_TABLE.table[id].idxTI].idtype == IT::IDTYPE::F)
			{
				std::string functionName = get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[id].idxTI]);

				auto function = user_functions[functionName];

				stack<char> parenthesis;
				parenthesis.push(LEX_LEFTTHESIS);
				std::vector<int> function_call_args;

				id += 2;
				while (id <= expression.back())
				{
					if (LEX_TABLE.table[id].lexema[0] == LEX_RIGHTTHESIS)
					{
						parenthesis.pop();
						if (parenthesis.size() == 0)
						{
							break;
						}
					}
					if (LEX_TABLE.table[id].lexema[0] == LEX_LEFTTHESIS)
					{
						parenthesis.push(id);
					}
					if (LEX_TABLE.table[id].lexema[0] == LEX_ID || LEX_TABLE.table[id].lexema[0] == LEX_LITERAL)
					{
						functionName += get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[id].idxTI]);
					}
					function_call_args.push_back(id++);
				}

				if (used_functions.find(functionName) == used_functions.end())
				{
					used_functions[functionName] = parse_function_call(function, function_call_args.front(), function_call_args.back());
				}
				output.push_back(functionName);
			}
			else if (LEX_TABLE.table[id].lexema[0] == LEX_ID || LEX_TABLE.table[id].lexema[0] == LEX_LITERAL) {
				output.push_back(get_id_name_in_data_segment(ID_TABLE.table[LEX_TABLE.table[id].idxTI])); // Накопление идентификатора или литерала
			}
			else if (LEX_TABLE.table[id].lexema[0] == LEX_MATH)
			{
				while (!operators.empty() &&
					getPrecedence(operators.top()) >= getPrecedence(LEX_TABLE.table[id].v)) {
					output.push_back(string(1, operators.top()));
					operators.pop();
				}
				operators.push(LEX_TABLE.table[id].v);
			}
			else if (LEX_TABLE.table[id].lexema[0] == LEX_LEFTTHESIS) {
				operators.push('('); // Открывающая скобка
			}
			else if (LEX_TABLE.table[id].lexema[0] == LEX_RIGHTTHESIS) {
				while (!operators.empty() && operators.top() != LEX_LEFTTHESIS) {
					output.push_back(string(1, operators.top()));
					operators.pop();
				}
				if (!operators.empty()) operators.pop(); // Удалить '(' из стека
			}
		}

		// Перенос оставшихся операторов в выход
		while (!operators.empty()) {
			output.push_back(string(1, operators.top()));
			operators.pop();
		}

		return output;
	}

	void generateOperation(std::vector<std::string>& masmCode, const std::string& operation)
	{
		if (operation == "+") {
			masmCode.push_back("add ax, bx");
		}
		else if (operation == "-") {
			masmCode.push_back("sub ax, bx");
		}
		else if (operation == "*") {
			masmCode.push_back("imul bx");
		}
		else if (operation == "/") {
			masmCode.push_back("cmp ebx, 0");
			masmCode.push_back("je division_error");
			masmCode.push_back("cdq");    // Расширение eax для деления
			masmCode.push_back("idiv bx");
		}
		else {
			throw runtime_error("Неизвестный оператор: " + operation);
		}
	}

	void generateMASM(std::vector<std::string>& masmCode, const vector<string>& rpn) {
		if (rpn.size() == 1)
		{
			if (used_functions.find(rpn[0]) != used_functions.end())
			{
				masmCode.push_back("; function call");
				masmCode.insert(masmCode.end(), used_functions[rpn[0]].begin(), used_functions[rpn[0]].end());
				return;
			}
			masmCode.push_back("push " + rpn[0]);
			return;
		}

		if (rpn.size() == 3 &&
			isOperator(rpn[2][0]))
		{
			if (used_functions.find(rpn[0]) != used_functions.end()
				&& used_functions.find(rpn[1]) != used_functions.end()) // если оба операнда - вызовы функции
			{
				masmCode.push_back("; function call"); // правый операнд вычисляем и помещаем в стек, так как вызов функции может его перезаписать
				masmCode.insert(masmCode.end(), used_functions[rpn[1]].begin(), used_functions[rpn[1]].end());
				masmCode.push_back("push ax");

				masmCode.push_back("; function call"); // левый вычисляется и остается результат в eax
				masmCode.insert(masmCode.end(), used_functions[rpn[0]].begin(), used_functions[rpn[0]].end());
				masmCode.push_back("pop bx"); // левый вычисляется и остается результат в eax
			}
			else if (used_functions.find(rpn[0]) != used_functions.end()) // если левый это рез функции
			{
				masmCode.push_back("; function call");
				masmCode.insert(masmCode.end(), used_functions[rpn[0]].begin(), used_functions[rpn[0]].end()); // оставляем его в eax
				masmCode.push_back("mov bx, " + rpn[1]); // правый в ebx
			}
			else if (used_functions.find(rpn[1]) != used_functions.end()) // если правый функция
			{
				masmCode.push_back("; function call");
				masmCode.insert(masmCode.end(), used_functions[rpn[1]].begin(), used_functions[rpn[1]].end());
				masmCode.push_back("mov bx, ax"); // сразу вычисляем его и в ebx
				masmCode.push_back("mov ax, " + rpn[0]); // в eax левый копируем
			}
			else
			{
				masmCode.push_back("mov ax, " + rpn[0]); // Первый операнд в eax
				masmCode.push_back("mov bx, " + rpn[1]); // Второй операнд в ebx
			}

			generateOperation(masmCode, rpn[2]);
			// Результат остается в eax, добавлять в стек не требуется, если это не нужно
			return;
		}

		for (const string& token : rpn) {
			if (token.size() > 1/*isIdentifier(token) || isLiteral(token)*/) {
				// Если токен — идентификатор или литерал, генерируем `push`
				if (used_functions.find(token) != used_functions.end())
				{
					masmCode.push_back("; function call");
					masmCode.insert(masmCode.end(), used_functions[token].begin(), used_functions[token].end());
					masmCode.push_back("push ax");
				}
				else
				{
					masmCode.push_back("push " + token);
				}
			}
			else if (isOperator(token[0])) {
				// Если токен — оператор, извлекаем два операнда из стека
				masmCode.push_back("pop bx"); // Второй операнд
				masmCode.push_back("pop ax"); // Первый операнд

				generateOperation(masmCode, token);

				// Помещаем результат обратно в стек
				masmCode.push_back("push ax");
			}
			else {
				throw runtime_error("Неизвестный токен: " + token);
			}
		}

		if (masmCode.back() == "push ax")
		{
			masmCode.pop_back(); // оставляем результат в eax, в стек не помещаем
		}
	}

	std::vector<std::string> CD::CodeGeneration::generate_math_expressions(const std::vector<int>& expr)
	{
		std::vector<std::string> masmCode;
		vector<string> result = __parse_math_expression(expr);
		ofstream f("polish.txt", ios::ate | ios::app);
		f << lexems_vector_to_source_string(expr) << "   ";
		for (const auto& r : result)
		{
			f << r;
		}
		f << "\n";
		f.close();
		generateMASM(masmCode, result);
		//for (const auto e : result)
		//{
		//	cout << e << " ";
		//}

		return masmCode;
	}
}