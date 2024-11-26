#include <Windows.h>
#include <iostream>
#include "deserialization.h"

using namespace std;

int main() {
	SetConsoleOutputCP(1251);

	try {
		ParseBinaryAndGenerateAssembly("output.bin", "GenAsm.asm");
		cout << ".asm file saved as GenAsm.asm";
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
