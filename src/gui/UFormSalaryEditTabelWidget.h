//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна реєстрації користувача
//
// Створений програмістами Компанії "Утех" 23.03.2010р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UFormSalaryEditTabelWidget_H
#define UFormSalaryEditTabelWidget_H

#include <QtGui>
#include <QSettings>
#include <QDir>
#include <QtSql>
#include <QTime>

#include "ui_UFormSalaryEditTabelWidget.h"

//--------------------utech--------------------utech--------------------utech--------------------
class UFormSalaryEditTabelWidget : public QWidget
{
  Q_OBJECT

  public:
	UFormSalaryEditTabelWidget(QWidget *parent = 0);
	~UFormSalaryEditTabelWidget();
	int currentPidrozdil();
	void fillTabelCell(int row, int col);
  public slots:
	void populateTabel();
	void tableWidget_tabel_itemDoubleClicked(QTableWidgetItem *item);
	void pushButton_findCell_clicked();
	void printTabel();
	void sumTabelCurRowChanged( const QModelIndex & curIndex, const QModelIndex & prevIndex);
  signals:
	void mainWindowClosed();
	void currentWorkerChanged(int NprId);
  private:
	Ui::UFormSalaryEditTabelWidget ui;
};
//--------------------utech--------------------utech--------------------utech--------------------
class USalaryTabelDayDelegate : public QItemDelegate
{	
   Q_OBJECT
  public:
	USalaryTabelDayDelegate(QObject *parent = 0);
	void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
	QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
  private:
	QFont font1;
	QFont font2;
	int mergin1;
	int mergin2;
	int spacingV;
	int spacingH;
	int width1;
	int height1;
	int width2;
	int height2;
	QHash<unsigned char, QString> tabelChars;
	QHash<unsigned char, QColor> tabelColors;
};
//--------------------utech--------------------utech--------------------utech--------------------
struct USalaryTabelDay
{
	short workType; //Тип присутності: на роботі, відпустка, лікарняні і т.д.
	QTime rob;
	QTime sv;
	QTime nich;
};
Q_DECLARE_METATYPE(USalaryTabelDay)
#endif
