//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "QtDebug"

#include "UDialogFirmInfo.h"
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UDialogFirmInfo::UDialogFirmInfo(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	
	setMaximumHeight(height());
	setMinimumSize(width(), height());
	
	populateOrgInfo();
	
	connect(ui.pushButton_accept, SIGNAL(clicked ()), this, SLOT(acceptDialog()));
	connect(ui.pushButton_saveOnly, SIGNAL(clicked ()), this, SLOT(onlySaveInfo()));
	connect(ui.pushButton_reject, SIGNAL(clicked ()), this, SLOT(reject()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogFirmInfo::acceptDialog()
{
	if (saveFirmInfo())
		accept();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogFirmInfo::onlySaveInfo()
{
	saveFirmInfo();
}
//--------------------utech--------------------utech--------------------utech--------------------
bool UDialogFirmInfo::saveFirmInfo()
{
	bool doneOk;
	QSqlQuery query;
	query.exec("SELECT count(*) FROM orgInfo WHERE PidprId=1");
	query.seek(0);
	if (query.value(0).toInt() < 0){
		query.exec("INSERT INTO `orgInfo` (PidprId) VALUES (1)");
	}
	doneOk = query.exec("UPDATE orgInfo SET \
							PovnaNazvaPidpr="+mysqlStr(ui.lineEdit_povnaNazvaPidpr->text().replace('\'','\"'))+", \
							ScorochNazvaPidpr="+mysqlStr(ui.lineEdit_scorochNazvaPidpr->text().replace('\'','\"'))+", \
							Edrpoy="+mysqlStr(ui.lineEdit_edrpoy->text().replace('\'','\"'))+", \
							Adresa="+mysqlStr(ui.lineEdit_adresa->text().replace('\'','\"'))+", \
							PipDurect="+mysqlStr(ui.lineEdit_PipDurect->text().replace('\'','\"'))+", \
							PipBuh="+mysqlStr(ui.lineEdit_PipBuh->text().replace('\'','\"'))+", \
							TelefonDurekt="+mysqlStr(ui.lineEdit_telefonDurekt->text().replace('\'','\"'))+", \
							TelefonBuh="+mysqlStr(ui.lineEdit_telefonBuh->text().replace('\'','\"'))+", \
							Email="+mysqlStr(ui.lineEdit_email->text().replace('\'','\"'))+",  \
							Fax="+mysqlStr(ui.lineEdit_fax->text().replace('\'','\"'))+", \
							Bank="+mysqlStr(ui.lineEdit_bank->text().replace('\'','\"'))+", \
							MFO="+mysqlStr(ui.lineEdit_MFO->text().replace('\'','\"'))+", \
							BankRahunok="+mysqlStr(ui.lineEdit_rahunok->text().replace('\'','\"'))+" \
						WHERE PidprId=1 LIMIT 1");
	if (doneOk)
		QMessageBox::information(this,"!!! Успішна зміна !!!","Інформацію про підприємство успішно змінено.");
	else
		QMessageBox::critical(this,"!!! Невдала зміна запису !!!","Програма не змогла змінити дані про підприємство.");
	
	if (doneOk)
		populateOrgInfo();
	return doneOk;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogFirmInfo::populateOrgInfo()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT * FROM orgInfo LIMIT 1");
	query->seek(0);
	QSqlRecord rec = query->record();
	
	ui.lineEdit_povnaNazvaPidpr->setText(rec.value("PovnaNazvaPidpr").toString());
	ui.lineEdit_scorochNazvaPidpr->setText(rec.value("ScorochNazvaPidpr").toString());
	ui.lineEdit_edrpoy->setText(rec.value("Edrpoy").toString());
	ui.lineEdit_adresa->setText(rec.value("Adresa").toString());
	ui.lineEdit_PipDurect->setText(rec.value("PipDurect").toString());
	ui.lineEdit_PipBuh->setText(rec.value("PipBuh").toString());
	ui.lineEdit_telefonDurekt->setText(rec.value("TelefonDurekt").toString());
	ui.lineEdit_telefonBuh->setText(rec.value("TelefonBuh").toString());
	ui.lineEdit_email->setText(rec.value("Email").toString());
	ui.lineEdit_fax->setText(rec.value("Fax").toString());
	ui.lineEdit_bank->setText(rec.value("Bank").toString());
	ui.lineEdit_MFO->setText(rec.value("MFO").toString());
	ui.lineEdit_rahunok->setText(rec.value("BankRahunok").toString());
	
	delete query;
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogFirmInfo::~UDialogFirmInfo()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
