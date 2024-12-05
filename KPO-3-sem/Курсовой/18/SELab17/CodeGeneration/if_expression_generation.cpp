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


// ��������� �������
void CD::CodeGeneration::IfElseGeneration::GenerateCondition(
	const vector<string>& operands, // ��� �������� - ����� � ������ 
	const string& comparison, // �������� ��������� (>, <, ==, !=, >=, <=)
	const string& trueLabel, // ��� ����� ���� ������� �����������
	const string& falseLabel, // ���� �� �����������
	std::vector<std::string>& instructions,
	bool isStringCmp
) {

	if (isStringCmp)
	{
		instructions.push_back(parent.tab * nestingLevel + "cmp eax, 0" + parent.tab + "; �������: " +
			operands[0] + comparison + operands[1]);
	}
	else
	{
		instructions.push_back(parent.tab * nestingLevel + "cmp eax, ebx" + parent.tab + "; �������: " +
			operands[0] + comparison + operands[1]);
	}

	if (comparison == ">") {
		instructions.push_back(parent.tab * nestingLevel + "jg " + trueLabel);
	}
	else if (comparison == "<") {
		instructions.push_back(parent.tab * nestingLevel + "jl " + trueLabel);
	}
	else if (comparison == "=") {
		instructions.push_back(parent.tab * nestingLevel + "je " + trueLabel);
	}
	else if (comparison == "!=") {
		instructions.push_back(parent.tab * nestingLevel + "jne " + trueLabel);
	}
	else if (comparison == ">=") {
		instructions.push_back(parent.tab * nestingLevel + "jge " + trueLabel);
	}
	else if (comparison == "<=") {
		instructions.push_back(parent.tab * nestingLevel + "jle " + trueLabel);
	}
	else {
		throw std::runtime_error("����������� �������� ��������� ��� ��������� if-else: " + comparison);
	}
	instructions.push_back(parent.tab * nestingLevel + "jmp " + falseLabel);
}

// ������ `if`
void CD::CodeGeneration::IfElseGeneration::StartIf(
	const vector<vector<int>>& operands, // ��� �������� - ����� � ������ 
	const string& comparison, // �������� ��������� (>, <, ==, !=, >=, <=)
	std::vector<std::string>& instructions // ������� ����������
) {
	string trueLabel = GenerateLabel("IF_TRUE", IFLabelsCount);
	string endLabel = GenerateLabel("IF_END", IFLabelsCount);
	currentElseLabel = IFLabelsCount++; // ����������� ����� else
	if_stack.push(endLabel);

	nestingLevel++;

	vector<string> comments_operands;
	string tmp;
	bool isStringCmp = false;

	for (const int n : operands[0])
	{
		tmp += parent.lexem_to_source(parent.LEX_TABLE.table[n]);
	}
	comments_operands.push_back(tmp);

	tmp.clear();

	for (const int n : operands[1])
	{
		tmp += parent.lexem_to_source(parent.LEX_TABLE.table[n]);
	}
	comments_operands.push_back(tmp);

	if (operands[0].size() == 1 && operands[1].size() == 1
		&& parent.ID_TABLE.table[parent.LEX_TABLE.table[operands[0][0]].idxTI].iddatatype == IT::IDDATATYPE::STR
		&& parent.ID_TABLE.table[parent.LEX_TABLE.table[operands[1][0]].idxTI].iddatatype == IT::IDDATATYPE::STR)
	{
		CompareStrings(instructions, parent.__get_string_value(operands[0][0]), parent.__get_string_value(operands[1][0]));
		isStringCmp = true;
	}
	else
	{
		vector<string> str_operands(2, "");
		str_operands[0] = parent.__lexemVectorIDStoString(operands[0]);
		str_operands[1] = parent.__lexemVectorIDStoString(operands[1]);

		CompareInts(instructions, str_operands);
	}

	instructions.push_back(parent.tab * nestingLevel + "; ������ if " + to_string(currentElseLabel));

	// ��������� �������
	GenerateCondition(comments_operands, comparison, trueLabel, endLabel, instructions, isStringCmp);
	instructions.push_back(parent.tab * (nestingLevel - 1) + trueLabel + ':');
}

