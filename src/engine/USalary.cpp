//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу нарахування зарплати 
// з допомогою скриптів для програми нарахування зарплати
// Створений програмістами Компанії "Утех" 08.10.2009р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "USalary.h"

#include <QMessageBox>
#include <QProcess>
#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QTcpSocket>
#include <QtDebug>
//--------------------utech--------------------utech--------------------utech--------------------
QString USalary::version()
{
	return "0.9.12";
}
//--------------------utech--------------------utech--------------------utech--------------------
int USalary::dbVersion()
{
	return 5;
}
//--------------------utech--------------------utech--------------------utech--------------------
#ifdef SALARY_LOCAL
bool USalaryLocal::MySqlCheck()
{
	if (USalaryLocal::MySqlServerWorking())
		return true;
	else
		return USalaryLocal::MySqlServerStart();
}
//--------------------utech--------------------utech--------------------utech--------------------
int USalaryLocal::MySqlPort()
{
	return 3310;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USalaryLocal::MySqlServerProgramName()
{
	return "mysqld-nt";
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryLocal::MySqlServerWorking()
{
	QProcess pr;
	QDir::setCurrent(QCoreApplication::applicationDirPath()+"/../mysql/bin");
	pr.start("mysqladmin --port="+QString::number(USalaryLocal::MySqlPort())+" --host=localhost --user=root --password=pswrd ping");
	if (!pr.waitForReadyRead(3000))
		return false;
	QString proResult = pr.readAll();
	pr.kill();
	qDebug() << proResult;
	if (proResult.contains("is alive",Qt::CaseInsensitive))
		return true;
	else
		return false;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryLocal::MySqlServerStart()
{
	USalaryLocal::MySqlServerSetConfigFile();
	QDir::setCurrent(QCoreApplication::applicationDirPath()+"/../mysql/bin");
	QProcess::startDetached(USalaryLocal::MySqlServerProgramName()+" --standalone");
	QTcpSocket socket;
	socket.connectToHost("localhost", USalaryLocal::MySqlPort());
	return socket.waitForConnected(5000);
}
//--------------------utech--------------------utech--------------------utech--------------------
void USalaryLocal::MySqlServerSetConfigFile()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath()+"/../mysql");
	QSettings *settings = new QSettings("./my.ini", QSettings::IniFormat);
	settings->beginGroup("mysqld");
	settings->setValue("port", USalaryLocal::MySqlPort());
	settings->setValue("basedir", QDir(QCoreApplication::applicationDirPath()+"/../mysql").absolutePath());
	settings->setValue("datadir", QDir(QCoreApplication::applicationDirPath()+"/../mysql/data").absolutePath());
	delete settings;
}
#endif
//--------------------utech--------------------utech--------------------utech--------------------
