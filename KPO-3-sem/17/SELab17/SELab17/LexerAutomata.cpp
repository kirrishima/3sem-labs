#include "stdafx.h"
#include "FST.h"
#include "LexerAutomata.h"
#include <memory>
#include <stack>
#include <iomanip>
#include <unordered_map>

bool stringFlag = false; // stringFlag
bool _isDeclare = false; // _isDeclare

LT::LexTable __LexTable = LT::Create(LT_MAXSIZE - 1);
IT::IdTable __IdTable = IT::Create(TI_MAXSIZE - 1);

char* str = new char[MAX_LEX_SIZE];
using FST::execute;

FST::FST* CreateIntegerFST(const char* str);
FST::FST* CreateStringFST(const char* str);
FST::FST* CreateFunctionFST(const char* str);
FST::FST* CreateDeclareFST(const char* str);
FST::FST* CreateReturnFST(const char* str);
FST::FST* CreateMainFST(const char* str);
FST::FST* CreatePrintFST(const char* str);
FST::FST* CreateINTLiteralFST(const char* str);
FST::FST* CreateIdentifierFST(const char* str);

const std::unordered_map<unsigned char, unsigned char> lexems = {
   {NEW_LINE, '\n'},
   {SEMICOLON, LEX_SEMICOLON},
   {COMMA, LEX_COMMA},
   {LEFT_BRACE, LEX_LEFTBRACE},
   {PLUS, LEX_PLUS},
   {MINUS, LEX_MINUS},
   {STAR, LEX_STAR},
   {DIRSLASH, LEX_DIRSLASH},
   {EQUAL, LEX_EQUAL}
};

// \0 in case of missmatch of everything
char LexAn::_determineLexeme()
{
	if (execute(*std::unique_ptr<FST::FST>(CreateIntegerFST(str))))
	{
		return LEX_INTEGER;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateStringFST(str))))
	{
		stringFlag = true;
		return LEX_STRING;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateFunctionFST(str))))
	{
		return LEX_FUNCTION;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateDeclareFST(str))))
	{
		return LEX_DECLARE;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateReturnFST(str))))
	{
		return LEX_RETURN;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateMainFST(str))))
	{
		return LEX_MAIN;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreatePrintFST(str))))
	{
		return LEX_PRINT;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateINTLiteralFST(str))))
	{
		return LEX_LITERAL;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateIdentifierFST(str))))
	{
		return LEX_ID;
	}

	return '\0';
}

