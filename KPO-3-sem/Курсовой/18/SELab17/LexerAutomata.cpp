#include "stdafx.h"
#include "FST.h"
#include "LexerAutomata.h"
#include "SVV.h"
#include <memory>
#include <stack>
#include <string>
#include "Utils.h"
#include "IT.h"

using namespace SVV;
using FST::execute;
using LexAn::Utils::printToFile;

bool stringFlag = false; // stringFlag
bool _isDeclare = false; // _isDeclare

LT::LexTable LexTable = LT::Create(LT_MAXSIZE - 1);
IT::ID_Table ID_Table = IT::Create(TI_MAXSIZE - 1);

char* str = new char[MAX_LEX_SIZE];

FST::FST* IntegerFST(CreateIntegerFST(str));
FST::FST* PrintFST(CreatePrintFST(str));
FST::FST* INTLiteralFST(CreateINTLiteralFST(str));
FST::FST* IdentifierFST(CreateIdentifierFST(str));
FST::FST* ifFST(CreateIfFST(str));
FST::FST* elseFST(CreateElseFST(str));
//FST::FST* compareFST(SVV::CreateCompareFST(str));

// \0 in case of mismatch of everything
char LexAn::determineLexeme()
{
	if (execute(*IntegerFST))
	{
		return LEX_INTEGER;
	}

	if (execute(*PrintFST))
	{
		return LEX_PRINT;
	}

	if (execute(*INTLiteralFST))
	{
		return LEX_LITERAL;
	}

	if (execute(*ifFST))
	{
		return LEX_IF;
	}

	if (execute(*elseFST))
	{
		return LEX_ELSE;
	}

	//if (execute(*compareFST))
	//{
	//	return LEX_COMPARE_EQUAL;
	//}

	if (execute(*IdentifierFST))
	{
		return LEX_ID;
	}

	return '\0';
}


