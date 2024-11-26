#include "../SerializationStaticLib/serializer.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
	std::vector<int> intVector;
	std::vector<std::string> stringVector;

	int n;

	std::cout << "Enter the number of elements for the integer vector: ";
	std::cin >> n;

	std::cout << "Enter " << n << " integers:\n";
	for (int i = 0; i < n; ++i) {
		int value;
		std::cin >> value;
		intVector.push_back(value);
	}

	std::cout << "Enter the number of elements for the string vector: ";
	std::cin >> n;

	std::cout << "Enter " << n << " strings:\n";
	std::cin.ignore();
	for (int i = 0; i < n; ++i) {
		std::string value;
		std::getline(std::cin, value);
		stringVector.push_back(value);
	}

	Data myData;

	myData.integers = intVector;
	myData.strings = stringVector;

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