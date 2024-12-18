#include "stdafx.h"
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


int _tmain(int argc, _TCHAR* argv[]) {

	int returnCode = 0;
	setlocale(LC_ALL, "rus");

	cout << R"(
*************************************************************
*                    Compiler GM-2024                       *
*     Course project for Software Engineering at BSTU       *
*************************************************************
)" << '\n';

	Log::LOG log;
	Out::OUT out;

	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		out = Out::getout(parm.out);
		log = Log::getlog(parm.log);

		Log::writeLog(log);
		Log::writeParm(log, parm);
		Out::writeOut(out, in);
		Log::writeIn(log, in);

		auto [LexTable, IdTable] = lex_analysis::lexAnalize(parm, in, log);

		cout << '\n';


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

		int x = semantic::check(IdTable, LexTable, &log);
		if (x)
		{
			cout << x << " ошибок, выход...\n";
			IT::Delete(IdTable);
			LT::Delete(LexTable);
			exit(1);
		}

		CD::CodeGeneration cd(IdTable, LexTable, &parm, &log);
		cd.generateCode();

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
			cout << "\nСтрока: " << e.sourceLine;
		}
		cout << endl;
		Log::writeError(log, e);
		Out::writeError(out, e);
		Log::close(log);
		Out::close(out);

	}
	catch (std::runtime_error& e)
	{
		cout << "Ошибка: " << e.what() << endl;
	}

	return 0;
}