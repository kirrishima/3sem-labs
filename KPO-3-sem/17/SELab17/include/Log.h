#ifndef LOG_H
#define LOG_H

#include "In.h"
#include "Parm.h"
#include "Error.h"
#include <string>

namespace Log	// Работа с протоколом
{
	struct LOG // протокол
	{
		std::wstring logfile;  // имя файла протокола
		std::ofstream* stream; // выходной поток протокола
	};

	static const LOG INIT_LOG{ L"", NULL };// структура для начальной инициализации LOG
	LOG getlog(const std::wstring& logfile);// сформировать структуру LOG
	void WriteLine(LOG log, char* c, ...);// вывести в протокол конкатенацию строк
	void WriteLine(LOG log, wchar_t* с, ...); //вывести в протокол конкатенацию строк
	void WriteLog(LOG log);// вывести в протокол заголовок
	void WriteParm(LOG log, Parm::PARM parm); // вывести в протокол информацию о входных параметрах
	void WriteIn(LOG log, In::IN in);// вывести в протокол информацию о входном потоке
	void WriteError(LOG log, Error::ERROR error); // вывести в протокол информацию об ошибке
	void Close(LOG log);
};

#endif // !LOG_H
