//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна реєстрації користувача
//
// Створений програмістами Компанії "Утех" 29.04.2010р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UFormSalaryTabelSumWidget_H
#define UFormSalaryTabelSumWidget_H

#include <QtGui>
#include <QSettings>
#include <QDir>
#include <QtSql>
#include <QTime>

#include "ui_UFormSalaryTabelSumWidget.h"

//--------------------utech--------------------utech--------------------utech--------------------
class UFormSalaryTabelSumWidget : public QWidget
{
  Q_OBJECT

  public:
	UFormSalaryTabelSumWidget(QDate dt, bool showWorkedWorkers=false, QWidget *parent = 0);
	~UFormSalaryTabelSumWidget();
	
  public slots:
	void populateSumTabel();
	void sumTabelCurRowChanged( const QModelIndex & curIndex, const QModelIndex & prevIndex);
	void updateSumTabelSQLRows(int row, int column);
	void showOnlyWorkWorkersChanged(int state);
  signals:
	void mainWindowClosed();
	void currentWorkerChanged(int NprId);
  private:
	Ui::UFormSalaryTabelSumWidget ui;
	bool showOnlyWorkWorkers;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
