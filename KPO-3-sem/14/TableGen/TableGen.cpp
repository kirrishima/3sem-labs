#include <iostream>
#include <vector>
#include <TCHAR.H>
#include <string>

using namespace std;

typedef unsigned char uchar;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");

	const int tableSize = 256;
	const uchar data[] = "MaximGormozaМаксимГормоза2006";

	string str(reinterpret_cast<const char*>(data));
	vector<int> codes;

	for (auto c : str) {
		codes.push_back(static_cast<int>(static_cast<uchar>(c)));
	}

	const string allowedCharTemplate = "IN::T";
	const string forbiddenCharTemplate = "IN::F";
	const string ignoredCharTemplate = "IN::I";
	const string replacementCharTemplate = "'-'";

	vector<string> table(tableSize, forbiddenCharTemplate);

	table[static_cast<int>(static_cast<uchar>('\n'))] = allowedCharTemplate; // Разрешить \n
	table[static_cast<int>(static_cast<uchar>('А'))] = replacementCharTemplate; // Заменить киррилическую 'А' на '-'
	table[static_cast<int>(static_cast<uchar>('X'))] = ignoredCharTemplate; // Игнорить латинскую 'X'

	for (const auto code : codes)
	{
		table[code] = allowedCharTemplate;
	}

	for (size_t i = 0; i < table.size(); i += 16)
	{
		for (size_t j = i; j < i + 16 && j < table.size(); j++)
		{
			cout << table[j] << ", ";
		}
		cout << "\\\n";
	}
	return 0;
}
