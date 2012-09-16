//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна налаштування програми
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGOPTIONS_H
#define UDIALOGOPTIONS_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QSqlQuery>

#include "ui_UDialogOptions.h"
#include "../additional/USalaryOptions.h"
//--------------------utech--------------------utech--------------------utech--------------------

//--------------------utech--------------------utech--------------------utech--------------------
class UDialogOptions : public QDialog, USalaryOptions
{
  Q_OBJECT

  public:
		UDialogOptions(QWidget *parent = 0);
		~UDialogOptions();
		
		void connectEditres();
		void disconnectEditres();
		void saveOptions();
		
  public slots:
		void populateOptions();
		void pushButton_accept_clicked();
		void pushButton_cancel_clicked();
		void pushButton_zastosuvaty_clicked();
		void optionsEdited();
		void tabWidget_Options_currentChanged(int);
		void populateOpzOrganizList();
		void populateOpzVersion();
		//ToolButtons
		void toolButton_vhSaldoScriptPath_clicked();
		void toolButton_osnNarahScriptPath_clicked();
		void toolButton_utrVidr01ScriptPath_clicked();
		void toolButton_utrVidr02ScriptPath_clicked();
		void toolButton_oplataScriptPath_clicked();
		void toolButton_opzDirPath_clicked();
  signals:
		void requestReloadSettings();
	  private:
		Ui::UDialogOptions ui;
		
		bool optEdited;
		QButtonGroup *buttonGroup;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
