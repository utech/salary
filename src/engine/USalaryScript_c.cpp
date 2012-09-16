//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу нарахування зарплати 
// з допомогою скриптів для програми нарахування зарплати
// Створений програмістами Компанії "Утех" 9.12.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "USalaryScript.h"

//--------------------utech--------------------utech--------------------utech--------------------
short UProgramOptions::TypIndexaciyi()
{
	QSqlQuery query;
	query.exec("SELECT Znach FROM progoptions WHERE id=7");
	if (query.seek(0))
		return query.value(0).toInt();
	else
		return -1;
}
//--------------------utech--------------------utech--------------------utech--------------------
short UProgramOptions::TypSocPilgy()
{
	QSqlQuery query;
	query.exec("SELECT Znach FROM progoptions WHERE id=8");
	if (query.seek(0))
		return query.value(0).toInt();
	else
		return -1;
}
//--------------------utech--------------------utech--------------------utech--------------------
