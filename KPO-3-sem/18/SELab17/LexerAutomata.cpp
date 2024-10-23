#include "stdafx.h"
#include "LexerAutomata.h"
#include "In.h"
#include "Error.h"
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <stack>
#include "SVV.h"
#include "Utils.h"

using namespace std;
using namespace SVV;

namespace LexAn
{
	LT::LexTable LexTable = LT::Create(LT_MAXSIZE - 1);  //Таблица для хранения лексем
	IT::ID_Table ID_Table = IT::Create(TI_MAXSIZE - 1);    //Таблица для хранения идентификаторов

	//различные состояния программы
	bool stringFlag = false;
	bool parmFlag = false;
	bool functionFlag = false;
	bool mainFlag = false;
	bool declareFunctionflag = false;
	bool addedToITFlag = false; // Флаг добавления в таблицу идентификаторов
	bool isMarkOpened = false;


	char* str = new char[MAX_LEX_SIZE];    //хранение текущей лексемы

	FST::FST* IntegerFST(CreateIntegerFST(str));
	FST::FST* StringFST(CreateStringFST(str));
	FST::FST* FunctionFST(CreateFunctionFST(str));
	FST::FST* DeclareFST(CreateDeclareFST(str));
	FST::FST* ReturnFST(CreateReturnFST(str));
	FST::FST* MainFST(CreateMainFST(str));
	FST::FST* PrintFST(CreatePrintFST(str));
	FST::FST* INTLiteralFST(CreateINTLiteralFST(str));
	FST::FST* IdentifierFST(CreateIdentifierFST(str));

	// \0 in case of mismatch of everything
	char determineLexeme()
	{
		if (execute(*IntegerFST))
		{
			return LEX_INTEGER;
		}

		if (execute(*StringFST))
		{
			stringFlag = true;
			return LEX_STRING;
		}

		if (execute(*FunctionFST))
		{
			return LEX_FUNCTION;
		}

		if (execute(*DeclareFST))
		{
			return LEX_DECLARE;
		}

		if (execute(*ReturnFST))
		{
			return LEX_RETURN;
		}

		if (execute(*MainFST))
		{
			return LEX_MAIN;
		}

		if (execute(*PrintFST))
		{
			return LEX_PRINT;
		}

		if (execute(*INTLiteralFST))
		{
			return LEX_LITERAL;
		}

		if (execute(*IdentifierFST))
		{
			return LEX_ID;
		}

		return '\0';
	}


