//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "QtDebug"

#include "UDialogLogin_salary.h"
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UDialogLogin_salary::UDialogLogin_salary(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(width(), height());
	//setAttribute(Qt::WA_DeleteOnClose, true);
	
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QSettings *settings = new QSettings("./salary.ini", QSettings::IniFormat);
	host = settings->value("Server/host").toString();
	user = settings->value("Server/user").toString();
	password = settings->value("Server/password").toString();
#ifdef SALARY_LOCAL
	port = USalaryLocal::MySqlPort();
#else
	port = settings->value("Server/port").toInt();
#endif
	driver = "QMYSQL";
	lastFirmId = settings->value("Organizations/lastLoaded").toInt();
	if (lastFirmId<1)
		lastFirmId=1;
	delete settings;
	
	loadFirms();
	
	connect(ui.pushButton_accept, SIGNAL(clicked ()), this, SLOT(acceptDialog()));
	connect(ui.pushButton_reject, SIGNAL(clicked ()), this, SLOT(reject()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogLogin_salary::acceptDialog()
{
	//Перевірка пароля
	QSqlQuery query(QSqlDatabase::database("slujb"));
	query.exec("SELECT true FROM sl_table \
				WHERE id="+sqlStr(ui.comboBox_pidpryemstva->itemData(ui.comboBox_pidpryemstva->currentIndex(),Qt::UserRole).toInt())+" \
					and password=md5("+sqlStr(ui.lineEdit_password->text())+")");
	if (!query.next()){
		QMessageBox::information(this, "Невірний пароль", "Ви внесли невірний пароль, спробуйте ще.");
		return;
	}
	//Під'єднання до бази даних
	QString databaseName = dbNameHash.value(ui.comboBox_pidpryemstva->itemData(ui.comboBox_pidpryemstva->currentIndex(), Qt::UserRole).toInt());
	QSqlDatabase db = QSqlDatabase::addDatabase(driver);
	db.setHostName(host);
	db.setUserName(user);
	db.setPassword(password);
	db.setPort(port);
	db.setDatabaseName(databaseName);
	if(db.open()){
		QSettings *settings = new QSettings("./salary.ini", QSettings::IniFormat);
		settings->beginGroup("Organizations");
		lastFirmId = ui.comboBox_pidpryemstva->itemData(ui.comboBox_pidpryemstva->currentIndex(), Qt::UserRole).toInt();
		settings->setValue("lastLoaded",lastFirmId);
		delete settings;
		QSqlDatabase::removeDatabase("slujb");
		accept();
	}
	else
		QMessageBox::critical(this, "Помилка під'єднання",
			"При під'єднанні до бази даних "+databaseName+" сталася помилка.\nОпис: "+db.lastError().text());
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogLogin_salary::loadFirms()
{
	QSqlDatabase db = QSqlDatabase::addDatabase(driver,"slujb");
	db.setHostName(host);
	db.setUserName(user);
	db.setPassword(password);
	db.setPort(port);
	db.setDatabaseName("salaryslujbova");
	if(db.open()){
		QSqlQuery query(QSqlDatabase::database("slujb"));
		query.exec("SELECT id, pidprname, db FROM sl_table");
		dbNameHash.clear();
		ui.comboBox_pidpryemstva->clear();
		while (query.next())
		{
			dbNameHash.insert(query.value(0).toInt(), query.value(2).toString());
			ui.comboBox_pidpryemstva->addItem(query.value(1).toString(), query.value(0));
			if (lastFirmId == query.value(0).toInt())
				ui.comboBox_pidpryemstva->setCurrentIndex(ui.comboBox_pidpryemstva->count()-1);
		}
		if (dbNameHash.count() < 2)
			ui.comboBox_pidpryemstva->setEnabled(false);
	}
	
	ui.lineEdit_password->setFocus();
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogLogin_salary::~UDialogLogin_salary()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
