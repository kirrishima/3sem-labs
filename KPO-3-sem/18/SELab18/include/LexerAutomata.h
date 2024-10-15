#pragma once

#include "In.h"
#include "IT.h"
#include "Parm.h"
#include "LT.h"

constexpr int MAX_LEX_SIZE = 4096;
constexpr char MARK = '\'';
constexpr char NEW_LINE = '|';
constexpr char SEMICOLON = ';';
constexpr char COMMA = ',';
constexpr char LEFT_BRACE = '{';
constexpr char RIGHT_BRACE = '}';
constexpr char LEFTTHESIS = '(';
constexpr char RIGHTTHESIS = ')';
constexpr char PLUS = '+';
constexpr char MINUS = '-';
constexpr char STAR = '*';
constexpr char EQUAL = '=';
constexpr char DIRSLASH = '/';
constexpr int ID_SIZE = ID_MAXSIZE - 1;

namespace LexAn
{
	struct LEX
	{
		IT::IdTable idTable;
		LT::LexTable lexTable;

		LEX(IT::IdTable idTable, LT::LexTable lexTable) :
			idTable(idTable), lexTable(lexTable) {}
	};
	char determineLexeme();
	LEX lexAnalize(Parm::PARM param, In::IN in);
}