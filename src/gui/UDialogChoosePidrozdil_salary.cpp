//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "QtDebug"

#include "UDialogChoosePidrozdil_salary.h"
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UDialogChoosePidrozdil_salary::UDialogChoosePidrozdil_salary(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(width(), height());
	
	QSqlQuery query;
	query.exec("SELECT id, Name FROM pidrozdily_salary ORDER BY Name");
	ui.comboBox_pidrozdily->addItem(" --- УСІ ПІДРОЗДІЛИ ---", query.value(0));
	while (query.next())
		ui.comboBox_pidrozdily->addItem(query.value(1).toString(), query.value(0));
	
	connect(ui.pushButton_accept, SIGNAL(clicked ()), this, SLOT(accept()));
	connect(ui.pushButton_reject, SIGNAL(clicked ()), this, SLOT(reject()));
}
//--------------------utech--------------------utech--------------------utech--------------------
int UDialogChoosePidrozdil_salary::pidrozdilId()
{
	return ui.comboBox_pidrozdily->itemData(ui.comboBox_pidrozdily->currentIndex(), Qt::UserRole).toInt();
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogChoosePidrozdil_salary::~UDialogChoosePidrozdil_salary()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
