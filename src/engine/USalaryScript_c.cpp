//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ��������� ����� ����������� �������� 
// � ��������� ������� ��� �������� ����������� ��������
// ��������� ������������ ������ "����" 9.12.2007�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "USalaryScript.h"

//--------------------utech--------------------utech--------------------utech--------------------
short UProgramOptions::TypIndexaciyi()
{
	QSqlQuery query;
	query.exec("SELECT Znach FROM progoptions WHERE id=7");
	if (query.seek(0))
		return query.value(0).toInt();
	else
		return -1;
}
//--------------------utech--------------------utech--------------------utech--------------------
short UProgramOptions::TypSocPilgy()
{
	QSqlQuery query;
	query.exec("SELECT Znach FROM progoptions WHERE id=8");
	if (query.seek(0))
		return query.value(0).toInt();
	else
		return -1;
}
//--------------------utech--------------------utech--------------------utech--------------------
