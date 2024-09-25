#ifndef PARAM_H
#define PARAM_H

#include <string>

#define PARM_IN L"-in:" // ���� ��� ����� ��������� ����
#define PARM_OUT L"-out:" // ���� ��� ����� ���������� ����
#define PARM_LOG L"-log:" // ���� ��� ����� �������
#define PARM_OUT_DEFAULT_EXT L".out" // ���������� ����� ���������� ���� �� ���������
#define PARM_LOG_DEFAULT_EXT L".log" // ���������� ����� ���������� ���� �� ���������

namespace Parm // ��������� ������� ���������� 
{
	struct PARM // ������� ���������	
	{
		std::wstring in;  // -in: ��� ����� ��������� ����
		std::wstring out; // -out: ��� ����� ���������� ����
		std::wstring log; // -log: ��� ����� ���������
	};

	PARM getparm(int argc, _TCHAR* argv[]); // ������������ struct PARM �� ������ ���������� ������� main

};

#endif // !PARAM_H