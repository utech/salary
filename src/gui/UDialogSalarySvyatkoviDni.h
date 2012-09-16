//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGSALARYSVYATKOVIDNI_H
#define UDIALOGSALARYSVYATKOVIDNI_H

#include <QDialog>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>
#include <UTableWidgetSqlExt>

#include "../additional/UDifFunc.h"

#include "ui_UDialogSalarySvyatkoviDni.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UDialogSalarySvyatkoviDni : public QDialog
{
  Q_OBJECT

  public:
    UDialogSalarySvyatkoviDni(QWidget *parent = 0);
    ~UDialogSalarySvyatkoviDni();	
  public slots:
    void pushButton_create_clicked();
	void pushButton_delete_clicked();
	void setFilterAndPopulate();
  private:
    Ui::UDialogSalarySvyatkoviDni ui;
    UTableWidgetSqlExt *twExt;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
