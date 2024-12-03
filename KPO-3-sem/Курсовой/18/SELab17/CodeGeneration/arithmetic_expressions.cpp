#include "stdafx.h"
#include "cd.h"
#include <cctype>
#include <format>
#include <sstream>

using namespace std;

namespace CD
{
	std::vector<std::string> operations;

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

	void generateMASM(const vector<string>& rpn) {
		bool printedNLafterpush = false;

		if (rpn.size() == 1)
		{
			operations.push_back("push " + rpn[0]);
			return;
		}

		for (const string& token : rpn) {
			if (isIdentifier(token) || isLiteral(token)) {
				if (!printedNLafterpush)
				{
					printedNLafterpush = true;
				}
				// ���� ����� � ������������� ��� �������, ���������� `push`
				operations.push_back("push " + token);
			}
			else if (isOperator(token[0])) {
				printedNLafterpush = false;
				// ���� ����� � ��������, ��������� ��� �������� �� �����
				operations.push_back("pop ebx"); // ������ �������
				operations.push_back("pop eax"); // ������ �������

				// ���������� ��� ��� ��������
				if (token == "+") {
					operations.push_back("add eax, ebx");
				}
				else if (token == "-") {
					operations.push_back("sub eax, ebx");
				}
				else if (token == "*") {
					operations.push_back("imul ebx");
				}
				else if (token == "/") {
					operations.push_back("cdq");    // ���������� eax ��� �������
					operations.push_back("idiv ebx");
				}

				// �������� ��������� ������� � ����
				operations.push_back("push eax");
			}
			else {
				throw runtime_error("����������� �����: " + token);
			}
		}

		// ��������� ��������� �� ����� � ��������� ��� � `RESULT`
		operations.push_back("pop eax");
	}

	std::vector<std::string> CD::CodeGeneration::__generate_math_expressions(const std::string& expr)
	{
		operations.clear();
		vector<string> result = parseExpression(expr);

		generateMASM(result);
		//for (const auto e : result)
		//{
		//	cout << e << " ";
		//}

		return operations;
	}
}