#include "stdafx.h"
#include "Log.h"
#pragma warning(disable:4996)

using namespace std;

namespace Log
{
	LOG getlog(wchar_t logfile[]) {
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (!log.stream->is_open()) {
			throw ERROR_THROW(112);
		}
		wcscpy_s(log.logfile, logfile);
		return log;
	}

	void WriteLine(LOG log, char* c, ...) {
		char** currentArg = &c;
		while (*currentArg != nullptr && *currentArg != "") {
			*log.stream << *currentArg;
			currentArg++;
		}
	}

	//void WriteLine(LOG log, wchar_t* c, ...)
	//{
	//	wchar_t** currentArg = &c;

	//	while (*currentArg != nullptr)
	//	{
	//		char tempBuffer[IN_MAX_LEN_TEXT];
	//		wcstombs_s(0, tempBuffer, *currentArg, sizeof(tempBuffer));
	//		*log.stream << tempBuffer;
	//		currentArg++;
	//	}
	//}

	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t** currentArg = &c;

		while (*currentArg != nullptr && *currentArg != L"")
		{
			//длина строки после преобразования
			size_t len = wcstombs(nullptr, *currentArg, 0) + 1; // +1 для \0
			if (len == (size_t)-1)
			{
				break; // ошибка преобразования https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/wcstombs-wcstombs-l?view=msvc-170
			}

			char* tempBuffer = (char*)malloc(len);

			if (tempBuffer != nullptr)
			{
				wcstombs(tempBuffer, *currentArg, len);
				*log.stream << tempBuffer;
				free(tempBuffer);
			}

			currentArg++;
		}
	}

	void WriteLog(LOG log)
	{
		char date[100];
		tm local;
		time_t currentTime;
		currentTime = time(NULL);
		localtime_s(&local, &currentTime);
		strftime(date, 100, "%d.%m.%Y %H:%M:%S ----", &local);
		*log.stream << " ----	Протокол	---- " << date << endl;

	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		char buff[PARM_MAX_SIZE];
		size_t tsize = 0;

		*log.stream << " ----	Параметры	---- " << endl;
		wcstombs_s(&tsize, buff, parm.log, PARM_MAX_SIZE);
		*log.stream << "-log: \"" << buff << "\"\n";
		wcstombs_s(&tsize, buff, parm.out, PARM_MAX_SIZE);
		*log.stream << "-out: \"" << buff << "\"\n";
		wcstombs_s(&tsize, buff, parm.in, PARM_MAX_SIZE);
		*log.stream << "-in : \"" << buff << "\"\n";

	}

	void WriteIn(LOG log, In::IN in) {

		*log.stream << " ----	Исходные данные	---- " << endl;
		*log.stream << "Количество символов : " << in.size << endl;
		*log.stream << "Проигнорировано     : " << in.ignore << endl;
		*log.stream << "Количество строк    : " << in.lines << endl;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream)
		{
			*log.stream << " ----	Ошибка	---- " << endl;
			*log.stream << "Ошибка " << error.id << ": " << error.message << endl;
			if (error.inext.line != -1)
			{
				*log.stream << "Строка: " << error.inext.line << endl << "Столбец: " << error.inext.col << endl << endl;
			}
		}
		else
			cout << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col << endl << endl;
	}

	void Close(LOG log) {
		log.stream->close();
		delete log.stream;
	}
}