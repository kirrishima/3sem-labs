#include "stdafx.h"
#include "Utils.h"
#include <iomanip>
#include <format>
#include <bitset>

namespace utils
{
	std::pair<int, int> trimmed_pos(const std::string& str)
	{
		size_t first = str.find_first_not_of(' ');
		if (first == std::string::npos) return std::make_pair<int, int>(-1, -1);

		size_t last = str.find_last_not_of(' ');
		return std::make_pair<int, int>(first, last);
	}


	std::string wstring_to_string(const std::wstring& wstr) {
		size_t required_size;
		wcstombs_s(&required_size, nullptr, 0, wstr.c_str(), _TRUNCATE);

		std::vector<char> buffer(required_size);

		size_t converted_size;
		wcstombs_s(&converted_size, buffer.data(), buffer.size(), wstr.c_str(), _TRUNCATE);

		std::string str(buffer.data(), converted_size - 1);

		return str;
	}
}

namespace LexAn::Utils
{
	void printToFile(const IT::ID_Table& ID_Table, const std::wstring& IT_filename,
		const LT::LexTable& LexTable, const std::wstring& LT_filename, const In::IN& in) {
		std::ofstream IT_file(IT_filename);
		std::ofstream LT_file(LT_filename);

		if (!IT_file.is_open()) {
			std::wcerr << L"Не удалось открыть файл \"" << IT_filename << L"\"\n";
			return;
		}

		if (!LT_file.is_open()) {
			std::wcerr << L"Не удалось открыть файл \"" << LT_filename << L"\"\n";
			return;
		}

		IT_file << "<!DOCTYPE html><html><head><title>Identifier Table</title></head><body>\n";
		IT_file << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">\n";
		IT_file << "<tr><th>ID</th><th>Datatype</th><th>ID Type</th><th>Line</th><th>Value</th><th>Scope</th></tr>\n";

		for (int i = 0; i < ID_Table.size; i++) {
			const auto& entry = IT::GetEntry(ID_Table, i);

			IT_file << "<tr><td>" << entry.id << "</td>";

			IT_file << "<td>";
			switch (entry.iddatatype) {
			case IT::INT:  IT_file << "INT"; break;
			case IT::STR:  IT_file << "STR"; break;
			case IT::CHAR: IT_file << "CHAR"; break;
			default:       IT_file << "-"; break;
			}
			IT_file << "</td>";

			IT_file << "<td>";
			switch (entry.idtype) {
			case IT::V: IT_file << "V"; break;
			case IT::L: IT_file << "L"; break;
			case IT::F: IT_file << "F"; break;
			case IT::P: IT_file << "P"; break;
			default:    IT_file << "-"; break;
			}
			IT_file << "</td>";

			IT_file << "<td>" << entry.idxfirstLE << "</td>";

			IT_file << "<td>";
			if (entry.iddatatype == IT::INT) {
				IT_file << entry.value.vint;
			}
			else if (entry.iddatatype == IT::STR || entry.iddatatype == IT::CHAR) {
				IT_file << entry.value.vstr->str;
			}
			else {
				IT_file << "-";
			}
			IT_file << "</td>";

			IT_file << "<td>";
			if (entry.scope != nullptr) {
				std::stack<IT::Entry*> scope_stack;
				auto tmp = entry.scope;
				while (tmp) {
					scope_stack.push(tmp);
					tmp = tmp->scope;
				}

				bool first = true;
				while (!scope_stack.empty()) {
					if (!first) IT_file << '.';
					IT_file << scope_stack.top()->id;
					scope_stack.pop();
					first = false;
				}
			}
			else {
				IT_file << "-";
			}
			IT_file << "</td></tr>\n";
		}

		IT_file << "</table></body></html>\n";

		LT_file << "<!DOCTYPE html><html><head><title>Lexem Table</title></head><body>\n";
		LT_file << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"0\">\n";
		LT_file << "<tr><th>Line</th><th>Lexem</th></tr>\n";

		int currentLine = 1;
		std::string tmp;
		for (int i = 0; i < LexTable.size; i++)
		{
			auto LT_entry = LT::GetEntry(LexTable, i);
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

	bool isValidShort(const std::string& str, short& result, int base = 10) {
		try {
			// Преобразуем строку в число с учетом основания
			long long value = std::stoll(str, nullptr, base);

			// Проверяем, входит ли число в диапазон short
			if (value >= std::numeric_limits<short>::min() && value <= std::numeric_limits<short>::max()) {
				result = static_cast<short>(value);
				return true;
			}
			else {
				throw std::out_of_range(std::format(
					"Литерал выходит за границы числа. Диапазон допустимых значений: [{}; {}]",
					std::numeric_limits<short>::min(),
					std::numeric_limits<short>::max()
				));
			}
		}
		catch (const std::invalid_argument&) {
			throw std::invalid_argument(std::format("Недопустимый формат числа: '{}'", str));
		}
		catch (const std::out_of_range&) {
			throw std::out_of_range(std::format("Значение слишком большое для типа", str));
		}
	}

	int stringToNumber(const std::string& str) {

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
		short number;
		bool isValid = isValidShort(str.substr(startIndex), number, base);

		return number;
	}

	bool isSingleCharacter(const unsigned char* input, std::size_t endPos) {
		if (endPos < 0 || endPos > 1) {
			return false;
		}

		if (endPos == 0 && input[0] != '\\') {
			return true;
		}

		if (input[0] == '\\' && endPos == 1) {
			unsigned char nextChar = input[1];
			switch (nextChar) {
			case 'n':
			case 't':
			case '\\':
			case '"':
			case '\'':
				return true;
			default:
				return false;
			}
		}

		return false;
	}
}