std::pair<LT::LexTable, IT::ID_Table> LexAn::lexAnalize(Parm::PARM param, In::IN in)
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
			str[bufferIndex] = '\0';
			LT_entry.lexema[0] = determineLexeme();

			switch (LT_entry.lexema[0]) {

			case LEX_MAIN:
			{
				memcpy(IT_entry.id, str, ID_SIZE);
				IT_entry.id[ID_SIZE] = '\0';
				IT_entry.iddatatype = IT::INT;
				IT_entry.idtype = IT::F;
				IT_entry.value.vint = NULL;
				IT_entry.idxfirstLE = currentLine;
				IT_entry.scope = NULL;
				indexIT = IT::search(ID_Table, IT_entry);

				if (indexIT != -1)
				{
					throw ERROR_THROW(105);
				}

				LT_entry.idxTI = ID_Table.size;
				IT::Add(ID_Table, IT_entry);

				break;
			}

			case LEX_LITERAL:
			{
				sprintf_s(IT_entry.id, "L%d", literalsCount++);
				IT_entry.id[ID_SIZE] = '\0';
				IT_entry.iddatatype = IT::INT;
				IT_entry.idtype = IT::L;
				IT_entry.idxfirstLE = currentLine;
				IT_entry.value.vint = atoi(str);

				IT_entry.scope = NULL;
				if (!scope.empty())
				{
					IT_entry.scope = scope.top();
				}

				LT_entry.idxTI = ID_Table.size;
				IT::Add(ID_Table, IT_entry);

				if (LexTable.size > 3 && LexTable.table[LexTable.size - 4].lexema[0] == LEX_DECLARE
					&& LexTable.table[LexTable.size - 3].lexema[0] == LEX_INTEGER
					&& LexTable.table[LexTable.size - 2].lexema[0] == LEX_ID
					&& LexTable.table[LexTable.size - 1].lexema[0] == LEX_EQUAL
					&& ID_Table.table[ID_Table.size - 1].iddatatype == IT::INT)
				{
					ID_Table.table[ID_Table.size - 2].value.vint = atoi(str);
				}
				break;
			}
			case LEX_IF:
				LT_entry.lexema[0] = LEX_IF;

				/*			LT_entry.sn = currentLine;
							LT::Add(LexTable, LT_entry);
							LT_entry.lexema[0] = NULL;*/
				break;
			case LEX_ELSE:
				LT_entry.lexema[0] = LEX_ELSE;
				//LT_entry.sn = currentLine;
				//LT::Add(LexTable, LT_entry);
				//LT_entry.lexema[0] = NULL;
				break;
				//case LEX_COMPARE_EQUAL:
				//{
				//	LT_entry.lexema[0] = LEX_COMPARE_LESS;
				//	LT_entry.sn = currentLine;
				//	LT_entry.c = str;
				//	LT::Add(LexTable, LT_entry);
				//	LT_entry.lexema[0] = NULL;
				//	break;
				//}
			case LEX_ID:
			{
				if (scope.empty())
					IT_entry.scope = NULL; //Если стек пуст, то текущему идентификатору IT_entry устанавливается область видимости (scope) равной NULL
				else
					IT_entry.scope = scope.top(); //устанавливается вершина стека как область видимости текущего идентификатора.

				LT_entry.idxTI = ID_Table.size; // индекс ID равен размеру таблицы
				memcpy(IT_entry.id, str, ID_SIZE); // копируем id из строки str, которая была распознана в determineLexeme как идентификатор
				IT_entry.id[ID_SIZE] = '\0'; // обрезаем ID до 5 символов

				if (strlen(str) > ID_SIZE)
				{
					std::cout << str << " слишком длинное имя идентификатора - допустимый размер - 5 символов. Оно будет обрезано до " << IT_entry.id << std::endl;
				}

				IT_entry.iddatatype = IT::INT; // по умолчанию расцениваем как INT
				IT_entry.value.vint = NULL; // не инициализирован
				IT_entry.idxfirstLE = currentLine; // номер строки для этого id в таблице лексем 
				IT_entry.idtype = IT::V; // по умолчанию расцениваем как обычную переменную

				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_INTEGER) // если это объявление переменной (ключ. слово declare + тип_данных + текущий_id)
				{
					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // если это была строка (stringFlag устанавливается в determineLexeme)
					{
						IT_entry.iddatatype = IT::STR;
						strcpy_s(IT_entry.value.vstr->str, ""); // по умолчанию без значения, оно будет обработано позднее (если это d t i = 'val')
						stringFlag = false;
					}

					indexIT = IT::search(ID_Table, IT_entry);//поиск текущего идентификатора в таблице идентификаторов 
					if (indexIT != -1) //Если идентификатор уже существует
					{
						throw ERROR_THROW(105);
					}

					LT_entry.idxTI = ID_Table.size;
					IT::Add(ID_Table, IT_entry); // добавляем id
					addedToITFlag = true;
				}

				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_FUNCTION) // если предыдущая лексема была фукнция (f, в коде function)
				{
					IT_entry.idtype = IT::F; // указываем это
					declareFunctionflag = true;

					if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING && stringFlag) // если функция возвращает строку (по умолчанию мы ставили INT)
					{
						IT_entry.iddatatype = IT::STR; // обновляем возвращаемый тип
						strcpy_s(IT_entry.value.vstr->str, "");
						stringFlag = false;
					}

					indexIT = IT::search(ID_Table, IT_entry); // отсутствие переопределения

					if (indexIT != -1)
					{
						throw ERROR_THROW(105);
					}

					LT_entry.idxTI = ID_Table.size;
					IT::Add(ID_Table, IT_entry);
					addedToITFlag = true;
				}

				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_LEFTTHESIS && // вид <идентификатор>(<тип> <идентификатор>...
					LexTable.table[LexTable.size - 2].lexema[0] == LEX_FUNCTION && // TODO пофиксить этот момент
					LexTable.table[LexTable.size - 2].idxTI == ID_Table.size - 1)  /*  &&
					ID_Table.table[ID_Table.size - 1].idtype == IT::F) //текущий идентификатор - параметр функции */
				{
					IT_entry.idtype = IT::P;

					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // если тип параметра - это строка
					{
						IT_entry.iddatatype = IT::STR;
						strcpy_s(IT_entry.value.vstr->str, "");
						stringFlag = false;
					}

					indexIT = IT::search(ID_Table, IT_entry);
					if (indexIT != -1)
					{
						throw ERROR_THROW(105);
					}

					LT_entry.idxTI = ID_Table.size;
					IT::Add(ID_Table, IT_entry);
					addedToITFlag = true;
				}

				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_COMMA // если это еще один параметр функции
					&& ID_Table.table[LexTable.table[LexTable.size - 2].idxTI].idtype == IT::P)
				{
					IT_entry.idtype = IT::P;

					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // опять же, по умолчанию он INT, поэтому проверяем только на STR
					{
						IT_entry.iddatatype = IT::STR;
						strcpy_s(IT_entry.value.vstr->str, "");
						stringFlag = false;
					}

					indexIT = IT::search(ID_Table, IT_entry);
					if (indexIT != -1)
					{
						throw ERROR_THROW(105);
					}

					IT::Add(ID_Table, IT_entry);
					addedToITFlag = true;
				}

				if (!addedToITFlag)
				{
					indexIT = IT::search(ID_Table, IT_entry);
					if (indexIT >= 0)
					{
						LT_entry.idxTI = indexIT;
					}
				}

				std::memset(IT_entry.id, NULL, ID_SIZE); // очищаем ID перед новой итерацией

				IT_entry.iddatatype = IT::INT; // по умолчанию INT
				IT_entry.value.vint = NULL; // без значения (по факту будет 0)
				addedToITFlag = false;

				break;
			}
			default:
				break;
			}

			bufferIndex = 0;
			std::memset(str, NULL, 50); // отчищаем также буфер str, в который считавается текст из in.text
		}

		if (LT_entry.lexema[0] != NULL) // если лексема была распознана - сохраняем ее
		{
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;
		}

		switch (in.text[i])
		{
		case SEMICOLON: // для поддерживаемых лексем в виде текущего символа
			LT_entry.lexema[0] = LEX_SEMICOLON; // сохраняем их
			goto add_LT_entry; // добавляем их в таблицу
			break;
		case COMMA:
			LT_entry.lexema[0] = LEX_COMMA;
			goto add_LT_entry;
			break;
		case PLUS:
			LT_entry.lexema[0] = LEX_PLUS;
			goto add_LT_entry;
			break;
		case MINUS:
			LT_entry.lexema[0] = LEX_MINUS;
			goto add_LT_entry;
			break;
		case STAR:
			LT_entry.lexema[0] = LEX_STAR;
			goto add_LT_entry;
			break;
		case DIRSLASH:
			LT_entry.lexema[0] = LEX_DIRSLASH;
			goto add_LT_entry;
			break;
		case '<':
		case '>':
			if (i + 1 < in.size && in.text[i + 1] != '=')
			{
				LT_entry.lexema[0] = LEX_COMPARE_LESS;
				LT_entry.sn = currentLine;
				LT_entry.c = in.text[i];
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
			}

			break;
		case EQUAL:
			if (i + 1 < in.size && in.text[i + 1] == '=')
			{
				LT_entry.lexema[0] = LEX_COMPARE_LESS;
				LT_entry.c = "=";
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				i += 1;
				break;
			}
			if (in.text[i - 1] == '>' || in.text[i - 1] == '<' || in.text[i - 1] == '!')
			{
				LT_entry.lexema[0] = LEX_COMPARE_LESS;
				LT_entry.c = "01";
				LT_entry.c[1] = '=';
				LT_entry.c[0] = in.text[i - 1];
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				break;
			}
			LT_entry.lexema[0] = LEX_EQUAL;

		add_LT_entry: // goto я не гей
			LT_entry.sn = currentLine;
			LT_entry.v = in.text[i];
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;
			break;

		case MARK: // отдельно для кавычек
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
		break;

		case NEW_LINE:
		{
			//LT_entry.lexema[0] = '|';
			LT_entry.sn = currentLine++;
			//LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;
			break;
		}

		case LEFT_BRACE:
		{
			LT_entry.lexema[0] = LEX_LEFTBRACE;
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

			for (int j = ID_Table.size - 1; j >= 0; j--) // ищем ближайшую функцию для установки области видимости в эту функцию
			{
				if (ID_Table.table[j].idtype == IT::F)
				{
					scope.push(&ID_Table.table[j]);
					break;
				}
			}
			break;
		}

		case RIGHT_BRACE:
		{
			LT_entry.lexema[0] = RIGHT_BRACE;
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

			if (!scope.empty()) // '}' - выходим из области видимости 
				scope.pop();

			break;
		}

		case LEFTTHESIS:
		{
			LT_entry.lexema[0] = LEX_LEFTTHESIS;
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

			if (declareFunctionflag)
			{
				for (int j = ID_Table.size - 1; j >= 0; j--)
				{
					if (ID_Table.table[j].idtype == IT::F)
					{
						scope.push(&ID_Table.table[j]);
						break;
					}
				}
			}
			break;
		}
		case RIGHTTHESIS:
		{
			LT_entry.lexema[0] = LEX_RIGHTTHESIS;
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

			if (!scope.empty() && declareFunctionflag)
			{
				scope.pop();
				declareFunctionflag = false;
			}
			break;
		}
		}
	}

	try
	{
#ifndef __DISABLE_IT_LT_TABLES_SAVE
		printToFile(ID_Table, param.it, LexTable, param.lt, in);
#endif // !__DISABLE_IT_LT_TABLES_SAVE

	}
	catch (...) {}

	delete[] str;

	delete elseFST;
	delete ifFST;
	delete IntegerFST;
	delete PrintFST;
	delete INTLiteralFST;
	delete IdentifierFST;

	return std::make_pair(LexTable, ID_Table);
}
