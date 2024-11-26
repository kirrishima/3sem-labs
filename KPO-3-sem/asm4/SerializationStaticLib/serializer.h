#pragma once
#include <string>
#include <vector>

enum TypeFlags {
	INT = 1,
	STR = 2
};

struct Data {
	std::vector<int> integers;
	std::vector<std::string> strings;
};

Data readFromBinaryFile(const std::string& filename);

void writeToBinaryFile(const std::string& filename, const Data& data);