#ifndef OUT_H
#define OUT_H

#include "In.h"
#include "Parm.h"
#include "Error.h"

namespace Out
{
	struct OUT
	{
		wchar_t outfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};
	static const OUT INITOUT = { L"", NULL };
	OUT getout(wchar_t outfile[]);
	void WriteOut(OUT out, In::IN in);
	void WriteError(OUT out, Error::ERROR error);
	void Close(OUT out);
};

#endif // !OUT_H