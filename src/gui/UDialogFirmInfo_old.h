//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGFIRMINFO_H
#define UDIALOGFIRMINFO_H

#include <QDialog>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>

#include "../additional/UDifFunc.h"

#include "ui_UDialogFirmInfo.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UDialogFirmInfo : public QDialog
{
  Q_OBJECT

  public:
		UDialogFirmInfo(QWidget *parent = 0);
		~UDialogFirmInfo();
		
		bool saveFirmInfo();
		void populateOrgInfo();
  public slots:
		void acceptDialog();
		void onlySaveInfo();
		
	  private:
		Ui::UDialogFirmInfo ui;

};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
