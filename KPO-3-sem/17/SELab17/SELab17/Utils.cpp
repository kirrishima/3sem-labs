#include "stdafx.h"
#include "Utils.h"
#include <regex>

namespace utils
{
	std::string trim(const std::string& str)
	{
		size_t first = str.find_first_not_of(' ');
		if (first == std::string::npos) return "";

		size_t last = str.find_last_not_of(' ');
		return str.substr(first, last - first + 1);
	}

	std::string removeExtraSpaces(const std::string& input) {
		std::string result;
		bool insideQuotes = false;  // ‘лаг, указывающий, находимс€ ли мы внутри кавычек
		int z = 0;
		int exited = 0;
		for (size_t i = 0; i < input.length(); ++i) {
			if (input[i] == '\'') {
				insideQuotes = !insideQuotes;  // ћен€ем состо€ние флага при нахождении кавычки
				exited++;
			}

			if (insideQuotes) {
				// ≈сли мы не внутри кавычек, используем регул€рное выражение дл€ удалени€ лишних пробелов
				static const std::regex spaceRegex("\\s{2,}"); // Ќаходим два и более пробелов
				if (exited % 2 == 0)
				{
					result += std::regex_replace(input.substr(z, i), spaceRegex, " ");
				}
				else
				{
					result += input[i];
				}
			}
		}

		return result;
	}
}
