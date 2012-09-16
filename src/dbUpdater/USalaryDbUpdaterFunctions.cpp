//------------------------------------------------------------
//
// Implamantation of dbUpdaterFunctions
// Created by Roman Mulyk 16.01.2009
//
//------------------------------------------------------------

#include <USqlAccessible>
#include <QCoreApplication>
#include <QDir>
#include <QPushButton>
#include <QtDebug>

#include "USalaryDbUpdaterFunctions.h"
#include "USalary.h"

//------------------------------------------------------------
USalaryDbUpdater::USalaryDbUpdater()
{
	
}
//------------------------------------------------------------
USalaryDbUpdater::~USalaryDbUpdater()
{
	
}
//------------------------------------------------------------
bool USalaryDbUpdater::checkVersions()
{
	bool ver_ok=false;
	QSqlQuery query("SELECT id, version FROM version_salary ORDER BY id");
	query.seek(0);
	versionId = query.value(0).toInt();
	curVersion = query.value(1).toInt();
	//qDebug() << "Версія поточної бази даних: " << curVersion;
	if (curVersion < USalary::dbVersion() ){ //Застаріла база даних
		int updatedToVersion = updateVersions();
		if (updatedToVersion == USalary::dbVersion()){
			QMessageBox::information(0,
				"!!! Поновлення бази !!!",
				"Базу даних \"Ель-Зар\" було успішно поновлено до версії: "+QString::number(updatedToVersion)+"."+
				(updateWarnings ? "\nАле при поновленні сталося кілька помилок, таким чином у деяких місцях програма може працювати некоректно":"" ),
				QMessageBox::Ok,
				QMessageBox::NoButton);
			return true;
		}
		else if (updatedToVersion == -1){
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! Помилка поновлення бази даних \"Ель-Зар\" !!!",
									"Програма не змогла поновити базу даних \"Ель-Зар\" через помилку системи.\n"
									"Робота з застарілою версією БД може призвести до некоректної роботи частини функцій програми.\n"
									"ПОРАДА! Спробуйте відкрити програму ще раз або звверніться до адміністратора.\n"
									"Якщо ви бажаєте продовжити роботу з програмою незважаючи на можливі помилки, клікніть \"Продовжити\".");
			mBox->addButton("Вийти", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("Продовжити", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
		else{
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! Помилка поновлення бази ПК \"Ель-Зар\" !!!",
									"Програма поновила БД до версії: "+QString::number(updatedToVersion)+",\nа для стабільної роботи програми необхідна база даних версії "+QString::number(USalary::dbVersion())+".\n"
									"Робота з застарілою версією БД може призвести до некоректної роботи частини функцій програми.\n"
									"ПОРАДА! Спробуйте відкрити програму ще раз або звверніться до адміністратора.\n"
									"Якщо ви бажаєте продовжити роботу з програмою незважаючи на можливі помилки, клікніть \"Продовжити\".");
			mBox->addButton("Вийти", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("Продовжити", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
	}
	else if (curVersion > USalary::dbVersion() ){ //Застаріла програма
		QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! Застаріла версія програми \"Ельзар\" !!!",
									"Завантажено застарілу версію програми \"Ель-Зар\" відносно робочої бази даних,\n"
									"що може призвести до некоректної роботи частини функцій програми.\n"
									"ПОРАДА! Завантажте нову версію програми \"Ель-Зар\".\n"
									"Якщо ви бажаєте продовжити роботу з програмою незважаючи на можливі помилки, клікніть \"Продовжити\".");
		mBox->addButton("Вийти", QMessageBox::ActionRole);
		QPushButton *contButton = mBox->addButton("Продовжити", QMessageBox::ActionRole);
		mBox->exec();
		if (mBox->clickedButton() == contButton)
			ver_ok = true;
		delete mBox;
	}
	else
		return true;
	return ver_ok;
}
//------------------------------------------------------------
int USalaryDbUpdater::updateVersions()
{
	int returnVersion = curVersion;
	
	updateWarnings = false;
	
	//Під'єднання до бази даних запитів
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSqlDatabase updateTkeDataBaseVerDB = QSqlDatabase::addDatabase("QSQLITE", "updateSalaryDataBaseVerDB" );
	updateTkeDataBaseVerDB.setDatabaseName("salaryDBUpdate");
	if (updateTkeDataBaseVerDB.open()){
		QSqlQuery updateQuery(updateTkeDataBaseVerDB);
		//Таблиця зміни структури
		updateQuery.exec(" \
CREATE TABLE IF NOT EXISTS updateFTFQuerys( \
	id INTEGER PRIMARY KEY AUTOINCREMENT, \
	Version INTEGER, \
	baseQuery TEXT, \
	trueQuery TEXT, \
	falseQuery TEXT, \
	failerQuery TEXT)");
		//Таблиця зміни даних
		updateQuery.exec(" \
CREATE TABLE IF NOT EXISTS updateNZPQuerys( \
	id INTEGER PRIMARY KEY AUTOINCREMENT, \
	Version INTEGER, \
	baseQuery TEXT, \
	positiveQuery TEXT, \
	negativeQuery TEXT, \
	zeroQuery TEXT)");
	}
	else{
		qDebug() << "Помилка. Не вдалося під'єднатися до бази даних salaryDBUpdate";
		return -1;
	}
	
	QSqlQuery updateQuery(updateTkeDataBaseVerDB);
	QSqlQuery versionsQuery(updateTkeDataBaseVerDB);
	QSqlQuery salaryQuery;
	
	versionsQuery.exec("SELECT version FROM updateFTFQuerys \
		WHERE version BETWEEN "+QString::number(curVersion+1)+" AND "+QString::number(USalary::dbVersion())+" \
		GROUP BY version");
	while (versionsQuery.next()){
		int updateVersion = versionsQuery.value(0).toInt();
		//Виконання запитів true-false-fail
		updateQuery.exec("SELECT * FROM updateFTFQuerys WHERE version="+QString::number(updateVersion)+" ORDER BY id");
		while (updateQuery.next()){
			int queryPos;
			if (salaryQuery.exec(updateQuery.value(2).toString()) && salaryQuery.seek(0)){
				if (salaryQuery.value(0).toBool()) //true query
					queryPos = 3;
				else //false query
					queryPos = 4;
			}
			else{ //fail Query
				queryPos = 5;
			}
			bool okQuery = salaryQuery.exec(updateQuery.value(queryPos).toString());
			if (!okQuery){
				updateWarnings = true;
				
				qDebug() << " --- Помилка поновлення FTF - id:" << updateQuery.value(0).toInt() << ";\n - query: " << updateQuery.value(queryPos).toString();
			}
		}
		
		//Виконання запитів negativ-zero-positiv (NZP)
		updateQuery.exec("SELECT * FROM updateNZPQuerys WHERE version="+QString::number(updateVersion)+" ORDER BY id");
		while (updateQuery.next()){
			int queryPos;
			if (salaryQuery.exec(updateQuery.value(2).toString()) && salaryQuery.seek(0)){
				double probValue = salaryQuery.value(0).toDouble();
				if (probValue>0.0000009)
					queryPos = 3;
				else if (probValue<-0.0000009)
					queryPos = 4;
				else //Нуль
					queryPos = 5;
			}
			else{ //fail Query
				queryPos = 5;
			}
			bool okQuery = salaryQuery.exec(updateQuery.value(queryPos).toString());
			if (!okQuery){
				updateWarnings = true;
				
				qDebug() << " --- Помилка поновлення NZP - id:" << updateQuery.value(0).toInt() << ";\n - query: " << updateQuery.value(queryPos).toString();
			}
		}
		
		salaryQuery.exec("UPDATE version_salary SET version="+sqlStr(updateVersion));
		returnVersion = updateVersion;
	}
	
	updateTkeDataBaseVerDB.close();
	QSqlDatabase::removeDatabase( "updateSalaryDataBaseVerDB" );
	
	return returnVersion;
}
//------------------------------------------------------------
