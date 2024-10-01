#include"stdafx.h"
#include"Error.h"
#include"In.h"
#include <cstring>
#include "Utils.h"

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

		int tmpLength = 0;

		file.open(infile);
		if (!file.is_open())
		{
			throw ERROR_THROW(110);
		}
		in_result.text = new unsigned char[IN_MAX_LEN_TEXT];

		std::string tmp;

		while (std::getline(file, tmp))
		{
			tmp = utils::trim(tmp);
			tmpLength = tmp.length();

			for (int position = 0; position < tmpLength; position++)
			{
				auto ch = static_cast<unsigned char>(tmp[position]);
				switch (in_result.code[ch])
				{
				case IN::T:
					in_result.text[in_result.size++] = ch;
					break;
				case IN::F:
					in_result.text[in_result.size] = '\0';
					throw ERROR_THROW_IN(111, in_result.lines, position + 1, in_result.text);
					break;
				case IN::I:
					in_result.ignore++;
					break;
				case IN::Space:
					if (in_result.size > 0 && in_result.text[in_result.size - 1] == SPACE)
					{
						in_result.ignore++;
						break;
					}
					in_result.text[in_result.size++] = ch;
					break;

				case IN::Asterisk:
				case IN::Equal:
				case IN::LeftBrace:
				case IN::LeftParen:
				case IN::Minus:
				case IN::Plus:
				case IN::RightBrace:
				case IN::RightParen:
				case IN::Semicolon:
				case IN::Slash:
					if (in_result.size > 0 && in_result.text[in_result.size - 1] == SPACE)
					{
						in_result.ignore++;
						break;
					}
					if (position + 1 < tmpLength && tmp[position + 1] == SPACE)
					{
						position++;
					}
					in_result.text[in_result.size++] = ch;
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