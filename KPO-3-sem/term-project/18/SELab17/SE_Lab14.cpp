﻿#include "stdafx.h"
#include "MFST.h"
#include "code_gen.h"
#include "algorithm"
#include <filesystem>
#include <set>
#include <locale>
#include <chrono>
#include "regex.h"
#include <map>
#include "semantic.h"

using namespace std;
using namespace MFST;
using namespace GRB;

namespace fs = std::filesystem;

// Удобный alias для работы с типом _TCHAR

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
	int returnCode = 0;
	setlocale(LC_ALL, "rus");

	cout << R"(
*************************************************************
*                    Compiler GM-2024                       *
*     Course project for Software Engineering at BSTU       *
*************************************************************
)" << '\n';
#ifdef _DEBUG
	if (argc == 1)
	{
		argc++;
		argv = new _TCHAR * [2];
		argv[1] = const_cast<_TCHAR*>(L"-in:input.txt");
	}
#endif // _DEBUG

	//// Вывод обработанных именованных параметров
	//tcout << _T("Именованные параметры:\n");
	//for (const auto& param : namedParams) {
	//	tcout << _T("  -") << param.first << _T(": ") << param.second << std::endl;
	//}

	//// Вывод обработанных флагов
	//tcout << _T("\nФлаги:\n");
	//for (const auto& flag : flags) {
	//	tcout << _T("  /") << flag.first << _T(": ") << (flag.second ? _T("true") : _T("false")) << std::endl;
	//}

	//const char* str = "!=";

	//FST::FST* elseFST(SVV::CreateCompareFST(str));

	//cout << FST::execute(*elseFST) << endl;


	//return 0;

	//std::wstring current_path = fs::current_path().wstring();
	//auto initial_files = get_files_in_directory(current_path);

#ifndef TEST

#ifndef __DISABLE_LOGS
	Log::LOG log;
	Out::OUT out;
#endif // !__DISABLE_LOGS

	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
#ifndef __DISABLE_LOGS
		out = Out::getout(parm.out);
		log = Log::getlog(parm.log);

		Log::writeLog(log);
		Log::writeParm(log, parm);
		Out::writeOut(out, in);
		Log::writeIn(log, in);
#endif // !__DISABLE_LOGS


		auto [LexTable, IdTable] = LexAn::lexAnalize(parm, in, log);

		cout << '\n';

#ifndef _DISABLE_MFST__

#ifdef _DEBUG
#ifndef _DISABLE_MFST_DEBUG
		MFST_TRACE_START;
#endif // !_DISABLE_MFST_DEBUG
#endif // _DEBUG

		MFST::Mfst mfst(LexTable, GRB::getGreibach());

		cout << '\n';

		if (!mfst.start(log))
		{
			exit(1);
		}
		if (parm.CST)
		{
			mfst.savededucation();
			mfst.printrules();
		}
		//bool hasP = false;

		//MfstState state;
		//GRB::Rule rule;
		//char rbuf[300];

		//for (unsigned short i = 0; i < mfst.storestate.size(); i++) // Перебор всех сохраненных состояний
		//{
		//	state = mfst.storestate.c[i]; // Получение состояния
		//	rule = mfst.grebach.getRule(state.nrule); // Получение правила
		//	rule.getCRule(rbuf, state.nrulechain);
		//	if (std::find(rbuf, rbuf + strlen(rbuf), 'p') != rbuf + strlen(rbuf))
		//	{
		//		hasP = true;
		//		break;
		//	}
		//};
#endif // !_DISABLE_MFST__

		int x = semantic::check(IdTable, LexTable, &log);
		if (x)
		{
			cout << x << " ошибок, выход...\n";
			IT::Delete(IdTable);
			LT::Delete(LexTable);
			exit(1);
		}
		//cout << "Ошибок: " << x << endl;

		CD::CodeGeneration cd(IdTable, LexTable, &parm);
		cd.generateCode(parm.masmDest);

		IT::Delete(IdTable);
		LT::Delete(LexTable);
	}
	catch (const char* e)
	{
		cout << "Произошла ошибка: " << e;
	}
	catch (std::string& e)
	{
		cout << "Произошла ошибка: " << e;
	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << ": " << e.message;
		if (e.sourceLine > 0)
		{
			cout << ". Строка: " << e.sourceLine;
		}
		cout << endl;
#ifndef __DISABLE_LOGS
		Log::writeError(log, e);
		Out::writeError(out, e);
		Log::close(log);
		Out::close(out);
#endif // !__DISABLE_LOGS
	}
	catch (std::runtime_error& e)
	{
		cout << "Ошибка: " << e.what() << endl;
	}

	//auto current_files = get_files_in_directory(current_path);

	//std::set<std::u8string> new_files;
	//for (const auto& file : current_files) {
	//	if (initial_files.find(file) == initial_files.end()) {
	//		new_files.insert(file);
	//	}
	//}

	//if (!new_files.empty()) {
	//	std::wstring script_name = L"cleanup.bat";
	//	create_delete_script(new_files, script_name);
	//}

#endif // !TEST
	return 0;
}