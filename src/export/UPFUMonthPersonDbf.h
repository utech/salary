//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ���������� ����� ��������� ���� 1��
//
// ��������� ������������ ������ "����" 12.10.2008�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UPFUMonthPersonDbf_H
#define UPFUMonthPersonDbf_H

#include <QSqlQuery>
#include <QObject>
#include <QXmlStreamWriter>
#include <QFile>
#include <QMessageBox>
#include <QTextCodec>

#include "../additional/USalaryOptions.h"
#include "../additional/UDifFunc.h"

//--------------------utech--------------------utech--------------------utech--------------------
class UPFUMonthPersonDbf
{
  public:
	UPFUMonthPersonDbf();
	~UPFUMonthPersonDbf();
	
	char * strToxBaseChar(QString str);
	void showCantDeleteDbfFileMessage(QString file);
	bool createElZvit(QString path, short year, short month);
  private:
	QTextCodec *funcCodec;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
