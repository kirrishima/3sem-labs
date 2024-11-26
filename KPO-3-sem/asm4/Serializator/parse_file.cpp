#include "parse_file.h"
#include "fstream"

using namespace std;

Data parse_dec_file(const char* filename)
{
	std::ifstream file(filename, ios::binary);
	Data data;

	if (!file.is_open()) {
		std::cerr << "Failed to open input file " << filename << std::endl;
		return data;
	}

	cout << "\nData from file:\n";

	while (true)
	{
		if (file.eof() || file.bad() || file.fail())
		{
			break;
		}

		int type = 0;
		file >> type;

		switch (type)
		{
		case TypeFlags::INT:
		{
			int value = 0;
			file >> value;
			cout << "INT: " << value << '\n';
			data.integers.push_back(value);
			break;
		}
		case TypeFlags::STR:
		{
			int length;
			file >> length;

			file.get(); // лишний пробел 

			char* buff = new   char[length + 1];
			file.read((buff), length);
			buff[length] = '\0';

			cout << "STR: " << reinterpret_cast<unsigned char*>(buff) << endl;;

			data.strings.push_back(std::string(buff));
			break;
		}
		default:
			break;
		}
	}
	file.close();
	return data;
}