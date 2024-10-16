#include "stdafx.h"
#include "Out.h"

namespace Out
{
	OUT getout(wchar_t outfile[])
	{
		OUT a;
		a.stream = new std::ofstream;
		a.stream->open(outfile);
		if (a.stream->fail()) {
			throw ERROR_THROW(113);
		}
		wcscpy_s(a.outfile, outfile);
		return a;
	}


	void WriteOut(OUT out, In::IN in)
	{
		*out.stream << in.text << std::endl;
	}

	void WriteError(OUT log, Error::ERROR error)
	{
		*log.stream << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col << std::endl;
	}

	void Close(OUT log)
	{
		log.stream->close();
		delete log.stream;
		log.stream = NULL;
	}
}