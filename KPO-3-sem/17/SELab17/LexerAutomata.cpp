#include "stdafx.h"
#include "FST.h"
#include "LexerAutomata.h"
#include "SVV.h"
#include <memory>
#include <stack>
#include <string>
#include "Utils.h"

using namespace SVV;
using FST::execute;
using LexAn::Utils::printToFile;

bool stringFlag = false; // stringFlag
bool _isDeclare = false; // _isDeclare

LT::LexTable LexTable = LT::Create(LT_MAXSIZE - 1);
IT::IdTable ID_Table = IT::Create(TI_MAXSIZE - 1);

char* str = new char[MAX_LEX_SIZE];

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
char LexAn::determineLexeme()
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


void LexAn::lexAnalize(Parm::PARM param, In::IN in)
{
	int indexIT; // ������ ��� ������� ���������������

	int bufferIndex = 0; // ������ ��� ������ ������'

	LT::Entry LT_entry; // ������� ������� ������� ������
	LT_entry.sn = 0; // ����� ������ ��� ������� �������
	LT_entry.idxTI = 0; // ������ �������������� � �������
	LT_entry.lexema[0] = NULL; // ��������� ������ �������

	std::stack<IT::Entry*> scope; // ���� ��� �������� ������� ���������
	scope.push(NULL); // ���������� ������ ������� ���������

	int literalsCount = 0; // ������� ���������
	bool addedToITFlag = false; // ���� ���������� � ������� ���������������
	bool declareFunctionflag = false; // ���� ���������� �������

	IT::Entry IT_entry; // ������� ������� ������� ���������������
	LexTable.size = 0; // ��������� ������� ������� ������
	int currentLine = 1; // ������� ������

	for (int i = 0; i < in.size; i++)
	{
		if (((in.text[i] >= 'A' && in.text[i] <= 'Z') || (in.text[i] >= 'a' && in.text[i] <= 'z') ||
			(in.text[i] >= '0' && in.text[i] <= '9') || in.text[i] == '\'') && in.text[i] != ' ')
		{
			str[bufferIndex++] = in.text[i]; // ���������� ������� � �����

			if (bufferIndex >= MAX_LEX_SIZE) {
				throw ERROR_THROW(119); // ������ ��� ������������ ������
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
				IT_entry.iddatatype = IT::INT;
				IT_entry.idtype = IT::L;
				IT_entry.idxfirstLE = currentLine;
				IT_entry.value.vint = atoi(str);
				IT_entry.scope = scope.top();
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

			case LEX_ID:
			{
				if (scope.empty())
					IT_entry.scope = NULL; //���� ���� ����, �� �������� �������������� IT_entry ��������������� ������� ��������� (scope) ������ NULL
				else
					IT_entry.scope = scope.top(); //��������������� ������� ����� ��� ������� ��������� �������� ��������������.

				LT_entry.idxTI = ID_Table.size; // ������ ID ����� ������� �������
				memcpy(IT_entry.id, str, ID_SIZE); // �������� id �� ������ str, ������� ���� ���������� � determineLexeme ��� �������������

				if (strlen(str) > ID_SIZE)
				{
					std::cout << str << " ������� ������� ��� �������������� - ���������� ������ - 5 ��������. ��� ����� �������� �� " << IT_entry.id << std::endl;
				}

				IT_entry.id[ID_SIZE] = '\0'; // �������� ID �� 5 ��������
				IT_entry.iddatatype = IT::INT; // �� ��������� ����������� ��� INT
				IT_entry.value.vint = NULL; // �� ���������������
				IT_entry.idxfirstLE = currentLine; // ����� ������ ��� ����� id � ������� ������ 
				IT_entry.idtype = IT::V; // �� ��������� ����������� ��� ������� ����������

				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_DECLARE) // ���� ��� ���������� ���������� (����. ����� declare + ���_������ + �������_id)
				{
					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // ���� ��� ���� ������ (stringFlag ��������������� � determineLexeme)
					{
						IT_entry.iddatatype = IT::STR;
						strcpy_s(IT_entry.value.vstr->str, ""); // �� ��������� ��� ��������, ��� ����� ���������� ������� (���� ��� d t i = 'val')
						stringFlag = false;
					}

					indexIT = IT::search(ID_Table, IT_entry);//����� �������� �������������� � ������� ��������������� 
					if (indexIT != -1) //���� ������������� ��� ����������
					{
						throw ERROR_THROW(105);
					}

					LT_entry.idxTI = ID_Table.size;
					IT::Add(ID_Table, IT_entry); // ��������� id
					addedToITFlag = true;
				}

				if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_FUNCTION) // ���� ���������� ������� ���� ������� (f, � ���� function)
				{
					IT_entry.idtype = IT::F; // ��������� ���
					declareFunctionflag = true;

					if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_STRING && stringFlag) // ���� ������� ���������� ������ (�� ��������� �� ������� INT)
					{
						IT_entry.iddatatype = IT::STR; // ��������� ������������ ���
						strcpy_s(IT_entry.value.vstr->str, "");
						stringFlag = false;
					}

					indexIT = IT::search(ID_Table, IT_entry); // ���������� ���������������

					if (indexIT != -1)
					{
						throw ERROR_THROW(105);
					}

					LT_entry.idxTI = ID_Table.size;
					IT::Add(ID_Table, IT_entry);
					addedToITFlag = true;
				}

				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_LEFTTHESIS && // ��� <�������������>(<���> <�������������>...
					LexTable.table[LexTable.size - 3].lexema[0] == LEX_ID &&
					LexTable.table[LexTable.size - 3].idxTI == ID_Table.size - 1 &&
					ID_Table.table[ID_Table.size - 1].idtype == IT::F) //������� ������������� - �������� �������
				{
					IT_entry.idtype = IT::P;

					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // ���� ��� ��������� - ��� ������
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

				if (LexTable.table[LexTable.size - 2].lexema[0] == LEX_COMMA // ���� ��� ��� ���� �������� �������
					&& ID_Table.table[LexTable.table[LexTable.size - 2].idxTI].idtype == IT::P)
				{
					IT_entry.idtype = IT::P;

					if (LexTable.table[LexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) // ����� ��, �� ��������� �� INT, ������� ��������� ������ �� STR
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

				std::memset(IT_entry.id, NULL, ID_SIZE); // ������� ID ����� ����� ���������

				IT_entry.iddatatype = IT::INT; // �� ��������� INT
				IT_entry.value.vint = NULL; // ��� �������� (�� ����� ����� 0)
				addedToITFlag = false;

				break;
			}
			default:
				break;
			}

			bufferIndex = 0;
			std::memset(str, NULL, bufferIndex + 1); // �������� ����� ����� str, � ������� ����������� ����� �� in.text
		}

		if (LT_entry.lexema[0] != NULL) // ���� ������� ���� ���������� - ��������� ��
		{
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;
		}
		switch (in.text[i])
		{
		case SEMICOLON: // ��� �������������� ������ � ���� �������� �������
			LT_entry.lexema[0] = LEX_SEMICOLON; // ��������� ��
			goto add_LT_entry; // ��������� �� � �������
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
		case EQUAL:
			LT_entry.lexema[0] = LEX_EQUAL;

		add_LT_entry: // goto � �� ���
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;
			break;

		case MARK: // �������� ��� �������
		{
			int index = i + 1; // ������ ������� ������� "'" (����� ���������� ��������)
			while (index < MAX_LEX_SIZE - 1 && in.text[index++] != MARK);

			index--; // ���� ������� ������ ������� �� ��������

			if (str[0] == '\'' && ID_Table.size > 0 // ���� ��� ��������� ������� �������������� ����������
				&& ID_Table.table[ID_Table.size - 1].idtype == IT::V
				&& ID_Table.table[ID_Table.size - 1].iddatatype == IT::STR)
			{
				if (in.text[index] == MARK) // ���� ���� ������� ������ �������
				{
					sprintf(ID_Table.table[ID_Table.size - 1].value.vstr->str, "L%d\0", literalsCount); // ��������� �� ��������, � ����� ��������. e.g L3 ��� L0
					// ����� ��������� �������� 
					// strncpy(ID_Table.table[ID_Table.size - 1].value.vstr->str, reinterpret_cast<const char*>(in.text + i), index - i + 1);
					int len = strlen(ID_Table.table[ID_Table.size - 1].value.vstr->str);
					ID_Table.table[ID_Table.size - 1].value.vstr->len = len;
				}
			}
			// � ��������� ��� ������� (� ����� ������)
			LT_entry.idxTI = ID_Table.size;

			str[bufferIndex] = '\0';
			LT_entry.lexema[0] = LEX_LITERAL;

			sprintf_s(IT_entry.id, "L%d", literalsCount++);

			IT_entry.iddatatype = IT::STR;
			IT_entry.idtype = IT::L;
			IT_entry.idxfirstLE = currentLine;

			int x = 0;
			// �������� �������� �������� (������� �������)
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
			LT_entry.lexema[0] = '\n';
			LT_entry.sn = currentLine++;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;
			break;
		}

		case LEFT_BRACE:
		{
			LT_entry.lexema[0] = LEX_LEFTBRACE;
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

			for (int j = ID_Table.size - 1; j >= 0; j--) // ���� ��������� ������� ��� ��������� ������� ��������� � ��� �������
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
			LT_entry.lexema[0] = LEX_RIGHTBRACE;
			LT_entry.sn = currentLine;
			LT::Add(LexTable, LT_entry);
			LT_entry.lexema[0] = NULL;

			if (!scope.empty()) // '}' - ������� �� ������� ��������� 
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
		printToFile(ID_Table, param.it, LexTable, param.lt, in);
	}
	catch (...) {}

	delete[] str;

	delete IntegerFST;
	delete StringFST;
	delete FunctionFST;
	delete DeclareFST;
	delete ReturnFST;
	delete MainFST;
	delete PrintFST;
	delete INTLiteralFST;
	delete IdentifierFST;
}
