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
			if (!validatePath(namedParams[PARM_LOG])) {
				throw ERROR_THROW(116); // Ошибка: некорректный путь для log
			}
			parm.log = namedParams[PARM_LOG];
		}

		// Проверка и установка `PARM_OUT`
		if (namedParams.find(PARM_OUT) == namedParams.end()) {
			parm.out = parm.in + PARM_OUT_DEFAULT_EXT;
		}
		else {
			if (!validatePath(namedParams[PARM_OUT])) {
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

		parm.asem = (fs::path(fs::absolute(parm.in)).parent_path() / (fs::path(parm.in).filename().string() + ".asm")).wstring();
		//parm.obj = (fs::path(fs::absolute(parm.out)).parent_path() / (fs::path(parm.in).filename().string() + ".obj")).wstring();
		//parm.exe = (fs::path(fs::absolute(parm.out)).parent_path() / (fs::path(parm.in).filename().string() + ".exe")).wstring();

		if (parm.in == parm.log)
		{
			throw (L"Параметры -in и -log не могут иметь одинаковых значений (было передано " + parm.in + L")");
		}
		if (parm.in == parm.out)
		{
			throw (L"Параметры -in и -out не могут иметь одинаковых значений (было передано " + parm.in + L")");
		}
		if (parm.out == parm.log)
		{
			throw (L"Параметры -out и -log не могут иметь одинаковых значений (было передано " + parm.out + L")");
		}

		return parm;
	}
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
	if (std::filesystem::is_directory(path)) {
		// Путь указывает на директорию — проверяем, существует ли она
		return std::filesystem::exists(path);
	}
	else if (std::filesystem::is_regular_file(path)) {
		// Путь указывает на файл — проверяем, что расширение .html
		return path.extension() == ".html" && std::filesystem::exists(path);
	}
	return false; // Неверный путь
}