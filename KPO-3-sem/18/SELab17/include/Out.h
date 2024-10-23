#ifndef OUT_H
#define OUT_H

#include "In.h"
#include "Parm.h"
#include "Error.h"
#include <string>

namespace Out
{
	struct OUT
	{
		std::wstring outfile;    // имя выходного файла
		std::ofstream* stream;   // выходной поток
	};
	static const OUT INIT_OUT = { L"", NULL };
	OUT getout(const std::wstring& outfile); // функция для открытия выходного файла
	void WriteOut(OUT out, In::IN in);       // запись данных из входного потока в выходной файл
	void WriteError(OUT out, Error::ERROR error); // запись ошибок в выходной файл
	void Close(OUT out);                     // закрытие выходного файла
};

#endif // !OUT_H
