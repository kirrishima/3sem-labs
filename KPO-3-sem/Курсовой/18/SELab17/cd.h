#pragma once
#include <string>
#include "LT.h"
#include "IT.h"

namespace CD
{
	void gen(const LT::LexTable& LEX_TABLE, const IT::ID_Table& ID_TABLE, const std::wstring& OUT_FILEPATH = L"E:\\3 sem\\KPO-3-sem\\Курсовой\\18\\Debug\\result.txt");
}