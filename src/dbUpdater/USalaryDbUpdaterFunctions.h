//------------------------------------------------------------
//
// Declaration of dbUpdaterFunctions
// Created by Roman Mulyk 16.01.2009
//
//------------------------------------------------------------

#ifndef USALARYDBUPDATERFUNCTIONS_H
#define USALARYDBUPDATERFUNCTIONS_H

#include <QSqlQuery>
#include <QMessageBox>

//------------------------------------------------------------
class USalaryDbUpdater : public QObject
{
  public:
	USalaryDbUpdater();
	~USalaryDbUpdater();
	bool checkVersions();
	int updateVersions();
  private:
	int versionId;
	int curVersion;
	bool updateWarnings;
}; 
//------------------------------------------------------------
#endif
