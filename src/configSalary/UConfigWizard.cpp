//--------------------utech--------------------utech--------------------utech----------------------------------------------------------------
//
// Copyright (C) 2005-2007 Utech Company. All rights reserved.
//
// Файл визначення класу головного вікна програми нарахування зарплати
//
// Створений програмістами Компанії "Утех" 29.11.2007р.
// Використані компоненти бібліотеки QT4.3.1
//
//--------------------utech--------------------utech--------------------utech----------------------------------------------------------------
#include <UConfigWizard.h>
#include "delegates.h"
#include <QFile>
#include <QTextStream>
#include <QtDebug>

short sproba;

//--------------------utech--------------------utech--------------------utech--------------------
UConfigWizard::UConfigWizard()
{
	setWindowTitle("Налаштування програми"); 
	//setWindowIcon(QIcon(QString::fromUtf8("")));
	
	addPage(new UInfoPage);
	addPage(new UServerSettingsPage);
	UNumOrgPage *numOrgPage = new UNumOrgPage();
	addPage(numOrgPage);
	UAcceptPage *acceptPage = new UAcceptPage;
	table = numOrgPage->orgTable();
	acceptPage->setOrgTable(table);
	textEdit = acceptPage->resTextEdit();
	addPage(acceptPage);

	setOptions(options() | QWizard::IndependentPages);
	//setStartId(Page_Info);
	
	setButtonText(QWizard::BackButton,"Назад");
	setButtonText(QWizard::NextButton,"Вперед");
	setButtonText(QWizard::FinishButton,"Виконати");
	setButtonText(QWizard::CancelButton,"Відмінити");
	setButtonText(QWizard::HelpButton,"Допомога");
	
	setOption(HaveHelpButton, true);
	setOptions(QWizard::NoBackButtonOnStartPage);
	
	setPixmap(QWizard::LogoPixmap, QPixmap(":/pictures/pictures/utech_120_80.png"));
	connect(this, SIGNAL(helpRequested()), this, SLOT(showHelp()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UConfigWizard::showHelp()
{
	QString message;
	switch (currentId()) 
	{
    case 0:
      message = "Ця сторінка призначена для введення шляхів до баз даних, \n"
                "Ви можете ввести їх вручну або вибравши потрібні файли з діалогу. ";
    break;
    case 1:
      message = "Ця сторінка призначена для збереження ідентифікатора підприємства,\n"
					"що необхідно для коректної роботи програми. Виберіть Ваше підприємство із списку\n"
					"і натисніть кнопку \"Вперед\".";
    break;
    case 2:
      message = "На цій сторінці відображаються зроблені Вами настойки. Ви можете змінити їх, \n"
				"натиснувши кнопку \"Назад\", або підтвердити, натиснувши кнопку \"Підтвердити\".";
    break;
    default:
        message = "Це невеличка допомога, яка призначена для полегшення настройки програми.";
	}

	QMessageBox::information(this, "Допомога", message);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UConfigWizard::accept()
{
	qDebug() << "void UConfigWizard::accept() loaded";
	QSqlDatabase db = QSqlDatabase::database("slujb");
	QString dbName, pidprName;
	bool doneOk=true, doneOkAll=true;
	if (sproba>1)
		textEdit->append("\n");
	textEdit->append("--- Спроба налаштування програми нарахування зарплати-"+QVariant(sproba).toString()+" ---");
	sproba++;
	
	QSettings *settings = new QSettings("./salary.ini", QSettings::IniFormat);
	if (settings->isWritable()){
		settings->beginGroup ("Server");
		settings->setValue("user", field("serverLogin_field").toString());
		settings->setValue("password", field("serverPassword_field").toString());
		settings->setValue("host", field("serverHost_field").toString());
		textEdit->append("OK-> Запис параметрів під'єднання до бази даних.");
	}
	else{
		textEdit->append("ER-> Помилка запис параметрів під'єднання до бази даних. Програма не змогла записати файл salary.ini");
		doneOk = false;
	}
	delete settings;

	
	if(!db.isOpen()){
		textEdit->append("ER-> Немає під'єднання до бази даних !!!");
		return;
	}
	else{
		QSqlQuery query(db);
		textEdit->append("ОК-> Підготовка до створення баз даних.");
		QFile data("../sql/salary_db.sql");
		if(!data.open(QIODevice::ReadOnly|QIODevice::Text)){
			textEdit->append("ER-> Не можу знайти файл з скриптом створення бази даних.\nПереконайтесь, що він у папці ../sql/ відносно configSalary.exe і називається salary_db.sql.");
			return;
		}
		else{
			textEdit->append("OK-> Зчитування файлу бази даних.");
			QString allData;
			QTextStream readStream(&data);
			allData = readStream.readAll();
			query.exec("DELETE FROM sl_table");
			query.exec("ALTER TABLE sl_table AUTO_INCREMENT=1");
			for(int iter=0; iter<table->rowCount(); iter++){
				doneOk=true;
				dbName = table->item(iter,2)->text();
				pidprName = table->item(iter,1)->text();
				pidprName = pidprName.replace('\'', '\"');
				bool needDelete = (static_cast<QCheckBox*>(table->cellWidget(iter, 3)))->isChecked();
				bool needCreate = (static_cast<QCheckBox*>(table->cellWidget(iter, 4)))->isChecked();
				bool needSet = (static_cast<QCheckBox*>(table->cellWidget(iter, 5)))->isChecked();
				textEdit->append(" OK-> Налаштування бази даних /"+dbName+"/ підприємства /"+pidprName+"/.");
				if(needDelete){
					if(!query.exec("DROP DATABASE "+dbName)){
						textEdit->append("   ER-> Не можу видалити базу даних "+dbName);
						doneOk=false;
					}
					else
						textEdit->append("   OK-> База даних "+dbName+" успішно видалена.");
				}
				if(needCreate && needSet){
					if(!query.exec("CREATE DATABASE "+dbName)){
						textEdit->append("   ER-> База даних "+dbName+" не створена. Можливо вона вже існує або не запущений сервер.");
						doneOk=false;
					}
					else{
						textEdit->append("   OK-> Створення бази даних "+dbName);
						QSqlDatabase dbCr = QSqlDatabase::addDatabase("QMYSQL");
						dbCr.setHostName(field("serverHost_field").toString());
						dbCr.setDatabaseName(dbName);
						dbCr.setUserName(field("serverLogin_field").toString());
						dbCr.setPassword(field("serverPassword_field").toString());
						dbCr.open();
						QSqlQuery queryCr(dbCr);
						if (queryCr.exec(allData))
							textEdit->append("   OK-> База даних "+dbName+" успішно створена");
						else{
							textEdit->append("   ER-> Програма не змогла створити базу баних "+dbName+".");
							textEdit->append("   ER-> Причина помилки -  "+queryCr.lastError().text()+".");
							doneOk=false;
						}
					}
				//create current database
				}
				if(needSet && ((!needDelete) || needCreate)){
					if (query.exec("INSERT INTO sl_table (pidprname, db) VALUES ('"+pidprName+"','"+dbName+"')")){
						textEdit->append(" OK-> База даних /"+dbName+"/ підприємства /"+pidprName+"/ успішно налаштована.");
					}
					else{
						textEdit->append(" ER-> База даних /"+dbName+"/ підприємства /"+pidprName+"/ не налаштована коректно.");
						textEdit->append(" ER-> Причина помилки -  "+query.lastError().text()+".");
						doneOk=false;
					}
				}
				if ((!needSet) && (!needDelete)){
					textEdit->append(" INF-> База даних /"+dbName+"/ підприємства /"+pidprName+"/ за Вашою вказівкою ігнорується.");
				}
				doneOkAll = doneOkAll && doneOk;
			}
		}
	}
	if (doneOkAll){
		textEdit->append(" --- Налаштування програми успішно виконано ---");
		QMessageBox::information(this, "Увага!!!",
				"Операція по настроюванню даних \n"
                   " була успішно виконана.",
				QMessageBox::Ok );
		QDialog::accept();
	}
	else
		textEdit->append(" --- Налаштування програми пройшло з можливими помилками ---");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UConfigWizard::reject()
{
	QDialog::reject();
}
//--------------------utech--------------------utech--------------------utech--------------------
UInfoPage::UInfoPage(QWidget *parent)
	: QWizardPage(parent)
{
	qDebug() << " - UInfoPage begins";
	setTitle("Інформація");
	setSubTitle("Коротка інформація про дану утилітку");

	QLabel *label = new QLabel("Цей помічник допоможе налаштувати Вашу програму нарахування зарплати");
	label->setWordWrap(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	setLayout(layout);
	
	qDebug() << " - UInfoPage ends";
}
//--------------------utech--------------------utech--------------------utech--------------------

UServerSettingsPage::UServerSettingsPage(QWidget *parent)
	: QWizardPage(parent)
{
	qDebug() << " - UServerSettingsPage begins";
	
	setTitle("Параметри під'єднання до сервера MySQL");
	setSubTitle("Внесення користувацьких даних під'єднання до MySQL сервера");
	
	labelLogin = new QLabel("Логін",this);
	labelPassWord = new QLabel("Пароль",this);
	labelHost = new QLabel("Ім'я хоста",this);
	
	lineEditLogin = new QLineEdit(this);
	lineEditPassWord = new QLineEdit(this);
	lineEditPassWord->setEchoMode(QLineEdit::Password);
	lineEditHost = new QLineEdit(this);
	
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(labelLogin,0,0);
	gridLayout->addWidget(lineEditLogin,0,1);
	
	gridLayout->addWidget(labelPassWord,1,0);
	gridLayout->addWidget(lineEditPassWord,1,1);

	gridLayout->addWidget(labelHost,2,0);
	gridLayout->addWidget(lineEditHost,2,1);
	
	registerField("serverLogin_field",lineEditLogin);
	setField("serverLogin_field",QVariant(lineEditLogin->text()));
	registerField("serverPassword_field",lineEditPassWord);
	setField("serverPassword_field",QVariant(lineEditPassWord->text()));
	registerField("serverHost_field",lineEditHost);
	setField("serverHost_field",QVariant(lineEditHost->text()));
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(gridLayout);
	mainLayout->addStretch();
	
	setLayout(mainLayout);
	qDebug() << " - UServerSettingsPage ends";
}
//--------------------------------------------------
void UServerSettingsPage::initializePage()
{
	QSettings settings ("./salary.ini", QSettings::IniFormat);
	lineEditLogin->setText(settings.value("Server/user").toString());
	lineEditPassWord->setText(settings.value("Server/password").toString());
	if(settings.value("Server/host").toString().isEmpty())
		lineEditHost->setText("localhost");
	else
		lineEditHost->setText(settings.value("Server/host").toString());
}
//--------------------utech--------------------utech--------------------utech--------------------

UNumOrgPage::UNumOrgPage(QWidget *parent)
	: QWizardPage(parent)
{
	qDebug() << " - UNumOrgPage begins";
	setTitle("Організації і їх бази даних");
	setSubTitle("Внесення і видалення з програми організацій");
	
	table = new QTableWidget(0,6,this);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	QStringList list;
	list 	<< "Стан" 
			<< "Коротка назва підприємства" 
			<< "БД підприємства" 
			<< "Видал.БД"
			<< "Створ.БД"
			<< "Налашт.БД";
	table->setHorizontalHeaderLabels(list);
	table->setColumnHidden(0,true);
	table->resizeColumnsToContents();
	list.clear();
	
	buttonAdd = new QPushButton("Додати",this);
	
	QVBoxLayout *vLayout = new QVBoxLayout(this);
	vLayout->setMargin(0);
	vLayout->addWidget(table);
	
	QHBoxLayout *hLayout = new QHBoxLayout(this);
	hLayout->addStretch();
	hLayout->addWidget(buttonAdd);
	
	vLayout->addLayout(hLayout);
	
	setLayout(vLayout);
		
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addPidprStr()));
	qDebug() << " - UNumOrgPage ends";
}
//--------------------utech--------------------utech--------------------utech--------------------
void UNumOrgPage::initializePage()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","slujb");
	db.setHostName(field("serverHost_field").toString());
	db.setUserName(field("serverLogin_field").toString());
	db.setPassword(field("serverPassword_field").toString());
	if(db.open()){
		QSqlQuery *tempQuery = new QSqlQuery(db);
		tempQuery->exec("CREATE DATABASE IF NOT EXISTS salarySlujbova");
		delete tempQuery;
		db.close();
	}
	//db.setHostName(field("serverHost_field").toString());
	db.setDatabaseName("salarySlujbova");
	//db.setUserName(field("serverLogin_field").toString());
	//db.setPassword(field("serverPassword_field").toString());
	if(db.open()){
		QSqlQuery *query = new QSqlQuery(db);
		query->exec("CREATE TABLE IF NOT EXISTS sl_table(id INT UNSIGNED \
						NOT NULL AUTO_INCREMENT,  pidprname varchar(90),db varchar(40), \
					PRIMARY KEY(id))");
		
		table->setRowCount(0);
		
		query->exec("SELECT * FROM sl_table");
		int row=0;
		while (query->next()){
			table->setRowCount(table->rowCount()+1);
			table->setItem(row,0,new QTableWidgetItem(query->value(0).toString()));
			table->setItem(row,1,new QTableWidgetItem(query->value(1).toString()));
			table->setItem(row,2,new QTableWidgetItem(query->value(2).toString()));
			table->setCellWidget(row,3,new QCheckBox("видал."));
			table->setCellWidget(row,4,new QCheckBox("створ."));
			QCheckBox *chBox = new QCheckBox("налашт.");
			chBox->setChecked(true);
			table->setCellWidget(row,5,chBox);
			row++;
		}
		delete query;
	}
	else{
		QMessageBox::information(0,"Увага!!!","Програма не змогла під'єднатись до сервера.\nВведіть правильні дані або перевірте чи запущено сервер.");
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UNumOrgPage::addPidprStr()
{
	int row=table->rowCount();
	table->setRowCount(row+1);
	table->setItem(row,0,new QTableWidgetItem("0"));
	table->setItem(row,1,new QTableWidgetItem("Скорочена назва організації"));
	table->setItem(row,2,new QTableWidgetItem("salaryDBPidprName"));
	table->setCellWidget(row,3,new QCheckBox("видал."));
	table->setCellWidget(row,4,new QCheckBox("створ."));
	QCheckBox *chBox = new QCheckBox("налашт.");
	chBox->setChecked(true);
	table->setCellWidget(row,5,chBox);
	table->setCurrentCell(row,1);
}
//--------------------utech--------------------utech--------------------utech--------------------
QTableWidget *UNumOrgPage::orgTable()
{
	return table;
}
//--------------------utech--------------------utech--------------------utech--------------------

UAcceptPage::UAcceptPage(QWidget *parent)
	: QWizardPage(parent)
{
	qDebug() << " - UAcceptPage begins";
	setTitle("Налаштування програми");
	setSubTitle("Загальна інформація про налаштування програми");
	
	textEdit = new QTextEdit;
	textEdit->setReadOnly(true);
	QVBoxLayout *vLayout = new QVBoxLayout(this);
	vLayout->setMargin(0);
	vLayout->addWidget(textEdit);
		
	setLayout(vLayout);
	qDebug() << " - UAcceptPage ends";
}
//--------------------utech--------------------utech--------------------utech--------------------
void UAcceptPage::initializePage()
{
	qDebug() << "void UAcceptPage::initializePage() loaded";
	textEdit->clear();
	sproba = 1;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UAcceptPage::setOrgTable(QTableWidget *orgTable)
{
	table = orgTable;
	
}
//--------------------utech--------------------utech--------------------utech--------------------
