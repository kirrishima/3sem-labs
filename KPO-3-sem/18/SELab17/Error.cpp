#include "stdafx.h" 
#include "Error.h"
namespace Error
{
	// ����� ������: 0 - 99 ��������� ������
	//				 100 - 199 - ������ ����������
	//				 110 - 119 - ������ �������� � ������ ������ 
	
	ERROR errors[ERROR_MAX_ENTRY] = //������� ������
	{
	ERROR_ENTRY(0, "������������ ��� ������"), ERROR_ENTRY(1, "��������� ����"),
	// ��� ������ ��� ��������� 0 - ERROR_MAX_ENTRY
	ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5), ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
	ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
	ERROR_ENTRY(100, "�������� -in ������ ���� �����"),
	ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103), ERROR_ENTRY(104, "��������� ����� �������� ���������"),
	ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
	ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
	ERROR_ENTRY(110, "������ ��� �������� ����� � �������� ����� (-in)"),
	ERROR_ENTRY(111, "������������ ������ � �������� ����� (-in)"),
	ERROR_ENTRY(112, "������ ��� �������� ����� ���������(-log)"),
	ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
	ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
	ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
	ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500), ERROR_ENTRY(600, "�������� ��������� ���������"),
	ERROR_ENTRY(601, "��������� ��������"),
	ERROR_ENTRY(602, "������ � ���������"),
	ERROR_ENTRY(603, "������ � ���������� �������"),
	ERROR_ENTRY(604, "������ � ���������� ��������� �������"),
	ERROR_ENTRY_NODEF(605), ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609), ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
	ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680),
	ERROR_ENTRY_NODEF10(690),
	ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
	ERROR geterror(int id) {

		if (id < 0 || id > ERROR_MAX_ENTRY) {   //���� ��� ��� ��������� ������� ������
			return errors[0];
		}

		else {
			return errors[id];
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1) {

		if (id < 0 || id > ERROR_MAX_ENTRY) {
			return errors[0];
		}

		else {
			errors[id].inext.line = line;
			errors[id].inext.col = col;
			return errors[id];
		}
	}
};