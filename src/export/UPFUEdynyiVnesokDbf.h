//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ���������� ����� ��������� ���� 1��
//
// ��������� ������������ ������ "����" 15.02.2011�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UPFUEDYNYIVNESOKDBF_H
#define UPFUEDYNYIVNESOKDBF_H

#include <QSqlQuery>
#include <QObject>
#include <QFile>
#include <QMessageBox>
#include <QTextCodec>

#include "../additional/USalaryOptions.h"
#include "../additional/UDifFunc.h"

//--------------------utech--------------------utech--------------------utech--------------------
class UPFUEdynyiVnesokDbf
{	
  public:
	UPFUEdynyiVnesokDbf();
	~UPFUEdynyiVnesokDbf();
	
	char * strToxBaseChar(QString str);
	void showCantDeleteDbfFileMessage(QString file);
	bool createElZvit(QString path, short year, short month);
  private:
	QTextCodec *funcCodec;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
