#include <iostream>
#include <filesystem>
#include <string>
#include <format>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <clocale>
#include <cstdlib>
#include <iosfwd>

int main()
{
	setlocale(LC_ALL, "ru");

	try
	{
		int x;

		int n = 0;

		for (const auto &directory : std::filesystem::directory_iterator(std::filesystem::current_path()))
		{
			int tmp;
			try
			{
				tmp = std::stoi(directory.path().filename().string());

				n = tmp > n ? tmp : n;
			}
			catch (...)
			{
			}
		}

		x = n + 1;

		std::string folderNameInFiles = x < 10 ? std::format("0{0}", x) : std::to_string(x);

		std::string directory = std::to_string(x);
		std::string htmlFile = std::format("lab_{0}.html", folderNameInFiles);
		std::string jsFile = std::format("lab_{0}.js", folderNameInFiles);

		auto htmlFilePath = std::filesystem::path(directory) / htmlFile;
		auto jsFilePath = std::filesystem::path(directory) / jsFile;

		if (!std::filesystem::exists(directory))
		{
			std::filesystem::create_directory(directory);
			std::cout << "Created folder " << directory << "\n";
		}
		else
		{
			std::cout << "Directory " << directory << " already exists. Would you like to rewrite it? (y/n): ";
			std::string input = "";
			std::getline(std::cin, input);

			std::transform(input.begin(), input.end(), input.begin(),
						   [](unsigned char c)
						   { return std::tolower(c); });

			if (input != "y")
			{
				std::cout << "Aborting...\n";
				return 0;
			}

			std::filesystem::remove_all(directory);
			std::filesystem::create_directory(directory);
		}

		std::ofstream file1(htmlFilePath, std::ios::out | std::ios::binary);

		if (!file1.is_open() || file1.fail() || file1.bad())
		{
			std::cout << "Couldn't create file " << htmlFilePath << std::endl;
		}
		else
		{
			std::u8string content = u8"<!DOCTYPE html>\n" + std::u8string(u8"<html lang=\"ru\">\n") + std::u8string(u8"\t<head>\n") + std::u8string(u8"\t\t<meta charset=\"UTF-8\"/>\n") + std::u8string(u8"\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>\n") + std::u8string(u8"\t\t<script src=\"") + std::u8string(jsFile.begin(), jsFile.end()) + std::u8string(u8"\"></script>\n") + std::u8string(u8"\t\t<title>Скриптовые языки разметки ") + std::u8string(folderNameInFiles.begin(), folderNameInFiles.end()) + std::u8string(u8"</title>\n") + std::u8string(u8"\t</head>\n") + std::u8string(u8"\t<body></body>\n") + std::u8string(u8"</html>");

			file1.write(reinterpret_cast<const char *>(content.c_str()), content.size());

			std::cout << "Created directory " << htmlFilePath << "\n";
		}

		std::ofstream file2(jsFilePath);

		if (!file2.is_open() || file2.fail() || file2.bad())
		{
			std::cout << "Couldn't create file " << jsFilePath << std::endl;
		}
		else
		{
			std::cout << "Created directory " << jsFilePath << "\n";
		}
	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << "\n";
		system("pause");
	}
	system("pause");
}
