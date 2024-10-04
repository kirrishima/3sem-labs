#include "stdafx.h"
#include "Error.h"
#include "In.h"
#include "Utils.h"

namespace In
{
	IN getin(std::wstring infile)
	{
		IN in;

		in.size = 0;
		in.lines = 1;
		in.ignore = 0;

		std::ifstream file(infile);
		if (!file.is_open())
		{
			throw ERROR_THROW(110);
		}

		in.text = new unsigned char[IN_MAX_LEN_TEXT];

		std::string tmp;
		int trimmedLength = 0;

		while (std::getline(file, tmp))
		{
			auto trimmedString = utils::trim(tmp);

			trimmedLength = trimmedString.length();

			in.ignore += tmp.length() - trimmedString.length();

			for (int position = 0; position < trimmedLength; position++)
			{
				auto ch = static_cast<unsigned char>(trimmedString[position]);
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
					if (position + 1 < trimmedLength && trimmedString[position + 1] == SPACE)
					{
						while (position + 1 < trimmedLength && trimmedString[position + 1] == SPACE) {
							position++;
							in.ignore++;
						}
					}
					if (in.size > 0 && in.text[in.size - 1] == SPACE)
					{
						in.text[in.size - 1] = ch;
						in.ignore++;
					}
					else
					{
						in.text[in.size++] = ch;
					}

					break;
				}
				default:
					in.text[in.size] = in.code[trimmedString[position]];
					in.size++;
				}
			}
			if (in.text[in.size - 1] != ',')
			{
				in.lines++;
				in.text[in.size++] = '|';
			}
		}

		in.text[--in.size] = '\0';
		return in;
	}
}