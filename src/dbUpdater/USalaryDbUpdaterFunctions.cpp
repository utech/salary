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
	//qDebug() << "����� ������� ���� �����: " << curVersion;
	if (curVersion < USalary::dbVersion() ){ //�������� ���� �����
		int updatedToVersion = updateVersions();
		if (updatedToVersion == USalary::dbVersion()){
			QMessageBox::information(0,
				"!!! ���������� ���� !!!",
				"���� ����� \"���-���\" ���� ������ ��������� �� ����: "+QString::number(updatedToVersion)+"."+
				(updateWarnings ? "\n��� ��� ��������� ������� ����� �������, ����� ����� � ������ ����� �������� ���� ��������� ����������":"" ),
				QMessageBox::Ok,
				QMessageBox::NoButton);
			return true;
		}
		else if (updatedToVersion == -1){
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! ������� ���������� ���� ����� \"���-���\" !!!",
									"�������� �� ������ �������� ���� ����� \"���-���\" ����� ������� �������.\n"
									"������ � ��������� ����� �� ���� ��������� �� ���������� ������ ������� ������� ��������.\n"
									"������! ��������� ������� �������� �� ��� ��� ���������� �� ������������.\n"
									"���� �� ������ ���������� ������ � ��������� ���������� �� ������ �������, ������ \"����������\".");
			mBox->addButton("�����", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("����������", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
		else{
			QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! ������� ���������� ���� �� \"���-���\" !!!",
									"�������� �������� �� �� ����: "+QString::number(updatedToVersion)+",\n� ��� �������� ������ �������� ��������� ���� ����� ���� "+QString::number(USalary::dbVersion())+".\n"
									"������ � ��������� ����� �� ���� ��������� �� ���������� ������ ������� ������� ��������.\n"
									"������! ��������� ������� �������� �� ��� ��� ���������� �� ������������.\n"
									"���� �� ������ ���������� ������ � ��������� ���������� �� ������ �������, ������ \"����������\".");
			mBox->addButton("�����", QMessageBox::ActionRole);
			QPushButton *contButton = mBox->addButton("����������", QMessageBox::ActionRole);
			mBox->exec();
			if (mBox->clickedButton() == contButton){
				ver_ok = true;
			}
			delete mBox;
		}
	}
	else if (curVersion > USalary::dbVersion() ){ //�������� ��������
		QMessageBox * mBox = new QMessageBox(QMessageBox::Critical, "!!! �������� ����� �������� \"������\" !!!",
									"����������� �������� ����� �������� \"���-���\" ������� ������ ���� �����,\n"
									"�� ���� ��������� �� ���������� ������ ������� ������� ��������.\n"
									"������! ���������� ���� ����� �������� \"���-���\".\n"
									"���� �� ������ ���������� ������ � ��������� ���������� �� ������ �������, ������ \"����������\".");
		mBox->addButton("�����", QMessageBox::ActionRole);
		QPushButton *contButton = mBox->addButton("����������", QMessageBox::ActionRole);
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
	
	//ϳ�'������� �� ���� ����� ������
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSqlDatabase updateTkeDataBaseVerDB = QSqlDatabase::addDatabase("QSQLITE", "updateSalaryDataBaseVerDB" );
	updateTkeDataBaseVerDB.setDatabaseName("salaryDBUpdate");
	if (updateTkeDataBaseVerDB.open()){
		QSqlQuery updateQuery(updateTkeDataBaseVerDB);
		//������� ���� ���������
		updateQuery.exec(" \
CREATE TABLE IF NOT EXISTS updateFTFQuerys( \
	id INTEGER PRIMARY KEY AUTOINCREMENT, \
	Version INTEGER, \
	baseQuery TEXT, \
	trueQuery TEXT, \
	falseQuery TEXT, \
	failerQuery TEXT)");
		//������� ���� �����
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
		qDebug() << "�������. �� ������� ��'�������� �� ���� ����� salaryDBUpdate";
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
		//��������� ������ true-false-fail
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
				
				qDebug() << " --- ������� ���������� FTF - id:" << updateQuery.value(0).toInt() << ";\n - query: " << updateQuery.value(queryPos).toString();
			}
		}
		
		//��������� ������ negativ-zero-positiv (NZP)
		updateQuery.exec("SELECT * FROM updateNZPQuerys WHERE version="+QString::number(updateVersion)+" ORDER BY id");
		while (updateQuery.next()){
			int queryPos;
			if (salaryQuery.exec(updateQuery.value(2).toString()) && salaryQuery.seek(0)){
				double probValue = salaryQuery.value(0).toDouble();
				if (probValue>0.0000009)
					queryPos = 3;
				else if (probValue<-0.0000009)
					queryPos = 4;
				else //����
					queryPos = 5;
			}
			else{ //fail Query
				queryPos = 5;
			}
			bool okQuery = salaryQuery.exec(updateQuery.value(queryPos).toString());
			if (!okQuery){
				updateWarnings = true;
				
				qDebug() << " --- ������� ���������� NZP - id:" << updateQuery.value(0).toInt() << ";\n - query: " << updateQuery.value(queryPos).toString();
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
