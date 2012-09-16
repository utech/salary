//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UWIDGETPRINTPLATIJKY_H
#define UWIDGETPRINTPLATIJKY_H

#include <QDialog>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>
#include <QCheckBox>

#include "../additional/UDifFunc.h"
#include "UPrintDocs.h"

#include "ui_UWidgetPrintPlatijky.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UWidgetPrintPlatijky : public QWidget
{
  Q_OBJECT

  public:
		UWidgetPrintPlatijky(QDate cDate, QWidget *parent = 0);
		~UWidgetPrintPlatijky();
		
		void populateTable(bool allChecked=false);
		void populateRow(int row, QSqlRecord rec);
		void populateItem(int row, int col, QString str, Qt::ItemFlags editFlag = Qt::ItemIsEditable);
  public slots:
		void pushButton_selectAll_clicked();
		void pushButton_deselectAll_clicked();
		void tableWidget_organizacii_itemChanged(QTableWidgetItem *item);
		void tableWidget_currentRowChanged();
		void pushButton_addRow_clicked();
		void pushButton_deleteRow_clicked();
		void pushButton_readSumyOplat_clicked();
		void pushButton_createCurPlatijka_clicked();
		void pushButton_createSelectedPlatijky_clicked();
  signals:
		void closeDockWindows();
	  private:
		Ui::UWidgetPrintPlatijky ui;
		
		UPrintDocs *printDocs;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
