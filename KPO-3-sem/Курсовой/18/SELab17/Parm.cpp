#include "stdafx.h"
#include "Parm.h"
#include <tchar.h>
#include "Error.h"
#include <filesystem>
#include <map>


#ifdef _UNICODE
#define tstring std::wstring
#define tcout std::wcout
#define tcerr std::wcerr
#else
#define tstring std::string
#define tcout std::cout
#define tcerr std::cerr
#endif

namespace fs = std::filesystem;

bool validatePath(const std::wstring& filePath);
bool validateHtmlPath(const std::filesystem::path& path);
void validatePaths(const Parm::PARM& parameters);
bool areDirectoriesValid(const fs::path& path);

namespace Parm {

	PARM getparm(int argc, _TCHAR* argv[]) {

		PARM parm;

		bool in = false, out = false, log = false;

		// Карты для хранения параметров и флагов
		std::map<tstring, tstring> namedParams;
		std::map<tstring, bool> flags;

		// Перебор аргументов командной строки
		for (int i = 1; i < argc; i++) {
			tstring arg = argv[i];

			// Проверяем формат "-название:значение"
			if (arg[0] == _T('-') && arg.find(_T(':')) != tstring::npos) {
				size_t colonPos = arg.find(_T(':'));
				tstring key = arg.substr(1, colonPos - 1);    // Убираем "-"
				tstring value = arg.substr(colonPos + 1);     // Всё после ":"
				namedParams[key] = value;
			}
			// Проверяем формат "/флаг"
			else if (arg[0] == _T('/') && arg.length() > 1) {
				tstring flag = arg.substr(1);                // Убираем "/"
				flags[flag] = true;
			}
			// Некорректный формат
			else {
				tcerr << _T("Неверный формат аргумента: ") << arg << std::endl;
			}
		}
		if (namedParams.find(PARM_IN) == namedParams.end()) {
			throw ERROR_THROW(100);
		}
		else {
			parm.in = namedParams[PARM_IN];
		}

		// Проверка и установка `PARM_LOG`
		if (namedParams.find(PARM_LOG) == namedParams.end()) {
			parm.log = parm.in + PARM_LOG_DEFAULT_EXT;
		}
		else {
			if (!areDirectoriesValid(namedParams[PARM_LOG])) {
				throw ERROR_THROW(116); // Ошибка: некорректный путь для log
			}
			parm.log = namedParams[PARM_LOG];
		}

		// Проверка и установка `PARM_OUT`
		if (namedParams.find(PARM_OUT) == namedParams.end()) {
			parm.out = parm.in + PARM_OUT_DEFAULT_EXT;
		}
		else {
			if (!areDirectoriesValid(namedParams[PARM_OUT])) {
				throw ERROR_THROW(117); // Ошибка: некорректный путь для out
			}
			parm.out = namedParams[PARM_OUT];
		}

		// Проверка и установка `PARM_LT`
		if (namedParams.find(PARM_LT) != namedParams.end()) {
			std::filesystem::path ltPath(namedParams[PARM_LT]);
			if (!validateHtmlPath(ltPath)) {
				throw ERROR_THROW(118); // Ошибка: некорректный путь для lt
			}
			parm.lt = namedParams[PARM_LT];
		}
		else {
			parm.lt = parm.in + PARM_LT_TABLE_DEFAULT;
		}

		// Проверка и установка `PARM_IT`
		if (namedParams.find(PARM_IT) != namedParams.end()) {
			std::filesystem::path itPath(namedParams[PARM_IT]);
			if (!validateHtmlPath(itPath)) {
				throw ERROR_THROW(119); // Ошибка: некорректный путь для it
			}
			parm.it = namedParams[PARM_IT];
		}
		else {
			parm.it = parm.in + PARM_IT_TABLE_DEFAULT;
		}

		if (namedParams.find(PARM_STACK) != namedParams.end()) {
			try {
				size_t stackSize = std::stoul(namedParams[PARM_STACK]); // преобразование строки в число

				// Проверка диапазона размера стека
				if (stackSize < STACK_MIN_SIZE || stackSize > STACK_MAX_SIZE) {
					throw ERROR_THROW(120); // Ошибка: некорректный размер стека
				}

				parm.stackSize = stackSize; // Установка значения в параметр
			}
			catch (const std::exception&) {
				throw ERROR_THROW(121); // Ошибка: некорректный формат значения размера стека
			}
		}
		else {
			parm.stackSize = PARM_STACK_DEFAULT_SIZE; // Установка значения по умолчанию, если параметр отсутствует
		}

		if (flags.find(PARM_LEX) != flags.end()) {
			parm.enableLexAnSave = true;
		}

		if (flags.find(PARM_CST) != flags.end()) {
			parm.CST = true;
		}

		/*	if (flags.find(PARM_LEX) != flags.end()) {
				parm.enableLexAnSave = true;
			}*/

		parm.masmDest = (fs::path(fs::absolute(parm.in)).parent_path() / (fs::path(parm.in).filename().string() + ".asm")).wstring();
		//parm.obj = (fs::path(fs::absolute(parm.out)).parent_path() / (fs::path(parm.in).filename().string() + ".obj")).wstring();
		//parm.exe = (fs::path(fs::absolute(parm.out)).parent_path() / (fs::path(parm.in).filename().string() + ".exe")).wstring();

		validatePaths(parm);

		return parm;
	}
}


bool areDirectoriesValid(const fs::path& path) {
	// итеративно проверяем существование всех родительских директорий
	fs::path parentPath = path.parent_path();
	while (!parentPath.empty()) {
		if (!fs::exists(parentPath)) {
			return false; // директория не существует
		}
		parentPath = parentPath.parent_path();
	}
	return true;
}

bool isFilePath(const fs::path& path) {
	// проверяем, что путь содержит имя файла (имеет имя и расширение)
	return path.has_filename();
}

bool validatePath(const std::wstring& filePath) {
	std::filesystem::path path(filePath);

	if (fs::is_directory(path)) {
		return false;
	}

	// Проверяем существование всех директорий в пути
	std::filesystem::path dirPath = path.parent_path();
	if (!std::filesystem::exists(dirPath)) {
		return false;
	}

	return true;
}

// Функция для проверки корректности пути (директория или файл с расширением .html)
bool validateHtmlPath(const std::filesystem::path& path) {

	if (isFilePath(path) && areDirectoriesValid(path)) {
		// Путь указывает на файл — проверяем, что расширение .html
		return path.extension() == ".html";
	}

	return false; // Неверный путь
}

void validatePaths(const Parm::PARM& parameters) {
	std::unordered_map<std::wstring, std::vector<std::wstring>> pathMap;

	// Поместим пути в map с метками их полей
	pathMap[parameters.in].push_back(L"in");
	pathMap[parameters.out].push_back(L"out");
	pathMap[parameters.log].push_back(L"log");
	pathMap[parameters.it].push_back(L"it");
	pathMap[parameters.lt].push_back(L"lt");

	// Проверяем, есть ли совпадения
	bool hasDuplicates = false;
	for (const auto& [path, fields] : pathMap) {
		if (fields.size() > 1) { // Если одно значение связано с несколькими полями
			hasDuplicates = true;
			std::wcout << L"Ошибка: путь '" << path << L"' совпадает в параметрах: ";
			for (const auto& field : fields) {
				std::wcout << field << L" ";
			}
			std::wcout << std::endl;
		}
	}

	if (hasDuplicates) {
		throw ERROR_THROW(121);
	}
}
