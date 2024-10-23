#pragma once

#define LEXEMA_FIXSIZE 1
#define	LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0XFFFFFFFF
#define LEX_INTEGER 't'
#define LEX_STRING 't'
#define LEX_ID 'i'
#define LEX_LITERAL 'l'
#define LEX_FUNCTION 'f'
#define LEX_DECLARE 'd'
#define LEX_RETURN 'r'
#define LEX_PRINT 'p'
#define LEX_MAIN 'm'
#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define LEX_BRACELET '}'
#define LEX_LEFTTHESIS '('
#define LEX_RIGHTTHESIS ')'
#define LEX_PLUS 'v'
#define LEX_MINUS 'v'
#define LEX_STAR 'v'
#define LEX_DIRSLASH 'v'
#define LEX_EQUAL '='

namespace LT              // таблица лексем
{
	struct Entry          // строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE];  // лексема
		int sn;                       // номер строки в исходном тексте
		int idxTI;                    // индекс в таблице идентификаторов или LT_TI_NULLIDX
	};

	struct LexTable        // экземпляр таблицы лексем
	{
		int maxsize;       // емкость таблицы лексем < LT_MAXSIZE
		int size;          // текущий размер таблицы лексем < maxsize
		Entry* table;      // массив строк таблицы лексем
	};

	LexTable Create(       // создать таблицу лексем
		int size           // емкость таблицы лексем < LT_MAXSIZE
	);

	void Add(               // добавить строку в таблицу лексем
		LexTable& lextable, // экземпляр таблицы лексем
		Entry entry         // строка таблицы лексем
	);

	Entry GetEntry(         // получить строку таблицы лексем
		const LexTable& lextable, // экземпляр таблицы лексем
		int n               // номер получаемой строки
	);

	void Delete(LexTable& lextable); // удалить таблицу лексем (освободить память)
}

