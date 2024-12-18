#include "stdafx.h"
#include "code_gen.h"
#include "vector"
#include "sstream"
#include "filesystem"
#define byte win_byte_override
#include <Windows.h>
#undef byte

using namespace std;

bool executeCommand(const std::string& command) {
	STARTUPINFOA si = { sizeof(STARTUPINFOA) };
	PROCESS_INFORMATION pi = { 0 };

	// Создание процесса
	if (!CreateProcessA(
		nullptr,               // Имя программы
		const_cast<char*>(command.c_str()), // Команда
		nullptr,               // Дескриптор защиты процесса
		nullptr,               // Дескриптор защиты потока
		FALSE,                 // Наследование дескрипторов
		0,                     // Флаги создания
		nullptr,               // Указатель на переменные окружения
		nullptr,               // Текущий каталог
		&si,                   // Структура STARTUPINFO
		&pi                    // Структура PROCESS_INFORMATION
	)) {
		cout << "Не удалось выполнить команду: " << command << std::endl;
		return false;
	}

	// Ожидание завершения процесса
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Получение кода возврата
	DWORD exitCode = 0;
	GetExitCodeProcess(pi.hProcess, &exitCode);

	// Закрытие дескрипторов
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return exitCode == 0; // Успех, если код возврата равен 0
}

namespace CD
{
	void CodeGeneration::generateCode(const std::wstring& OUT_FILEPATH)
	{
		if (!OUT_ASM_FILE.is_open() || OUT_ASM_FILE.fail() || OUT_ASM_FILE.bad()) {
			cout << "Не удалось открыть файл\n";
			return;
		}

		OUT_ASM_FILE << BASE;
		OUT_ASM_FILE << '\n' << ".stack " << parm->stackSize;
		OUT_ASM_FILE << "\n\n";
		__s_const();
		OUT_ASM_FILE << '\n';
		__s_data();
		OUT_ASM_FILE << "\n\n.code";

		int bracesCount = 0;

		//OUT_ASM_FILE << "\nmain proc\nstart:\n";
		int start = 0;
		int end = LEX_TABLE.size;

		vector<pair<int, int>> functions;

		for (size_t i = 0; i < end; i++)
		{
			if (LEX_TABLE.table[i].lexema[0] == LEX_LEFTBRACE)
			{
				bracesCount++;
			}
			else if (LEX_TABLE.table[i].lexema[0] == LEX_BRACELET)
			{
				if (--bracesCount == 0)
				{

					functions.push_back({ start, i });
					start = i + 1;
				}
			}
		}

		for (const auto& p : functions)
		{
			parse_function(p.first, p.second);
		}

		int padding = 1;
		for (const auto& function : __user_functions)
		{
			OUT_ASM_FILE << '\n';
			for (const auto& cl : function->code)
			{
				if (isMASMLabel(cl) || isFunctionEnd(cl) || isFunctionStart(cl))
				{
					OUT_ASM_FILE << cl << '\n';
					continue;
				}
				OUT_ASM_FILE << tab << cl << '\n';
			}
		}
		OUT_ASM_FILE.close();

		if (!executeCommand(R"(cmd /c ""C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"")")) {
			cout << "Не удалось настроить среду Visual Studio." << std::endl;
			return;
		}

		// Компиляция с помощью ML
		if (!executeCommand(R"(cmd /c "ml /c /nologo /Zi /Fo program.obj input.txt.asm")")) {
			cout << "Компиляция не удалась. Проверьте ваш ASM-код на наличие ошибок." << std::endl;
			return;
		}

#ifdef _DEBUG
		// Линковка
		if (!executeCommand(R"(cmd /c "link /nologo /DEBUG /subsystem:console program.obj GMSStandardLib.lib libucrt.lib /NODEFAULTLIB:libcmtd /NODEFAULTLIB:MSVCRTD /OPT:NOREF")")) {
			cout << "Линковка не удалась. Проверьте пути к библиотекам и их совместимость." << std::endl;
			return;
		}
#endif // _DEBUG

#ifdef _RELEASE
		if (!executeCommand(R"(cmd /c "link /nologo /RELEASE /subsystem:console program.obj GMSStandardLib.lib libucrt.lib libcmt.lib libvcruntime.lib kernel32.lib /NODEFAULTLIB:libcmtd /NODEFAULTLIB:MSVCRTD /OPT:NOREF /OPT:ICF")")) {
			cout << "Линковка не удалась. Проверьте пути к библиотекам и их совместимость." << std::endl;
			return;
		}
#endif // _RELEASE

		cout << "Компиляция завершена успешно.\n";
	}
}