#include "stdafx.h"
#include "cd.h"

using namespace std;

// Перегрузка оператора << для повторения строки n раз
std::ostream& operator<<(std::ostream& os, const std::pair<std::string, int>& str_times) {
	const std::string& str = str_times.first;
	int times = str_times.second;
	for (int i = 0; i < times; ++i) {
		os << str;
	}
	return os;
}

//// для std::string и int (уже имеется)
//std::pair<std::string, int> operator*(const std::string& str, int times) {
//	return { str, times };
//}

// для std::string и int (уже имеется)
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

// Генерация условия
void CD::CodeGeneration::GenerateCondition(const vector<string>& operands, const string& comparison, const string& trueLabel, const string& falseLabel) {

	instructions.push_back(tab * nestingLevel + "; Условие: " +
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
		throw std::runtime_error("Неожиданная операция сравнения");
	}
	instructions.push_back(tab * nestingLevel + "jmp " + falseLabel);
}

// Начало `if`
void CD::CodeGeneration::StartIf(const vector<string>& operands, const string& comparison) {
	string trueLabel = GenerateLabel("IF_TRUE", nestingLevel);
	string endLabel = GenerateLabel("IF_END", nestingLevel);
	if_stack.push(endLabel);

	nestingLevel++;
	instructions.push_back(tab * nestingLevel + "; Начало if");
	instructions.push_back(tab * nestingLevel + "; Вычисляем значения операндов");

	// Генерация кода для вычисления операндов
	instructions.push_back(tab * nestingLevel + "pop eax");
	instructions.push_back(tab * nestingLevel + "pop ebx");

	// Генерация условия
	GenerateCondition(operands, comparison, trueLabel, endLabel);
	instructions.push_back(tab * (nestingLevel - 1) + trueLabel + ':');
}

// Генерация блока `else`
void CD::CodeGeneration::StartElse() {
	if (if_stack.empty()) {
		throw std::runtime_error("Ошибка: стек if-переходов пуст!");
	}
	string endLabel = if_stack.top();
	string elseLabel = GenerateLabel("ELSE", nestingLevel - 1); // -1 так как это отступы, а нумерация будет меньше на 1

	for (int i = 0; i < instructions.size(); i++)
	{
		if (instructions[i].find(endLabel) != string::npos)
		{
			instructions[i] = tab * nestingLevel + "jmp " + elseLabel;
			break;
		}

	}
	//instructions.push_back();
}

// Завершение `if` или `else`
void CD::CodeGeneration::EndIfOrElse() {
	if (if_stack.empty()) {
		throw std::runtime_error("Ошибка: стек if-переходов пуст!");
	}

	string endLabel = if_stack.top();
	instructions.push_back(tab * nestingLevel + "; Переход к выходу из этого if-else блока");
	instructions.push_back(tab * nestingLevel + "jmp " + endLabel);
}

// Завершение `if` или `else`
void CD::CodeGeneration::EndExpression() {
	if (if_stack.empty()) {
		throw std::runtime_error("Ошибка: стек if-переходов пуст!");
	}

	string endLabel = if_stack.top();
	if_stack.pop();

	instructions.push_back(tab * --nestingLevel + endLabel + ':');
}

//if (LEX_TABLE.size <= i + 1)
//{
//	throw "Таблица лексем неожиданно закончилась при разборе if-else: не удалось найти выражение ()";
//}
//else if (LEX_TABLE.table[i + 1].lexema[0] != '(')
//{
//	throw "Таблица лексем имеет неверную последовательность: ожидалась лексема '(', встречена "
//		+ to_string(LEX_TABLE.table[i + 1].lexema[0]);
//}

void CD::CodeGeneration::/*IfElseGeneration::*/generateIfStatement(int& i)
{
	while (true) {
		if (LEX_TABLE.table[i].lexema[0] == '?')
		{
			i += 2; // пропускаем '(' и переходим к первому символу условия
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
						throw "Ошибки при генерации if-else: больше одного сравнения не поддерживается";
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

			// Генерация вложенного `if`
			StartIf(operands, operation);
		}

		std::string expression = "";

		i++; // skip '{'
		while (i < LEX_TABLE.size && LEX_TABLE.table[i].lexema[0] != '}')
		{
			switch (LEX_TABLE.table[i].lexema[0]) {
			case 'v':
				expression += LEX_TABLE.table[i].v;
				break;
			case '?':
				if (!expression.empty())
				{
					OUT_ASM_FILE << tab * nestingLevel << ';' << expression;
					expression.clear();
				}
				generateIfStatement(i);
				break;
			case 'i':
			case 'l':
				expression += __getIDnameInDataSegment(ID_TABLE.table[LEX_TABLE.table[i].idxTI]);
				break;
			default:
				expression += LEX_TABLE.table[i].lexema[0];
			}
			i++;
		}
		if (!expression.empty())
		{
			instructions.push_back(tab * nestingLevel + "; code в виде лексем:");
			instructions.push_back(tab * nestingLevel + ";:: " + expression);
			expression.clear();
		}
		EndIfOrElse();
		i++;
		if (LEX_TABLE.table[i].lexema[0] == ':')
		{
			i++;
			StartElse();
			continue;
		}
		break;
	}
	EndExpression();

	for (const std::string& str : instructions)
	{
		OUT_ASM_FILE << str << '\n';
	}
}