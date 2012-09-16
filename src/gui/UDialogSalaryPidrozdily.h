//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGSALARYPIDROZDILY_H
#define UDIALOGSALARYPIDROZDILY_H

#include <QDialog>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>
#include <UTableWidgetSqlExt>

#include "../additional/UDifFunc.h"

#include "ui_UDialogSalaryPidrozdily.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UDialogSalaryPidrozdily : public QDialog
{
  Q_OBJECT

  public:
    UDialogSalaryPidrozdily(QWidget *parent = 0);
    ~UDialogSalaryPidrozdily();	
  public slots:
    void pushButton_create_clicked();
	void pushButton_delete_clicked();
  private:
    Ui::UDialogSalaryPidrozdily ui;
    UTableWidgetSqlExt *twExt;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
