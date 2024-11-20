#include "stdafx.h"
#include "MFST.h"
#include "cd.h"
#include "algorithm"
#include <filesystem>
#include <set>
#include <locale>
#include <chrono>

#define _DISABLE_MFST__

using namespace std;
using namespace MFST;
using namespace GRB;
namespace fs = std::filesystem;

std::set<std::u8string> get_files_in_directory(const std::wstring& path) {
	std::set<std::u8string> files;
	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			files.insert(entry.path().u8string());
		}
	}
	return files;
}

void create_delete_script(const std::set<std::u8string>& files, const std::wstring& script_name) {
	std::ofstream script(script_name);
	if (!script.is_open()) {
		std::wcerr << L"Не удалось создать .bat файл: " << script_name << std::endl;
		return;
	}

	std::u8string e = u8"@echo off\nchcp 65001 > nul\n";
	script.write(reinterpret_cast<const char*>(e.c_str()), e.size());

	std::u8string del = u8"del /f /q \"";
	std::u8string end = u8"\"\n";

	for (const auto& file : files) {
		script.write(reinterpret_cast<const char*>(del.c_str()), del.size());
		script.write(reinterpret_cast<const char*>(file.c_str()), file.size());
		script.write(reinterpret_cast<const char*>(end.c_str()), end.size());
	}

	std::u8string endd = u8"del /f /q \"%~f0\"";

	script.write(reinterpret_cast<const char*>(endd.c_str()), endd.size());
	script.close();

	std::wcout << L"Скрипт для удаления создан: " << script_name << std::endl;
}

int _tmain(int argc, _TCHAR* argv[]) {

	// asasasasasa
	// asasasasasa
	// asasasasasa
	// asasasasasa

	setlocale(LC_ALL, "rus");

	std::wstring current_path = fs::current_path().wstring();
	auto initial_files = get_files_in_directory(current_path);

#ifndef TEST
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		cout << in.text << endl;
		cout << "Всего символов: " << in.size << endl;
		cout << "Всего строк: " << in.lines << endl;
		cout << "Пропущено: " << in.ignore << endl;
	}
	catch (Error::ERROR e) {
		cout << "Ошибка " << e.id << ": " << e.message << endl << endl;
		cout << "строка " << e.inext.line << " позиция " << e.inext.col << endl << endl;
	};

	Log::LOG log = Log::INIT_LOG;
	Parm::PARM parm = Parm::getparm(argc, argv);
	Out::OUT out = Out::INIT_OUT;
	In::IN in = In::getin(parm.in);
	cout << '\n';
	try {
		out = Out::getout(parm.out);
		log = Log::getlog(parm.log);

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Out::WriteOut(out, in);
		Log::WriteIn(log, in);
		cout << '\n';

		auto [LexTable, IdTable] = LexAn::lexAnalize(parm, in);

		cout << '\n';

#ifndef _DISABLE_MFST__
		MFST_TRACE_START
			MFST::Mfst mfst(LexTable, GRB::getGreibach());
		mfst.start();
		cout << '\n';
		mfst.savededucation();
		mfst.printrules();

		bool hasP = false;

		MfstState state;
		GRB::Rule rule;
		char rbuf[300];

		for (unsigned short i = 0; i < mfst.storestate.size(); i++) // Перебор всех сохраненных состояний
		{
			state = mfst.storestate.c[i]; // Получение состояния
			rule = mfst.grebach.getRule(state.nrule); // Получение правила
			rule.getCRule(rbuf, state.nrulechain);
			if (std::find(rbuf, rbuf + strlen(rbuf), 'p') != rbuf + strlen(rbuf))
			{
				hasP = true;
				break;
			}
		};
#endif // !_DISABLE_MFST__

#ifdef _DISABLE_MFST__
		bool hasP = true;
#endif // _DISABLE_MFST__

		Log::Close(log);
		Out::Close(out);

		cout << "\n";

		CD::gen(LexTable, IdTable, parm.asem, hasP);
	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка" << e.id << ':' << e.message << endl << endl;
		if (e.inext.line)
			Log::WriteError(log, e);
		Out::WriteError(out, e);
		Log::Close(log);
		Out::Close(out);
	}

	auto current_files = get_files_in_directory(current_path);

	std::set<std::u8string> new_files;
	for (const auto& file : current_files) {
		if (initial_files.find(file) == initial_files.end()) {
			new_files.insert(file);
		}
	}

	if (!new_files.empty()) {
		std::wstring script_name = L"cleanup.bat";
		create_delete_script(new_files, script_name);
	}

#endif // !TEST
	return 0;
}