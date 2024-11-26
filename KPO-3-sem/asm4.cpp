#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

template<typename T>
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << (int)i;
	return stream.str();
}


int main() {
	char buff[50];

	cout << "Type: ";
	cin >> buff;

	buff[49] = '\0';

	if (strcmp(buff, "int") == 0)
	{
		int val;
		cout << "Value: ";

		cin >> buff;
		buff[49] = '\0';

		cout << int_to_hex(stoi(buff));
	}

	if (strcmp(buff, "uint") == 0)
	{
		unsigned int val;
		cout << "Value: ";

		cin >> buff;
		buff[49] = '\0';

		cout << int_to_hex(stoul(buff));
	}

	if (strcmp(buff, "short") == 0)
	{
		short val;
		cout << "Value: ";

		cin >> buff;
		buff[49] = '\0';

		cout << int_to_hex((short)stoi(buff));
	}

	if (strcmp(buff, "ushort") == 0)
	{
		unsigned short val;
		cout << "Value: ";

		cin >> buff;
		buff[49] = '\0';

		cout << int_to_hex((unsigned short)stoul(buff));
	}

	if (strcmp(buff, "byte") == 0)
	{
		char val;
		cout << "Value: ";

		cin >> buff;
		buff[49] = '\0';

		auto v = stoi(buff);

		cout << int_to_hex((char)v);
	}

	if (strcmp(buff, "ubyte") == 0)
	{
		unsigned short val;
		cout << "Value: ";

		cin >> buff;
		buff[49] = '\0';

		cout << int_to_hex((unsigned char)stol(buff));
	}

	return 0;
}
