//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна довідника робочих днів
//
// Створений програмістами Компанії "Утех" 19.02.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UMONTHLYBUDJETNINORMYFORM_H
#define UMONTHLYBUDJETNINORMYFORM_H

#include <QWidget>
#include <QtSql>
#include <QHeaderView>
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QTableView>
#include <QHash>

#include <UMath>
#include <UStandardDelegate>
#include "../additional/UDifFunc.h"
#include "ui_UMonthlyBudjetniNormyForm.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UMonthlyBudjetniNormyForm : public QDialog
{
  Q_OBJECT

  public:
		UMonthlyBudjetniNormyForm(QWidget *parent = 0);
		~UMonthlyBudjetniNormyForm();
  public slots:
		void populateWorkDaysTableWidget();
		void setFiltrYear(int year);
		void saveWorkDays(QTableWidgetItem *item);
		
	  private:
		Ui::UMonthlyBudjetniNormyForm ui;

};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
