#include "deserialization.h"
#include "../SerializationStaticLib/serializer.h"
#include <iostream>
#include <fstream>

using namespace std;

string tab(4, ' ');

void add_var_inspection(vector <std::string>& vars, std::string var, int num)
{
	std::string formattedVar = var + to_string(num) + "\n";
	vars.push_back("\tmov eax, type        " + formattedVar);
	vars.push_back("\tmov eax, lengthof    " + formattedVar);
	vars.push_back("\tmov eax, sizeof      " + formattedVar);
	vars.push_back("\n");
}

void ParseBinaryAndGenerateAssembly(const char* binaryFilename, const char* outputFilename)
{
	try
	{
		Data readData = readFromBinaryFile(binaryFilename);
		std::cout << "Data read from output.bin:" << std::endl;

		std::cout << "Integers: ";
		for (int number : readData.integers) {
			std::cout << number << " ";
		}
		std::cout << "\nStrings: ";
		for (const auto& str : readData.strings) {
			std::cout << "\"" << str << "\" ";
		}
		std::cout << std::endl << std::endl;

		ofstream file(outputFilename);

		file << ".586\n";
		file << ".model flat, stdcall\n";
		file << "ExitProcess PROTO : DWORD\n";
		file << "\n.stack 4096\n\n";

		file << ".data\n";
		int countVar = 0;

		vector <std::string> vars;

		if (readData.integers.size() > 0)
		{
			file << ";Parsed ints: \n";
			for (const int number : readData.integers)
			{
				file << "_SDW" << countVar << tab << "sdword" << tab;
				file << number;
				file << '\n';

				add_var_inspection(vars, "_SDW", countVar++);
			}
		}

		countVar = 0;
		if (readData.strings.size() > 0)
		{
			file << "\n;Parsed strings: \n";
			for (const string str : readData.strings)
			{
				file << "_ByteArray" << countVar << tab << "byte" << tab;
				for (int i = 0; i < str.size(); i++)
				{
					file << '\'' << str[i] << '\'';
					file << ", ";
				}
				file << "'\\0'\n";
				add_var_inspection(vars, "_ByteArray", countVar++);
			}
		}

		file << "\n\n.CODE\nmain PROC\nSTART:\n";

		for (const std::string str : vars)
		{
			file << str;
		}

		file << "\tpush 0\n\tcall ExitProcess\n\nmain ENDP\nend  main";
	}
	catch (const char* e)
	{
		cout << "An error occured: " << e;
		cout << "\nCheck if path " << binaryFilename << " exists.";
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}