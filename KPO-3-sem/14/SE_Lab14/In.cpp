#include "stdafx.h"
#include "In.h"
#include "Error.h"

using namespace std;

namespace In
{
	IN getin(wchar_t infile[])
	{
				IN in;
		in.size = 0; in.lines = 0; in.ignore = 0; // 14 лр, стр. 12: ...номер строки (отсчет от 0) и номер позиции в строке (отсчет от 0)...
		int col = 0;

		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];

		ifstream file(infile);
		if (!file.is_open())
			throw ERROR_THROW(110);

		while (in.size < IN_MAX_LEN_TEXT)
		{
			unsigned char ch = static_cast<unsigned char>(file.get());

			if (file.eof())
			{
				text[in.size] = '\0';
				break;
			}
			switch (in.code[ch])
			{
			case in.T:
				text[in.size] = ch;
				in.size++;
				col++;
				break;

			case in.I:
				in.ignore++;
				col++;
				break;

			case in.F:
				throw ERROR_THROW_IN(111, in.lines, col);

			default:
				text[in.size++] = in.code[ch];
				col++;
				break;
			}
			if (ch == IN_CODE_ENDL)
			{
				in.lines++;
				col = 0;
			}
		}
		in.text = text;
		return in;
	}
}

