#include"stdafx.h"
#include"In.h"
#include"Error.h"
#include"Out.h"
#include"Parm.h"
#include"Log.h"
#include <cstring>
#include <cstdlib>

namespace In
{
	IN getin(std::wstring infile)
	{
		IN in_result;
		unsigned char symbol;
		in_result.size = 0;
		in_result.lines = 0;
		bool failSpace = true;
		std::ifstream file;

		file.open(infile);
		if (!file.is_open())
		{
			throw ERROR_THROW(110);
		}
		in_result.text = new unsigned char[IN_MAX_LEN_TEXT];
		char* tmp = new char[IN_MAX_LEN_TEXT];

		while (file.getline(tmp, 1000))
		{
			for (int position = 0; position < strlen(tmp); position++)
			{
				switch (in_result.code[int((unsigned char)tmp[position])])
				{
				case IN::T:
					if (tmp[position] == 32 && position == 0)
					{
						while (tmp[position] == ' ')
						{
							position++;
						}
					}
					if (tmp[position] == 32 && tmp[position + 1] == 32)
					{
						in_result.ignore++;
						break;
					}
					if (tmp[position] == 32 && (tmp[position + 1] == '{' || tmp[position + 1] == '}' || tmp[position + 1] == '(' || tmp[position + 1] == ')' || tmp[position + 1] == ';' ||
						tmp[position + 1] == '+' || tmp[position + 1] == '-' || tmp[position + 1] == '*' || tmp[position + 1] == '/' || tmp[position + 1] == '='))
					{
						in_result.ignore++;
						break;
					}
					if (tmp[position] == 32 && (tmp[position - 1] == '{' || tmp[position - 1] == '}' || tmp[position - 1] == '(' || tmp[position - 1] == ')' || tmp[position - 1] == ';' ||
						tmp[position - 1] == '+' || tmp[position - 1] == '-' || tmp[position - 1] == '*' || tmp[position - 1] == '/' || tmp[position - 1] == '=' || tmp[position - 1] == ','))
					{
						in_result.ignore++;
						break;
					}
					if (position == strlen(tmp) - 1 && tmp[strlen(tmp) - 1] == 32)
					{
						in_result.ignore++;
						break;
					}
					in_result.text[in_result.size] = (unsigned)tmp[position];
					in_result.size++;
					break;
				case IN::F:
					in_result.text[in_result.size] = '\0';
					/*Out::WriteText(out, in_result);
					Out::WriteError(out, ERROR_THROW_IN(111, in_result.lines, position+1));
					Log::WriteError(log, ERROR_THROW_IN(111, in_result.lines, position+1));
					Log::Close(log);
					Out::Close(out);*/
					throw ERROR_THROW_IN(111, in_result.lines, position + 1, in_result.text);
					break;
				case IN::I:
					in_result.ignore++;
					break;
				default:
					in_result.text[in_result.size] = in_result.code[tmp[position]];
					in_result.size++;
				}
			}
			in_result.lines++;
			in_result.text[in_result.size] = '|';
			in_result.size++;
		}
		in_result.text[in_result.size] = '\0';
		return in_result;
	}
}