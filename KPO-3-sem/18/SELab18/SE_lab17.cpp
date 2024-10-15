#include "stdafx.h"
#include "iostream"
#include "Error.h"
#include "In.h"
#include "Log.h"
#include "Parm.h"
#include "Out.h"
#include "LexerAutomata.h"
#include "MFST.h"
#include <tchar.h>
#include <clocale>
#include "GRB.h"

using namespace std;
int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "rus");
	Parm::PARM parm = Parm::getparm(argc, argv);
	Log::LOG log = Log::INIT_LOG;
	Out::OUT out = Out::INIT_OUT;
	try
	{
		out = Out::getout(parm.out);
		log = Log::getlog(parm.log);
		In::IN in_result = In::getin(parm.in);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Out::WriteOut(out, in_result);
		Log::WriteIn(log, in_result);
		LexAn::LEX LEX = LexAn::lexAnalize(parm, in_result);

		MFST_TRACE_START
			MFST::Mfst mfst(LEX.lexTable, GRB::getGreibach());			//автомат
		mfst.start();											// старт синтаксического анализа

		mfst.savededucation();									//сохранить правила вывода

		mfst.printrules();
		Log::Close(log);
		Out::Close(out);
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

	return 0;
};