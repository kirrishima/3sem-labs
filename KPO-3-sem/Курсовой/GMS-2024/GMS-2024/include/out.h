#ifndef OUT_H
#define OUT_H

#include "in.h"
#include "parm.h"
#include "error.h"
#include <string>

namespace Out
{
	struct OUT
	{
		std::wstring outfile;    // имя выходного файла
		std::unique_ptr<std::ofstream> stream;   // выходной поток
	};
	static const OUT INIT_OUT = { L"", NULL };
	OUT getout(const std::wstring& outfile); // функция для открытия выходного файла
	void writeOut(OUT& out, In::IN& in);       // запись данных из входного потока в выходной файл
	void writeError(OUT& out, Error::ERROR& error); // запись ошибок в выходной файл
	void close(OUT& out);                     // закрытие выходного файла
};

#endif // !OUT_H
