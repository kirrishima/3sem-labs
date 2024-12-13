#pragma once
#include <string>
#include <IT.h>
#include <LT.h>

namespace utils
{
	std::pair<int, int> trimmed_pos(const std::string& str);
}

namespace LexAn::Utils
{
	void printToFile(const IT::ID_Table& ID_Table, const std::wstring& IT_filename,
		const LT::LexTable& LexTable, const std::wstring& LT_filename, const In::IN& in);

	int stringToNumber(const std::string& str);
	bool isSingleCharacter(const unsigned char* input, std::size_t endPos);
}