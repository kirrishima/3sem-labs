#include "../SerializationStaticLib/serializer.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "parse_file.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "ru");

	std::vector<std::string> tokens;

	Data myData = parse_dec_file("input.txt");

	if (myData.integers.empty() && myData.strings.empty())
	{
		return 1;
	}
	cout << '\n';

	try {
		writeToBinaryFile("output.bin", myData);
		std::cout << "Data written to output.bin successfully." << std::endl;
	}
	catch (const char* e) {
		std::cerr << "Error: " << e << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Standart Error: " << e.what() << std::endl;
	}

	std::cout << "\n";

	return 0;
}