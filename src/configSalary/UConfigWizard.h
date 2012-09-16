//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу головного вікна програми нарахування зарплати
// Створений програмістами Компанії "Утех" 29.11.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#ifndef UCONFIGWIZARD_H
#define UCONFIGWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QStringList>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QVariant>
#include <QSettings>
#include <QTableWidget>
#include <QPushButton>
#include <QStringList>
#include <QCoreApplication>
#include <QCheckBox>
#include <QTextEdit>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class UConfigWizard: public QWizard
{
		Q_OBJECT
	public:
		UConfigWizard();
		~UConfigWizard(){};
		void accept();
		void reject();
		
	public slots:
		void showHelp();

	private:
		QTableWidget *table;
		QTextEdit *textEdit;
};
//--------------------utech--------------------utech--------------------utech--------------------
class UInfoPage : public QWizardPage
{
    Q_OBJECT
	public:
		UInfoPage(QWidget *parent = 0);
};
//--------------------utech--------------------utech--------------------utech--------------------
class UServerSettingsPage : public QWizardPage
{
    Q_OBJECT
	public:
    UServerSettingsPage(QWidget *parent = 0);
		void initializePage();
		
	private:
		QLabel *labelLogin;
		QLabel *labelPassWord;
		QLabel *labelHost;
		QLineEdit *lineEditLogin;
		QLineEdit *lineEditPassWord;
		QLineEdit *lineEditHost;
};
//--------------------utech--------------------utech--------------------utech--------------------
class UNumOrgPage : public QWizardPage
{
    Q_OBJECT
	public:
		UNumOrgPage(QWidget *parent = 0);
		void initializePage();
		QTableWidget *orgTable();
		
	public slots:
		void addPidprStr();
		
	private:
		QTableWidget *table;
		QPushButton *buttonAdd;
};
//--------------------utech--------------------utech--------------------utech--------------------
class UAcceptPage : public QWizardPage
{
    Q_OBJECT
	public:
		UAcceptPage(QWidget *parent = 0);
		void initializePage();
		void setOrgTable(QTableWidget *orgTable);
		QTextEdit *resTextEdit(){return textEdit;};
		
	private:
		QTextEdit *textEdit;
		QTableWidget *table;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
