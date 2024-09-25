#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include <vector>  
#include <fstream>

using namespace std;

typedef unsigned char uchar;

namespace In
{
	IN getin(std::wstring infile)
	{
		IN in;
		in.size = 0;
		in.lines = 0;
		in.ignore = 0;
		int col = 0;
		bool last_was_space = false;
		bool last_was_quote = false;

		ifstream file(infile);
		if (!file.is_open())
			throw Error::geterror(110);

		uchar* text = new uchar[IN_MAX_LEN_TEXT];
		size_t textLen = 0;

		auto func = [&]() {
			in.lines++;
			col = 0;
			last_was_space = false;

			bool semicolon = false;

			if (text[in.size - 1] == (uchar)';')
				semicolon = true;

			while (text[in.size - 1] == (uchar)' ' || text[in.size - 1] == (uchar)';')
				text[in.size--] = '\0';

			if (semicolon)
				text[in.size++] = ';';
			};

		while (in.size < IN_MAX_LEN_TEXT)
		{
			uchar ch = static_cast<uchar>(file.get());

			if (file.eof())
			{
				func();
				break;
			}

			if (ch == IN_CODE_ENDL)
			{
				func();
				text[in.size++] = ch;
			}
			else
			{
				switch (in.code[ch])
				{
				case in.T:
					if (!(last_was_space && ch == ' '))
					{
						text[in.size++] = ch;
						col++;
					}
					last_was_space = (ch == ' ') && !last_was_quote;

					if (ch == '\'')
					{
						last_was_quote = ~last_was_quote;
					}
					break;

				case in.I:
					in.ignore++;
					col++;
					break;

				case in.F:
					throw ERROR_THROW_IN(111, in.lines, col);

				default:
					throw ERROR_THROW_IN(111, in.lines, col);
					break;
				}
			}
		}

		text[in.size++] = '\0';

		size_t finalLen = strlen(reinterpret_cast<char*>(text)) + 1;
		in.text = new uchar[finalLen];
		memcpy(in.text, text, finalLen);
		return in;
	}
}