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

std::vector<std::string> instructions;

// ��������� �������
void CD::CodeGeneration::GenerateCondition(const vector<string>& operands, const string& comparison, const string& trueLabel, const string& falseLabel) {

	instructions.push_back(tab * nestingLevel + "; �������: " +
		operands[0] + " " + comparison + operands[1]);

	instructions.push_back(tab * nestingLevel + "cmp eax, ebx");
	if (comparison == ">") {
		instructions.push_back(tab * nestingLevel + "jg " + trueLabel);
	}
	else if (comparison == "<") {
		instructions.push_back(tab * nestingLevel + "jl " + trueLabel);
	}
	else if (comparison == "=") {
		instructions.push_back(tab * nestingLevel + "je " + trueLabel);
	}
	else if (comparison == ">=") {
		instructions.push_back(tab * nestingLevel + "jge " + trueLabel);
	}
	else if (comparison == "<=") {
		instructions.push_back(tab * nestingLevel + "jle " + trueLabel);
	}
	else {
		throw std::runtime_error("����������� �������� ���������");
	}
	instructions.push_back(tab * nestingLevel + "jmp " + falseLabel);
}

// ������ `if`
void CD::CodeGeneration::StartIf(const vector<string>& operands, const string& comparison) {
	string trueLabel = GenerateLabel("IF_TRUE", nestingLevel);
	string endLabel = GenerateLabel("IF_END", nestingLevel);
	if_stack.push(endLabel);

	nestingLevel++;
	instructions.push_back(tab * nestingLevel + "; ������ if");
	instructions.push_back(tab * nestingLevel + "; ��������� �������� ���������");

	// ��������� ���� ��� ���������� ���������
	instructions.push_back(tab * nestingLevel + "pop eax");
	instructions.push_back(tab * nestingLevel + "pop ebx");

	// ��������� �������
	GenerateCondition(operands, comparison, trueLabel, endLabel);
	instructions.push_back(tab * (nestingLevel - 1) + trueLabel + ':');
}

// ��������� ����� `else`
void CD::CodeGeneration::StartElse() {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}
	string endLabel = if_stack.top();
	string elseLabel = GenerateLabel("ELSE", nestingLevel - 1); // -1 ��� ��� ��� �������, � ��������� ����� ������ �� 1

	for (int i = 0; i < instructions.size(); i++)
	{
		if (instructions[i].find(endLabel) != string::npos)
		{
			instructions[i] = tab * nestingLevel + "jmp " + elseLabel;
			break;
		}

	}
	instructions.push_back(tab * (nestingLevel - 1) + elseLabel + ':');
}

// ���������� `if` ��� `else`
void CD::CodeGeneration::EndIfOrElse() {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}

	string endLabel = if_stack.top();
	instructions.push_back(tab * nestingLevel + "; ������� � ������ �� ����� if-else �����");
	instructions.push_back(tab * nestingLevel + "jmp " + endLabel);
}

// ���������� `if` ��� `else`
void CD::CodeGeneration::EndExpression() {
	if (if_stack.empty()) {
		throw std::runtime_error("������: ���� if-��������� ����!");
	}

	string endLabel = if_stack.top();
	if_stack.pop();

	instructions.push_back(tab * --nestingLevel + endLabel + ':');
}

//if (LEX_TABLE.size <= i + 1)
//{
//	throw "������� ������ ���������� ����������� ��� ������� if-else: �� ������� ����� ��������� ()";
//}
//else if (LEX_TABLE.table[i + 1].lexema[0] != '(')
//{
//	throw "������� ������ ����� �������� ������������������: ��������� ������� '(', ��������� "
//		+ to_string(LEX_TABLE.table[i + 1].lexema[0]);
//}

bool wasLastInstructionElse = false;

