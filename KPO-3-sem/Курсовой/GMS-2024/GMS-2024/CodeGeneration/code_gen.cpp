#include "stdafx.h"
#include "code_gen.h"
#include "vector"
#include "sstream"
#include "filesystem"
#define byte win_byte_override
#include <Windows.h>
#undef byte

using namespace std;

bool executeCommand(const std::string& command, std::unique_ptr<std::ofstream>& outputStream) {
	STARTUPINFOA si = { sizeof(STARTUPINFOA) };
	PROCESS_INFORMATION pi = { 0 };
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };

	HANDLE hWritePipe = nullptr, hReadPipe = nullptr;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
		return false;
	}

	if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0)) {
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		return false;
	}

	si.hStdOutput = hWritePipe;
	si.hStdError = hWritePipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	if (!CreateProcessA(
		nullptr,
		const_cast<char*>(command.c_str()),
		nullptr,
		nullptr,
		TRUE,
		0,
		nullptr,
		nullptr,
		&si,
		&pi
	)) {
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		return false;
	}

	CloseHandle(hWritePipe);

	char buffer[4096];
	DWORD bytesRead;
	while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
		buffer[bytesRead] = '\0';
		if (outputStream && outputStream->is_open()) {
			(*outputStream) << buffer;
			outputStream->flush();
		}
	}

	CloseHandle(hReadPipe);
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode = 0;
	GetExitCodeProcess(pi.hProcess, &exitCode);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return exitCode == 0;
}


namespace CD
{
	void CodeGeneration::generateCode()
	{
		if (!OUT_ASM_FILE.is_open() || OUT_ASM_FILE.fail() || OUT_ASM_FILE.bad()) {
			cout << "Не удалось открыть файл .asm для записи\n";
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

		*log->stream << "\n----    Начало компиляции и сборки    ----\n";
		*log->stream << "\nАктивация среды разработчика Visual Studio...\n";

		if (!executeCommand(R"(cmd /c ""C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"")", log->stream)) {
			cout << "Не удалось настроить среду Visual Studio." << std::endl;
			*log->stream << "Не удалось настроить среду Visual Studio." << std::endl;
			return;
		}
		*log->stream << "\nАктивация среды разработчика Visual Studio выполнена.\n";

		*log->stream << "\nКомпиляция, используя " << format(COMPILE_COMMAND, utils::wstring_to_string(parm->_objName), utils::wstring_to_string(parm->_masmDest)) << endl;
		if (!executeCommand(format(COMPILE_COMMAND, utils::wstring_to_string(parm->_objName), utils::wstring_to_string(parm->_masmDest)), log->stream)) {
			cout << "Компиляция не удалась. Проверьте ASM-код на наличие ошибок." << std::endl;
			*log->stream << "Компиляция не удалась. Проверьте ASM-код на наличие ошибок." << std::endl;
			return;
		}

		*log->stream << "\nВыполнение линковки, используя " << format(LINK_COMMAND,
			utils::wstring_to_string(parm->exeName), utils::wstring_to_string(parm->_objName)) << '\n';
#ifdef _DEBUG
		if (!executeCommand(format(LINK_COMMAND, utils::wstring_to_string(parm->exeName),
			utils::wstring_to_string(parm->_objName)), log->stream)) {
			cout << "Линковка не удалась. Проверьте пути к библиотекам и их совместимость." << std::endl;
			*log->stream << "Линковка не удалась. Проверьте пути к библиотекам и их совместимость." << std::endl;
			return;
		}
#endif  

#ifdef _RELEASE
		if (!executeCommand(R"(cmd /c "link /nologo /RELEASE /subsystem:console program.obj gms2024stdlib.lib libucrt.lib libcmt.lib libvcruntime.lib kernel32.lib /NODEFAULTLIB:libcmtd /NODEFAULTLIB:MSVCRTD /OPT:NOREF /OPT:ICF")", log->stream)) {
			cout << "Линковка не удалась. Проверьте пути к библиотекам и их совместимость." << std::endl;
			return;
		}
#endif  

		cout << "\nКомпиляция и линковка завершена успешно. Выходной файл: \n" + utils::wstring_to_string(parm->exeName);
		*log->stream << "\nКомпиляция и линковка завершена успешно. Выходной файл: \n" + utils::wstring_to_string(parm->exeName);
	}
}