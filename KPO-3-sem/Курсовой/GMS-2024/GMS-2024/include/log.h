#ifndef LOG_H
#define LOG_H

#include "in.h"
#include "parm.h"
#include "error.h"
#include <string>

namespace Log	// Работа с протоколом
{
	struct LOG // протокол
	{
		std::wstring logfile;  // имя файла протокола
		std::unique_ptr<std::ofstream> stream; // выходной поток протокола
	};

	static const LOG INIT_LOG{ L"", nullptr };// структура для начальной инициализации LOG
	LOG getlog(const std::wstring& logfile);// сформировать структуру LOG
	void writeLine(LOG& log, char* c, ...);// вывести в протокол конкатенацию строк
	void writeLine(LOG& log, wchar_t* с, ...); //вывести в протокол конкатенацию строк
	void writeLine(LOG& log, const std::string& str);
	void writeLine(LOG& log, const std::wstring& str);
	void writeLog(LOG& log);// вывести в протокол заголовок
	void writeParm(LOG& log, Parm::PARM parm); // вывести в протокол информацию о входных параметрах
	void writeIn(LOG& log, In::IN& in);// вывести в протокол информацию о входном потоке
	void writeError(LOG& log, Error::ERROR& error); // вывести в протокол информацию об ошибке
	void close(LOG& log);
};

#endif // !LOG_H
