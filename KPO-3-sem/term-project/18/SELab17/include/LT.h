#pragma once

#define LEXEMA_FIXSIZE 1          // Фиксированный размер лексемы.
#define LT_MAXSIZE 4096           // Максимальный размер таблицы лексем.
#define LT_TI_NULLIDX 0XFFFFFFFF  // Указатель на отсутствующий индекс в таблице.

// Лексемы для типов данных.
#define LEX_TYPE		't'
#define LEX_INTEGER		LEX_TYPE           // Целое число.
#define LEX_STRING		LEX_TYPE           // Строка.
#define LEX_CHAR		LEX_TYPE           // Символ.

// Лексемы для идентификаторов и литералов.
#define LEX_ID		'i'                // Идентификатор.
#define LEX_LITERAL 'l'				   // Литерал.

// Лексемы для ключевых слов.
#define LEX_FUNCTION	'f'          // Ключевое слово: функция.
#define LEX_DECLARE		'd'          // Ключевое слово: объявление переменной.
#define LEX_RETURN		'r'          // Ключевое слово: возврат.
#define LEX_PRINT		'p'          // Ключевое слово: печать.
#define LEX_WRITE		'w'          // Ключевое слово: печать.
#define LEX_STRCMP		's'          // Ключевое слово: печать.
#define LEX_MAIN		'm'          // Ключевое слово: главная функция.
#define LEX_STR_CMP

// Лексемы для разделителей.
#define LEX_SEMICOLON ';'         // Символ: точка с запятой.
#define LEX_COMMA ','             // Символ: запятая.
#define LEX_LEFTBRACE '{'         // Символ: левая фигурная скобка.
#define LEX_BRACELET '}'          // Символ: правая фигурная скобка.
#define LEX_LEFTTHESIS '('        // Символ: левая круглая скобка.
#define LEX_RIGHTTHESIS ')'       // Символ: правая круглая скобка.

// Лексемы для арифметических операций.
#define LEX_MATH 'v'
#define LEX_PLUS		LEX_MATH          // Операция: плюс.
#define LEX_MINUS		LEX_MATH          // Операция: минус.
#define LEX_STAR		LEX_MATH          // Операция: умножение.
#define LEX_DIRSLASH	LEX_MATH          // Операция: деление.

// Лексема для присваивания.
#define LEX_EQUAL '='             // Операция: присваивание.

// Лексемы для операций сравнения.
#define LEX_COMPARE 'c'

#define LEX_COMPARE_EQUAL			LEX_COMPARE		// Операция: равно.
#define LEX_COMPARE_GREATER			LEX_COMPARE     // Операция: больше.
#define LEX_COMPARE_LESS			LEX_COMPARE		// Операция: меньше.
#define LEX_COMPARE_NOT_EQUAL		LEX_COMPARE     // Операция: не равно.
#define LEX_COMPARE_LESS_EQUAL		LEX_COMPARE     // Операция: меньше или равно.
#define LEX_COMPARE_GREATER_EQUAL	LEX_COMPARE		// Операция: больше или равно.

// Лексема для условного оператора.
#define LEX_IF '?'               // Условный оператор: if.
#define LEX_ELSE ':'             // Условный оператор: else.


namespace LT              // таблица лексем
{
	struct Entry          // строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE];  // лексема
		int sn;                       // номер строки в исходном тексте
		int idxTI;                    // индекс в таблице идентификаторов или LT_TI_NULLIDX
		char v; // математическая операция в виде символа(+,-,*,/)
		std::string c; // оператор сравнения
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

	bool hasMainLexem(const LexTable& table);
	void Delete(LexTable& lextable); // удалить таблицу лексем (освободить память)
}

