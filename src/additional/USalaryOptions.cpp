//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконаня класу опцій
//
// Створений програмістами Компанії "Утех" 27.10.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "USalaryOptions.h"

#include <QMessageBox>
#include <QtDebug>

//--------------------utech--------------------utech--------------------utech--------------------
QString USalaryOptions::getOption(int optionId)
{
	QSqlQuery __query;
	__query.exec("SELECT Znach FROM progOptions WHERE id=" + QString::number(optionId) + " LIMIT 1");
	if (__query.seek(0))
		return __query.value(0).toString();
	else
		return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryOptions::saveOption(int optionId, QString znach)
{
	QSqlQuery __query;
	if (__query.exec("UPDATE progOptions SET Znach='"+znach+"' WHERE id="+QString::number(optionId)+" LIMIT 1")){
		return true;
	}
	else{
		return false;
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryOptions::saveOption(int optionId, int znach)
{
	return saveOption(optionId, QString::number(znach));
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryOptions::saveOption(int optionId, double znach)
{
	return saveOption(optionId, QString::number(znach));
}
//--------------------utech--------------------utech--------------------utech--------------------
QStringList USalaryOptions::getOpzOrgsList(QString opzPath)
{
	QXmlStreamReader reader;
	QString opzSettingsFileName = opzPath+"/"+OPZ_ORGS_SETTINGS_PATH;
	QFile file(opzSettingsFileName);
	QStringList orgsList;
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		qDebug() << "Помилка: Не можу прочитати файл " << qPrintable(opzSettingsFileName)
				<< ": " << qPrintable(file.errorString());
		return orgsList;
    }
	reader.setDevice(&file);
	reader.readNext();
	while (!reader.atEnd()) {
		if (reader.isStartElement() && (reader.name()=="ROW")) {
			while (reader.name()!="NAME" && !reader.atEnd())
				reader.readNext();
			orgsList << reader.readElementText();
		}
		else{
			reader.readNext();
		}
	}
	file.close();
	return orgsList;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USalaryOptions::opzVersion(QString opzPath)
{
	QXmlStreamReader reader;
	QString opzVersionFileName = opzPath+"/"+OPZ_PROG_VERSION_FILE_PATH;
	QFile file(opzVersionFileName);
	QString versionStr="-";
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		qDebug() << "Помилка: Не можу прочитати файл " << qPrintable(opzVersionFileName)
				<< ": " << qPrintable(file.errorString());
		return versionStr;
    }
	reader.setDevice(&file);
	reader.readNext();
	while (!reader.atEnd()) {
		if (reader.isStartElement() && (reader.name()=="version")) {
			versionStr = reader.attributes().value("number").toString();
			break;
		}
		else{
			reader.readNext();
		}
	}
	file.close();
	return versionStr;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USalaryOptions::opzDpiName(QString opzPath, QString _opzDpiId)
{
	QXmlStreamReader reader;
	QString opzDpiNameFileName = opzPath+"/"+OPZ_DPI_NAME_FILE_PATH;
	if (_opzDpiId == "")
		_opzDpiId = opzDpiId(opzPath);
	QFile file(opzDpiNameFileName);
	QString dpiName="";
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		qDebug() << "Помилка: Не можу прочитати файл " << qPrintable(opzDpiNameFileName)
				<< ": " << qPrintable(file.errorString());
		return dpiName;
    }
	reader.setDevice(&file);
	reader.readNext();
	while (!reader.atEnd()) {
		if (reader.isStartElement() && reader.name()=="C_STI" && reader.readElementText()==_opzDpiId) {
			while (!reader.atEnd()) {
				if (reader.isStartElement() && reader.name()=="NAME_STI") {
					dpiName = reader.readElementText();
					file.close();
					return dpiName;
				}
				else{
					reader.readNext();
				}
			}
		}
		else{
			reader.readNext();
		}
	}
	file.close();
	return "";
}
//--------------------utech--------------------utech--------------------utech--------------------
QString USalaryOptions::opzDpiId(QString opzPath)
{
	QXmlStreamReader reader;
	QString opzDpiNameFileName = opzPath+"/"+OPZ_ORGS_SETTINGS_PATH;
	QFile file(opzDpiNameFileName);
	QString dpiID="";
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		qDebug() << "Помилка: Не можу прочитати файл " << qPrintable(opzDpiNameFileName)
				<< ": " << qPrintable(file.errorString());
		return dpiID;
    }
	reader.setDevice(&file);
	do{
		reader.readNext();
		if (reader.atEnd())
			return dpiID;
	}
	while (reader.isStartElement() && (reader.name()=="ROW") && (reader.attributes().value("num") == getOption(101)));
	do{
		reader.readNext();
		if (reader.atEnd())
			break;
		if (reader.isStartElement() && reader.name()=="C_STI"){
			dpiID = reader.readElementText();
			break;
		}
	} while (!(reader.isEndElement() && reader.name()=="ROW" && !reader.atEnd()));
	file.close();
	return dpiID;
}
//--------------------utech--------------------utech--------------------utech--------------------
