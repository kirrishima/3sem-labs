#pragma once
#include <string>
#include <IT.h>
#include <LT.h>

namespace utils
{
	/*std::string trimmed_pos(const std::string& str);*/
	std::pair<int, int> trimmed_pos(const std::string& str);
}

namespace LexAn::Utils
{
	void printToFile(const IT::IdTable& ID_Table, const std::wstring& IT_filename,
		const LT::LexTable& LexTable, const std::wstring& LT_filename, const In::IN& in);
}