#ifndef PARAM_H
#define PARAM_H

#include <string>

#define PARM_IN L"-in:" // ключ для файла исходного кода
#define PARM_OUT L"-out:" // ключ для файла объектного кода
#define PARM_LOG L"-log:" // ключ для файла журнала
#define PARM_OUT_DEFAULT_EXT L".out" // расширение файла объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log" // расширение файла объектного кода по умолчанию

namespace Parm // обработка входных параметров 
{
	struct PARM // входные параметры	
	{
		std::wstring in;  // -in: имя файла исходного кода
		std::wstring out; // -out: имя файла объектного кода
		std::wstring log; // -log: имя файла протокола
	};

	PARM getparm(int argc, _TCHAR* argv[]); // сформировать struct PARM на основе параметров функции main

};

#endif // !PARAM_H