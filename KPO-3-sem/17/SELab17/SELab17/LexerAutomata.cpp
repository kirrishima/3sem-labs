#include "stdafx.h"
#include "LexerAutomata.h"
#include "FST.h"
#include <memory>
#include <stack>

using FST::execute;

namespace
{
	FST::FST* CreateIntegerFST(const char* _str);
	FST::FST* CreateStringFST(const char* _str);
	FST::FST* CreateFunctionFST(const char* _str);
	FST::FST* CreateDeclareFST(const char* _str);
	FST::FST* CreateReturnFST(const char* _str);
	FST::FST* CreateMainFST(const char* _str);
	FST::FST* CreatePrintFST(const char* _str);
	FST::FST* CreateINTLiteralFST(const char* _str);
	FST::FST* CreateIdentifierFST(const char* _str);
}

// \0 in case of missmatch of everything
char LexAn::LexicalAnalyzer::_determineLexeme()
{
	if (execute(*std::unique_ptr<FST::FST>(CreateIntegerFST(_str))))
	{
		return LEX_INTEGER;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateStringFST(_str))))
	{
		_isStringLiteral = true;
		return LEX_STRING;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateFunctionFST(_str))))
	{
		return LEX_FUNCTION;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateDeclareFST(_str))))
	{
		return LEX_DECLARE;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateReturnFST(_str))))
	{
		return LEX_RETURN;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateMainFST(_str))))
	{
		return LEX_MAIN;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreatePrintFST(_str))))
	{
		return LEX_PRINT;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateINTLiteralFST(_str))))
	{
		return LEX_LITERAL;
	}

	if (execute(*std::unique_ptr<FST::FST>(CreateIdentifierFST(_str))))
	{
		return LEX_ID;
	}

	return '\0';
}

void LexAn::LexicalAnalyzer::_lexAnalize(Parm::PARM  param, In::IN in)
{
	int indexIT; // Индекс для таблицы идентификаторов
	setlocale(LC_ALL, "rus"); // Установка локали для русскоязычных символов

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
	_lexTable.size = 0; // Обнуление размера таблицы лексем
	int currentLine = 1; // Текущая строка

	std::ofstream LT_file("LT.txt"); // Файл для записи таблицы лексем
	std::ofstream IT_file("IT.txt"); // Файл для записи таблицы идентификаторов

	for (int i = 0; i < in.size; i++)
	{
		if (((in.text[i] >= 'A' && in.text[i] <= 'Z') || (in.text[i] >= 'a' && in.text[i] <= 'z') ||
			(in.text[i] >= '0' && in.text[i] <= '9') || in.text[i] == '\'') && in.text[i] != ' ') {
			_str[bufferIndex++] = in.text[i]; // Сохранение символа в буфер

			if (bufferIndex >= MAX_LEX_SIZE) {
				throw ERROR_THROW(119); // Ошибка при переполнении буфера
			}

			else
			{
				_str[bufferIndex] = '\0'; // Завершение строки
				current_entry_LT.lexema[0] = _determineLexeme(); // Получение первой лексемы

				int lexemeLength = strlen(_str) + 1;

				// Обработка специальной лексемы "main"
				if (current_entry_LT.lexema[0] == LEX_MAIN) {
					current_entry_LT.idxTI = _idTable.size; // Установка индекса идентификатора
					memcpy(current_entry_IT.id, _str, 5); // Копирование идентификатора
					current_entry_IT.id[5] = '\0'; // Завершение идентификатора
					current_entry_IT.iddatatype = IT::INT; // Установка типа данных
					current_entry_IT.idtype = IT::F; // Установка типа идентификатора как функции
					current_entry_IT.value.vint = NULL; // Инициализация значения
					current_entry_IT.idxfirstLE = currentLine; // Установка первой строки
					current_entry_IT.scope = NULL; // Установка области видимости

					// Поиск идентификатора в таблице
					indexIT = IT::search(_idTable, current_entry_IT);
					if (indexIT != -1) {
						throw ERROR_THROW(105); // Ошибка, если идентификатор уже существует
					}

					current_entry_LT.idxTI = _idTable.size; // Установка индекса
					IT::Add(_idTable, &current_entry_IT); // Добавление в таблицу идентификаторов
				}

			}
		}
	}

}


namespace
{
	FST::FST* CreateIntegerFST(const char* _str)
	{
		return new FST::FST(_str,
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

	FST::FST* CreateStringFST(const char* _str)
	{
		return new FST::FST(_str,
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

	FST::FST* CreateFunctionFST(const char* _str)
	{
		return new FST::FST(_str,
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

	FST::FST* CreateDeclareFST(const char* _str)
	{
		return new FST::FST(_str,
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

	FST::FST* CreateReturnFST(const char* _str)
	{
		return new FST::FST(_str,
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

	FST::FST* CreateMainFST(const char* _str)
	{
		return new FST::FST(_str,
			5,
			FST::NODE(1, FST::RELATION('m', 1)),
			FST::NODE(1, FST::RELATION('a', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE()
		);
	}

	FST::FST* CreatePrintFST(const char* _str)
	{
		return new FST::FST(_str,
			6,
			FST::NODE(1, FST::RELATION('p', 1)),
			FST::NODE(1, FST::RELATION('r', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE()
		);
	}

	FST::FST* CreateINTLiteralFST(const char* _str)
	{
		return new FST::FST(_str,
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

	FST::FST* CreateIdentifierFST(const char* _str)
	{
		return new FST::FST(_str,
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

}