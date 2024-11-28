#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cctype>

using namespace std;

// Функция для преобразования числа в hex с разделением на 8 байт
std::string decimal_to_hex_with_spaces(unsigned long long num)
{
	std::stringstream stream;
	stream << std::hex << std::setfill('0') << std::uppercase;
	for (int i = 7; i >= 0; --i)
	{
		stream << std::setw(2) << ((num >> (i * 8)) & 0xFF);
		if (i > 0)
			stream << " ";
	}
	return stream.str();
}

// Функция для преобразования числа в hex с разделением на 8 байт
std::string decimal_to_hex(unsigned long long num)
{
	std::stringstream stream;
	stream << "0x" << std::hex << std::setfill('0') << std::uppercase;
	for (int i = 7; i >= 0; --i)
	{
		stream << std::setw(2) << ((num >> (i * 8)) & 0xFF);
	}
	return stream.str();
}

// Функция для проверки корректности шестнадцатеричного ввода
bool is_valid_hex(const std::string &str)
{
	if (str.size() < 2 || str[0] != '0' || tolower(str[1]) != 'x')
		return false;
	for (size_t i = 2; i < str.size(); ++i)
	{
		if (!isxdigit(str[i]))
			return false;
	}
	return true;
}

int main(int argc, char **argv)
{
	if (argc == 2 && (strcmp(argv[1], "/h") == 0 || strcmp(argv[1], "/?") == 0))
	{
		cout << "Usage: " << argv[0] << " < -d | -h > <value>" << endl;
		cout << "If '-d' is specified, value must be a integer number.\n"
				"If '-h' is specified, value must be a valid hex value with leading '0x'.";
		return 0;
	}

	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " < -d | -h > <value>" << endl;
		return 1;
	}

	string mode = argv[1];
	string value = argv[2];

	if (mode == "-d")
	{
		try
		{
			unsigned long long num = stoull(value);
			cout << decimal_to_hex(num) << endl;
			cout << decimal_to_hex_with_spaces(num) << endl;
		}
		catch (exception &e)
		{
			cerr << "Invalid decimal value: " << value << endl;
			return 1;
		}
	}
	else if (mode == "-h")
	{
		if (!is_valid_hex(value))
		{
			cerr << "Invalid hex value: " << value << endl;
			return 1;
		}

		try
		{
			unsigned long long unsigned_val = stoull(value, nullptr, 16);
			long long signed_val = stoll(value, nullptr, 16);
			cout << "Signed:   " << signed_val << endl;
			cout << "Unsigned: " << unsigned_val << endl;
		}
		catch (exception &e)
		{
			cerr << "Error parsing hex value: " << value << endl;
			return 1;
		}
	}
	else
	{
		cerr << "Invalid mode: " << mode << ". Use -d or -h." << endl;
		return 1;
	}

	return 0;
}
