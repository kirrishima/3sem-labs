#include "stdafx.h"
#include "MFST.h"
#include "cd.h"
#include "algorithm"

#define NO_MFST

using namespace std;
using namespace MFST;
using namespace GRB;

int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "rus");

#ifdef TEST
	int x = 0;
	auto table = LT::Create(30);
	LT::Add(table, { 't', 1 });
	LT::Add(table, { 'f', 1 });
	LT::Add(table, { 'i', 1 });

	LT::Add(table, { '(', 1 });
	LT::Add(table, { 't', 1 });
	LT::Add(table, { 'i', 1 });

	LT::Add(table, { ',', 1 });
	LT::Add(table, { 't', 1 });
	LT::Add(table, { 'i', 1 });
	LT::Add(table, { ')', 1 });

	LT::Add(table, { '{', 1 });
	LT::Add(table, { 'd', 1 });
	LT::Add(table, { 't', 1 });
	LT::Add(table, { 'i', 1 });
	LT::Add(table, { ';', 1 });

	LT::Add(table, { 'i', 1 });
	LT::Add(table, { '=', 1 });
	LT::Add(table, { 'i', 1 });
	LT::Add(table, { 'v', 1 });


	LT::Add(table, { '(', 1 });
	LT::Add(table, { 'i', 1 });
	LT::Add(table, { 'v', 1 });
	LT::Add(table, { 'i', 1 });
	LT::Add(table, { ')', 1 });


	//LT::Add(table, { '}', 1 });

	LT::Add(table, { ';', 1 });

	MFST_TRACE_START
		MFST::Mfst mfst(table, GRB::getGreibach());
	mfst.start();
#endif // TEST


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

#ifndef NO_MFST
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
#endif // !NO_MFST

#ifdef NO_MFST
		bool hasP = true;
#endif // NO_MFST

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

	//system("pause");
#endif // !TEST
	return 0;
}
