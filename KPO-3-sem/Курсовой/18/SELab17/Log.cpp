#include "stdafx.h"
#include "Log.h"
#pragma warning(disable:4996)

using namespace std;

namespace Log
{
	LOG getlog(const std::wstring& logfile) {
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (!log.stream->is_open()) {
			throw ERROR_THROW(112);
		}
		log.logfile = logfile;
		return log;
	}

	void WriteLine(LOG log, char* c, ...) {
		char** currentArg = &c;
		while (*currentArg != nullptr && *currentArg != "") {
			*log.stream << *currentArg;
			currentArg++;
		}
	}

	void WriteLine(LOG log, wchar_t* c, ...) {
		wchar_t** currentArg = &c;

		while (*currentArg != nullptr && *currentArg != L"") {
			size_t len = wcstombs(nullptr, *currentArg, 0) + 1;
			if (len == (size_t)-1) {
				break;
			}

			char* tempBuffer = (char*)malloc(len);
			if (tempBuffer != nullptr) {
				wcstombs(tempBuffer, *currentArg, len);
				*log.stream << tempBuffer;
				free(tempBuffer);
			}
			currentArg++;
		}
	}

	void WriteLog(LOG log) {
		char date[100];
		tm local;
		time_t currentTime = time(NULL);
		localtime_s(&local, &currentTime);
		strftime(date, 100, "%d.%m.%Y %H:%M:%S ----", &local);
		*log.stream << " ----	Протокол	---- " << date << endl;
	}

	void WriteParm(LOG log, Parm::PARM parm) {
		*log.stream << " ----	Параметры	---- " << endl;
		*log.stream << "-log: \"" << std::string(parm.log.begin(), parm.log.end()) << "\"\n";
		*log.stream << "-out: \"" << std::string(parm.out.begin(), parm.out.end()) << "\"\n";
		*log.stream << "-in : \"" << std::string(parm.in.begin(), parm.in.end()) << "\"\n";
	}

	void WriteIn(LOG log, In::IN in) {
		*log.stream << " ----	Исходные данные	---- " << endl;
		*log.stream << "Количество символов : " << in.size << endl;
		*log.stream << "Проигнорировано     : " << in.ignore << endl;
		*log.stream << "Количество строк    : " << in.lines << endl;
	}

	void WriteError(LOG log, Error::ERROR error) {
		if (log.stream) {
			*log.stream << " ----	Ошибка	---- " << endl;
			*log.stream << "Ошибка " << error.id << ": " << error.message << endl;
			if (error.inext.col >= 0 && error.inext.line >= 0) {
				*log.stream << "Строка: " << error.inext.line << endl << "Столбец: " << error.inext.col << endl << endl;
			}
		}
		else {
			cout << "Ошибка " << error.id << ": " << error.message;
			if (error.inext.col >= 0 && error.inext.line >= 0) {
				cout << ", строка " << error.inext.line << ", позиция " << error.inext.col;
			}
			cout << " \n\n";
		}
	}

	void Close(LOG log) {
		log.stream->close();
		delete log.stream;
	}
}
