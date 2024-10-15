#ifndef LOG_H
#define LOG_H

#include "In.h"
#include "Parm.h"
#include "Error.h"
#include <string>

namespace Log	// ������ � ����������
{
	struct LOG // ��������
	{
		std::wstring logfile;  // ��� ����� ���������
		std::ofstream* stream; // �������� ����� ���������
	};

	static const LOG INIT_LOG{ L"", NULL };// ��������� ��� ��������� ������������� LOG
	LOG getlog(const std::wstring& logfile);// ������������ ��������� LOG
	void WriteLine(LOG log, char* c, ...);// ������� � �������� ������������ �����
	void WriteLine(LOG log, wchar_t* �, ...); //������� � �������� ������������ �����
	void WriteLog(LOG log);// ������� � �������� ���������
	void WriteParm(LOG log, Parm::PARM parm); // ������� � �������� ���������� � ������� ����������
	void WriteIn(LOG log, In::IN in);// ������� � �������� ���������� � ������� ������
	void WriteError(LOG log, Error::ERROR error); // ������� � �������� ���������� �� ������
	void Close(LOG log);
};

#endif // !LOG_H
