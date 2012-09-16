//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу UPrintDocs програми нарахування зарплати
//
// Створений програмістами Компанії "Утех" 17.07.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UPRINTDOCS_H
#define UPRINTDOCS_H

#include <QString>
#include <QVariant>
#include <QTextCodec>
#include <QTextLength>
#include <QTextTable>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QTextTableCell>
#include <QTextCursor>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QTextFrameFormat>
#include "../additional/UDifFunc.h"
#include <UPrintForm>
#include <UMath>
#include <UNumberToStringConverter>
#include <UPostupForm>

#include <QImage>
#include <QPainter>
#include <QFontMetrics>
//--------------------utech--------------------utech--------------------utech--------------------
struct UPlatijkyInfo
{
	QDate		date;
	QString		oderjuvach;
	QString		oderjuvachEdrpou;
	QString		oderjuvachBank;
	QString		oderjuvachMFO;
	QString		oderjuvachRahunok;
	QString		pryznachennyaPlateju;
	double		suma;
	QString		platnyk;
	QString		platnykEdrpou;
	QString		platnykBank;
	QString		platnykMFO;
	QString		platnykRahunok;
	QString		platijkaNum;
};
//--------------------utech--------------------utech--------------------utech--------------------
class UPrintDocs
{
    public:
        UPrintDocs();
		virtual ~UPrintDocs();
		
		void printFragment(QTextDocumentFragment fragment, QString docName="Документ \"Ель-Зар\"", QPrinter::Orientation printerOrientation=QPrinter::Portrait);
		void perelik_pracivnykiv(short pidrozdilId, bool pracFilter = false);
		void print_narahuvannya_za_misac(int year, int month, short pidrozdilId, bool pracFilter = false);
		void print_worker_cards(int year, int month, short pidrozdilId, bool pracFilter = false);
		void print_cur_worker_card(int year, int month, int workerId);
		void print_year_cards(int year, short pidrozdilId, bool pracFilter = false);
		void print_year_cur_worker_card(int year, int workerId);
		void print_year_utrymannya_cards(int year, short pidrozdilId, bool pracFilter = false);
		void print_year_utrymannya_cur_worker_card(int year, int workerId);
		
		void print_vidomist_utrymannya(int year, int month, short pidrozdilId, bool pracFilter = false);
		void print_vidomist_vidrahuvannya(int year, int month, short pidrozdilId, bool pracFilter = false);
		void print_vidomist_oplaty_i_zaborgovanosti(int year, int month, short pidrozdilId, bool pracFilter = false);
		
		void print_vidomist_narah_i_vidrah_by_buhrahunok(int year, int month, bool pracFilter = false);
		
		//UPrintDocks_2
		void print_tabel_for_pidrozdil(short year, short month, int pidrozdilId);
		
		void print_onePlatijka(UPlatijkyInfo p);
		
		QTextDocumentFragment createWorkerCardFragment(int worker_id, int year, int month);
		QTextDocumentFragment createYearCardFragment(int worker_id, short year);
		QTextDocumentFragment createYearUtrymannyaCardFragment(int worker_id, short year);
		QTextDocumentFragment createPlatijkaFragment(UPlatijkyInfo p, QTextFormat::PageBreakFlag pageBrake = QTextFormat::PageBreak_Auto);
		
		UPrintForm *printform;
		
		QImage verticalTextToImage(const QString & text, QTextCharFormat & textCharFormat){
			QFontMetrics fontMetr(textCharFormat.font());
			int width = fontMetr.width(text);
			QImage img(fontMetr.height(), width, QImage::Format_Mono);
			QPainter painter(&img);
			painter.setFont(textCharFormat.font());
			painter.setPen(QColor(Qt::black));
			painter.fillRect(img.rect(), QColor(Qt::white));
			QMatrix matrix;
			matrix.rotate(-90);
			painter.setMatrix(matrix);
			painter.drawText(-width, fontMetr.height()-fontMetr.xHeight(), text);
			return img;
		};
		int timeToSec(QString time){
			return time.section(':',0,0).toInt()*60 + time.section(':',1,1).toInt();
		};
		int timeToMinute(QTime time){
			return time.hour()*60 + time.minute();
		};
		QString secToTime(int sec){
			if (sec==0)
				return "-";
			return QString::number(int (sec/60))+":"+QString::number(fmod(sec,60)).rightJustified(2,'0');
		};
		
    public slots:

	signals:
		
    private:
		QStringList monthlist;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