void CD::CodeGeneration::/*IfElseGeneration::*/generateIfStatement(int& i)
{
	while (true) {
		if (LEX_TABLE.table[i].lexema[0] == '?')
		{
			wasLastInstructionElse = false;
			i += 2; // ���������� '(' � ��������� � ������� ������� �������
			std::vector<std::string> operands(2, "");
			std::string operation;
			int currentOperand = 0;

			while (i < LEX_TABLE.size && LEX_TABLE.table[i].lexema[0] != '{')
			{
				switch (LEX_TABLE.table[i].lexema[0]) {
				case 'i':
				case 'l':
					operands[currentOperand] += __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[i].idxTI]);
					break;

				case 'c':
					operation = LEX_TABLE.table[i].c;
					currentOperand++;
					if (currentOperand != 1)
					{
						throw "������ ��� ��������� if-else: ������ ������ ��������� �� ��������������";
					}
					break;
				case ')':
					if (LEX_TABLE.table[i + 1].lexema[0] == '{')
					{
						break;
					}
				default:
					operands[currentOperand] += LEX_TABLE.table[i].lexema[0];
					break;
				}
				i++;
			} // while

			auto math_instructions = __generate_math_expressions(operands[1]);

			for (const std::string& instr : math_instructions)
			{
				instructions.push_back(tab * (nestingLevel + 1) + instr);
			}

			math_instructions = __generate_math_expressions(operands[0]);

			for (const std::string& instr : math_instructions)
			{
				instructions.push_back(tab * (nestingLevel + 1) + instr);
			}

			// ��������� ���������� `if`
			StartIf(operands, operation);
		}

		//std::string expression = "";

		//i++; // skip '{'

		while (i < LEX_TABLE.size && LEX_TABLE.table[i].lexema[0] != '}')
		{
			switch (LEX_TABLE.table[i].lexema[0]) {

			case '?':
				//if (!expression.empty())
				//{
				//	instructions.push_back(tab * nestingLevel + ';' + expression);
				//	expression.clear();
				//}
				generateIfStatement(i);
				//if (LEX_TABLE.table[i].lexema[0] == '}')
				//{
				//	i--;
				//}
				break;
			case '=':
			case 'p':
			{
				auto res = parse_expression(i);
				for (const std::string& s : res)
				{
					instructions.push_back(tab * nestingLevel + s);
				}
				break;
			}
			default:
				//expression += lexem_to_source(LEX_TABLE.table[i]);
				//expression += ' ';
				break;
			}
			i++;
		}
		//if (!expression.empty())
		//{
		//	instructions.push_back(tab * nestingLevel + "; code � ���� ������:");
		//	instructions.push_back(tab * nestingLevel + ";:: " + expression);
		//	expression.clear();
		//}
		EndIfOrElse();

		while (i < LEX_TABLE.size && LEX_TABLE.table[i].lexema[0] == '}') i++;

		//if (LEX_TABLE.table[i + nestingLevel - 1].lexema[0] == ':')
		//{
		//	//if (if_stack.size() > 1)
		//	//{
		//	//	EndExpression();
		//	//}
		//	i += nestingLevel - 1;
		//}
		if (LEX_TABLE.table[i].lexema[0] == ':')
		{

			wasLastInstructionElse = true;
			i++;
			StartElse();
			while (i < LEX_TABLE.size && LEX_TABLE.table[i].lexema[0] != '}')
			{
				switch (LEX_TABLE.table[i].lexema[0]) {

				case '?':
					//if (!expression.empty())
					//{
					//	instructions.push_back(tab * nestingLevel + ';' + expression);
					//	expression.clear();
					//}
					generateIfStatement(i);
					//if (LEX_TABLE.table[i].lexema[0] == '}')
					//{
					//	i--;
					//}
					break;
				case '=':
				case 'p':
				{
					auto res = parse_expression(i);
					for (const std::string& s : res)
					{
						instructions.push_back(tab * nestingLevel + s);
					}
					break;
				}
				default:
					//expression += lexem_to_source(LEX_TABLE.table[i]);
					//expression += ' ';
					break;
				}
				i++;
			}
		}
		break;
	}
	EndIfOrElse();
	EndExpression();


	for (const std::string& str : instructions)
	{
		OUT_ASM_FILE << str << '\n';
	}
	instructions.clear();
}