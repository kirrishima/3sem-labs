#include "stdafx.h"
#include <iostream>
#include <locale>
#include <cwchar>
#include <TCHAR.H>
#include <time.h>

#include "Out.h"
#include "Error.h"     // обработка ошибок
#include "Parm.h"      // обработка параметров
#include "Log.h"       // работа с протоколом
#include "In.h"        // ввод исходного файла
//#include "LexerAutomata.h"
#include "FST.h"
#include "SVV.h"
#include "Utils.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");

	/*cout << "----    тест Error::geterror    ----\n\n";
	try { throw ERROR_THROW(100); }
	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << ": " << e.message << "\n\n";
	};

	cout << "----    тест Error::geterrorin    ----\n\n";
	try { throw ERROR_THROW_IN(111, 7, 7); }
	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << ": " << e.message;
		if (e.inext.col >= 0 && e.inext.line >= 0)
		{
			cout << ", строка " << e.inext.line << ", позиция " << e.inext.col;
		}
		cout << " \n\n";
	}

	cout << "---- Тест Parm::getparm ----\n\n";
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		wcout << L"-in:" << parm.in << L", -out:" << parm.out << L", -log:" << parm.log << "\n\n";
	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << ": " << e.message << "\n\n";
	}
*/


//char a[8];

//a[0] = 'z';
//a[1] = 'z';
//a[2] = 'z';
//a[3] = '\0';

//auto z = SVV::CreateIdentifierFST();



	//cout << FST::execute(z, a);



		cout << "---- Тест In::getin -----\n\n";
		try
		{
			Parm::PARM parm = Parm::getparm(argc, argv);
			In::IN in = In::getin(parm.in);
			cout << in.text << endl;
			cout << "Всего символов: " << in.size << endl;
			cout << "Всего строк: " << in.lines << endl;
			cout << "Пропущено: " << in.ignore << endl;
		}
		catch (Error::ERROR e)
		{
			cout << "Ошибка " << e.id << ": " << e.message << endl;
			cout << "Cтрока " << e.inext.line << " позиция " << e.inext.col << "\n\n";
		}
		catch (std::wstring msg)
		{
			wcout << msg << endl;
		};

		Log::LOG log = Log::INIT_LOG;
		Out::OUT out = Out::INIT_OUT;
		try
		{
			Parm::PARM parm = Parm::getparm(argc, argv);
			log = Log::getlog(parm.log);
			out = Out::getout(parm.out);
			Log::WriteLine(log, (char*)"Тест:", (char*)" без ошибок \n", "");
			Log::WriteLine(log, (wchar_t*)L"Тест:", (wchar_t*)L" без ошибок \n", L"");
			Log::WriteLog(log);
			Log::WriteParm(log, parm);
			In::IN in = In::getin(parm.in);
			Log::WriteIn(log, in);
			Log::Close(log);
			Out::WriteOut(out, in);
			Out::Close(out);

			LexAn::_lexAnalize(parm, in);
		}
		catch (Error::ERROR e)
		{
			Log::WriteError(log, e);
			Out::WriteError(out, e);
		}
		catch (std::wstring msg)
		{
			wcout << msg << endl;
		};

		system("pause");




		//char a[10];

		//a[0] = 'm';
		//a[1] = 'a';
		//a[2] = 'i';
		//a[3] = 'n';
		//a[4] = '\0';



		//cout << strlen(a);
		//const char* str = "string ";

		//LexAn::LexicalAnalyzer l;
		//l.str = str;
		//cout << l._determineLexeme();

		//if (FST::execute(*LexAn::CreateIdentifierFST(str)))
		//{
		//	cout << "ANOBA";
		//}
		//else {
		//	cout << "BOBA";
		//}

	return 0;
}
