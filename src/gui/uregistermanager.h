#ifndef UREGISTERMANAGER
#define UREGISTERMANAGER

#include "ui_registerForm.h"
#include "UUserDelegate.h"
#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextBrowser>
#include <QListWidgetItem>

//--------------------utech--------------------utech--------------------utech--------------------
class URegisterWindow: public QDialog
{
	Q_OBJECT
	public: 
		URegisterWindow(QWidget *parent = 0);
		inline int idGroup(){return idGroup_;};
		void showError(const QString);
		void updateData();
		void setConnectionOptions(const QString hostName, const QString userName, const QString pass, const QString driverName );
		void loadDatabase(const QString databaseName);
		void loadFirms();
		int groupId();
		
	private:
		Ui::Dialog ui;
		int idGroup_;
		QHash<int, QString> dbNameHash;//збер≥гаЇ ≥ндекс ≥ назву ф≥рми, €к≥ вит€гуЇ з реЇстру
		QHash<int, QString> workers;
		QString mainRegistryPath;//збер≥гаЇ шл€х в реЇстр≥ до основних настройок, ≥н≥ц≥ал≥зуЇтьс€ в конструктор≥.
		QString host;
		QString user;
		QString password;
		int port;
		QString driver;
		int lastFirmId;
				
	private slots:
		void verifyData();
		inline void exit(){reject();};
		void showHelp();
		void findText(const QString);
		void setDataInLineEdit(QListWidgetItem*);
		void loadFirmWorkers();
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
