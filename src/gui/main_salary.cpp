#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QLabel>
#include <QHash>

#include <UDebugForm>
//#include "uregistermanager.h"
#include "UDialogLogin_salary.h"
#include "UPeopleSalaryMainWindow.h"
#include "USalary.h"
#include "USalaryDbUpdaterFunctions.h"
//--------------------utech--------------------utech--------------------utech--------------------
int main(int argc, char **argv)
{
	QApplication app(argc,argv);
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
	
#ifdef SALARY_LOCAL
	USalaryLocal::MySqlCheck();
#endif
	
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	UDebug::createDebugForm();
	
	UDialogLogin_salary * d = new UDialogLogin_salary();
	if (d->exec() != QDialog::Accepted)
		return 0;
	delete d;
	
	USalaryDbUpdater *upd = new USalaryDbUpdater();
	if (!upd->checkVersions())
		return 0;
	delete upd;
	
	UPeopleSalaryMainWindow *mainWindow = new UPeopleSalaryMainWindow();
	mainWindow->show();
	QApplication::connect(mainWindow, SIGNAL(requestDebugWindow()), UDebug::debugFormPointer, SLOT(showDebug()));
	QApplication::connect(mainWindow, SIGNAL(mainWindowClosed()), UDebug::debugFormPointer, SLOT(close()));

	app.connect(&app,SIGNAL(lastWindowClosed()), &app,SLOT(quit()));
	return app.exec();
}
//--------------------utech--------------------utech--------------------utech--------------------
