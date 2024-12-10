#include "stdafx.h"
#include "Utils.h"
#include <iomanip>
#include <format>

namespace utils
{
	std::pair<int, int> trimmed_pos(const std::string& str)
	{
		size_t first = str.find_first_not_of(' ');
		if (first == std::string::npos) return std::make_pair<int, int>(-1, -1);

		size_t last = str.find_last_not_of(' ');
		return std::make_pair<int, int>(first, last);
	}
}


namespace LexAn::Utils
{
	void printToFile(const IT::ID_Table& ID_Table, const std::wstring& IT_filename,
		const LT::LexTable& LexTable, const std::wstring& LT_filename, const In::IN& in)
	{
		IT::Entry IT_entry;
		LT::Entry LT_entry;
		int SETW_VALUE = 8;

		std::ofstream LT_file(LT_filename); // Файл для записи таблицы лексем
		std::ofstream IT_file(IT_filename); // Файл для записи таблицы идентификаторов

		if (!LT_file.is_open())
		{
			std::wcout << L"Не удалось открыть файл \"" << LT_filename << L"\"\n";
			return;
		}

		if (!IT_file.is_open())
		{
			std::wcout << L"Не удалось открыть файл \"" << IT_filename << L"\"\n";
			return;
		}

		// Начало HTML-документа
		IT_file << "<!DOCTYPE html><html><head><title>Identifier Table</title></head><body>" << std::endl;
		IT_file << "<pre><table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">" << std::endl;

		// Заголовок таблицы
		IT_file << "<tr>"
			<< "<th>ID</th>"
			<< "<th>Datatype</th>"
			<< "<th>ID Type</th>"
			<< "<th>Line</th>"
			<< "<th>Value</th>"
			<< "<th>Scope</th>"
			<< "</tr>" << std::endl;

		// Заполнение таблицы данными
		for (int i = 0; i < ID_Table.size; i++) {
			IT_entry = IT::GetEntry(ID_Table, i);

			IT_file << "<tr>";

			// ID
			IT_file << "<td>" << IT_entry.id << "</td>";

			// Datatype
			if (IT_entry.iddatatype == 1)
				IT_file << "<td>INT</td>";
			else if (IT_entry.iddatatype == 2)
				IT_file << "<td>STR</td>";
			else
				IT_file << "<td>-</td>";

			// ID Type
			IT_file << "<td>";
			if (IT_entry.idtype == IT::V)
				IT_file << "V";
			else if (IT_entry.idtype == IT::L)
				IT_file << "L";
			else if (IT_entry.idtype == IT::F)
				IT_file << "F";
			else if (IT_entry.idtype == IT::P)
				IT_file << "P";
			else
				IT_file << "-";
			IT_file << "</td>";

			// Line
			IT_file << "<td>" << IT_entry.idxfirstLE << "</td>";

			// Value
			IT_file << "<td>";
			if (IT_entry.iddatatype == IT::INT) {
				IT_file << IT_entry.value.vint;
			}
			else if (IT_entry.iddatatype == IT::STR) {
				std::string strValue(IT_entry.value.vstr->str);
				IT_file << strValue;
			}
			else {
				IT_file << "-";
			}
			IT_file << "</td>";

			// Scope
			IT_file << "<td>";
			if (IT_entry.scope != NULL) {
				std::stack<IT::Entry*> scope;
				bool isFirst = true;

				auto tmp = IT_entry.scope;
				scope.push(tmp);

				while ((*tmp).scope != nullptr)
				{
					scope.push(tmp->scope);
					tmp = (*tmp).scope;
				}

				while (!scope.empty())
				{
					if (!isFirst)
					{
						IT_file << '.';
					}
					else
					{
						isFirst = false;
					}
					IT_file << scope.top()->id;
					scope.pop();
				}

			}
			else {
				IT_file << "-";
			}
			IT_file << "</td>";

			IT_file << "</tr>" << std::endl;
		}


		// Закрытие таблицы и HTML-документа
		IT_file << "</table></pre></body></html>" << std::endl;

		IT_file.close();

		LT_file << "<!DOCTYPE html><html><head><title>Lexem Table</title></head><body>" << std::endl;
		LT_file << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">" << std::endl;

		// Заголовок таблицы
		LT_file << "<tr>"
			<< "<th>Line</th>"
			<< "<th>Lexem</th>"
			<< "</tr>" << std::endl;

		int currentLine = 1;

		std::string tmp;
		for (int i = 0; i < LexTable.size; i++)
		{
			LT_entry = LT::GetEntry(LexTable, i);
			if (currentLine != LT_entry.sn)
			{
				LT_file << "<tr><td>" << (currentLine > 9 ? std::to_string(currentLine) : ("0" + std::to_string(currentLine))) << "</td>";
				LT_file << "<td>" << tmp << "</td></tr>";
				tmp.clear();
				currentLine = LT_entry.sn;
				tmp += LT_entry.lexema[0];
				continue;
			}

			tmp += LT_entry.lexema[0];
		}
		LT_file << "<tr><td>" << (currentLine > 9 ? std::to_string(currentLine) : ("0" + std::to_string(currentLine))) << "</td>";
		LT_file << "<td>" << tmp << "</td></tr>";
		LT_file << "</table></body></html>" << std::endl;
		LT_file.close();
	}

	int stringToNumber(const std::string& str) {
		if (str.empty()) {
			throw std::invalid_argument("Input string is empty");
		}

		// Определяем базу (по умолчанию 10)
		int base = 10;
		std::size_t startIndex = 0;

		if (str.size() > 2 && str[0] == '0') {
			if (str[1] == 'b' || str[1] == 'B') {
				base = 2;
				startIndex = 2;
			}
			else if (str[1] == 'x' || str[1] == 'X') {
				base = 16;
				startIndex = 2;
			}
			else if (std::isdigit(str[1])) {
				base = 8;
				startIndex = 1;
			}
		}

		// Преобразуем строку в 64-битное число, чтобы избежать переполнения
		int64_t number = std::stoll(str.substr(startIndex), nullptr, base);

		// Приведение к знаковому значению, если используется 16-битное представление
		if (base == 16) {
			number &= 0xFFFF; // Ограничиваем до 16 бит
			if (number > 0x7FFF) {
				number -= 0x10000; // Преобразуем в отрицательное число
			}
		}

		// Проверяем, помещается ли число в диапазон short
		if (number < std::numeric_limits<short>::min() || number > std::numeric_limits<short>::max()) {
			throw std::format("Литерал выходит за границы числа. диапазон допустимых значений: [{}; {}]", SHRT_MIN, SHRT_MAX);
		}

		return static_cast<short>(number);
	}
}