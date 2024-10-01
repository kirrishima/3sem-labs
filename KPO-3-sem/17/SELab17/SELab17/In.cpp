#include"stdafx.h"
#include"Error.h"
#include"In.h"
#include <cstring>
#include "Utils.h"

namespace In
{
	IN getin(std::wstring infile)
	{
		IN in;

		in.size = 0;
		in.lines = 1;

		std::ifstream file(infile);
		if (!file.is_open())
		{
			throw ERROR_THROW(110);
		}

		in.text = new unsigned char[IN_MAX_LEN_TEXT];

		std::string tmp;
		int tmpLength = 0;

		while (std::getline(file, tmp))
		{
			tmp = utils::trim(tmp);
			tmpLength = tmp.length();

			for (int position = 0; position < tmpLength; position++)
			{
				auto ch = static_cast<unsigned char>(tmp[position]);
				switch (in.code[ch])
				{
				case IN::T:
					in.text[in.size++] = ch;
					break;
				case IN::F:
					in.text[in.size] = '\0';
					throw ERROR_THROW_IN(111, in.lines, position + 1, in.text);
					break;
				case IN::I:
					in.ignore++;
					break;
				case IN::Space:
					if (in.size > 0 && in.text[in.size - 1] == SPACE)
					{
						in.ignore++;
						break;
					}
					in.text[in.size++] = ch;
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
				case IN::Comma:
				{
					if (position + 1 < tmpLength && tmp[position + 1] == SPACE)
					{
						while (position + 1 < tmpLength && tmp[position + 1] == SPACE) {
							position++;
						}
					}
					if (in.size > 0 && in.text[in.size - 1] == SPACE)
					{
						in.text[in.size - 1] = ch;
					}
					else
					{
						in.text[in.size++] = ch;
					}

					break;
				}
				default:
					in.text[in.size] = in.code[tmp[position]];
					in.size++;
				}
			}
			if (in.text[in.size - 1] != ',')
			{
				in.lines++;
				in.text[in.size++] = '|';
			}
		}

		in.text[in.size--] = '\0';
		return in;
	}
}