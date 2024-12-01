#include "stdafx.h"
#include "cd.h"
#include <cctype>
#include <format>
#include <sstream>

using namespace std;

namespace CD
{
	// ����������� ����������� ��������
	int getPrecedence(char op) {
		if (op == '+' || op == '-') return 1;
		if (op == '*' || op == '/') return 2;
		if (op == '(') return 0; // ��������� ��� ����������� ������
		return -1; // ����������� ��������
	}

	// ��������, �������� �� ������ ����������
	bool isOperator(char ch) {
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	}

	// ��������, �������� �� ������ ���������������
	bool isIdentifier(const string& token) {
		if (token.empty() || isdigit(token[0])) return false;
		for (char ch : token) {
			if (!isalnum(ch) && ch != '_' && token.front() != '[' && token.back() != '[') return false;
		}
		return true;
	}

	// ��������, �������� �� ������ ���������
	bool isLiteral(const string& token) {
		return token.size() > 2 && token[0] == '_' && token[1] == '_';
	}

	// ������� ��� ������� ��������� � ������ ���������� ��������
	vector<string> parseExpression(const string& expression) {
		stack<char> operators;         // ���� ��� ����������
		vector<string> output;         // �������� ������� (� �������� ����� RPN)
		string token;                  // ��� ���������� ��������������� � ����������

		for (char ch : expression) {
			if (isspace(ch)) continue; // ������� ��������

			if (isalnum(ch) || ch == '_') {
				token += ch; // ���������� �������������� ��� ��������
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
					// ��������� ������������� ��� �������
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
					operators.push(ch); // ����������� ������
				}
				else if (ch == ')') {
					while (!operators.empty() && operators.top() != '(') {
						output.push_back(string(1, operators.top()));
						operators.pop();
					}
					if (!operators.empty()) operators.pop(); // ������� '(' �� �����
				}
			}
		}

		if (!token.empty()) {
			output.push_back(token); // ��������� �����
		}

		// ������� ���������� ���������� � �����
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
				// ���� ����� � ������������� ��� �������, ���������� `push`
				masmCode << "\tpush " << token << "\n";
			}
			else if (isOperator(token[0])) {
				printedNLafterpush = false;
				// ���� ����� � ��������, ��������� ��� �������� �� �����
				masmCode << "\n\tpop ebx\n"; // ������ �������
				masmCode << "\tpop eax\n"; // ������ �������

				// ���������� ��� ��� ��������
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
					masmCode << "\tcdq\n";    // ���������� eax ��� �������
					masmCode << "\tidiv ebx\n";
				}

				// �������� ��������� ������� � ����
				masmCode << "\tpush eax\n";
			}
			else {
				throw runtime_error("����������� �����: " + token);
			}
		}

		// ��������� ��������� �� ����� � ��������� ��� � `RESULT`
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