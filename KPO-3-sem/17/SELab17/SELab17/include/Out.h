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
		std::wstring outfile;    // ��� ��������� �����
		std::ofstream* stream;   // �������� �����
	};
	static const OUT INIT_OUT = { L"", NULL };
	OUT getout(const std::wstring& outfile); // ������� ��� �������� ��������� �����
	void WriteOut(OUT out, In::IN in);       // ������ ������ �� �������� ������ � �������� ����
	void WriteError(OUT out, Error::ERROR error); // ������ ������ � �������� ����
	void Close(OUT out);                     // �������� ��������� �����
};

#endif // !OUT_H
