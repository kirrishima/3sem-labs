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
	void writeLog(LOG log);// вывести в протокол заголовок
	void writeParm(LOG log, Parm::PARM parm); // вывести в протокол информацию о входных параметрах
	void writeIn(LOG log, In::IN in);// вывести в протокол информацию о входном потоке
	void writeError(LOG log, Error::ERROR error); // вывести в протокол информацию об ошибке
	void close(LOG log);
};

#endif // !LOG_H
