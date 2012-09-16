//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 2010.02.10р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDialogChoosePidrozdil_salary_H
#define UDialogChoosePidrozdil_salary_H

#include <QDialog>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>

#include "../additional/UDifFunc.h"

#include "ui_UDialogChoosePidrozdil_salary.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UDialogChoosePidrozdil_salary : public QDialog
{
  Q_OBJECT

  public:
	UDialogChoosePidrozdil_salary(QWidget *parent = 0);
	~UDialogChoosePidrozdil_salary();
	
	int pidrozdilId();
  public slots:
		
  private:
	Ui::UDialogChoosePidrozdil_salary ui;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
