#include "stdafx.h"
#include "Out.h"

namespace Out
{
	OUT getout(const std::wstring& outfile)
	{
		OUT a;
		a.stream = new std::ofstream;
		a.stream->open(outfile);
		if (a.stream->fail()) {
			throw ERROR_THROW(113);
		}
		a.outfile = outfile; // ��������� ��� ����� � ������������ ������
		return a;
	}

	void WriteOut(OUT out, In::IN in)
	{
		*out.stream << in.text << std::endl;
	}

	void WriteError(OUT out, Error::ERROR error)
	{
		if (out.stream)
		{
			*out.stream << " ----	������	---- " << std::endl;
			*out.stream << "������ " << error.id << ": " << error.message << std::endl;
			if (error.inext.col >= 0 && error.inext.line >= 0)
			{
				*out.stream << "������: " << error.inext.line << std::endl << "�������: " << error.inext.col << std::endl << std::endl;
			}
		}
	}

	void Close(OUT out)
	{
		if (out.stream) {
			out.stream->close();
			delete out.stream;
			out.stream = NULL;
		}
	}
}