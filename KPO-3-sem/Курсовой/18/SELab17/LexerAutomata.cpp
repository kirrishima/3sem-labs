#include "stdafx.h"
#include "FST.h"
#include "LexerAutomata.h"
#include "regex.h"
#include <memory>
#include <stack>
#include <string>
#include "Utils.h"
#include "IT.h"
#include <format>

using namespace REGEX;
using FST::execute;
using LexAn::Utils::printToFile;

bool stringFlag = false; // stringFlag
bool charFlag = false; // stringFlag
bool _isDeclare = false; // _isDeclare

LT::LexTable LexTable = LT::Create(LT_MAXSIZE - 1);
IT::ID_Table ID_Table = IT::Create(TI_MAXSIZE - 1);

using namespace std;

unordered_map<string, bool> long_ids;

char* str = new char[MAX_LEX_SIZE];

FST::FST* IntegerFST(CreateIntegerFST(str));
FST::FST* StringFST(CreateStringFST(str));
FST::FST* CharFST(CreateCharFST(str));

FST::FST* PrintFST(CreatePrintFST(str));
FST::FST* MainFST(CreateMainFST(str));
FST::FST* ReturnFST(CreateReturnFST(str));

FST::FST* IntDECIMALLiteralFST(CreateIntDECIMALLiteralFST(str));
FST::FST* IntBINARYLiteralFST(CreateIntBINARYLiteralFST(str));
FST::FST* IntOCTALLiteralFST(CreateIntOCTALLiteralFST(str));
FST::FST* IntHEXLiteralFST(CreateIntHEXLiteralFST(str));

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

	if (execute(*CharFST))
	{
		charFlag = true;
		return LEX_CHAR;
	}

	if (execute(*StringFST))
	{
		stringFlag = true;
		return LEX_STRING;
	}

	if (execute(*PrintFST))
	{
		return LEX_PRINT;
	}

	if (execute(*MainFST))
	{
		return LEX_MAIN;
	}

	if (execute(*IntDECIMALLiteralFST) || execute(*IntHEXLiteralFST)
		|| execute(*IntOCTALLiteralFST) || execute(*IntBINARYLiteralFST))
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

	if (execute(*ReturnFST))
	{
		return LEX_RETURN;
	}

	if (execute(*IdentifierFST))
	{
		return LEX_ID;
	}

	return '\0';
}


