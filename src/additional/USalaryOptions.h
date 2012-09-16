//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу опцій
//
// Створений програмістами Компанії "Утех" 27.10.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef USALARYOPTIONS_H
#define USALARYOPTIONS_H

#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QSettings>
#include <QDate>
#include <QColor>
#include <QFile>
#include <QXmlStreamReader>

#include <UMath>

#define OPZ_DEFAULT_PATH "C:\\opz\\"
#define OPZ_ORGS_SETTINGS_PATH "spr/SETTINGS.XML"
#define OPZ_PROG_VERSION_FILE_PATH "spr/version.xml"
#define OPZ_DPI_NAME_FILE_PATH "spr/spr_sti.xml"
//--------------------utech--------------------utech--------------------utech--------------------
class USalaryOptions
{
  public:
	static QString getOption(int optionId);
	static bool saveOption(int optionId, QString znach);
	static bool saveOption(int optionId, int znach);
	static bool saveOption(int optionId, double znach);
	QStringList getOpzOrgsList(QString opzPath);
	QString opzVersion(QString opzPath);
	QString opzDpiName(QString opzPath, QString _opzDpiId = "");
	QString opzDpiId(QString opzPath);
  private:
	
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
