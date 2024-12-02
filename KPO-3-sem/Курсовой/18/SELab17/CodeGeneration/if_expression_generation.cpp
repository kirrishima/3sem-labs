#include "stdafx.h"
#include "cd.h"

using namespace std;

// ���������� ��������� << ��� ���������� ������ n ���
std::ostream& operator<<(std::ostream& os, const std::pair<std::string, int>& str_times) {
	const std::string& str = str_times.first;
	int times = str_times.second;
	for (int i = 0; i < times; ++i) {
		os << str;
	}
	return os;
}

//// ��� std::string � int (��� �������)
//std::pair<std::string, int> operator*(const std::string& str, int times) {
//	return { str, times };
//}

// ��� std::string � int (��� �������)
std::string operator*(const std::string& str, int times) {
	std::string new_str;
	for (size_t i = 0; i < times; i++)
	{
		new_str += str;
	}
	return new_str;
}

int nestingLevel = 0;

// ��������� �������
void CD::CodeGeneration::IfElseGeneration::GenerateCondition(
	const vector<string>& operands, // ��� �������� - ����� � ������ 
	const string& comparison, // �������� ��������� (>, <, ==, !=, >=, <=)
	const string& trueLabel, // ��� ����� ���� ������� �����������
	const string& falseLabel, // ���� �� �����������
	std::vector<std::string>& instructions // ������� ����������
) {

	instructions.push_back(parent.tab * nestingLevel + "; �������: " +
		operands[0] + " " + comparison + operands[1]);

	instructions.push_back(parent.tab * nestingLevel + "cmp eax, ebx");
	if (comparison == ">") {
		instructions.push_back(parent.tab * nestingLevel + "jg " + trueLabel);
	}
	else if (comparison == "<") {
		instructions.push_back(parent.tab * nestingLevel + "jl " + trueLabel);
	}
	else if (comparison == "=") {
		instructions.push_back(parent.tab * nestingLevel + "je " + trueLabel);
	}
	else if (comparison == ">=") {
		instructions.push_back(parent.tab * nestingLevel + "jge " + trueLabel);
	}
	else if (comparison == "<=") {
		instructions.push_back(parent.tab * nestingLevel + "jle " + trueLabel);
	}
	else {
		throw std::runtime_error("����������� �������� ���������");
	}
	instructions.push_back(parent.tab * nestingLevel + "jmp " + falseLabel);
}

// ������ `if`
void CD::CodeGeneration::IfElseGeneration::StartIf(
	const vector<string>& operands, // ��� �������� - ����� � ������ 
	const string& comparison, // �������� ��������� (>, <, ==, !=, >=, <=)
	std::vector<std::string>& instructions // ������� ����������
) {
	string trueLabel = GenerateLabel("IF_TRUE", nestingLevel);
	string endLabel = GenerateLabel("IF_END", nestingLevel);
	if_stack.push(endLabel);

	auto math_instructions = parent.__generate_math_expressions(operands[1]);

	for (const std::string& instr : math_instructions)
	{
		instructions.push_back(parent.tab * (nestingLevel + 1) + instr);
	}

	math_instructions = parent.__generate_math_expressions(operands[0]);

	for (const std::string& instr : math_instructions)
	{
		instructions.push_back(parent.tab * (nestingLevel + 1) + instr);
	}

	nestingLevel++;
	instructions.push_back(parent.tab * nestingLevel + "; ������ if");
	instructions.push_back(parent.tab * nestingLevel + "; ��������� �������� ���������");

	// ��������� ���� ��� ���������� ���������
	instructions.push_back(parent.tab * nestingLevel + "pop eax");
	instructions.push_back(parent.tab * nestingLevel + "pop ebx");

	// ��������� �������
	GenerateCondition(operands, comparison, trueLabel, endLabel, instructions);
	instructions.push_back(parent.tab * (nestingLevel - 1) + trueLabel + ':');
}

// ��������� ����� `else`
void CD::CodeGeneration::IfElseGeneration::StartElse(std::vector<std::string>& instructions) {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}
	string endLabel = if_stack.top();
	string elseLabel = GenerateLabel("ELSE", nestingLevel - 1); // -1 ��� ��� ��� �������, � ��������� ����� ������ �� 1

	for (int i = 0; i < instructions.size(); i++)
	{
		if (instructions[i].find(endLabel) != string::npos)
		{
			instructions[i] = parent.tab * nestingLevel + "jmp " + elseLabel;
			break;
		}

	}
	instructions.push_back(parent.tab * (nestingLevel - 1) + elseLabel + ':');
}

// ���������� `if` ��� `else`
void CD::CodeGeneration::IfElseGeneration::EndIfOrElse(std::vector<std::string>& instructions) {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}

	string endLabel = if_stack.top();
	instructions.push_back(parent.tab * nestingLevel + "; ������� � ������ �� ����� if-else �����");
	instructions.push_back(parent.tab * nestingLevel + "jmp " + endLabel);
}

// ���������� `if` ��� `else`
void CD::CodeGeneration::IfElseGeneration::EndExpression(std::vector<std::string>& instructions) {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}

	string endLabel = if_stack.top();
	if_stack.pop();

	instructions.push_back(parent.tab * --nestingLevel + endLabel + ':');
}


std::vector<std::string> CD::CodeGeneration::IfElseGeneration::generateIfStatement(int& i)
{
	std::vector<std::string> instructions;
	while (true) {
		if (parent.LEX_TABLE.table[i].lexema[0] == '?')
		{
			i += 2; // ���������� '(' � ��������� � ������� ������� �������
			std::vector<std::string> operands(2, "");
			std::string operation;
			int currentOperand = 0;

			while (i < parent.LEX_TABLE.size && parent.LEX_TABLE.table[i].lexema[0] != '{')
			{
				switch (parent.LEX_TABLE.table[i].lexema[0]) {
				case 'i':
				case 'l':
					operands[currentOperand] += parent.__getIDnameInDataSegment(parent.ID_TABLE.table[parent.LEX_TABLE.table[i].idxTI]);
					break;

				case 'c':
					operation = parent.LEX_TABLE.table[i].c;
					currentOperand++;
					if (currentOperand != 1)
					{
						throw "������ ��� ��������� if-else: ������ ������ ��������� �� ��������������";
					}
					break;
				case ')':
					if (parent.LEX_TABLE.table[i + 1].lexema[0] == '{')
					{
						break;
					}
				default:
					operands[currentOperand] += parent.LEX_TABLE.table[i].lexema[0];
					break;
				}
				i++;
			} // while

			StartIf(operands, operation, instructions);
		}

		while (i < parent.LEX_TABLE.size && parent.LEX_TABLE.table[i].lexema[0] != '}')
		{
			switch (parent.LEX_TABLE.table[i].lexema[0]) {

			case '?':
				for (const std::string& str : generateIfStatement(i))
				{
					instructions.push_back(str);
				}
				break;
			case '=':
			case 'p':
			{
				auto res = parent.parse_expression(i);
				for (const std::string& s : res)
				{
					instructions.push_back(parent.tab * nestingLevel + s);
				}
				break;
			}
			default:
				break;
			}
			i++;
		}

		EndIfOrElse(instructions);

		if (parent.LEX_TABLE.table[i + 1].lexema[0] == ':')
		{
			i += 3;
			StartElse(instructions);
			continue;
		}
		break;
	}
	EndExpression(instructions);

	return instructions;
}