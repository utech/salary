//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ���������� ����� ����������� �������� 
// � ��������� ������� ��� �������� ����������� ��������
// ��������� ������������ ������ "����" 08.10.2009�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#ifndef USALARY_H
#define USALARY_H

#include <QObject>
#include <QString>

//--------------utech--------------------utech--------------------utech--------------------
class USalary
{
  public:
	static QString version();
	static int dbVersion();
};
#ifdef SALARY_LOCAL
class USalaryLocal : public QObject
{
	Q_OBJECT
  public:
	static bool MySqlCheck();
	static int MySqlPort();
	static QString MySqlServerProgramName();
	static bool MySqlServerWorking();
	static bool MySqlServerStart();
	static void MySqlServerSetConfigFile();
  private:
	
};
#endif
//--------------------utech--------------------utech--------------------utech--------------------

#endif
