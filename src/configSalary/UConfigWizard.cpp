//--------------------utech--------------------utech--------------------utech----------------------------------------------------------------
//
// Copyright (C) 2005-2007 Utech Company. All rights reserved.
//
// ���� ���������� ����� ��������� ���� �������� ����������� ��������
//
// ��������� ������������ ������ "����" 29.11.2007�.
// ���������� ���������� �������� QT4.3.1
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
	setWindowTitle("������������ ��������"); 
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
	
	setButtonText(QWizard::BackButton,"�����");
	setButtonText(QWizard::NextButton,"������");
	setButtonText(QWizard::FinishButton,"��������");
	setButtonText(QWizard::CancelButton,"³������");
	setButtonText(QWizard::HelpButton,"��������");
	
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
      message = "�� ������� ���������� ��� �������� ������ �� ��� �����, \n"
                "�� ������ ������ �� ������ ��� �������� ������ ����� � ������. ";
    break;
    case 1:
      message = "�� ������� ���������� ��� ���������� �������������� ����������,\n"
					"�� ��������� ��� �������� ������ ��������. ������� ���� ���������� �� ������\n"
					"� �������� ������ \"������\".";
    break;
    case 2:
      message = "�� ��� ������� ������������� ������� ���� ��������. �� ������ ������ ��, \n"
				"���������� ������ \"�����\", ��� ����������, ���������� ������ \"ϳ���������\".";
    break;
    default:
        message = "�� ��������� ��������, ��� ���������� ��� ���������� ��������� ��������.";
	}

	QMessageBox::information(this, "��������", message);
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
	textEdit->append("--- ������ ������������ �������� ����������� ��������-"+QVariant(sproba).toString()+" ---");
	sproba++;
	
	QSettings *settings = new QSettings("./salary.ini", QSettings::IniFormat);
	if (settings->isWritable()){
		settings->beginGroup ("Server");
		settings->setValue("user", field("serverLogin_field").toString());
		settings->setValue("password", field("serverPassword_field").toString());
		settings->setValue("host", field("serverHost_field").toString());
		textEdit->append("OK-> ����� ��������� ��'������� �� ���� �����.");
	}
	else{
		textEdit->append("ER-> ������� ����� ��������� ��'������� �� ���� �����. �������� �� ������ �������� ���� salary.ini");
		doneOk = false;
	}
	delete settings;

	
	if(!db.isOpen()){
		textEdit->append("ER-> ���� ��'������� �� ���� ����� !!!");
		return;
	}
	else{
		QSqlQuery query(db);
		textEdit->append("��-> ϳ�������� �� ��������� ��� �����.");
		QFile data("../sql/salary_db.sql");
		if(!data.open(QIODevice::ReadOnly|QIODevice::Text)){
			textEdit->append("ER-> �� ���� ������ ���� � �������� ��������� ���� �����.\n�������������, �� �� � ����� ../sql/ ������� configSalary.exe � ���������� salary_db.sql.");
			return;
		}
		else{
			textEdit->append("OK-> ���������� ����� ���� �����.");
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
				textEdit->append(" OK-> ������������ ���� ����� /"+dbName+"/ ���������� /"+pidprName+"/.");
				if(needDelete){
					if(!query.exec("DROP DATABASE "+dbName)){
						textEdit->append("   ER-> �� ���� �������� ���� ����� "+dbName);
						doneOk=false;
					}
					else
						textEdit->append("   OK-> ���� ����� "+dbName+" ������ ��������.");
				}
				if(needCreate && needSet){
					if(!query.exec("CREATE DATABASE "+dbName)){
						textEdit->append("   ER-> ���� ����� "+dbName+" �� ��������. ������� ���� ��� ���� ��� �� ��������� ������.");
						doneOk=false;
					}
					else{
						textEdit->append("   OK-> ��������� ���� ����� "+dbName);
						QSqlDatabase dbCr = QSqlDatabase::addDatabase("QMYSQL");
						dbCr.setHostName(field("serverHost_field").toString());
						dbCr.setDatabaseName(dbName);
						dbCr.setUserName(field("serverLogin_field").toString());
						dbCr.setPassword(field("serverPassword_field").toString());
						dbCr.open();
						QSqlQuery queryCr(dbCr);
						if (queryCr.exec(allData))
							textEdit->append("   OK-> ���� ����� "+dbName+" ������ ��������");
						else{
							textEdit->append("   ER-> �������� �� ������ �������� ���� ����� "+dbName+".");
							textEdit->append("   ER-> ������� ������� -  "+queryCr.lastError().text()+".");
							doneOk=false;
						}
					}
				//create current database
				}
				if(needSet && ((!needDelete) || needCreate)){
					if (query.exec("INSERT INTO sl_table (pidprname, db) VALUES ('"+pidprName+"','"+dbName+"')")){
						textEdit->append(" OK-> ���� ����� /"+dbName+"/ ���������� /"+pidprName+"/ ������ �����������.");
					}
					else{
						textEdit->append(" ER-> ���� ����� /"+dbName+"/ ���������� /"+pidprName+"/ �� ����������� ��������.");
						textEdit->append(" ER-> ������� ������� -  "+query.lastError().text()+".");
						doneOk=false;
					}
				}
				if ((!needSet) && (!needDelete)){
					textEdit->append(" INF-> ���� ����� /"+dbName+"/ ���������� /"+pidprName+"/ �� ����� �������� ����������.");
				}
				doneOkAll = doneOkAll && doneOk;
			}
		}
	}
	if (doneOkAll){
		textEdit->append(" --- ������������ �������� ������ �������� ---");
		QMessageBox::information(this, "�����!!!",
				"�������� �� ������������ ����� \n"
                   " ���� ������ ��������.",
				QMessageBox::Ok );
		QDialog::accept();
	}
	else
		textEdit->append(" --- ������������ �������� ������� � ��������� ��������� ---");
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
	setTitle("����������");
	setSubTitle("������� ���������� ��� ���� �������");

	QLabel *label = new QLabel("��� ������� �������� ����������� ���� �������� ����������� ��������");
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
	
	setTitle("��������� ��'������� �� ������� MySQL");
	setSubTitle("�������� �������������� ����� ��'������� �� MySQL �������");
	
	labelLogin = new QLabel("����",this);
	labelPassWord = new QLabel("������",this);
	labelHost = new QLabel("��'� �����",this);
	
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
	setTitle("���������� � �� ���� �����");
	setSubTitle("�������� � ��������� � �������� ����������");
	
	table = new QTableWidget(0,6,this);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	QStringList list;
	list 	<< "����" 
			<< "������� ����� ����������" 
			<< "�� ����������" 
			<< "�����.��"
			<< "�����.��"
			<< "������.��";
	table->setHorizontalHeaderLabels(list);
	table->setColumnHidden(0,true);
	table->resizeColumnsToContents();
	list.clear();
	
	buttonAdd = new QPushButton("������",this);
	
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
			table->setCellWidget(row,3,new QCheckBox("�����."));
			table->setCellWidget(row,4,new QCheckBox("�����."));
			QCheckBox *chBox = new QCheckBox("������.");
			chBox->setChecked(true);
			table->setCellWidget(row,5,chBox);
			row++;
		}
		delete query;
	}
	else{
		QMessageBox::information(0,"�����!!!","�������� �� ������ ��'�������� �� �������.\n������ �������� ��� ��� �������� �� �������� ������.");
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UNumOrgPage::addPidprStr()
{
	int row=table->rowCount();
	table->setRowCount(row+1);
	table->setItem(row,0,new QTableWidgetItem("0"));
	table->setItem(row,1,new QTableWidgetItem("��������� ����� ����������"));
	table->setItem(row,2,new QTableWidgetItem("salaryDBPidprName"));
	table->setCellWidget(row,3,new QCheckBox("�����."));
	table->setCellWidget(row,4,new QCheckBox("�����."));
	QCheckBox *chBox = new QCheckBox("������.");
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
	setTitle("������������ ��������");
	setSubTitle("�������� ���������� ��� ������������ ��������");
	
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
