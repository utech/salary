//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ���������� ����� ��������� ���� 1��
//
// ��������� ������������ ������ "����" 12.10.2008�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UZVIT1DF_H
#define UZVIT1DF_H

#define C_DOC_VER "2"

#include <QSqlQuery>
#include <QObject>
#include <QXmlStreamWriter>
#include <QFile>
#include <QMessageBox>

#include "../additional/USalaryOptions.h"
#include "../additional/UDifFunc.h"

//--------------------utech--------------------utech--------------------utech--------------------
class UZvit1df : QObject, USalaryOptions
{	
   Q_OBJECT
public:
		UZvit1df( QObject *parent = 0);
		
		bool readHeader();
		QString registerFileInOpz(int kvartal, int year, QDate impDate);
		bool import1dfToFile(int kvartal, int year, QDate impDate);
		
		void writeOpzTableElement(QString qualifiedName, QString text, int ROWNUM);
		void writeDocDeclBlock(QXmlStreamWriter *writer, int kvartal, int year);
		void writePeriodBlock(QXmlStreamWriter *writer, int kvartal, int year);
		void writeDocInfoBlock(QXmlStreamWriter *writer, int kvartal, int year, int docCounter);
		QString opz1DfFileName(int kvartal, int year, int docCounter);
		~UZvit1df();
public slots:
		
private:
		QXmlStreamWriter *xmlStream;
		QXmlStreamReader *reader;
		
		QString _C_STI;	// ��� ������ (2) + ��� ������
		QString _TIN;	// ������
		QString _NAME;	// ����� ����������
		QString _DFIO;	// ϲ� ���������
		QString _BFIO;	// ϲ� ����������
		QString _DINN;	// ���� ���������
		QString _BINN;	// ���� ����������
		QString _SAVE_PATH;		// ���� ���������� ���������
		QString _PERSON_TYPE;	// Գ�����/�������� �����
		QString _SOFTWARE;		// "opz" + �����
		QString _HSTI;			// ����� �ϲ
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