std::pair<LT::LexTable, IT::ID_Table> LexAn::lexAnalize(Parm::PARM param, In::IN in, Log::LOG& log)
{
	int indexIT; // Индекс для таблицы идентификаторов

	int bufferIndex = 0; // Индекс для буфера лексем'

	LT::Entry LT_entry; // Текущий элемент таблицы лексем
	LT_entry.sn = 0; // Номер строки для текущей лексемы
	LT_entry.idxTI = 0; // Индекс идентификатора в таблице
	LT_entry.lexema[0] = NULL; // Обнуление первой лексемы

	std::stack<IT::Entry*> scope; // Стек для хранения области видимости
	scope.push(NULL); // Добавление пустой области видимости
	int block = 0;
	int openningParentthesis = 0;

	int literalsCount = 0; // Счетчик литералов
	bool addedToITFlag = false; // Флаг добавления в таблицу идентификаторов
	bool declareFunctionflag = false; // Флаг объявления функции

	IT::Entry IT_entry; // Текущий элемент таблицы идентификаторов
	LexTable.size = 0; // Обнуление размера таблицы лексем
	int currentLine = 1; // Текущая строка

	for (int i = 0; i < in.size; i++)
	{

		if (((in.text[i] >= 'A' && in.text[i] <= 'Z') || (in.text[i] >= 'a' && in.text[i] <= 'z') ||
			(in.text[i] >= '0' && in.text[i] <= '9') || in.text[i] == '_') && in.text[i] != ' ')
		{
			str[bufferIndex++] = in.text[i]; // Сохранение символа в буфер

			if (bufferIndex >= MAX_LEX_SIZE) {
				throw ERROR_THROW(145); // Ошибка при переполнении буфера
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
					//std::cout << "Вторая точка входа определена в строке " << currentLine << std::endl;
					throw ERROR_THROW_LINE(140, currentLine);
				}

				LT_entry.idxTI = ID_Table.size;
				IT::Add(ID_Table, IT_entry);
				scope.push(&ID_Table.table[ID_Table.size - 1]);
				break;
			}
			case LEX_LITERAL:
			{
				IT_entry.iddatatype = IT::IDDATATYPE::INT;
				IT_entry.idtype = IT::IDTYPE::L;

				try
				{
					IT_entry.value.vint = Utils::stringToNumber(str);
				}
				catch (const std::out_of_range& ex)
				{
					std::cout << "Ошибка в числовом литерале '" << str << "': " << ex.what() << std::endl;
					throw ERROR_THROW_LINE(149, currentLine);
				}
				catch (const std::string& ex) {
					std::cout << "Ошибка в числовом литерале '" << str << "'. " << ex << currentLine << std::endl;
					throw ERROR_THROW_LINE(149, currentLine);
				}
				catch (const std::exception& ex) {
					std::cout << "Ошибка в числовом литерале '" << str << "'" << std::endl;
					std::cout << "Допустимые значения: " << format("[{}; {}]", SHRT_MIN, SHRT_MAX)
						<< ". Допустимые форматы записи: десятичная, двоичная (префикс '0b'), шестнадцатеричная (префикс '0x'), восьмеричная (префикс '0')" << std::endl;
					throw ERROR_THROW_LINE(149, currentLine);
				}

				int index = i - 1;
				while (index > 0 && isdigit(in.text[index])) index--;

				if (in.text[index] == '-' && (in.text[index - 1] == EQUAL
					|| in.text[index - 1] == LEFTTHESIS || in.text[index - 1] == COMMA
					|| (LexTable.table[LexTable.size - 1].v == MINUS && LexTable.table[LexTable.size - 2].lexema[0] == LEX_RETURN))
					)
				{
					IT_entry.value.vint *= -1;
					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_MATH && LexTable.table[LexTable.size - 1].v == MINUS)
					{
						LexTable.size--;
					}
				}

				int pos = IT::search(ID_Table, IT_entry);
				if (pos >= 0)
				{
					IT_entry = ID_Table.table[pos];
					LT_entry.idxTI = pos;
					break;
				}

				sprintf_s(IT_entry.id, "L%d", literalsCount++);
				IT_entry.id[ID_SIZE] = '\0';

				IT_entry.idxfirstLE = currentLine;
				IT_entry.scope = NULL;

				LT_entry.idxTI = ID_Table.size;
				IT::Add(ID_Table, IT_entry);
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
				if (isdigit(str[0]))
				{
					//std::cout << "Строка " << currentLine << std::endl;
					throw ERROR_THROW_LINE(150, currentLine);
				}

				if (scope.empty())
					IT_entry.scope = NULL; //Если стек пуст, то текущему идентификатору IT_entry устанавливается область видимости (scope) равной NULL
				else
					IT_entry.scope = scope.top(); //устанавливается вершина стека как область видимости текущего идентификатора.

				LT_entry.idxTI = ID_Table.size; // индекс ID равен размеру таблицы
				memcpy(IT_entry.id, str, ID_SIZE); // копируем id из строки str, которая была распознана в determineLexeme как идентификатор
				IT_entry.id[ID_SIZE] = '\0'; // обрезаем ID до 5 символов

				if (strlen(str) > ID_SIZE)
				{
					if (long_ids.find(string(str)) == long_ids.end())
					{
						long_ids[string(str)] = true;
						std::cout << str << " слишком длинное имя идентификатора - допустимый размер - 5 символов."
							"Оно будет обрезано до " << IT_entry.id << ". Возможен конфликт имен и сопутствущие ошибки." << std::endl;
					}
				}

				IT_entry.iddatatype = IT::INT; // по умолчанию расцениваем как INT
				IT_entry.value.vint = NULL; // не инициализирован
				IT_entry.idxfirstLE = currentLine; // номер строки для этого id в таблице лексем 
				IT_entry.idtype = IT::V; // по умолчанию расцениваем как обычную переменную

				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_TYPE &&
					(LexTable.size == 1 || LexTable.table[LexTable.size - 2].lexema[0] != LEX_COMMA)) // если это объявление переменной (ключ. слово declare + тип_данных + текущий_id)
				{
					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // если это была строка (stringFlag устанавливается в determineLexeme)
					{
						IT_entry.iddatatype = IT::STR;
						strcpy_s(IT_entry.value.vstr->str, ""); // по умолчанию без значения, оно будет обработано позднее (если это d t i = 'val')
						stringFlag = false;
					}
					else if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_CHAR && charFlag) // опять же, по умолчанию он INT, поэтому проверяем только на STR
					{
						IT_entry.iddatatype = IT::CHAR;
						strcpy_s(IT_entry.value.vstr->str, "");
						charFlag = false;
					}
					indexIT = IT::search(ID_Table, IT_entry);//поиск текущего идентификатора в таблице идентификаторов 
					if (indexIT != -1) //Если идентификатор уже существует
					{
						std::cout << "Ошибка в строке " << currentLine << std::endl;
						throw ERROR_THROW_LINE(148, currentLine);
					}

					LT_entry.idxTI = ID_Table.size;
					IT::Add(ID_Table, IT_entry); // добавляем id
					addedToITFlag = true;
				}
				else if (in.text[i] == LEX_EQUAL)
				{
					if (IT::search(ID_Table, IT_entry) == -1
						&& LexTable.table[LexTable.size - 2].lexema[0] != LEX_TYPE)
					{
						std::cout << "Идентификатор " << IT_entry.id
							<< " использован до объявления в строке " << currentLine << std::endl;
						throw ERROR_THROW_LINE(142, currentLine);
					}
				}
				//if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_FUNCTION) // если предыдущая лексема была фукнция (f, в коде function)
				//{
				//	IT_entry.idtype = IT::F; // указываем это
				//	declareFunctionflag = true;

				//	if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING && stringFlag) // если функция возвращает строку (по умолчанию мы ставили INT)
				//	{
				//		IT_entry.iddatatype = IT::STR; // обновляем возвращаемый тип
				//		strcpy_s(IT_entry.value.vstr->str, "");
				//		stringFlag = false;
				//	}

				//	indexIT = IT::search(ID_Table, IT_entry); // отсутствие переопределения

				//	if (indexIT != -1)
				//	{
				//		std::cout << "Ошибка в строке " << currentLine << std::endl;
				//		throw ERROR_THROW(146);
				//	}

				//	LT_entry.idxTI = ID_Table.size;
				//	IT::Add(ID_Table, IT_entry);
				//	addedToITFlag = true;
				//}

				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_TYPE && // вид <идентификатор>(<тип> <идентификатор>...
					LexTable.table[LexTable.size - 2].lexema[0] == LEX_LEFTTHESIS && // TODO пофиксить этот момент
					LexTable.table[LexTable.size - 3].lexema[0] == LEX_ID
					&& ID_Table.table[ID_Table.size - 2].idtype == IT::F)  /*  &&
					ID_Table.table[ID_Table.size - 1].idtype == IT::F) //текущий идентификатор - параметр функции */
				{
					ID_Table.table[ID_Table.size - 1].idtype = IT::P;

					//if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // если тип параметра - это строка
					//{
					//	IT_entry.iddatatype = IT::STR;
					//	strcpy_s(IT_entry.value.vstr->str, "");
					//	stringFlag = false;
					//}

					//indexIT = IT::search(ID_Table, IT_entry);
					//if (indexIT != -1)
					//{
					//	throw ERROR_THROW(105);
					//}

					//LT_entry.idxTI = ID_Table.size;
					//IT::Add(ID_Table, IT_entry);
					//addedToITFlag = true;
				}

				if (LexTable.size > 3 && LexTable.table[LexTable.size - 2].lexema[0] == LEX_COMMA // если это еще один параметр функции
					&& ID_Table.table[LexTable.table[LexTable.size - 3].idxTI].idtype == IT::P)
				{
					IT_entry.idtype = IT::P;

					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // опять же, по умолчанию он INT, поэтому проверяем только на STR
					{
						IT_entry.iddatatype = IT::STR;
						strcpy_s(IT_entry.value.vstr->str, "");
						stringFlag = false;
					}
					else if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_CHAR && charFlag) // опять же, по умолчанию он INT, поэтому проверяем только на STR
					{
						IT_entry.iddatatype = IT::CHAR;
						strcpy_s(IT_entry.value.vstr->str, "");
						charFlag = false;
					}

					indexIT = IT::search(ID_Table, IT_entry);
					if (indexIT != -1)
					{
						std::cout << "Ошибка в строке " << currentLine << std::endl;
						throw ERROR_THROW(147);
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
					else
					{
						std::cout << "Ошибка в строке " << currentLine << std::endl;
						throw ERROR_THROW_LINE(142, currentLine);
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

			/*if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_ID)
			{
				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_EQUAL
					&& LexTable.table[LexTable.size - 3].lexema[0] == LEX_ID)
				{
					if (ID_Table.table[LexTable.table[LexTable.size - 1].idxTI].iddatatype !=
						ID_Table.table[LexTable.table[LexTable.size - 3].idxTI].iddatatype)
					{
						std::cout << "Строка " << currentLine << ", типы операндов не совпадают.\n";

						if (ID_Table.table[LexTable.table[LexTable.size - 1].idxTI].iddatatype == IT::INT)
							throw ERROR_THROW(143)
						else ERROR_THROW(144)
					}
				}
			}*/
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
			if (i > 0 && in.text[i - 1] == EQUAL)
			{
				break;
			}
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

		case DOUBLE_QUOTES: // отдельно для кавычек
		{
			int index = i + 1; // индекс второго символа "'" (конец строкового литерала)

			while (index < MAX_LEX_SIZE - 1 && in.size > index && (in.text[index++] != DOUBLE_QUOTES || (in.text[index - 1] == DOUBLE_QUOTES && in.text[index - 2] == '\\')));

			index--; // цикл находит индекс символа за ковычкой

			if (ID_Table.size > 0 // если это строковой литерал инициализирует переменную
				&& ID_Table.table[ID_Table.size - 1].idtype == IT::V && LexTable.size > 2
				&& LexTable.table[LexTable.size - 1].lexema[0] == LEX_EQUAL)
			{

				if (ID_Table.table[ID_Table.size - 1].iddatatype != IT::STR)
				{
					std::cout << "Ошибка в строке " << currentLine << std::endl;
					throw ERROR_THROW_LINE(143, currentLine);
				}
				if (in.text[index] == DOUBLE_QUOTES /*&& LexTable.table[LexTable.size - 1].lexema[0] == LEX_EQUAL*/) // если была найдена вторая кавычка
				{
					sprintf(ID_Table.table[ID_Table.size - 1].value.vstr->str, "L%d\0", literalsCount); // сохраняем не значение, а номер литерала. e.g L3 или L0
					// чтобы сохранить значение 
					// strncpy(ID_Table.table[ID_Table.size - 1].value.vstr->str, reinterpret_cast<const char*>(in.text + i), index - i + 1);
					//if (ID_Table.table[ID_Table.size - 1].value.vstr->len <= 0)
					//{
					//	strncpy(ID_Table.table[ID_Table.size - 1].value.vstr->str, reinterpret_cast<const char*>(in.text + i + 1), index - i - 1);
					//	ID_Table.table[ID_Table.size - 1].value.vstr->str[index - i - 1] = '\0';
					//	int len = strlen(ID_Table.table[ID_Table.size - 1].value.vstr->str);
					//	ID_Table.table[ID_Table.size - 1].value.vstr->len = len;

					//	i = index;

					//	break;
					//}
				}
			}
			// и добавляем сам литерал (в любом случае)
			LT_entry.idxTI = ID_Table.size;

			str[bufferIndex] = '\0';
			LT_entry.lexema[0] = LEX_LITERAL;

			sprintf_s(IT_entry.id, "L%d", literalsCount++);

			IT_entry.iddatatype = IT::STR;
			IT_entry.idtype = IT::L;
			// копирует значение литерала (включая кавычки)
			strncpy(IT_entry.value.vstr->str, reinterpret_cast<const char*>(in.text + i + 1), index - i - 1);

			IT_entry.value.vstr->str[index - i - 1] = '\0';

			//std::string escapedString = utils::processEscapeSequences(IT_entry.value.vstr->str);
			//strcpy(IT_entry.value.vstr->str, escapedString.c_str());

			IT_entry.value.vstr->len = strlen(IT_entry.value.vstr->str);

			if (IT_entry.value.vstr->len <= 0)
			{
				std::cout << "Пустой литерал в строке " << currentLine << std::endl;
				ERROR_THROW_LINE(141, currentLine);
			}

			LT_entry.sn = currentLine;
			IT_entry.scope = NULL;
			IT_entry.idxfirstLE = currentLine;


			if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING)
			{
				IT_entry.idxfirstLE = LexTable.size - 1;
			}

			int pos = IT::search(ID_Table, IT_entry);

			if (pos != -1)
			{
				LT_entry.idxTI = pos;
			}
			else
			{
				IT::Add(ID_Table, IT_entry);
			}

			LT::Add(LexTable, LT_entry);


			LT_entry.lexema[0] = NULL;

			i = index;
		}
		break;
		case MARK:
		{
			int index = i + 1; // индекс второго символа "'" (конец строкового литерала)

			while (index < MAX_LEX_SIZE - 1 && in.size > index && (in.text[index] != MARK
				|| (in.text[index - 1] == MARK && in.text[index - 2] == '\\')))index++;

			if (in.text[index] == MARK)
			{

				if (!Utils::isSingleCharacter(in.text + i + 1, index - 1 - i))
				{
					throw ERROR_THROW_LINE(151, currentLine);
				}
			}

			//if (ID_Table.size > 0 // если это строковой литерал инициализирует переменную
			//	&& ID_Table.table[ID_Table.size - 1].idtype == IT::V && LexTable.size > 2
			//	&& LexTable.table[LexTable.size - 1].lexema[0] == LEX_EQUAL)
			//{
			//	if (ID_Table.table[ID_Table.size - 1].iddatatype == IT::CHAR)
			//	{
			//		ID_Table.table[ID_Table.size - 1].value.vstr->len = 1;
			//		strncpy(ID_Table.table[ID_Table.size - 1].value.vstr->str, reinterpret_cast<const char*>(in.text + i + 1), index - 1 - i);
			//		ID_Table.table[ID_Table.size - 1].value.vstr->str[index - 1 - i] = '\0';
			//		i = index + 1;
			//	}
			//}
			LT_entry.idxTI = ID_Table.size;

			str[bufferIndex] = '\0';
			LT_entry.lexema[0] = LEX_LITERAL;

			sprintf_s(IT_entry.id, "L%d", literalsCount++);

			IT_entry.iddatatype = IT::CHAR;
			IT_entry.idtype = IT::L;
			// копирует значение литерала (включая кавычки)
			strncpy(IT_entry.value.vstr->str, reinterpret_cast<const char*>(in.text + i + 1), index - i - 1);

			IT_entry.value.vstr->str[index - i - 1] = '\0';

			//std::string escapedString = utils::processEscapeSequences(IT_entry.value.vstr->str);
			//strcpy(IT_entry.value.vstr->str, escapedString.c_str());

			IT_entry.value.vstr->len = strlen(IT_entry.value.vstr->str);

			if (IT_entry.value.vstr->len <= 0)
			{
				std::cout << "Пустой литерал в строке " << currentLine << std::endl;
				ERROR_THROW_LINE(141, currentLine);
			}

			LT_entry.sn = currentLine;
			IT_entry.scope = NULL;
			IT_entry.idxfirstLE = currentLine;

			if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING)
			{
				IT_entry.idxfirstLE = LexTable.size - 1;
			}

			int pos = IT::search(ID_Table, IT_entry);

			if (pos != -1)
				LT_entry.idxTI = pos;
			else
				IT::Add(ID_Table, IT_entry);


			LT::Add(LexTable, LT_entry);


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
			if (!scope.empty() && openningParentthesis != 0)
			{
				IT::Entry* code_of_block = new IT::Entry();
				code_of_block->scope = NULL;
				code_of_block->scope = scope.top();
				scope.push(code_of_block);
				sprintf_s(code_of_block->id, "IF%d", block++);
			}

			openningParentthesis++;

			LT_entry.lexema[0] = LEX_LEFTBRACE;
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

			//for (int j = ID_Table.size - 1; j >= 0; j--) // ищем ближайшую функцию для установки области видимости в эту функцию
			//{
			//	if (ID_Table.table[j].idtype == IT::F)
			//	{
			//		scope.push(&ID_Table.table[j]);
			//		break;
			//	}
			//}
			break;
		}

		case RIGHT_BRACE:
		{
			openningParentthesis--;
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

			if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_ID
				&& LexTable.table[LexTable.size - 3].lexema[0] == LEX_TYPE)
			{
				ID_Table.table[LexTable.table[LexTable.size - 2].idxTI].idtype = IT::F;
				scope.push(&ID_Table.table[LexTable.table[LexTable.size - 2].idxTI]);
			}
			//if (declareFunctionflag)
			//{
			//	for (int j = ID_Table.size - 1; j >= 0; j--)
			//	{
			//		if (ID_Table.table[j].idtype == IT::F)
			//		{
			//			scope.push(&ID_Table.table[j]);
			//			break;
			//		}
			//	}
			//}
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
		if (param.enableLexAnSave)
		{
			printToFile(ID_Table, param.it, LexTable, param.lt, in);
		}

	}
	catch (...) {}

	delete[] str;

	delete IntegerFST;
	delete StringFST;
	delete CharFST;
	delete PrintFST;
	delete MainFST;
	delete ReturnFST;
	delete IntDECIMALLiteralFST;
	delete IntBINARYLiteralFST;
	delete IntOCTALLiteralFST;
	delete IntHEXLiteralFST;
	delete IdentifierFST;
	delete ifFST;
	delete elseFST;

	long_ids.clear();

	return std::make_pair(LexTable, ID_Table);
}
