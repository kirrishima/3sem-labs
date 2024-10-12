#include "stdafx.h"
#include "FST.h"
#include "LexerAutomata.h"
#include "SVV.h"
#include <iomanip>
#include <memory>
#include <stack>
#include <unordered_map>

bool stringFlag = false; // stringFlag
bool _isDeclare = false; // _isDeclare

LT::LexTable LexTable = LT::Create(LT_MAXSIZE - 1);
IT::IdTable ID_Table = IT::Create(TI_MAXSIZE - 1);

char* str = new char[MAX_LEX_SIZE];
using FST::execute;
using namespace SVV;

void printToFile(IT::Entry& IT_entry, const std::wstring IT_filename, LT::Entry& LT_entry, const std::wstring LT_filename);


const std::unordered_map<unsigned char, unsigned char> lexems = {
   {SEMICOLON, LEX_SEMICOLON},
   {COMMA, LEX_COMMA},
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
			LT_entry.lexema[0] = _determineLexeme();

			switch (LT_entry.lexema[0]) {

			case LEX_MAIN:
			{
				LT_entry.idxTI = ID_Table.size;
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
				LT_entry.idxTI = ID_Table.size;
				sprintf_s(IT_entry.id, "L%d", literalsCount);
				IT_entry.iddatatype = IT::INT;
				IT_entry.idtype = IT::L;
				IT_entry.idxfirstLE = currentLine;
				IT_entry.value.vint = atoi(str);
				IT_entry.scope = scope.top();
				LT_entry.idxTI = ID_Table.size;
				IT::Add(ID_Table, IT_entry);
				literalsCount++;
				break;
			}

			case LEX_ID:
			{
				if (scope.empty())
					IT_entry.scope = NULL; //Если стек пуст, то текущему идентификатору IT_entry устанавливается область видимости (scope) равной NULL
				else
					IT_entry.scope = scope.top(); //устанавливается вершина стека как область видимости текущего идентификатора.
				LT_entry.idxTI = ID_Table.size;
				memcpy(IT_entry.id, str, ID_SIZE);
				IT_entry.id[ID_SIZE] = '\0';
				IT_entry.iddatatype = IT::INT;
				IT_entry.value.vint = NULL; // не инициализирован
				IT_entry.idxfirstLE = currentLine;
				IT_entry.idtype = IT::V;

				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_DECLARE)
				{
					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
					{
						IT_entry.iddatatype = IT::STR;
						strcpy_s(IT_entry.value.vstr->str, "");
						stringFlag = false;
					}
					indexIT = IT::search(ID_Table, IT_entry);//поиск текущего идентификатора в таблице идентификаторов 
					if (indexIT != -1) //Если идентификатор уже существует
					{
						throw ERROR_THROW(105);
					}
					LT_entry.idxTI = ID_Table.size;
					IT::Add(ID_Table, IT_entry);
					addedToITFlag = true;
				}

				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_FUNCTION)
				{
					IT_entry.idtype = IT::F;
					declareFunctionflag = true;
					if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING && stringFlag)
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

				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_LEFTTHESIS &&
					LexTable.table[LexTable.size - 3].lexema[0] == LEX_ID &&
					LexTable.table[LexTable.size - 3].idxTI == ID_Table.size - 1 &&
					ID_Table.table[ID_Table.size - 1].idtype == IT::F) //текущий идентификатор - параметр функции
				{
					IT_entry.idtype = IT::P;
					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
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

				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_COMMA && ID_Table.table[LexTable.table[LexTable.size - 2].idxTI].idtype == IT::P)
				{
					IT_entry.idtype = IT::P;

					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
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

				std::memset(IT_entry.id, NULL, ID_SIZE);

				IT_entry.iddatatype = IT::INT;
				IT_entry.value.vint = NULL;
				addedToITFlag = false;

				break;
			}
			default:
				break;
			}

			bufferIndex = 0;
			std::memset(str, NULL, bufferIndex + 1);
		}

		if (LT_entry.lexema[0] != NULL)
		{
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

		}

		if (lexems.find(in.text[i]) != lexems.end())
		{
			LT_entry.lexema[0] = lexems.at(in.text[i]);
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;
		}

		else
		{
			switch (in.text[i])
			{
			case MARK:
			{
				int index = i + 1;
				while (index < MAX_LEX_SIZE - 1 && in.text[index++] != MARK);

				if (str[0] == '\'' && ID_Table.size > 0 && ID_Table.table[ID_Table.size - 1].idtype == IT::V && ID_Table.table[ID_Table.size - 1].iddatatype == IT::STR)
				{
					if (in.text[index - 1] == MARK)
					{
						//strncpy(ID_Table.table[ID_Table.size - 1].value.vstr->str, reinterpret_cast<const char*>(in.text + i), index - i);

						sprintf(ID_Table.table[ID_Table.size - 1].value.vstr->str, "L%d\0", literalsCount);

						int len = strlen(ID_Table.table[ID_Table.size - 1].value.vstr->str);
						//ID_Table.table[ID_Table.size - 1].value.vstr->str[len] = ;
						ID_Table.table[ID_Table.size - 1].value.vstr->len = len;

						//i = index;
					}

					LT_entry.idxTI = ID_Table.size;

					str[bufferIndex] = '\0';
					LT_entry.lexema[0] = LEX_LITERAL;

					sprintf_s(IT_entry.id, "L%d", literalsCount++);

					IT_entry.iddatatype = IT::STR;
					IT_entry.idtype = IT::L;
					IT_entry.idxfirstLE = currentLine;

					int x = 0;
					for (; i < index; i++)
					{
						IT_entry.value.vstr->str[x++] = in.text[i];
					}

					IT_entry.value.vstr->str[x] = '\0';
					IT_entry.value.vstr->len = strlen(IT_entry.value.vstr->str);
					LT_entry.sn = currentLine;
					IT_entry.scope = scope.top();

					LT::Add(LexTable, LT_entry);
					IT::Add(ID_Table, IT_entry);

					LT_entry.lexema[0] = NULL;

					break;
				}
				else
				{
					LT_entry.idxTI = ID_Table.size;

					str[bufferIndex] = '\0';
					LT_entry.lexema[0] = LEX_LITERAL;

					sprintf_s(IT_entry.id, "L%d", ++literalsCount);

					IT_entry.iddatatype = IT::STR;
					IT_entry.idtype = IT::L;
					IT_entry.idxfirstLE = currentLine;

					int x = 0;
					for (; i < index; i++)
					{
						IT_entry.value.vstr->str[x++] = in.text[i];
					}

					IT_entry.value.vstr->str[x] = '\0';
					IT_entry.value.vstr->len = strlen(IT_entry.value.vstr->str);
					LT_entry.sn = currentLine;
					IT_entry.scope = scope.top();

					LT::Add(LexTable, LT_entry);
					IT::Add(ID_Table, IT_entry);

					LT_entry.lexema[0] = NULL;
				}
			}

			break;
			case NEW_LINE:
				LT_entry.lexema[0] = '\n';
				LT_entry.sn = currentLine;
				currentLine++;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				break;
			case LEFT_BRACE:
				LT_entry.lexema[0] = LEX_LEFTBRACE;
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				for (int j = ID_Table.size - 1; j >= 0; j--)
				{
					if (ID_Table.table[j].idtype == IT::F)
					{
						scope.push(&ID_Table.table[j]);
						break;
					}
				}
				break;
			case RIGHT_BRACE:
				LT_entry.lexema[0] = LEX_RIGHTBRACE;
				LT_entry.sn = currentLine;
				LT::Add(LexTable, LT_entry);
				LT_entry.lexema[0] = NULL;
				if (!scope.empty())
					scope.pop();
				break;
			case LEFTTHESIS:
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
			case RIGHTTHESIS:
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
	std::cout << "aaaa\n";
	printToFile(IT_entry, param.it, LT_entry, param.lt);

}

void printLine(std::ofstream& file) {
	// В HTML можно опустить линии, поэтому этот метод больше не нужен
}

void printToFile(IT::Entry& IT_entry, const std::wstring IT_filename, LT::Entry& LT_entry, const std::wstring LT_filename)
{
	int SETW_VALUE = 8;

	std::ofstream LT_file(LT_filename); // Файл для записи таблицы лексем
	std::ofstream IT_file(IT_filename); // Файл для записи таблицы идентификаторов

	if (!LT_file.is_open())
	{
		std::wcout << L"Не удалось открыть файл " << LT_filename << L"\n";
		return;
	}

	if (!IT_file.is_open())
	{
		std::wcout << L"Не удалось открыть файл " << IT_filename << L"\n";
		return;
	}

	// Начало HTML-документа
	IT_file << "<!DOCTYPE html><html><head><title>Identifier Table</title></head><body>" << std::endl;
	IT_file << "<pre><table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">" << std::endl;

	// Заголовок таблицы
	IT_file << "<tr>"
		<< "<th>ID</th>"
		<< "<th>Datatype</th>"
		<< "<th>ID Type</th>"
		<< "<th>Line</th>"
		<< "<th>Value</th>"
		<< "<th>Scope</th>"
		<< "</tr>" << std::endl;

	// Заполнение таблицы данными
	for (int i = 0; i < ID_Table.size; i++) {
		IT_entry = IT::GetEntry(ID_Table, i);

		IT_file << "<tr>";

		// ID
		IT_file << "<td>" << IT_entry.id << "</td>";

		// Datatype
		if (IT_entry.iddatatype == 1)
			IT_file << "<td>INT</td>";
		else if (IT_entry.iddatatype == 2)
			IT_file << "<td>STR</td>";
		else
			IT_file << "<td>-</td>";

		// ID Type
		IT_file << "<td>";
		if (IT_entry.idtype == IT::V)
			IT_file << "V";
		else if (IT_entry.idtype == IT::L)
			IT_file << "L";
		else if (IT_entry.idtype == IT::F)
			IT_file << "F";
		else if (IT_entry.idtype == IT::P)
			IT_file << "P";
		else
			IT_file << "-";
		IT_file << "</td>";

		// Line
		IT_file << "<td>" << IT_entry.idxfirstLE << "</td>";

		// Value
		IT_file << "<td>";
		if (IT_entry.iddatatype == IT::INT) {
			IT_file << IT_entry.value.vint;
		}
		else if (IT_entry.iddatatype == IT::STR) {
			std::string strValue(IT_entry.value.vstr->str);
			IT_file << strValue;
		}
		else {
			IT_file << "-";
		}
		IT_file << "</td>";

		// Scope
		IT_file << "<td>";
		if (IT_entry.scope != NULL) {
			std::string scopeValue(IT_entry.scope->id);
			IT_file << scopeValue;
		}
		else {
			IT_file << "-";
		}
		IT_file << "</td>";

		IT_file << "</tr>" << std::endl;
	}


	// Закрытие таблицы и HTML-документа
	IT_file << "</table></pre></body></html>" << std::endl;

	IT_file.close();

	LT_file << "<!DOCTYPE html><html><head><title>Lexem Table</title></head><body>" << std::endl;
	LT_file << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">" << std::endl;

	// Заголовок таблицы
	LT_file << "<tr>"
		<< "<th>Line</th>"
		<< "<th>Lexem</th>"
		<< "</tr>" << std::endl;

	int currentLine = 1;
	LT_file << "<tr><td>01</td>" << std::setw(8);

	std::string tmp;
	for (int i = 0; i < LexTable.size; i++)
	{
		LT_entry = LT::GetEntry(LexTable, i);
		if (currentLine != LT_entry.sn)
		{
			currentLine = LT_entry.sn;
			LT_file << "<tr><td>" << (currentLine > 9 ? std::to_string(currentLine) : ("0" + std::to_string(currentLine))) << "</td>";
		}
		if (LT_entry.lexema[0] != In::IN::Newline)
		{
			tmp += LT_entry.lexema[0];
		}
		else
		{
			LT_file << "<td>" << tmp << "</td></tr>";
			tmp.clear();
		}
	}
	LT_file << "<td>" << tmp << "</td></tr>";
	LT_file << "</table></body></html>" << std::endl;
	LT_file.close();
}