	LT::LexTable lexAnalize(Parm::PARM parm, In::IN in)
	{
		int indexIT; // Индекс для таблицы идентификаторов

		int bufferIndex = 0; // Индекс для буфера лексем'

		LT::Entry LT_entry; // Текущий элемент таблицы лексем
		LT_entry.sn = 0; // Номер строки для текущей лексемы
		LT_entry.idxTI = 0; // Индекс идентификатора в таблице
		LT_entry.lexema[0] = NULL; // Обнуление первой лексемы

		std::stack<IT::Entry*> scope; // Стек для хранения области видимости
		scope.push(NULL); // Добавление пустой области видимости

		int literalsCount = 0; // Счетчик литералов
		bool addedToITFlag = false; // Флаг добавления в таблицу идентификаторов
		bool declareFunctionflag = false; // Флаг объявления функции

		IT::Entry IT_entry; // Текущий элемент таблицы идентификаторов
		LexTable.size = 0; // Обнуление размера таблицы лексем
		int currentLine = 1; // Текущая строка


		for (int i = 0; i < in.size; i++)
		{
			if ((in.code[(int)in.text[i]] == In::IN::T || in.text[i] == '\''))
			{
				str[bufferIndex++] = in.text[i];
				if (bufferIndex >= MAX_LEX_SIZE)
				{
					throw ERROR_THROW(119);
				}
			}
			else
			{
				str[bufferIndex] = '\0';
				LT_entry.lexema[0] = determineLexeme();
				switch (LT_entry.lexema[0])
				{
				case LEX_MAIN:
					mainFlag = true;
					LT_entry.idxTI = ID_Table.size;
					memcpy(IT_entry.id, str, 5);

					IT_entry.id[5] = '\0';
					IT_entry.iddatatype = IT::INT;
					IT_entry.value.vint = NULL;
					IT_entry.idxfirstLE = currentLine;
					IT_entry.scope = NULL;

					indexIT = IT::search(ID_Table, IT_entry);

					if (indexIT != -1)
					{
						throw ERROR_THROW(105);
					}
					IT::Add(ID_Table, IT_entry);

					break;

				case LEX_LITERAL:
					LT_entry.idxTI = ID_Table.size;
					sprintf_s(IT_entry.id, "L%d", ++literalsCount);

					IT_entry.iddatatype = IT::INT;
					IT_entry.idtype = IT::L;
					IT_entry.idxfirstLE = currentLine;
					IT_entry.value.vint = atoi(str);
					IT_entry.scope = scope.top();

					LT_entry.idxTI = ID_Table.size; // Установка индекса для лексемы
					IT::Add(ID_Table, IT_entry);
					break;
				case LEX_ID:
					// Проверка области видимости
					if (scope.empty())
						IT_entry.scope = NULL; // Если стек пуст, область видимости равна NULL
					else
						IT_entry.scope = scope.top(); // Установка области видимости как вершину стека

					LT_entry.idxTI = ID_Table.size; // Установка индекса для лексемы
					memcpy(IT_entry.id, str, ID_SIZE); // Копирование идентификатора в таблицу идентификаторов

					IT_entry.id[ID_SIZE] = '\0'; // Завершение строки идентификатора
					IT_entry.iddatatype = IT::INT; // Установка типа данных идентификатора
					IT_entry.value.vint = NULL; // Значение не инициализировано
					IT_entry.idxfirstLE = currentLine; // Номер строки, где находится идентификатор
					IT_entry.idtype = IT::V; // Установка типа идентификатора как переменной

					// Обработка случая, если предыдущая лексема - это объявление
					if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_DECLARE)
					{
						// Если последняя лексема - строка и установлен флаг
						if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
						{
							IT_entry.iddatatype = IT::STR; // Установка типа данных идентификатора как строки
							strcpy_s(IT_entry.value.vstr->str, ""); // Инициализация строки
							stringFlag = false; // Сброс флага
						}
						indexIT = IT::search(ID_Table, IT_entry); // Поиск идентификатора в таблице

						if (indexIT != -1) // Если идентификатор уже существует
						{
							throw ERROR_THROW(105); // Ошибка: идентификатор уже существует
						}

						LT_entry.idxTI = ID_Table.size; // Установка индекса для лексемы
						IT::Add(ID_Table, IT_entry); // Добавление идентификатора в таблицу
						addedToITFlag = true; // Установка флага добавления
					}

					// Обработка случая, если предыдущая лексема - это функция
					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_FUNCTION)
					{
						IT_entry.idtype = IT::F; // Установка типа идентификатора как функции
						declareFunctionflag = true; // Установка флага объявления функции
						// Если предыдущая лексема - строка и установлен флаг
						if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING && stringFlag)
						{
							IT_entry.iddatatype = IT::STR; // Установка типа данных идентификатора как строки
							strcpy_s(IT_entry.value.vstr->str, ""); // Инициализация строки
							stringFlag = false; // Сброс флага
						}
						indexIT = IT::search(ID_Table, IT_entry); // Поиск идентификатора в таблице

						if (indexIT != -1) // Если идентификатор уже существует
						{
							throw ERROR_THROW(105); // Ошибка: идентификатор уже существует
						}

						LT_entry.idxTI = ID_Table.size; // Установка индекса для лексемы
						IT::Add(ID_Table, IT_entry); // Добавление идентификатора в таблицу
						addedToITFlag = true; // Установка флага добавления
					}

					// Проверка, является ли текущий идентификатор параметром функции
					if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_LEFTTHESIS &&
						LexTable.table[LexTable.size - 3].lexema[0] == LEX_ID &&
						LexTable.table[LexTable.size - 3].idxTI == ID_Table.size - 1 &&
						ID_Table.table[ID_Table.size - 1].idtype == IT::F) // текущий идентификатор - параметр функции
					{
						IT_entry.idtype = IT::P; // Установка типа идентификатора как параметра
						// Если последняя лексема - строка и установлен флаг
						if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
						{
							IT_entry.iddatatype = IT::STR; // Установка типа данных идентификатора как строки
							strcpy_s(IT_entry.value.vstr->str, ""); // Инициализация строки
							stringFlag = false; // Сброс флага
						}
						indexIT = IT::search(ID_Table, IT_entry); // Поиск идентификатора в таблице

						if (indexIT != -1) // Если идентификатор уже существует
						{
							throw ERROR_THROW(105); // Ошибка: идентификатор уже существует

						}

						LT_entry.idxTI = ID_Table.size; // Установка индекса для лексемы
						IT::Add(ID_Table, IT_entry); // Добавление идентификатора в таблицу
						addedToITFlag = true; // Установка флага добавления
					}