void CD::CodeGeneration::IfElseGeneration::CompareInts(std::vector<std::string>& instructions, const vector<string>& operands)
{
	auto math_instructionsRight = parent.__generate_math_expressions(operands[1]);
	auto math_instructionsLeft = parent.__generate_math_expressions(operands[0]);

	if (math_instructionsRight.size() == 1 && math_instructionsLeft.size() == 1)
	{
		instructions.push_back(parent.tab * nestingLevel + "mov eax, " + operands[0] + parent.tab + "; lefthand �������");
		instructions.push_back(parent.tab * nestingLevel + "mov ebx, " + operands[1] + parent.tab + "; righthand �������");
		instructions.push_back("");
	}
	else
	{
		instructions.push_back(parent.tab * nestingLevel + "; ��������� righthand �������:");
		for (const std::string& instr : math_instructionsRight)
		{
			instructions.push_back(parent.tab * nestingLevel + instr);
		}
		if (instructions.back() != parent.tab * nestingLevel + "push eax" && math_instructionsRight.size() != 1)
		{
			instructions.push_back(parent.tab * nestingLevel + "push eax");
		}

		if (math_instructionsLeft.size() != 1)
		{
			instructions.push_back(parent.tab * nestingLevel + "; ��������� lefthand �������:");
			for (const std::string& instr : math_instructionsLeft)
			{
				instructions.push_back(parent.tab * nestingLevel + instr);
			}
			instructions.push_back(parent.tab * nestingLevel + "; �������� lefhand ��� � eax");
		}
		else
		{
			instructions.push_back("\n" + parent.tab * nestingLevel + "mov eax, " + operands[0] + parent.tab + "; lefthand �������");
		}

		instructions.push_back(parent.tab * nestingLevel + "pop ebx ; ��������� �������� righthand ��������");
	}
}

void CD::CodeGeneration::IfElseGeneration::CompareStrings(std::vector<std::string>& instructions, const string& str1Name, const string& str2Name)
{
	instructions.push_back(parent.tab * nestingLevel + format("StrCmpCallMACRO {}, {}", str1Name, str2Name));
}

// ��������� ����� `else`
void CD::CodeGeneration::IfElseGeneration::StartElse(std::vector<std::string>& instructions) {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}
	string endLabel = if_stack.top();
	string elseLabel = GenerateLabel("ELSE", currentElseLabel); // -1 ��� ��� ��� ������� ������� � ���������, 
	//��� �� ��������� if, � else �� 1 ������

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
	instructions.push_back(parent.tab * nestingLevel + "jmp " + endLabel +
		parent.tab + "; ������� � ������ �� " + to_string(currentElseLabel) + " if-else");
}

// ���������� `if` ��� `else`
void CD::CodeGeneration::IfElseGeneration::EndExpression(std::vector<std::string>& instructions) {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}

	currentElseLabel--;

	string endLabel = if_stack.top();
	if_stack.pop();

	instructions.push_back(parent.tab * --nestingLevel + endLabel + ':');
}


std::vector<std::string> CD::CodeGeneration::IfElseGeneration::generateIfStatement(int& i)
{
	std::vector<std::string> instructions;
	int ifcounts = 0;
	while (true) {
		if (parent.LEX_TABLE.table[i].lexema[0] == '?')
		{
			i += 2; // ���������� '(' � ��������� � ������� ������� �������
			std::vector<std::vector<int>> operands(2, std::vector<int>(0));
			std::string operation;
			int currentOperand = 0;

			while (i < parent.LEX_TABLE.size && parent.LEX_TABLE.table[i].lexema[0] != '{')
			{
				switch (parent.LEX_TABLE.table[i].lexema[0]) {
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
					operands[currentOperand].push_back(i);
					break;
				}
				i++;
			} // while
			ifcounts++;
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
				auto res = parent.parse_lexem(i);
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
			i += 2;
			StartElse(instructions);

			if (parent.LEX_TABLE.table[i].lexema[0] == '?')
			{
				std::vector<std::string> insts = generateIfStatement(i);
				for (const std::string& ins : insts)
				{
					instructions.push_back(ins);
				}
			}

			continue;
		}
		break;
	}
	EndExpression(instructions);

	return instructions;
}