#include "../engine/USalary.h"
#include "uregistermanager.h"
#include <QSqlError>
#include <QSettings>
#include <QHash>
#include <QDir>
//--------------------utech--------------------utech--------------------utech--------------------
URegisterWindow::URegisterWindow(QWidget *parent):QDialog(parent)
{
	ui.setupUi(this);
	idGroup_ = 0;
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
	connect(ui.acceptPbtn,SIGNAL(clicked()),this,SLOT(verifyData()));
	connect(ui.cancelPbtn,SIGNAL(clicked()),this,SLOT(exit()));
	connect(ui.loginLe,SIGNAL(textEdited(const QString&)),this,SLOT(findText(const QString&)));
	connect(ui.listWidget,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(setDataInLineEdit(QListWidgetItem*)));
	connect(ui.informLbl,SIGNAL(linkActivated(const QString&)),this,SLOT(showHelp()));
	loadFirms();
}
//--------------------utech--------------------utech--------------------utech--------------------
int URegisterWindow::groupId()
{
	if(exec())
		return idGroup();
	else
		return -1;
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::loadFirms()
{
	disconnect(ui.pidprCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(loadFirmWorkers()));
	QSqlDatabase db = QSqlDatabase::addDatabase(driver,"slujb");
	db.setHostName(host);
	db.setUserName(user);
	db.setPassword(password);
	db.setPort(port);
	db.setDatabaseName("salaryslujbova");
	if(db.open()){
		QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("slujb"));
		query->exec("SELECT count(*) FROM sl_table");
		query->seek(0);
		int numFirms = query->value(0).toInt();
		query->exec("SELECT * FROM sl_table");
		if(numFirms>1)
		{
			dbNameHash.clear();
			int iter=0;
			while (query->next())
			{
				dbNameHash.insert(iter, query->value(2).toString());
				ui.pidprCombo->addItem(query->value(1).toString());
				iter++;
			}
			ui.pidprCombo->setCurrentIndex(lastFirmId-1);
			loadDatabase(dbNameHash[ui.pidprCombo->currentIndex()]);
		}
		else if (numFirms==1)
		{
			ui.pidprCombo->hide();
			ui.pidprLbl->hide();
			query->seek(0);
			loadDatabase(query->value(2).toString());
		}
		else{
			//Частина, яка виконується при помилці під'єднання до БД або неправильному налаштуванні програми
		}
		delete query;
	}
	QSqlDatabase::removeDatabase("slujb");
	connect(ui.pidprCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(loadFirmWorkers()));
}
//--------------------utech--------------------utech--------------------utech--------------------=
void URegisterWindow::findText(const QString text)
{
	Qt::MatchFlags flag = Qt::MatchFixedString|Qt::MatchStartsWith;
	QListWidgetItem *item;
	QList<QListWidgetItem *> found = ui.listWidget->findItems(text,flag);
	foreach(item, found)
	  ui.listWidget->setCurrentItem(item);
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::setDataInLineEdit(QListWidgetItem* currentItem)
{
	ui.loginLe->setText(currentItem->data(Qt::DisplayRole).toString());
	ui.passLe->setFocus();
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::verifyData()
{
	QSqlQuery verify;
	if(!ui.loginLe->text().isEmpty())
	{
		if(verify.exec(QString("SELECT Grupa_id FROM npr WHERE id = '%1' AND Password = '%2'")
		.arg(workers.key(ui.loginLe->text()))
		.arg(ui.passLe->text())))
		{
			verify.seek(0);
			if(verify.value(0).toString()== QString::null)
			{
				idGroup_ = -1;
				ui.informLbl->setText("Ви ввели неправильний пароль або логін!!!");
			}
			else
			{
				ui.informLbl->setText("Привіт, як життя?");
				idGroup_ = verify.value(0).toInt();
				accept();
			}
		}
		else
			idGroup_ = -1;
	}
	else
		ui.informLbl->setText("Введіть дані!");
};
//--------------------utech--------------------utech--------------------utech--------------------=======
void URegisterWindow::updateData()
{
	if(ui.listWidget->count()>=0)
		ui.listWidget->clear();
	QSqlQuery s;
	s.exec("select id, CONCAT_WS(' ',Prizv,Imia,Pobatk) from npr");
	while(s.next()){
		workers[s.value(0).toInt()] = s.value(1).toString();
		ui.listWidget->addItem(s.value(1).toString());
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::showHelp()
{
	QTextEdit* t = new QTextEdit;
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->setWindowModality(Qt::ApplicationModal);
	t->setPlainText("Ваші дії:\n \
  1.Перевірте мережу.\n \
  2.Попробуйте запустити сервер.\n \
  3.Увімкніть серверний комп'ютер і запустіть на ньому сам сервер. \n \
  4.Зверніться до адміністратора.");
	t->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::showError(const QString text)
{
	ui.groupBox->setEnabled(false);
	ui.groupBox_2->setEnabled(false);
	ui.acceptPbtn->setEnabled(false);
	ui.cancelPbtn->setEnabled(false);
	ui.informLbl->setText(QString("<html>"
"<body link=blue vlink=purple>"
"<p>При під'єднанні до бази даних сталася помилка "
" -%1- <span><a href=\"hj.html\">"
"Що робити далі?</a></span></p>"
"</body>"
"</html>").arg(text));
	ui.informLbl->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::setConnectionOptions(const QString hostName, const QString userName, const QString pass, const QString driverName)
{
	host  = hostName;
	user = userName;
	password = pass;
	driver = driverName;
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::loadDatabase(const QString databaseName)
{
	QSqlDatabase db = QSqlDatabase::addDatabase(driver);
	db.setHostName(host);
	db.setUserName(user);
	db.setPassword(password);
	db.setPort(port);
	db.setDatabaseName(databaseName);
	if(!db.open())
		showError(db.lastError().text());
	else
		updateData();
	QSettings *settings = new QSettings("./salary.ini", QSettings::IniFormat);
	settings->beginGroup ("Organizations");
	settings->setValue("lastLoaded",ui.pidprCombo->currentIndex()+1);
	lastFirmId = ui.pidprCombo->currentIndex()+1;
	delete settings;
}
//--------------------utech--------------------utech--------------------utech--------------------
void URegisterWindow::loadFirmWorkers()
{
	loadDatabase(dbNameHash[ui.pidprCombo->currentIndex()]);
}
//--------------------utech--------------------utech--------------------utech--------------------