					if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_COMMA && ID_Table.table[LexTable.table[LexTable.size - 2].idxTI].idtype == IT::P)
					{
						IT_entry.idtype = IT::P; // Установка типа идентификатора как параметра

						if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
						{
							IT_entry.iddatatype = IT::STR; // Установка типа данных идентификатора как строки
							strcpy_s(IT_entry.value.vstr->str, ""); // Инициализация строки литерала
							stringFlag = false; // Сброс флага для строки
						}

						indexIT = IT::search(ID_Table, IT_entry); // Поиск идентификатора в таблице

						if (indexIT != -1) // Если идентификатор уже существует
						{
							throw ERROR_THROW(105); // Генерация ошибки: идентификатор уже существует
						}

						IT::Add(ID_Table, IT_entry); // Добавление идентификатора в таблицу
						addedToITFlag = true; // Установка флага, что идентификатор был добавлен
					}

					if (!addedToITFlag) // Если идентификатор не был добавлен
					{
						indexIT = IT::search(ID_Table, IT_entry); // Повторный поиск идентификатора в таблице

						if (indexIT >= 0) // Если идентификатор найден
						{

							LT_entry.idxTI = indexIT; // Установка индекса идентификатора для лексемы
						}
					}

					memset(IT_entry.id, NULL, ID_SIZE); // Обнуление идентификатора

					IT_entry.iddatatype = IT::INT; // Установка типа данных идентификатора как целого числа
					IT_entry.value.vint = NULL; // Инициализация значения идентификатора
					addedToITFlag = false; // Сброс флага добавления

					break; // Переход к следующему шагу
				}

				//Если записывается строковая константа
				if (isMarkOpened) {
					str[bufferIndex++] += in.text[i];
					continue;
				}
				bufferIndex = 0;
				memset(str, 0, sizeof(str));;

			}
			if (LT_entry.lexema[0] != NULL)    //если лексема распозналась
			{
				LT_entry.sn = currentLine;    // добавляем ее в таблицу
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;    // обнуляем лексему


			}
			if (in.text[i] == MARK)
			{
				int index = i + 1; // индекс второго символа "'" (конец строкового литерала)
				while (index < MAX_LEX_SIZE - 1 && in.text[index++] != MARK);

				index--; // цикл находит индекс символа за ковычкой

				if (str[0] == '\'' && ID_Table.size > 0 // если это строковой литерал инициализирует переменную
					&& ID_Table.table[ID_Table.size - 1].idtype == IT::V
					&& ID_Table.table[ID_Table.size - 1].iddatatype == IT::STR)
				{
					if (in.text[index] == MARK) // если была найдена вторая кавычка
					{
						sprintf(ID_Table.table[ID_Table.size - 1].value.vstr->str, "L%d\0", literalsCount); // сохраняем не значение, а номер литерала. e.g L3 или L0
						// чтобы сохранить значение 
						// strncpy(ID_Table.table[ID_Table.size - 1].value.vstr->str, reinterpret_cast<const char*>(in.text + i), index - i + 1);
						int len = strlen(ID_Table.table[ID_Table.size - 1].value.vstr->str);
						ID_Table.table[ID_Table.size - 1].value.vstr->len = len;
					}
				}
				// и добавляем сам литерал (в любом случае)
				LT_entry.idxTI = ID_Table.size;

				str[bufferIndex] = '\0';
				LT_entry.lexema[0] = LEX_LITERAL;

				sprintf_s(IT_entry.id, "L%d", literalsCount++);

				IT_entry.iddatatype = IT::STR;
				IT_entry.idtype = IT::L;
				IT_entry.idxfirstLE = currentLine;

				int x = 0;
				// копирует значение литерала (включая кавычки)
				strncpy(IT_entry.value.vstr->str, reinterpret_cast<const char*>(in.text + i), index - i + 1);

				IT_entry.value.vstr->str[index - i + 1] = '\0';
				IT_entry.value.vstr->len = strlen(IT_entry.value.vstr->str);
				LT_entry.sn = currentLine;
				IT_entry.scope = scope.top();

				LT::Add(LexTable, LT_entry);
				IT::Add(ID_Table, IT_entry);

				LT_entry.lexema[0] = NULL;

				i = index;
			}

			else if (in.text[i] == NEW_LINE || in.text[i] == COMMA || in.text[i] == PLUS || in.text[i] == MINUS || in.text[i] == STAR || in.text[i] == DIRSLASH || in.text[i] == EQUAL) {
				LT_entry.lexema[0] = in.text[i];

				if (in.text[i] == NEW_LINE)
				{
					LT_entry.sn = currentLine++;
				}

				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
			}

			else if (in.text[i] == SEMICOLON)
			{
				LT_entry.lexema[0] = LEX_SEMICOLON;
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_BRACELET)
				{
					scope.pop();
					functionFlag = false;
				}

			}

			else if (in.text[i] == LEFT_BRACE)
			{
				if (mainFlag)
				{
					scope.push(&ID_Table.table[ID_Table.size - 1]);
				}
				LT_entry.lexema[0] = LEX_LEFTBRACE;
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				for (int j = ID_Table.size - 1; j >= 0; j--) // Перебор таблицы идентификаторов с конца
				{
					if (ID_Table.table[j].idtype == IT::F) // Если идентификатор - функция
					{
						scope.push(&ID_Table.table[j]); // Добавление функции в область видимости
						break; // Выход из цикла
					}
				}
			}
			else if (in.text[i] == RIGHT_BRACE)
			{
				LT_entry.lexema[0] = LEX_BRACELET;
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				if (!scope.empty()) // Если стек области видимости не пуст
					scope.pop(); // Удаление верхнего элемента из стека
			}
			else if (in.text[i] == LEFTTHESIS)
			{
				LT_entry.lexema[0] = LEX_LEFTTHESIS;
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				if (declareFunctionflag)
				{
					for (int j = ID_Table.size - 1; j >= 0; j--) // Перебор таблицы идентификаторов с конца
					{
						if (ID_Table.table[j].idtype == IT::F) // Если идентификатор - функция
						{
							scope.push(&ID_Table.table[j]); // Добавление функции в область видимости
							break; // Выход из цикла
						}
					}
				}
			}

			else if (in.text[i] == RIGHTTHESIS)
			{
				LT_entry.lexema[0] = LEX_RIGHTTHESIS;
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				parmFlag = false;
				if (!scope.empty() && functionFlag)
				{
					scope.pop(); // Удаление верхнего элемента из стека области видимости
					functionFlag = false;
				}
			}
		}

		Utils::printToFile(ID_Table, parm.it, LexTable, parm.lt, in);

		return LexTable;
	}
}
