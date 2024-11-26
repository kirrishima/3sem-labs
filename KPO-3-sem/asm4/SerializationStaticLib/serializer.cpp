#include "pch.h"
#include "serializer.h"

void writeToBinaryFile(const std::string& filename, const Data& data) {
	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile) {
		throw "Failed to open file for writing";
	}

	for (int number : data.integers) {
		uint8_t typeFlag = TypeFlags::INTEGER;
		outFile.write(reinterpret_cast<char*>(&typeFlag), sizeof(typeFlag));
		outFile.write(reinterpret_cast<char*>(&number), sizeof(number));
	}

	for (const auto& str : data.strings) {
		uint8_t typeFlag = TypeFlags::STR;
		uint32_t size = static_cast<uint32_t>(str.size());

		outFile.write(reinterpret_cast<char*>(&typeFlag), sizeof(typeFlag));
		outFile.write(reinterpret_cast<char*>(&size), sizeof(size));
		outFile.write(str.data(), size);
	}

	outFile.close();
}


Data readFromBinaryFile(const std::string& filename) {
	std::ifstream inFile(filename, std::ios::binary);
	if (!inFile) {
		throw "Failed to open file for reading";
	}

	Data data;

	while (inFile) {
		uint8_t typeFlag;
		if (!inFile.read(reinterpret_cast<char*>(&typeFlag), sizeof(typeFlag))) {
			break;
		}

		if (typeFlag == TypeFlags::INTEGER) {
			int number;
			inFile.read(reinterpret_cast<char*>(&number), sizeof(number));
			data.integers.push_back(number);
		}
		else if (typeFlag == TypeFlags::STR) {
			uint32_t size;
			inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

			char* buff = new char[size + 1];
			inFile.read(buff, size);
			buff[size] = '\0';
			data.strings.push_back(std::string(buff));
		}
		else {
			throw "Unknown type flag encountered in file";
		}
	}

	inFile.close();
	return data;
}