void LexAn::_lexAnalize(Parm::PARM param, In::IN in)
{

	int indexIT; // Индекс для таблицы идентификаторов

	int bufferIndex = 0; // Индекс для буфера лексем'

	LT::Entry current_entry_LT; // Текущий элемент таблицы лексем
	current_entry_LT.sn = 0; // Номер строки для текущей лексемы
	current_entry_LT.idxTI = 0; // Индекс идентификатора в таблице
	current_entry_LT.lexema[0] = NULL; // Обнуление первой лексемы

	std::stack<IT::Entry*> scope; // Стек для хранения области видимости
	scope.push(NULL); // Добавление пустой области видимости

	int number_literal = 0; // Счетчик литералов
	bool addedToITFlag = false; // Флаг добавления в таблицу идентификаторов
	bool declareFunctionflag = false; // Флаг объявления функции

	IT::Entry current_entry_IT; // Текущий элемент таблицы идентификаторов
	__LexTable.size = 0; // Обнуление размера таблицы лексем
	int currentLine = 1; // Текущая строка

	std::ofstream LT_file("LT.txt"); // Файл для записи таблицы лексем
	std::ofstream IT_file("IT.txt"); // Файл для записи таблицы идентификаторов

	for (int i = 0; i < in.size; i++)
	{
		if (((in.text[i] >= 'A' && in.text[i] <= 'Z') || (in.text[i] >= 'a' && in.text[i] <= 'z') ||
			(in.text[i] >= '0' && in.text[i] <= '9') || in.text[i] == '\'') && in.text[i] != ' ')
		{
			str[bufferIndex++] = in.text[i]; // Сохранение символа в буфер

			if (bufferIndex >= MAX_LEX_SIZE) {
				throw ERROR_THROW(119); // Ошибка при переполнении буфера
			}
		}
		else
		{
			str[bufferIndex] = '\0'; // Завершение строки
			current_entry_LT.lexema[0] = _determineLexeme(); // Получение первой лексемы

			int lexemeLength = strlen(str) + 1;

			// Обработка специальной лексемы "main"
			switch (current_entry_LT.lexema[0])
			{
			case LEX_MAIN:
			{
				current_entry_LT.idxTI = __IdTable.size; // Установка индекса идентификатора
				memcpy(current_entry_IT.id, str, ID_SIZE); // Копирование идентификатора
				current_entry_IT.id[ID_SIZE] = '\0'; // Завершение идентификатора
				current_entry_IT.iddatatype = IT::INT; // Установка типа данных
				current_entry_IT.idtype = IT::F; // Установка типа идентификатора как функции
				current_entry_IT.value.vint = NULL; // Инициализация значения
				current_entry_IT.idxfirstLE = currentLine; // Установка первой строки
				current_entry_IT.scope = NULL; // Установка области видимости

				// Поиск идентификатора в таблице
				indexIT = IT::search(__IdTable, current_entry_IT);
				if (indexIT != -1) {
					throw ERROR_THROW(105); // Ошибка, если идентификатор уже существует
				}

				current_entry_LT.idxTI = __IdTable.size; // Установка индекса
				IT::Add(__IdTable, current_entry_IT); // Добавление в таблицу идентификаторов

				break;
			}

			case LEX_LITERAL:
			{
				current_entry_LT.idxTI = __IdTable.size;
				sprintf_s(current_entry_IT.id, "L%d", number_literal);
				current_entry_IT.iddatatype = IT::INT;
				current_entry_IT.idtype = IT::L;
				current_entry_IT.idxfirstLE = currentLine;
				current_entry_IT.value.vint = atoi(str);
				current_entry_IT.scope = scope.top();
				current_entry_LT.idxTI = __IdTable.size;
				IT::Add(__IdTable, current_entry_IT);
				number_literal++;

				break;
			}

			case LEX_ID:
			{
				if (scope.empty())
					current_entry_IT.scope = NULL; // Если стек пуст, то текущему идентификатору current_entry_IT устанавливается область видимости (scope) равной NULL
				else
					current_entry_IT.scope = scope.top(); // устанавливается вершина стека как область видимости текущего идентификатора.
				current_entry_LT.idxTI = __IdTable.size;
				memcpy(current_entry_IT.id, str, ID_SIZE);
				current_entry_IT.id[ID_SIZE] = '\0';
				current_entry_IT.iddatatype = IT::INT;
				current_entry_IT.value.vint = NULL; // не инициализирован
				current_entry_IT.idxfirstLE = currentLine;
				current_entry_IT.idtype = IT::V;

				if (__LexTable.table[__LexTable.size - 2].lexema[0] == LEX_DECLARE)
				{
					std::cout << __LexTable.table[__LexTable.size - 1].lexema[0];
					if (__LexTable.table[__LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
					{
						current_entry_IT.iddatatype = IT::STR;
						strcpy_s(current_entry_IT.value.vstr->str, "");
						stringFlag = false;
					}
					indexIT = IT::search(__IdTable, current_entry_IT); // поиск текущего идентификатора в таблице идентификаторов
					if (indexIT >= 0)								   // Если идентификатор уже существует
					{
						throw ERROR_THROW(105);
					}
					if (indexIT == -1)
					{
						current_entry_LT.idxTI = __IdTable.size;
						IT::Add(__IdTable, current_entry_IT);
						addedToITFlag = true;
					}
				}
				if (__LexTable.table[__LexTable.size - 1].lexema[0] == LEX_FUNCTION)
				{
					current_entry_IT.idtype = IT::F;
					declareFunctionflag = true;
					if (__LexTable.table[__LexTable.size - 2].lexema[0] == LEX_STRING && stringFlag)
					{
						current_entry_IT.iddatatype = IT::STR;
						strcpy_s(current_entry_IT.value.vstr->str, "");
						stringFlag = false;
					}
					indexIT = IT::search(__IdTable, current_entry_IT);
					if (indexIT >= 0)
					{
						throw ERROR_THROW(105);
					}
					if (indexIT == -1)
					{

						current_entry_LT.idxTI = __IdTable.size;
						IT::Add(__IdTable, current_entry_IT);
						addedToITFlag = true;
					}
				}
				if (__LexTable.table[__LexTable.size - 2].lexema[0] == LEX_LEFTTHESIS &&
					__LexTable.table[__LexTable.size - 3].lexema[0] == LEX_ID &&
					__LexTable.table[__LexTable.size - 3].idxTI == __IdTable.size - 1 &&
					__IdTable.table[__IdTable.size - 1].idtype == IT::F) // текущий идентификатор - параметр функции
				{
					current_entry_IT.idtype = IT::P;
					if (__LexTable.table[__LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
					{
						current_entry_IT.iddatatype = IT::STR;
						strcpy_s(current_entry_IT.value.vstr->str, "");
						stringFlag = false;
					}
					indexIT = IT::search(__IdTable, current_entry_IT);
					if (indexIT >= 0)
					{
						throw ERROR_THROW(105);
					}
					current_entry_LT.idxTI = __IdTable.size;
					IT::Add(__IdTable, current_entry_IT);
					addedToITFlag = true;
				}
				if (__LexTable.table[__LexTable.size - 2].lexema[0] == LEX_COMMA && __IdTable.table[__LexTable.table[__LexTable.size - 2].idxTI].idtype == IT::P)
				{
					current_entry_IT.idtype = IT::P;
					if (__LexTable.table[__LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
					{
						current_entry_IT.iddatatype = IT::STR;
						strcpy_s(current_entry_IT.value.vstr->str, "");
						stringFlag = false;
					}
					indexIT = IT::search(__IdTable, current_entry_IT);
					if (indexIT >= 0)
					{
						throw ERROR_THROW(105);
					}
					IT::Add(__IdTable, current_entry_IT);
					addedToITFlag = true;
				}
				if (!addedToITFlag)
				{
					indexIT = IT::search(__IdTable, current_entry_IT);
					if (indexIT >= 0)
					{
						for (int i = 0; i < strlen(current_entry_IT.id); i++)
							std::cout << current_entry_IT.id[i];
						std::cout << __IdTable.table[indexIT].scope->id;
						std::cout << std::endl;
						current_entry_LT.idxTI = indexIT;
					}
				}
				std::memset(current_entry_IT.id, NULL, 5);
				current_entry_IT.iddatatype = IT::INT;
				current_entry_IT.value.vint = NULL;
				addedToITFlag = false;

				break;
			}
			default:
				break;
			}

			std::memset(str, NULL, bufferIndex);
			bufferIndex = 0;
		}

		if (current_entry_LT.lexema[0] != NULL)
		{
			current_entry_LT.sn = currentLine;
			LT::Add(__LexTable, current_entry_LT);
			current_entry_LT.lexema[0] = NULL;
		}

		if (lexems.find(in.text[i]) != lexems.end())
		{
			current_entry_LT.lexema[0] = lexems.at(in.text[i]);
			current_entry_LT.sn = currentLine++;
			LT::Add(__LexTable, current_entry_LT);
			current_entry_LT.lexema[0] = NULL;
			break;
		}

		else
		{
			switch (in.text[i])
			{
			case MARK:
				if (str[0] == '\'' && bufferIndex != 1)
				{
					current_entry_LT.idxTI = __IdTable.size;
					str[bufferIndex] = '\0';
					current_entry_LT.lexema[0] = LEX_LITERAL;
					number_literal++;
					sprintf_s(current_entry_IT.id, "L%d", number_literal);
					current_entry_IT.iddatatype = IT::STR;
					current_entry_IT.idtype = IT::L;
					current_entry_IT.idxfirstLE = currentLine;
					for (int i = 0; i < strlen(str); i++)
					{
						current_entry_IT.value.vstr->str[i] = str[i];
					}
					current_entry_IT.value.vstr->str[strlen(str)] = '\0';
					current_entry_IT.value.vstr->len = strlen(current_entry_IT.value.vstr->str);
					current_entry_LT.sn = currentLine;
					current_entry_IT.scope = scope.top();
					LT::Add(__LexTable, current_entry_LT);
					IT::Add(__IdTable, current_entry_IT);
					current_entry_LT.lexema[0] = NULL;
					break;
				}
				break;
			case LEFT_BRACE:
				current_entry_LT.lexema[0] = LEX_LEFTBRACE;
				current_entry_LT.sn = currentLine;
				LT::Add(__LexTable, current_entry_LT);
				current_entry_LT.lexema[0] = NULL;
				for (int j = __IdTable.size - 1; j >= 0; j--)
				{
					if (__IdTable.table[j].idtype == IT::F)
					{
						scope.push(&__IdTable.table[j]);
						break;
					}
				}
				break;
			case RIGHT_BRACE:
				current_entry_LT.lexema[0] = LEX_RIGHTBRACE;
				current_entry_LT.sn = currentLine;
				LT::Add(__LexTable, current_entry_LT);
				current_entry_LT.lexema[0] = NULL;
				if (!scope.empty())
					scope.pop();
				break;

			case LEFTTHESIS:
				current_entry_LT.lexema[0] = LEX_LEFTTHESIS;
				current_entry_LT.sn = currentLine;
				LT::Add(__LexTable, current_entry_LT);
				current_entry_LT.lexema[0] = NULL;
				if (declareFunctionflag)
				{
					for (int j = __IdTable.size - 1; j >= 0; j--)
					{
						if (__IdTable.table[j].idtype == IT::F)
						{
							scope.push(&__IdTable.table[j]);
							break;
						}
					}
				}
				break;
			case RIGHTTHESIS:
				current_entry_LT.lexema[0] = LEX_RIGHTTHESIS;
				current_entry_LT.sn = currentLine;
				LT::Add(__LexTable, current_entry_LT);
				current_entry_LT.lexema[0] = NULL;
				if (!scope.empty() && declareFunctionflag)
				{
					scope.pop();
					declareFunctionflag = false;
				}
				break;
			}
		}
	}
	currentLine = 1;
	LT_file << currentLine;
	LT_file << '\t';
	for (int i = 0; i < __LexTable.size; i++)
	{
		current_entry_LT = LT::GetEntry(__LexTable, i);
		if (currentLine != current_entry_LT.sn)
		{
			currentLine = current_entry_LT.sn;
			LT_file << currentLine;
			LT_file << '\t';
		}
		LT_file << current_entry_LT.lexema[0];
	}
	LT_file.close();
	IT_file << std::setw(5) << "id"
		<< std::setw(10) << "datatype"
		<< std::setw(10) << "idtype"
		<< std::setw(10) << "Line"
		<< std::setw(10) << "value"
		<< std::setw(10) << "Scope" << std::endl;

	for (int i = 0; i < __IdTable.size; i++)
	{
		current_entry_IT = IT::GetEntry(__IdTable, i);
		IT_file << std::setw(5) << current_entry_IT.id;
		if (current_entry_IT.iddatatype == 1)
			IT_file << std::setw(10) << "INT";
		if (current_entry_IT.iddatatype == 2)
			IT_file << std::setw(10) << "STR";
		if (current_entry_IT.idtype == IT::V)
			IT_file << std::setw(10) << "V";
		if (current_entry_IT.idtype == IT::L)
			IT_file << std::setw(10) << "L";
		if (current_entry_IT.idtype == IT::F)
			IT_file << std::setw(10) << "F";
		if (current_entry_IT.idtype == IT::P)
			IT_file << std::setw(10) << "P";
		IT_file << std::setw(10) << current_entry_IT.idxfirstLE;

		if (current_entry_IT.iddatatype == IT::INT)
		{
			IT_file << std::setw(10) << current_entry_IT.value.vint;
		}
		if (current_entry_IT.iddatatype == IT::STR)
		{
			IT_file << std::setw(7);
			for (int j = 0; j < strlen(current_entry_IT.value.vstr->str); j++)
			{
				IT_file << current_entry_IT.value.vstr->str[j];
			}
			IT_file << std::setw(10);
		}
		IT_file << std::setw(10);
		if (current_entry_IT.scope != NULL)
		{
			for (int j = 0; j < strlen(current_entry_IT.scope->id); j++)
			{
				IT_file << current_entry_IT.scope->id[j];
			}
		}
		IT_file << std::endl;
	}
	IT_file.close();
}

FST::FST* CreateIntegerFST(const char* str)
{
	return new FST::FST(str,
		8,
		FST::NODE(1, FST::RELATION('i', 1)),
		FST::NODE(1, FST::RELATION('n', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('e', 4)),
		FST::NODE(1, FST::RELATION('g', 5)),
		FST::NODE(1, FST::RELATION('e', 6)),
		FST::NODE(1, FST::RELATION('r', 7)),
		FST::NODE()
	);
}

FST::FST* CreateStringFST(const char* str)
{
	return new FST::FST(str,
		7,
		FST::NODE(1, FST::RELATION('s', 1)),
		FST::NODE(1, FST::RELATION('t', 2)),
		FST::NODE(1, FST::RELATION('r', 3)),
		FST::NODE(1, FST::RELATION('i', 4)),
		FST::NODE(1, FST::RELATION('n', 5)),
		FST::NODE(1, FST::RELATION('g', 6)),
		FST::NODE()
	);
}

FST::FST* CreateFunctionFST(const char* str)
{
	return new FST::FST(str,
		9,
		FST::NODE(1, FST::RELATION('f', 1)),
		FST::NODE(1, FST::RELATION('u', 2)),
		FST::NODE(1, FST::RELATION('n', 3)),
		FST::NODE(1, FST::RELATION('c', 4)),
		FST::NODE(1, FST::RELATION('t', 5)),
		FST::NODE(1, FST::RELATION('i', 6)),
		FST::NODE(1, FST::RELATION('o', 7)),
		FST::NODE(1, FST::RELATION('n', 8)),
		FST::NODE()
	);
}

FST::FST* CreateDeclareFST(const char* str)
{
	return new FST::FST(str,
		8,
		FST::NODE(1, FST::RELATION('d', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('c', 3)),
		FST::NODE(1, FST::RELATION('l', 4)),
		FST::NODE(1, FST::RELATION('a', 5)),
		FST::NODE(1, FST::RELATION('r', 6)),
		FST::NODE(1, FST::RELATION('e', 7)),
		FST::NODE()
	);
}

FST::FST* CreateReturnFST(const char* str)
{
	return new FST::FST(str,
		7,
		FST::NODE(1, FST::RELATION('r', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('u', 4)),
		FST::NODE(1, FST::RELATION('r', 5)),
		FST::NODE(1, FST::RELATION('n', 6)),
		FST::NODE()
	);
}

FST::FST* CreateMainFST(const char* str)
{
	return new FST::FST(str,
		5,
		FST::NODE(1, FST::RELATION('m', 1)),
		FST::NODE(1, FST::RELATION('a', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE()
	);
}

FST::FST* CreatePrintFST(const char* str)
{
	return new FST::FST(str,
		6,
		FST::NODE(1, FST::RELATION('p', 1)),
		FST::NODE(1, FST::RELATION('r', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE(1, FST::RELATION('t', 5)),
		FST::NODE()
	);
}

FST::FST* CreateINTLiteralFST(const char* str)
{
	return new FST::FST(str,
		2,
		FST::NODE(20,
			FST::RELATION('0', 0), FST::RELATION('1', 0), FST::RELATION('2', 0),
			FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0),
			FST::RELATION('6', 0), FST::RELATION('7', 0), FST::RELATION('8', 0),
			FST::RELATION('9', 0), FST::RELATION('0', 1), FST::RELATION('1', 1),
			FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
			FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1),
			FST::RELATION('8', 1), FST::RELATION('9', 1)),
		FST::NODE()
	);
}

constexpr int NUM_LETTERS = 26;

#define relationsForState(state) FST::RELATION('a', state), FST::RELATION('b', state), FST::RELATION('c', state),\
	FST::RELATION('d', state), FST::RELATION('e', state), FST::RELATION('f', state),\
	FST::RELATION('g', state), FST::RELATION('h', state), FST::RELATION('i', state),\
	FST::RELATION('j', state), FST::RELATION('k', state), FST::RELATION('l', state),\
	FST::RELATION('m', state), FST::RELATION('n', state), FST::RELATION('o', state),\
	FST::RELATION('p', state), FST::RELATION('q', state), FST::RELATION('r', state),\
	FST::RELATION('s', state), FST::RELATION('t', state), FST::RELATION('u', state),\
	FST::RELATION('v', state), FST::RELATION('w', state), FST::RELATION('x', state),\
	FST::RELATION('y', state), FST::RELATION('z', state)

FST::FST* CreateIdentifierFST(const char* str)
{
	return new FST::FST(str,
		6,
		FST::NODE(NUM_LETTERS, relationsForState(1)),
		FST::NODE(NUM_LETTERS + 1, relationsForState(2), FST::RELATION('\0', 2)),
		FST::NODE(NUM_LETTERS + 1, relationsForState(3), FST::RELATION('\0', 3)),
		FST::NODE(NUM_LETTERS + 1, relationsForState(4), FST::RELATION('\0', 4)),
		FST::NODE(NUM_LETTERS + 1, relationsForState(5), FST::RELATION('\0', 5)),
		FST::NODE()
	);
}
#undef relationsForState
