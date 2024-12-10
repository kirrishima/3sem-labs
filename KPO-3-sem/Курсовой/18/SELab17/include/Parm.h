#ifndef PARAM_H
#define PARAM_H

#include <string>

#define PARM_IN L"in" // ключ для файла исходного кода
#define PARM_OUT L"out" // ключ для файла объектного кода
#define PARM_LOG L"log" // ключ для файла журнала
#define PARM_LT L"lt" // ключ для таблицы лексем
#define PARM_IT L"it" // ключ для таблицы id
#define PARM_STACK L"stack" // ключ для таблицы id
#define PARM_F_LEX L"lex" // отключение сохранения таблицы id
#define PARM_OUT_DEFAULT_EXT L".out" // расширение файла объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log" // расширение файла объектного кода по умолчанию
#define PARM_LT_TABLE_DEFAULT L".LT.html" // расширение файла объектного кода по умолчанию
#define PARM_IT_TABLE_DEFAULT L".IT.html" // расширение файла объектного кода по умолчанию
#define PARM_STACK_DEFAULT_SIZE 4096 // расширение файла объектного кода по умолчанию
#define STACK_MIN_SIZE 1024
#define STACK_MAX_SIZE STACK_MIN_SIZE * STACK_MIN_SIZE

namespace Parm // обработка входных параметров 
{
	struct PARM // входные параметры	
	{
		std::wstring in;  // -in: имя файла исходного кода
		std::wstring out; // -out: имя файла объектного кода
		std::wstring log; // -log: имя файла протокола
		std::wstring it;
		std::wstring lt;
		std::wstring masmDest;
		std::wstring obj;
		std::wstring exe;
		size_t stackSize;
		bool enableLexAnSave = false;
	};

	PARM getparm(int argc, _TCHAR* argv[]); // сформировать struct PARM на основе параметров функции main

};

#endif // !PARAM_H