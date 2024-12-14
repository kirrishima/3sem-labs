#include "stdafx.h"
#include "Log.h"
#include <Utils.h>
#include <iomanip>

#pragma warning(disable:4996)

using namespace std;

namespace Log
{
	const int fieldWidth = 8;

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
		strftime(date, 100, "	%d.%m.%Y %H:%M:%S	----", &local);
		*log.stream << " ----	Протокол" << date << endl;
	}

	void WriteParm(LOG log, Parm::PARM parm) {
		*log.stream << " ----    Параметры    ---- " << std::endl;

		*log.stream << std::left;
		*log.stream << std::setw(fieldWidth) << "-log:" << "\"" << utils::wstring_to_string(parm.log) << "\"" << std::endl;
		*log.stream << std::setw(fieldWidth) << "-out:" << "\"" << utils::wstring_to_string(parm.out) << "\"" << std::endl;
		*log.stream << std::setw(fieldWidth) << "-in:" << "\"" << utils::wstring_to_string(parm.in) << "\"" << std::endl;
		*log.stream << std::setw(fieldWidth) << "-lt:" << "\"" << utils::wstring_to_string(parm.lt) << "\"" << std::endl;
		*log.stream << std::setw(fieldWidth) << "-it:" << "\"" << utils::wstring_to_string(parm.it) << "\"" << std::endl;
		*log.stream << std::setw(fieldWidth) << "-stack:" << parm.stackSize << std::endl;

		*log.stream << "\n ----    Опции    ---- " << std::endl;

		*log.stream << std::setw(fieldWidth) << "/lex:" << (parm.enableLexAnSave ? "True" : "False") << std::endl;
		*log.stream << std::setw(fieldWidth) << "/cst:" << (parm.CST ? "True" : "False") << std::endl << std::endl;
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
