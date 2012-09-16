//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу створення звіту 1ДФ
//
// Створений програмістами Компанії "Утех" 12.10.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
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
		
		QString _C_STI;	// Код області (2) + код району
		QString _TIN;	// ЄДРПОУ
		QString _NAME;	// Назва організації
		QString _DFIO;	// ПІБ директора
		QString _BFIO;	// ПІБ бухгалтера
		QString _DINN;	// ДРФО директора
		QString _BINN;	// ДРФО бухгалтера
		QString _SAVE_PATH;		// Шлях збереження документів
		QString _PERSON_TYPE;	// Фізична/юридична особа
		QString _SOFTWARE;		// "opz" + версія
		QString _HSTI;			// Назва ДПІ
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
