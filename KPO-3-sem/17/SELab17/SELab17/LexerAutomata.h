#pragma once

#include "LT.h"
#include "IT.h"
#include "FST.h"
#include "Parm.h"
#include "In.h"

#define MAX_LEX_SIZE 4096
#define MARK '\''
#define NEW_LINE '|'
#define SEMICOLON ';'
#define COMMA ','
#define LEFT_BRACE '{'
#define RIGHT_BRACE '}'
#define LEFTTHESIS '('
#define RIGHTTHESIS ')'
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define EQUAL '='
#define DIRSLASH '/'

namespace LexAn
{
	struct LexicalAnalyzer
	{
		bool _isStringLiteral = false;
		bool _isDeclare = false;

		LT::LexTable _lexTable = LT::Create(LT_MAXSIZE - 1);
		IT::IdTable _idTable = IT::Create(TI_MAXSIZE - 1);

		char* _str = new char[MAX_LEX_SIZE];

		char _determineLexeme();
		void _lexAnalize(Parm::PARM  param, In::IN in);
	};
}