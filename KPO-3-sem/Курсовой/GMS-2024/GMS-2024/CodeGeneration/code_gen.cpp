#include "stdafx.h"
#include "code_gen.h"

#include <filesystem>
#define byte win_byte_override
#include <Windows.h>
#undef byte
#include <thread>


using namespace std;

class PersistentConsole {
private:
    HANDLE hWritePipeIn, hReadPipeOut;
    HANDLE hProcess;
    bool initialized;

public:
    PersistentConsole() : hWritePipeIn(nullptr), hReadPipeOut(nullptr), hProcess(nullptr), initialized(false) {}

    bool initialize() {
        // Создание анонимных каналов
        SECURITY_ATTRIBUTES sa{ sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
        HANDLE hReadPipeIn, hWritePipeOut;

        if (!CreatePipe(&hReadPipeIn, &hWritePipeIn, &sa, 0) ||  // stdin
            !CreatePipe(&hReadPipeOut, &hWritePipeOut, &sa, 0)) {  // stdout
            return false;
        }

        // Запуск cmd.exe
        STARTUPINFOA si{};
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = hReadPipeIn;
        si.hStdOutput = hWritePipeOut;
        si.hStdError = hWritePipeOut;

        PROCESS_INFORMATION pi{};
        if (!CreateProcessA(
            nullptr,
            const_cast<char*>("cmd.exe /Q"),  // Тихий режим
            nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) {
            CloseHandle(hReadPipeIn);
            CloseHandle(hWritePipeIn);
            CloseHandle(hWritePipeOut);
            CloseHandle(hReadPipeOut);
            return false;
        }

        // Закрываем неиспользуемые дескрипторы
        CloseHandle(hReadPipeIn);
        CloseHandle(hWritePipeOut);

        // Сохраняем дескрипторы
        hWritePipeIn = hWritePipeIn;
        hReadPipeOut = hReadPipeOut;
        hProcess = pi.hProcess;
        initialized = true;

        // Закрываем поток cmd.exe
        CloseHandle(pi.hThread);

        return true;
    }

    bool executeCommand(const std::string& command, std::string& output, DWORD timeoutMs = 5000) {
        if (!initialized) return false;

        // Отправляем команду в процесс
        std::string cmd = command + "\r\n";
        DWORD written;
        if (!WriteFile(hWritePipeIn, cmd.c_str(), static_cast<DWORD>(cmd.size()), &written, nullptr)) {
            return false;
        }

        // Читаем вывод из процесса
        char buffer[4096];
        DWORD bytesRead;
        std::ostringstream oss;
        auto start = std::chrono::high_resolution_clock::now();

        while (true) {
            // Тайм-аут
            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > timeoutMs) {
                break;
            }

            // Проверяем, есть ли данные для чтения
            DWORD available = 0;
            if (!PeekNamedPipe(hReadPipeOut, nullptr, 0, nullptr, &available, nullptr) || available == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }

            // Читаем данные
            if (ReadFile(hReadPipeOut, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                oss << buffer;
                if (oss.str().find(">") != std::string::npos) {  // Проверка приглашения вида "C:\>"
                    break;
                }
            }
        }

        output = oss.str();
        return true;
    }

    ~PersistentConsole() {
        if (hWritePipeIn) CloseHandle(hWritePipeIn);
        if (hReadPipeOut) CloseHandle(hReadPipeOut);
        if (hProcess) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
    }
};

// Реализация генерации кода
namespace CD {
    void CodeGeneration::generateCode() {
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

        for (size_t i = 0; i < end; i++) {
            if (LEX_TABLE.table[i].lexema[0] == LEX_LEFTBRACE) {
                bracesCount++;
            }
            else if (LEX_TABLE.table[i].lexema[0] == LEX_BRACELET) {
                if (--bracesCount == 0) {
                    functions.push_back({ start, i });
                    start = i + 1;
                }
            }
        }

        for (const auto& p : functions) {
            parse_function(p.first, p.second);
        }

        for (const auto& function : __user_functions) {
            OUT_ASM_FILE << '\n';
            for (const auto& cl : function->code) {
                if (isMASMLabel(cl) || isFunctionEnd(cl) || isFunctionStart(cl)) {
                    OUT_ASM_FILE << cl << '\n';
                    continue;
                }
                OUT_ASM_FILE << tab << cl << '\n';
            }
        }
        OUT_ASM_FILE.close();

        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

        PersistentConsole console;
        console.initialize();
        std::string   output;

        *log->stream << "\n----    Начало компиляции и сборки    ----\n";

        // Активация среды разработчика Visual Studio
        *log->stream << "\nАктивация среды разработчика Visual Studio...\n";
        if (!console.executeCommand(R"( "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat")", output)) {
            *log->stream << "Не удалось настроить среду Visual Studio.\n";
            return;
        }

        *log->stream << output;
        
        output.clear();


        // Компиляция
        *log->stream << "\nКомпиляция...\n";
        std::string compileCmd = format(COMPILE_COMMAND, utils::wstring_to_string(parm->_objName), utils::wstring_to_string(parm->_masmDest));
        if (!console.executeCommand(compileCmd, output)) {
            *log->stream << "Компиляция не удалась. Проверьте ASM-код.\n";
            return;
        }

        *log->stream << output;
        
        output.clear();

        // Линковка
        *log->stream << "\nЛинковка...\n";
        std::string linkCmd = format(LINK_COMMAND, utils::wstring_to_string(parm->exeName), utils::wstring_to_string(parm->_objName));
        if (!console.executeCommand(linkCmd, output)) {
            *log->stream << "Линковка не удалась.\n";
            return;
        }

        *log->stream << output;
        output.clear();

        *log->stream << "\nКомпиляция и линковка завершена успешно.\n";
        cout << "Выходной файл: " << utils::wstring_to_string(parm->exeName) << '\n';
    }
}
