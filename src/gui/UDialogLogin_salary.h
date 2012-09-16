//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 2010.02.10р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGLOGIN_SALARY_H
#define UDIALOGLOGIN_SALARY_H

#include <QDialog>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>

#include "../additional/UDifFunc.h"

#include "ui_UDialogLogin_salary.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UDialogLogin_salary : public QDialog
{
  Q_OBJECT

  public:
	UDialogLogin_salary(QWidget *parent = 0);
	~UDialogLogin_salary();
	
	void loadFirms();
  public slots:
	void acceptDialog();
		
  private:
	Ui::UDialogLogin_salary ui;
	
	QHash<int, QString> dbNameHash;
	QString host;
	QString user;
	QString password;
	int port;
	QString driver;
	int lastFirmId;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
