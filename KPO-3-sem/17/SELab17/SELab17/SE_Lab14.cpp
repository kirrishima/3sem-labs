#include "stdafx.h"
#include <iostream>
#include <locale>
#include <cwchar>
#include <TCHAR.H>
#include <time.h>

#include "Out.h"
#include "Error.h"     // ��������� ������
#include "Parm.h"      // ��������� ����������
#include "Log.h"       // ������ � ����������
#include "In.h"        // ���� ��������� �����
//#include "LexerAutomata.h"
#include "FST.h"
#include "SVV.h"
#include "Utils.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");

	/*cout << "----    ���� Error::geterror    ----\n\n";
	try { throw ERROR_THROW(100); }
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << "\n\n";
	};

	cout << "----    ���� Error::geterrorin    ----\n\n";
	try { throw ERROR_THROW_IN(111, 7, 7); }
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message;
		if (e.inext.col >= 0 && e.inext.line >= 0)
		{
			cout << ", ������ " << e.inext.line << ", ������� " << e.inext.col;
		}
		cout << " \n\n";
	}

	cout << "---- ���� Parm::getparm ----\n\n";
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		wcout << L"-in:" << parm.in << L", -out:" << parm.out << L", -log:" << parm.log << "\n\n";
	}
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << "\n\n";
	}
*/


//char a[8];

//a[0] = 'z';
//a[1] = 'z';
//a[2] = 'z';
//a[3] = '\0';

//auto z = SVV::CreateIdentifierFST();



	//cout << FST::execute(z, a);



		cout << "---- ���� In::getin -----\n\n";
		try
		{
			Parm::PARM parm = Parm::getparm(argc, argv);
			In::IN in = In::getin(parm.in);
			cout << in.text << endl;
			cout << "����� ��������: " << in.size << endl;
			cout << "����� �����: " << in.lines << endl;
			cout << "���������: " << in.ignore << endl;
		}
		catch (Error::ERROR e)
		{
			cout << "������ " << e.id << ": " << e.message << endl;
			cout << "C����� " << e.inext.line << " ������� " << e.inext.col << "\n\n";
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
			Log::WriteLine(log, (char*)"����:", (char*)" ��� ������ \n", "");
			Log::WriteLine(log, (wchar_t*)L"����:", (wchar_t*)L" ��� ������ \n", L"");
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
