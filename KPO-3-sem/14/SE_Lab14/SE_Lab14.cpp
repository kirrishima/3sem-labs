#include "stdafx.h"
#include <iostream>
#include <locale>
#include <cwchar>
#include <TCHAR.H>
#include <time.h>

#include "Out.h"
#include "Error.h"		//��������� ������
#include "Parm.h"		//��������� ����������
#include "Log.h"		//������ � ����������
#include "In.h"			//���� ��������� �����

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");

	cout << "----    ���� Error::geterror    ----\n\n";
	try { throw ERROR_THROW(100); }
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << "\n\n";
	};

	cout << "----    ���� Error::geterrorin    ----\n\n";
	try { throw ERROR_THROW_IN(111, 7, 7); }
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << ", ������ " << e.inext.line << ", ������� " << e.inext.col << " \n\n";
	};

	cout << "---- ���� Parm::getparm ----\n\n";
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		wcout << "-in:" << parm.in << ", -out:" << parm.out << ", -log:" << parm.log << "\n\n";
	}
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << "\n\n";
	}

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

	Log::LOG log = Log::INITLOG;
	Out::OUT out = Out::INITOUT;
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
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		Out::WriteError(out, e);
	};
	system("pause");

	return 0;
}
