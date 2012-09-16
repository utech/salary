//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу головного вікна програми нарахування зарплати
//
// Створений програмістами Компанії "Утех" 17.07.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UPeopleSalaryMainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <USqlAccessible>
#include <UMath>
#include <UStandardDelegate>
#include <USqlAccessible>

#include <QLocale>

#include <../additional/UDifFunc.h>
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::execSalarySelectQuery()
{
	disconnect(ui.comboBox_Poshuk_PIB, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int)));
	disconnect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int)));
	
	QString workWorkersFilter = "";
	if (ui.checkBox_show_only_work_workers->isChecked()){
		workWorkersFilter = "WHERE Pracuye=true";
	}
	
	nprSelectQuery->clear();
	nprSelectQuery->exec("SELECT * FROM npr "+workWorkersFilter+" ORDER BY Prizv, Imia, Pobatk");
	int query_size=0;
	ui.comboBox_Poshuk_PIB->clear();
	ui.comboBox_Poshuk_PIB->addItem("");
	ui.comboBox_poshuk_osoba_id->clear();
	ui.comboBox_poshuk_osoba_id->addItem("");
	nprSelectQuery->seek( -1 );
	nprList.clear();
	while (nprSelectQuery->next()){
		query_size++;
		xRecord = nprSelectQuery->record();
		ui.comboBox_Poshuk_PIB->addItem(xRecord.value("Prizv").toString() + " " + xRecord.value("Imia").toString() + " " + xRecord.value("Pobatk").toString() );
		ui.comboBox_poshuk_osoba_id->addItem(xRecord.value("id").toString());
		nprList << xRecord.value("id").toInt();
	}
	ui.spinBox_navigator->setRange( 1, query_size );
	ui.label_max_rows_salary->setText(codec->toUnicode("із ") + QVariant(query_size).toString() + codec->toUnicode(".") );
	connect(ui.comboBox_Poshuk_PIB, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int)));
	connect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::populateNprComboBoxes()
{
	QSqlQuery query;
	int id;
	//Комбо Помади
	query.exec("SELECT id, Posada FROM taryfni_sitky ORDER BY Posada");
	ui.comboBox_Posada->blockSignals(true);
	if (ui.comboBox_Posada->count()>0)
		id = ui.comboBox_Posada->itemData(ui.comboBox_Posada->currentIndex(), Qt::UserRole).toInt();
	else
		id=0;
	ui.comboBox_Posada->clear();
	ui.comboBox_Posada->addItem("", 0);
	while (query.next())
		ui.comboBox_Posada->addItem(query.value(1).toString(), query.value(0));
	ui.comboBox_Posada->setCurrentIndex(ui.comboBox_Posada->findData(id, Qt::UserRole));
	ui.comboBox_Posada->blockSignals(false);
	//Комбо підрозділу
	query.exec("SELECT id, Name FROM pidrozdily_salary ORDER BY Name");
	ui.comboBox_pidrozdil->blockSignals(true);
	if (ui.comboBox_pidrozdil->count()>0)
		id = ui.comboBox_pidrozdil->itemData(ui.comboBox_pidrozdil->currentIndex(), Qt::UserRole).toInt();
	else
		id=0;
	ui.comboBox_pidrozdil->clear();
	ui.comboBox_pidrozdil->addItem("", 0);
	while (query.next())
		ui.comboBox_pidrozdil->addItem(query.value(1).toString(), query.value(0));
	ui.comboBox_pidrozdil->setCurrentIndex(ui.comboBox_pidrozdil->findData(id, Qt::UserRole));
	ui.comboBox_pidrozdil->blockSignals(false);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::checkBox_show_only_work_workers_stateChanged(int state)
{
	int id = ui.spinBox_id->value();
	execSalarySelectQuery();
	int indexNpr = nprList.indexOf(id)+1;
	if (indexNpr == 0)
		indexNpr = 1;
	if (indexNpr == ui.spinBox_navigator->value())
		populateSalaryForm( indexNpr );
	else
		ui.spinBox_navigator->setValue(indexNpr);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryConnectEditres()
{
	connect(ui.spinBox_id, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.lineEdit_Prizv, SIGNAL(textEdited (const QString &)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.lineEdit_Imya, SIGNAL(textEdited (const QString &)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.lineEdit_Batk, SIGNAL(textEdited (const QString &)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.comboBox_gender, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.dateEdit_BirthDate, SIGNAL(dateChanged (const QDate &)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.lineEdit_IdKod, SIGNAL(textEdited (const QString &)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.spinBox_DityNum, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.dateEdit_PryinDate, SIGNAL(dateChanged (const QDate &)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.comboBox_Posada, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.comboBox_typ_roboty, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.comboBox_typ_oplaty, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.lineEdit_Tel, SIGNAL(textEdited(const QString &)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.spinBox_rozryad, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.doubleSpinBox_Stavka, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.comboBox_pidrozdil, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.spinBox_OznPilgy, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.doubleSpinBox_KlasnistVodiya, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.doubleSpinBox_doplata, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.doubleSpinBox_alimenty, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.spinBox_buh_rahunok, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.checkBox_Pensioner, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.checkBox_ChlenProfspilky, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.checkBox_Invalid, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
	connect(ui.checkBox_WorkerWorks, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryDisConnectEditres()
{
	disconnect(ui.spinBox_id, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.lineEdit_Prizv, SIGNAL(textEdited(const QString &)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.lineEdit_Imya, SIGNAL(textEdited(const QString &)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.lineEdit_Batk, SIGNAL(textEdited(const QString &)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.comboBox_gender, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.dateEdit_BirthDate, SIGNAL(dateChanged (const QDate &)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.lineEdit_IdKod, SIGNAL(textEdited (const QString &)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.spinBox_DityNum, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.dateEdit_PryinDate, SIGNAL(dateChanged (const QDate &)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.comboBox_typ_roboty, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.comboBox_typ_oplaty, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.comboBox_Posada, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.spinBox_rozryad, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.doubleSpinBox_Stavka, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.comboBox_pidrozdil, SIGNAL(currentIndexChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.lineEdit_Tel, SIGNAL(textEdited(const QString &)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.spinBox_OznPilgy, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.doubleSpinBox_KlasnistVodiya, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.doubleSpinBox_doplata, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.doubleSpinBox_alimenty, SIGNAL(valueChanged (double)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.spinBox_buh_rahunok, SIGNAL(valueChanged (int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.checkBox_Pensioner, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.checkBox_ChlenProfspilky, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.checkBox_Invalid, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
	disconnect(ui.checkBox_WorkerWorks, SIGNAL(stateChanged(int)), this, SLOT(salaryFormRecordEditing()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormRecordEditing()
{
	ui.line->setPalette ( *palette );
	ui.line_2->setPalette ( *palette );
	ui.line_3->setPalette ( *palette );
	ui.line_4->setPalette ( *palette );
	ui.pushButton_new->setEnabled( true );
	ui.pushButton_salary_form_save->setEnabled( true );
	ui.checkBox_show_only_work_workers->setEnabled( false );
	record_edited = 1;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormRecordEndEditing()
{
	ui.line->setPalette ( *default_palette );
	ui.line_2->setPalette ( *default_palette );
	ui.line_3->setPalette ( *default_palette );
	ui.line_4->setPalette ( *default_palette );
	ui.pushButton_salary_form_save->setEnabled( false );
	ui.checkBox_show_only_work_workers->setEnabled( true );
	record_edited = 0;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormUpdateRecord()
{
	QSqlQuery query;
	query.exec("UPDATE npr SET \
					Prizv="+escapeSqlStr(ui.lineEdit_Prizv->text())+", \
					Imia="+escapeSqlStr(ui.lineEdit_Imya->text())+", \
					Pobatk="+escapeSqlStr(ui.lineEdit_Batk->text())+", \
					Gender="+sqlStr(ui.comboBox_gender->itemData(ui.comboBox_gender->currentIndex(),Qt::UserRole).toInt())+", \
					Data_nar="+sqlStr(ui.dateEdit_BirthDate->date())+", \
					Posada_id="+sqlStr(ui.comboBox_Posada->itemData(ui.comboBox_Posada->currentIndex(),Qt::UserRole).toInt())+", \
					TypRoboty="+sqlStr(ui.comboBox_typ_roboty->currentIndex())+", \
					TypOplaty="+sqlStr(ui.comboBox_typ_oplaty->currentIndex())+", \
					Iden_kod="+sqlStr(ui.lineEdit_IdKod->text())+", \
					Tel="+sqlStr(ui.lineEdit_Tel->text())+", \
					Rozryad="+sqlStr(ui.spinBox_rozryad->value())+", \
					Stavka="+sqlStr(ui.doubleSpinBox_Stavka->value())+", \
					Pidrozdil_id="+sqlStr(ui.comboBox_pidrozdil->itemData(ui.comboBox_pidrozdil->currentIndex(),Qt::UserRole).toInt())+", \
					Num_dit="+sqlStr(ui.spinBox_DityNum->value())+", \
					Ozn_pilgy="+sqlStr(ui.spinBox_OznPilgy->value())+", \
					Data_pruin_na_rob="+sqlStr(ui.dateEdit_PryinDate->date())+", \
					Klasnist_vodiya="+sqlStr(ui.doubleSpinBox_KlasnistVodiya->value())+", \
					VidsDoplaty="+sqlStr(ui.doubleSpinBox_doplata->value())+", \
					Alimenty="+sqlStr(ui.doubleSpinBox_alimenty->value())+", \
					Pensioner="+sqlStr(ui.checkBox_Pensioner->isChecked ())+", \
					Buh_rahunok="+sqlStr(ui.spinBox_buh_rahunok->value())+", \
					ChlenProfspilky="+sqlStr(ui.checkBox_ChlenProfspilky->isChecked())+", \
					Invalid="+sqlStr(ui.checkBox_Invalid->isChecked())+", \
					Pracuye="+sqlStr(ui.checkBox_WorkerWorks->isChecked())+" \
				WHERE id="+nprSelectQuery->value(0).toString());
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormInsertRecord()
{	
	QSqlQuery query;
	bool ok = query.exec("INSERT INTO npr(Prizv, Imia, Pobatk, Gender, Data_nar, Posada_id, TypRoboty, TypOplaty, Iden_kod, Tel, \
							Rozryad, Stavka, Pidrozdil_id, Num_dit, Ozn_pilgy, Data_pruin_na_rob, Klasnist_vodiya, \
							VidsDoplaty, Alimenty, Pensioner, Buh_rahunok, ChlenProfspilky, Invalid, Pracuye) \
				VALUES \
					("+escapeSqlStr(ui.lineEdit_Prizv->text())+", \
					"+escapeSqlStr(ui.lineEdit_Imya->text())+", \
					"+escapeSqlStr(ui.lineEdit_Batk->text())+", \
					"+sqlStr(ui.comboBox_gender->itemData(ui.comboBox_gender->currentIndex(),Qt::UserRole).toInt())+", \
					"+sqlStr(ui.dateEdit_BirthDate->date())+", \
					"+sqlStr(ui.comboBox_Posada->itemData(ui.comboBox_Posada->currentIndex(),Qt::UserRole).toInt())+", \
					"+sqlStr(ui.comboBox_typ_roboty->currentIndex())+", \
					"+sqlStr(ui.comboBox_typ_oplaty->currentIndex())+", \
					"+sqlStr(ui.lineEdit_IdKod->text())+", \
					"+sqlStr(ui.lineEdit_Tel->text())+", \
					"+sqlStr(ui.spinBox_rozryad->value())+", \
					"+sqlStr(ui.doubleSpinBox_Stavka->value())+", \
					"+sqlStr(ui.comboBox_pidrozdil->itemData(ui.comboBox_pidrozdil->currentIndex(),Qt::UserRole).toInt())+", \
					"+sqlStr(ui.spinBox_DityNum->value())+", \
					"+sqlStr(ui.spinBox_OznPilgy->value())+", \
					"+sqlStr(ui.dateEdit_PryinDate->date())+", \
					"+sqlStr(ui.doubleSpinBox_KlasnistVodiya->value())+", \
					"+sqlStr(ui.doubleSpinBox_doplata->value())+", \
					"+sqlStr(ui.doubleSpinBox_alimenty->value())+", \
					"+sqlStr(ui.checkBox_Pensioner->isChecked())+", \
					"+sqlStr(ui.spinBox_buh_rahunok->value())+", \
					"+sqlStr(ui.checkBox_ChlenProfspilky->isChecked())+", \
					"+sqlStr(ui.checkBox_Invalid->isChecked())+", \
					"+sqlStr(ui.checkBox_WorkerWorks->isChecked())+")");
	
	if (ok){
	QMessageBox::information(this,
				"!!! Додавання запису !!!",
				"Запис успішно додано до бази даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
	}
	else
		QMessageBox::critical(this,
				"!!! Невдале додавання запису !!!",
				"Програма не змогла додати запис до бази даних./nПричина - "
								+ query.lastError().text(),
				QMessageBox::Ok,
				QMessageBox::NoButton);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormFirst()
{
	ui.spinBox_navigator->setValue( 1 );
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormPrevious()
{
	ui.spinBox_navigator->setValue( ui.spinBox_navigator->value()-1 );
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormNext()
{
	ui.spinBox_navigator->setValue( ui.spinBox_navigator->value()+1 );
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormLast()
{
	ui.spinBox_navigator->setValue( ui.spinBox_navigator->maximum() );
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormNew()
{
	salaryDisConnectEditres();
	disconnect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(populateSalaryForm(int)));

	ui.pushButton_new->setEnabled( FALSE );
	if ( (new_record==1) && (record_edited==1) ){
		salaryFormRecordEndEditing();
		salaryFormInsertRecord();
		execSalarySelectQuery();
	}
	ui.spinBox_navigator->setMaximum( ui.spinBox_navigator->maximum()+1 );
	ui.spinBox_navigator->setValue( ui.spinBox_navigator->maximum() );
	//zadannya znachen za zamovchuvannyam
	QSqlQuery * query = new QSqlQuery;
	query->exec("SELECT max(id) FROM npr");
	if (query->seek(0))
		ui.spinBox_id->setValue(query->value(0).toInt()+1);
	else
		ui.spinBox_id->setValue(1);
	ui.lineEdit_Prizv->setText("");
	ui.lineEdit_Imya->setText("");
	ui.lineEdit_Batk->setText("");
	ui.comboBox_gender->setCurrentIndex(0);
	ui.dateEdit_BirthDate->setDate(ui.dateEdit_BirthDate->minimumDate());
	ui.comboBox_Posada->setCurrentIndex(0);
	ui.comboBox_typ_roboty->setCurrentIndex(0);
	ui.comboBox_typ_oplaty->setCurrentIndex(0);
	ui.lineEdit_IdKod->setText("");
	ui.lineEdit_Tel->setText("");
	ui.spinBox_rozryad->setValue(1);
	ui.doubleSpinBox_Stavka->setValue(0);
	ui.comboBox_pidrozdil->setCurrentIndex(0);
	ui.spinBox_DityNum->setValue(0);
	ui.spinBox_OznPilgy->setValue(0);
	ui.dateEdit_PryinDate->setDate(date);
	ui.doubleSpinBox_KlasnistVodiya->setValue(0);
	ui.doubleSpinBox_doplata->setValue(0);
	ui.doubleSpinBox_alimenty->setValue(0);
	ui.checkBox_Pensioner->setChecked(false);
	ui.checkBox_ChlenProfspilky->setChecked(false);
	ui.spinBox_buh_rahunok->setValue(0);
	ui.checkBox_Invalid->setChecked(false);
	ui.checkBox_WorkerWorks->setChecked(true);
	
	salaryConnectEditres();
	connect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(populateSalaryForm(int)));
	new_record = 1;
	record_edited=0;
	delete query;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFormDeleteRec()
{
	if	( QMessageBox::question(
                this,
                "!!! Видалення працівника !!!",
                "Ви дійсно бажаєте видалити спрацівника?\n"
				"Після видалення буде неможливо відновити інформацію працівника",
                "Так",
				"Ні-ні-ні", 
				QString(), 0, 1))
		return;
	
	salaryDisConnectEditres();
	disconnect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(populateSalaryForm(int)));
	
	int prevIndex = ui.spinBox_navigator->value();
	QSqlQuery query;
	bool doneOk = true;;
	if (!query.exec("DELETE FROM npr WHERE id="+mysqlStr(ui.spinBox_id->value())))
		doneOk = false;
	if (!query.exec("DELETE FROM operacii WHERE Npr_ID="+mysqlStr(ui.spinBox_id->value())))
		doneOk = false;
	execSalarySelectQuery();
	ui.spinBox_navigator->setValue(prevIndex);
	populateSalaryForm( prevIndex );
	if (doneOk)
		QMessageBox::information(this,
                "!!! Видалення працівника !!!",
                "Система вдало видалила працівника?",
                "Так");
	else
		QMessageBox::critical(this,
                "!!! Видалення працівника !!!",
                "Помикла видалення працівника?",
                "Так");
	
	connect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(populateSalaryForm(int)));
	salaryConnectEditres();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryButtonSaveClicked()
{
	populateSalaryForm( ui.spinBox_navigator->value() );
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryButtonCancelClicked()
{
	salaryFormRecordEndEditing();
	populateSalaryForm( ui.spinBox_navigator->value() );
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryPopulateLikarnyaniTable()
{
	disconnect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
	
	QSqlQuery query;
	query.exec("SELECT count(*) FROM likarnyani \
				WHERE Npr_id="+mysqlStr(ui.spinBox_id->value())+" \
					and year(Cur_date)="+mysqlStr(ui.spinBox_year_filtr->value())+" \
					and month(Cur_date)="+mysqlStr(ui.spinBox_month_filtr->value()));
	if (query.seek(0)){
		int numRows = query.value(0).toInt();
		QTableWidgetItem *item;
		query.exec("SELECT id, Narah_date, Suma, NumDays, Narah_type_id, Comentar FROM likarnyani \
					WHERE Npr_id="+mysqlStr(ui.spinBox_id->value())+" \
						and year(Cur_date)="+mysqlStr(ui.spinBox_year_filtr->value())+" \
						and month(Cur_date)="+mysqlStr(ui.spinBox_month_filtr->value()));
		ui.tableWidget_Likarnyani->setRowCount(numRows);
		for (int row=0; (row<numRows) && query.next(); row++){
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(0));
			ui.tableWidget_Likarnyani->setItem(row, 0, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(1).toDate().month());
			ui.tableWidget_Likarnyani->setItem(row, 1, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(1).toDate().year());
			ui.tableWidget_Likarnyani->setItem(row, 2, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(2).toDouble());
			ui.tableWidget_Likarnyani->setItem(row, 3, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(3).toDouble());
			ui.tableWidget_Likarnyani->setItem(row, 4, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(4).toInt());
			ui.tableWidget_Likarnyani->setItem(row, 5, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(5).toString());
			ui.tableWidget_Likarnyani->setItem(row, 6, item);
		}
	}
	ui.tableWidget_Likarnyani->resizeRowsToContents();
	
	calcLikarnyaniTable();
	
	connect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_insert_Likarnyani_clicked()
{
	disconnect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
	
	QSqlQuery query;
	QTableWidgetItem *item;
	QDate curDate(ui.spinBox_year_filtr->value(), ui.spinBox_month_filtr->value(), 1);
	if (query.exec("INSERT INTO likarnyani (Cur_date, Npr_ID, Narah_date, Suma, NumDays, Narah_type_id, Comentar) \
			VALUES ("+mysqlStr(curDate)+", "+mysqlStr(ui.spinBox_id->value())+", "+mysqlStr(curDate)+", "+mysqlStr(0)+", "+mysqlStr(0)+", 1, '')")){
		query.exec("SELECT last_insert_id()");
		query.seek(0);
		int id = query.value(0).toInt();
		int row = ui.tableWidget_Likarnyani->rowCount();
		ui.tableWidget_Likarnyani->insertRow(row);
		
		query.exec("SELECT id, Narah_date, Suma, NumDays, Narah_type_id, Comentar FROM likarnyani \
					WHERE id="+mysqlStr(id)+" LIMIT 1");
		query.seek(0);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, id);
		ui.tableWidget_Likarnyani->setItem(row, 0, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(1).toDate().month());
		ui.tableWidget_Likarnyani->setItem(row, 1, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(1).toDate().year());
		ui.tableWidget_Likarnyani->setItem(row, 2, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(2).toDouble());
		ui.tableWidget_Likarnyani->setItem(row, 3, item);
			
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(3).toDouble());
		ui.tableWidget_Likarnyani->setItem(row, 4, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(4).toInt());
		ui.tableWidget_Likarnyani->setItem(row, 5, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(5).toString());
		ui.tableWidget_Likarnyani->setItem(row, 6, item);
		
		ui.tableWidget_Likarnyani->resizeRowToContents(row);
		
		calcLikarnyaniTable();
	}
	
	connect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_del_Likarnyani_clicked()
{
	if (ui.tableWidget_Likarnyani->rowCount()== 0)
		return;
	disconnect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
	
	if	( ! QMessageBox::question(
                this,
                "!!! Видалення стрічки !!!",
                "Ви дійсно бажаєте видалити сирічку лікарняних?",
                "Так",
				"Ні-ні-ні",
                QString(), 0, 1)
		){
		QSqlQuery query;
		int curRow = ui.tableWidget_Likarnyani->currentRow();
		if (query.exec("DELETE FROM likarnyani WHERE id = "+ui.tableWidget_Likarnyani->item(curRow,0)->text())){
			ui.tableWidget_Likarnyani->removeRow(curRow);
			curRow--;
			if (curRow<0)
				curRow = 0;
			ui.tableWidget_Likarnyani->setCurrentCell(curRow, 1);
		}
		calcLikarnyaniTable();
	}
	
	connect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::saveLikarnyaniRecord(QTableWidgetItem* item)
{
	disconnect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
	
	int row = item->row();
	int id = ui.tableWidget_Likarnyani->item(row,0)->data(Qt::EditRole).toInt();
	QSqlQuery query;
	query.exec("UPDATE likarnyani SET \
					Narah_date = "+mysqlStr(QDate(ui.tableWidget_Likarnyani->item(row,2)->data(Qt::EditRole).toInt(),ui.tableWidget_Likarnyani->item(row,1)->data(Qt::EditRole).toInt(),1))+", \
					Suma="+mysqlStr(ui.tableWidget_Likarnyani->item(row,3)->data(Qt::EditRole).toDouble())+", \
					NumDays="+mysqlStr(ui.tableWidget_Likarnyani->item(row,4)->data(Qt::EditRole).toInt())+", \
					Narah_type_id="+mysqlStr(ui.tableWidget_Likarnyani->item(row,5)->data(Qt::EditRole).toInt())+", \
					Comentar="+mysqlStr(ui.tableWidget_Likarnyani->item(row,6)->data(Qt::EditRole).toString())+
				" WHERE id="+mysqlStr(id)+" LIMIT 1");
	
	query.exec("SELECT id, Narah_date, Suma, NumDays, Narah_type_id, Comentar FROM likarnyani \
				WHERE id="+mysqlStr(id)+" LIMIT 1");
	query.seek(0);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(1).toDate().month());
	ui.tableWidget_Likarnyani->setItem(row, 1, item);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(1).toDate().year());
	ui.tableWidget_Likarnyani->setItem(row, 2, item);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(2).toDouble());
	ui.tableWidget_Likarnyani->setItem(row, 3, item);
		
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(3).toDouble());
	ui.tableWidget_Likarnyani->setItem(row, 4, item);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(4).toInt());
	ui.tableWidget_Likarnyani->setItem(row, 5, item);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(5).toString());
	ui.tableWidget_Likarnyani->setItem(row, 6, item);
	
	ui.tableWidget_Likarnyani->resizeRowToContents(row);
	
	calcLikarnyaniTable();
	
	connect(ui.tableWidget_Likarnyani, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveLikarnyaniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::calcLikarnyaniTable()
{
	double sum=0;
	for (int row=0; row<ui.tableWidget_Likarnyani->rowCount(); row++){
		sum += ui.tableWidget_Likarnyani->item(row, 3)->data(Qt::EditRole).toDouble();
	}
	ui.label_LikarnyaniSum->setText("Сума лікарняних: " +uMToStr2(sum)+" грн.");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryPopulateVidpuskniTable()
{
	disconnect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
	
	QSqlQuery query;
	query.exec("SELECT count(*) FROM vidpuskni \
				WHERE Npr_id="+mysqlStr(ui.spinBox_id->value())+" \
					and year(Cur_date)="+mysqlStr(ui.spinBox_year_filtr->value())+" \
					and month(Cur_date)="+mysqlStr(ui.spinBox_month_filtr->value()));
	if (query.seek(0)){
		int numRows = query.value(0).toInt();
		QTableWidgetItem *item;
		query.exec("SELECT id, Narah_date, Suma, NumDays, Comentar FROM vidpuskni \
					WHERE Npr_id="+mysqlStr(ui.spinBox_id->value())+" \
						and year(Cur_date)="+mysqlStr(ui.spinBox_year_filtr->value())+" \
						and month(Cur_date)="+mysqlStr(ui.spinBox_month_filtr->value()));
		ui.tableWidget_Vidpuskni->setRowCount(numRows);
		for (int row=0; (row<numRows) && query.next(); row++){
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(0));
			ui.tableWidget_Vidpuskni->setItem(row, 0, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(1).toDate().month());
			ui.tableWidget_Vidpuskni->setItem(row, 1, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(1).toDate().year());
			ui.tableWidget_Vidpuskni->setItem(row, 2, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(2).toDouble());
			ui.tableWidget_Vidpuskni->setItem(row, 3, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(3).toDouble());
			ui.tableWidget_Vidpuskni->setItem(row, 4, item);
			
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			item->setData(Qt::EditRole, query.value(4).toString());
			ui.tableWidget_Vidpuskni->setItem(row, 5, item);
		}
	}
	
	ui.tableWidget_Vidpuskni->resizeRowsToContents();
	
	calcVidpuskniTable();
	
	connect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_insert_Vidpuskni_clicked()
{
	disconnect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
	
	QSqlQuery query;
	QTableWidgetItem *item;
	QDate curDate(ui.spinBox_year_filtr->value(), ui.spinBox_month_filtr->value(), 1);
	if (query.exec("INSERT INTO vidpuskni (Cur_date, Npr_ID, Narah_date, Suma, NumDays, Comentar) \
			VALUES ("+mysqlStr(curDate)+", "+mysqlStr(ui.spinBox_id->value())+", "+mysqlStr(curDate)+", "+mysqlStr(0)+", "+mysqlStr(0)+", '')")){
		query.exec("SELECT last_insert_id()");
		query.seek(0);
		int id = query.value(0).toInt();
		int row = ui.tableWidget_Vidpuskni->rowCount();
		ui.tableWidget_Vidpuskni->insertRow(row);
		
		query.exec("SELECT id, Narah_date, Suma, NumDays, Comentar FROM vidpuskni \
					WHERE id="+mysqlStr(id)+" LIMIT 1");
		query.seek(0);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, id);
		ui.tableWidget_Vidpuskni->setItem(row, 0, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(1).toDate().month());
		ui.tableWidget_Vidpuskni->setItem(row, 1, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(1).toDate().year());
		ui.tableWidget_Vidpuskni->setItem(row, 2, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(2).toDouble());
		ui.tableWidget_Vidpuskni->setItem(row, 3, item);
			
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(3).toDouble());
		ui.tableWidget_Vidpuskni->setItem(row, 4, item);
		
		item = new QTableWidgetItem();
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		item->setData(Qt::EditRole, query.value(4).toString());
		ui.tableWidget_Vidpuskni->setItem(row, 5, item);
		
		ui.tableWidget_Vidpuskni->resizeRowToContents(row);
		
		calcVidpuskniTable();
	}
	
	connect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_del_Vidpuskni_clicked()
{
	if (ui.tableWidget_Vidpuskni->rowCount()== 0)
		return;
	disconnect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
	
	if	( ! QMessageBox::question(
                this,
                "!!! Видалення стрічки !!!",
                "Ви дійсно бажаєте видалити сирічку відпускних?",
                "Так",
				"Ні-ні-ні",
                QString(), 0, 1)
		){
		QSqlQuery query;
		int curRow = ui.tableWidget_Vidpuskni->currentRow();
		if (query.exec("DELETE FROM vidpuskni WHERE id = "+ui.tableWidget_Vidpuskni->item(curRow,0)->text())){
			ui.tableWidget_Vidpuskni->removeRow(curRow);
			curRow--;
			if (curRow<0)
				curRow = 0;
			ui.tableWidget_Vidpuskni->setCurrentCell(curRow, 1);
		}
		calcVidpuskniTable();
	}
	
	connect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::saveVidpuskniRecord(QTableWidgetItem* item)
{
	disconnect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
	
	int row = item->row();
	int id = ui.tableWidget_Vidpuskni->item(row,0)->data(Qt::EditRole).toInt();
	QSqlQuery query;
	query.exec("UPDATE vidpuskni SET \
					Narah_date = "+mysqlStr(QDate(ui.tableWidget_Vidpuskni->item(row,2)->data(Qt::EditRole).toInt(),ui.tableWidget_Vidpuskni->item(row,1)->data(Qt::EditRole).toInt(),1))+", \
					Suma="+mysqlStr(ui.tableWidget_Vidpuskni->item(row,3)->data(Qt::EditRole).toDouble())+", \
					NumDays="+mysqlStr(ui.tableWidget_Vidpuskni->item(row,4)->data(Qt::EditRole).toInt())+", \
					Comentar="+mysqlStr(ui.tableWidget_Vidpuskni->item(row,5)->data(Qt::EditRole).toString())+
				" WHERE id="+mysqlStr(id)+" LIMIT 1");
	
	query.exec("SELECT id, Narah_date, Suma, NumDays, Comentar FROM vidpuskni \
				WHERE id="+mysqlStr(id)+" LIMIT 1");
	query.seek(0);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(1).toDate().month());
	ui.tableWidget_Vidpuskni->setItem(row, 1, item);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(1).toDate().year());
	ui.tableWidget_Vidpuskni->setItem(row, 2, item);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(2).toDouble());
	ui.tableWidget_Vidpuskni->setItem(row, 3, item);
		
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(3).toDouble());
	ui.tableWidget_Vidpuskni->setItem(row, 4, item);
	
	item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	item->setData(Qt::EditRole, query.value(4).toString());
	ui.tableWidget_Vidpuskni->setItem(row, 5, item);
	
	ui.tableWidget_Vidpuskni->resizeRowToContents(row);
	
	calcVidpuskniTable();
	
	connect(ui.tableWidget_Vidpuskni, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveVidpuskniRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::calcVidpuskniTable()
{
	double sum=0;
	for (int row=0; row<ui.tableWidget_Vidpuskni->rowCount(); row++){
		sum += ui.tableWidget_Vidpuskni->item(row, 3)->data(Qt::EditRole).toDouble();
	}
	ui.label_LikarnyaniSum->setText("Сума відпускних: " +uMToStr2(sum)+" грн.");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryPopulateOperaciiTable()
{
	disconnect(ui.tableWidget_Operacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveMainNarahRecord(QTableWidgetItem*)));
	
	clearOperaciiTable();
	QSqlQuery query("SELECT month(CurDate), id, Comentar, \
							I_PromIndex, I_Index, I_ProindexovanaSuma, I_SumaDlyaIndexacii, \
							N_Osn_narah, N_VuslRokiv, N_Likarn, N_Vidpuskni, \
							N_Svatkovi_dni, N_Nichni, N_Nadurochni, N_Klasn_vodiya, N_Premia, \
							N_MaterDopomoga, N_Doplata, N_Indexaciya, N_VyplProfspilky, N_InshiNarahuvannya, \
							U_PensFond, U_EV_Likarn, U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, \
							U_VidrahProfspilka, U_InshiVidrahuvannya, \
							V_PensFond, V_SocStrah, V_Fond_zainat, V_NeshchVyp, V_Inshi,\
							VydTabelyaInMonth, I_BaseMonth \
					FROM operacii \
					WHERE Npr_id = "+ui.spinBox_id->text()+" \
							and OznakaDohodu=1 and year(CurDate) = "+ui.spinBox_year_filtr->text());
	//int fieldsCount=query.record().count();
	QSqlRecord rec;
	while (query.next()){
		rec = query.record();
		int curMonth=query.value(0).toInt();
		
		populateStandartTableItem(curMonth-1, 0, query.value(1).toString());
		populateStandartTableItem(curMonth-1, 1, tabelResString(ui.spinBox_id->value(), ui.spinBox_year_filtr->value(), curMonth, rec.value("VydTabelyaInMonth").toInt(), ui.comboBox_typ_oplaty->currentIndex()), 0);
		populateStandartTableItem(curMonth-1, 2, query.value(2).toString());
		//Індексація
		populateStandartTableItem(curMonth-1, 3, QString::number(query.value(3).toDouble(), 'f', 1).replace('.',','));
		ui.tableWidget_Operacii->item(curMonth-1, 3)->setData(Qt::UserRole, query.value(34).toBool());
		ui.tableWidget_Operacii->item(curMonth-1, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		populateStandartTableItem(curMonth-1, 4, QString::number(query.value(4).toDouble(), 'f', 1).replace('.',','));
		populateStandartTableItem(curMonth-1, 5, uMToStr2(query.value(5).toDouble()));
		populateStandartTableItem(curMonth-1, 6, uMToStr2(query.value(6).toDouble()));
		//Нарахування
		populateStandartTableItem(curMonth-1, 7, uMToStr2(query.value(7).toDouble()));
		populateStandartTableItem(curMonth-1, 8, uMToStr2(query.value(8).toDouble()));
		populateStandartTableItem(curMonth-1, 9, uMToStr2(query.value(9).toDouble()));
		populateStandartTableItem(curMonth-1, 10, uMToStr2(query.value(10).toDouble()));
		populateStandartTableItem(curMonth-1, 11, uMToStr2(query.value(11).toDouble()));
		populateStandartTableItem(curMonth-1, 12, uMToStr2(query.value(12).toDouble()));
		populateStandartTableItem(curMonth-1, 13, uMToStr2(query.value(13).toDouble()));
		populateStandartTableItem(curMonth-1, 14, uMToStr2(query.value(14).toDouble()));
		populateStandartTableItem(curMonth-1, 15, uMToStr2(query.value(15).toDouble()));
		populateStandartTableItem(curMonth-1, 16, uMToStr2(query.value(16).toDouble()));
		populateStandartTableItem(curMonth-1, 17, uMToStr2(query.value(17).toDouble()));
		populateStandartTableItem(curMonth-1, 18, uMToStr2(query.value(18).toDouble()));
		populateStandartTableItem(curMonth-1, 19, uMToStr2(query.value(19).toDouble()));
		populateStandartTableItem(curMonth-1, 20, uMToStr2(query.value(20).toDouble()));
		//Утримання
		populateStandartTableItem(curMonth-1, 22, uMToStr2(query.value(21).toDouble()));
		
		populateStandartTableItem(curMonth-1, 23, uMToStr2(query.value(22).toDouble()));
		
		populateStandartTableItem(curMonth-1, 24, uMToStr2(query.value(23).toDouble()));
		populateStandartTableItem(curMonth-1, 25, uMToStr2(query.value(24).toDouble()));
		populateStandartTableItem(curMonth-1, 26, uMToStr2(query.value(25).toDouble()));
		populateStandartTableItem(curMonth-1, 27, uMToStr2(query.value(26).toDouble()));
		populateStandartTableItem(curMonth-1, 28, uMToStr2(query.value(27).toDouble()));
		populateStandartTableItem(curMonth-1, 29, uMToStr2(query.value(28).toDouble()));
		//Відрахування
		populateStandartTableItem(curMonth-1, 31, uMToStr2(query.value(29).toDouble()));
		populateStandartTableItem(curMonth-1, 32, uMToStr2(query.value(30).toDouble()));
		populateStandartTableItem(curMonth-1, 33, uMToStr2(query.value(31).toDouble()));
		populateStandartTableItem(curMonth-1, 34, uMToStr2(query.value(32).toDouble()));
		populateStandartTableItem(curMonth-1, 35, uMToStr2(query.value(33).toDouble()));
	}
	calcMainNarahOperaciiTableSum();
	connect(ui.tableWidget_Operacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveMainNarahRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::calcMainNarahOperaciiTableSum()
{
	int row, col;
	double sum=0;
	for (row=0; row<12; ++row){
		//Підбиття суми нарахування
		sum=0;
		for (col=NARAH_BEGIN_COLUMN; col<=NARAH_END_COLUMN; ++col){
			sum += ui.tableWidget_Operacii->item(row,col)->data(Qt::DisplayRole).toDouble();
		}
		ui.tableWidget_Operacii->item(row,NARAH_SUM_COLUMN)->setText(uMToStr2(sum));
		
		//Підбиття суми утримання
		sum=0;
		for (col=NARAH_SUM_COLUMN+1; col<UTRYM_SUM_COLUMN; ++col){
			if (col != SOC_PILGA_COLUMN)
				sum += ui.tableWidget_Operacii->item(row,col)->data(Qt::DisplayRole).toDouble();
		}
		ui.tableWidget_Operacii->item(row,UTRYM_SUM_COLUMN)->setText(uMToStr2(sum));
		
		//Підбиття суми відрахування
		sum=0;
		for (col=UTRYM_SUM_COLUMN+1; col<VIDRAH_SUM_COLUMN; ++col){
			sum += ui.tableWidget_Operacii->item(row,col)->data(Qt::DisplayRole).toDouble();
		}
		ui.tableWidget_Operacii->item(row,VIDRAH_SUM_COLUMN)->setText(uMToStr2(sum));
		
	}
	
	//Підбиття сум по колонках
	for(col = 3; col<ui.tableWidget_Operacii->columnCount(); col++)
	{
		sum = 0;
		for(int row = 0; row<ui.tableWidget_Operacii->rowCount()-1; row++)
			sum += ui.tableWidget_Operacii->item(row,col)->data(Qt::DisplayRole).toDouble();
		ui.tableWidget_Operacii->item(12, col)->setText(uMToStr2(sum));
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::populateStandartTableItem(int row, int col, QString str, Qt::ItemFlags editFlag)
{
	QTableWidgetItem *tableWidgetItem = ui.tableWidget_Operacii->item(row, col);
	tableWidgetItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | editFlag);
	tableWidgetItem->setText(str);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryPopulateOtherDohodyTable()
{
	disconnect(ui.tableWidget_dodatkovi_dohody, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveOtherDohodyRecord(QTableWidgetItem*)));
	
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) \
				FROM operacii \
				WHERE OznakaDohodu<>1 and Npr_id="+ui.spinBox_id->text()+" \
					and year(CurDate)="+ui.spinBox_year_filtr->text()+" and month(CurDate)="+ui.spinBox_month_filtr->text());
	if (query->seek(0)){
		int numRows = query->value(0).toInt();
		query->exec("SELECT id, OznakaDohodu, Comentar, N_Osn_narah, U_PensFond, \
								U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, \
								U_VidrahProfspilka, U_InshiVidrahuvannya, O_VsogoNaRuky, V_PensFond, V_SocStrah, \
								V_Fond_zainat, V_NeshchVyp, V_Inshi, OznakaDohodu \
					FROM operacii \
					WHERE OznakaDohodu<>1 and Npr_id="+ui.spinBox_id->text()+" \
						and year(CurDate)="+ui.spinBox_year_filtr->text()+" and month(CurDate)="+ui.spinBox_month_filtr->text());
		ui.tableWidget_dodatkovi_dohody->setRowCount(numRows);
		for (int i=0; (i<numRows) && query->next(); i++){
			populateOtherDohodyTableRow(i, query->record());
		}
	}
	delete query;
	calcOtherNarahOperaciiTableSum();
	
	connect(ui.tableWidget_dodatkovi_dohody, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveOtherDohodyRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::populateOtherDohodyTableRow(int row, QSqlRecord rec)
{
	populateOtherDohodyItem(row, 0, rec.value("id").toString());
	populateOtherDohodyItem(row, 1, rec.value("OznakaDohodu").toString());
	populateOtherDohodyItem(row, 2, rec.value("Comentar").toString());
	//Нарахування
	populateOtherDohodyItem(row, 3, uMToStr2(rec.value("N_Osn_narah").toDouble()));
	//Утримання
	populateOtherDohodyItem(row, 4, uMToStr2(rec.value("U_PensFond").toDouble()));
	populateOtherDohodyItem(row, 5, uMToStr2(rec.value("U_Fond_zainat").toDouble()));
	populateOtherDohodyItem(row, 6, uMToStr2(rec.value("U_SocStrah").toDouble()));
	populateOtherDohodyItem(row, 7, uMToStr2(rec.value("U_SocPilga").toDouble()));
	populateOtherDohodyItem(row, 8, uMToStr2(rec.value("U_Prybutkov_podatok").toDouble()));
	populateOtherDohodyItem(row, 9, uMToStr2(rec.value("U_VidrahProfspilka").toDouble()));
	populateOtherDohodyItem(row, 10, uMToStr2(rec.value("U_InshiVidrahuvannya").toDouble()));
	//populateOtherDohodyItem(row, 10, uMToStr2(rec.value("").toDouble()));	Сума утримань
	//Оплата
	populateOtherDohodyItem(row, 12, uMToStr2(rec.value("O_VsogoNaRuky").toDouble()));
	//Відрахування
	populateOtherDohodyItem(row, 13, uMToStr2(rec.value("V_PensFond").toDouble()));
	populateOtherDohodyItem(row, 14, uMToStr2(rec.value("V_SocStrah").toDouble()));
	populateOtherDohodyItem(row, 15, uMToStr2(rec.value("V_Fond_zainat").toDouble()));
	populateOtherDohodyItem(row, 16, uMToStr2(rec.value("V_NeshchVyp").toDouble()));
	populateOtherDohodyItem(row, 17, uMToStr2(rec.value("V_Inshi").toDouble()));
	//populateOtherDohodyItem(row, 16, uMToStr2(rec.value("").toDouble()));	Сума відрахувань
	ui.tableWidget_dodatkovi_dohody->resizeRowToContents(row);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::populateOtherDohodyItem(int row, int col, QString str, Qt::ItemFlags editFlag)
{
	QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(str);
	tableWidgetItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | editFlag);
	tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	if (col>2 && col<=OTHER_DOHID_NARAH_SUM_COLUMN)
		tableWidgetItem->setBackgroundColor(operaciiNarahColor);
	else if (col>OTHER_DOHID_NARAH_SUM_COLUMN && col<=OTHER_DOHID_UTRYM_SUM_COLUMN)
		tableWidgetItem->setBackgroundColor(operaciiUtrymColor);
	else if (col==OTHER_DOHID_NA_RUKY_COLUMN)
		tableWidgetItem->setBackgroundColor(operaciiOplataColor);
	else if (col>OTHER_DOHID_NA_RUKY_COLUMN && col<=OTHER_DOHID_VIDRAH_SUM_COLUMN)
		tableWidgetItem->setBackgroundColor(operaciiVidrahColor);
	ui.tableWidget_dodatkovi_dohody->setItem(row, col, tableWidgetItem);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::calcOtherNarahOperaciiTableSum()
{
	int row, col;
	double sum=0;
	for (row=0; row<ui.tableWidget_dodatkovi_dohody->rowCount(); ++row){
		//Підбиття суми утримання
		sum=0;
		for (col=OTHER_DOHID_NARAH_SUM_COLUMN+1; col<OTHER_DOHID_UTRYM_SUM_COLUMN; ++col){
			if (col != OTHER_DOHID_SOC_PILGA_COLUMN)
				sum += ui.tableWidget_dodatkovi_dohody->item(row,col)->data(Qt::DisplayRole).toDouble();
		}
		populateOtherDohodyItem(row, OTHER_DOHID_UTRYM_SUM_COLUMN, uMToStr2(sum), 0);
		
		//Підбиття суми відрахування
		sum=0;
		for (col=OTHER_DOHID_NA_RUKY_COLUMN+1; col<OTHER_DOHID_VIDRAH_SUM_COLUMN; ++col){
			sum += ui.tableWidget_dodatkovi_dohody->item(row,col)->data(Qt::DisplayRole).toDouble();
		}
		populateOtherDohodyItem(row, OTHER_DOHID_VIDRAH_SUM_COLUMN, uMToStr2(sum), 0);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_insert_other_dohid_clicked()
{
	disconnect(ui.tableWidget_dodatkovi_dohody, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveOtherDohodyRecord(QTableWidgetItem*)));
	
	QSqlQuery *query = new QSqlQuery();
	query->prepare("INSERT INTO operacii (Npr_id, CurDate, Comentar, N_Osn_narah, U_PensFond, \
								U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, \
								U_VidrahProfspilka, U_InshiVidrahuvannya, O_VsogoNaRuky, V_PensFond, V_SocStrah, \
								V_Fond_zainat, V_NeshchVyp, V_Inshi, OznakaDohodu) \
					VALUES  (\
								"+QString::number(ui.spinBox_id->value())+", \
								'"+QDate(ui.spinBox_year_filtr->value(), ui.spinBox_month_filtr->value(), 1).toString("yyyy-MM-dd")+"', \
								'', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2)");
	if (query->exec()){
		query->exec("SELECT last_insert_id()");
		query->seek(0);
		query->exec("SELECT id, OznakaDohodu, Comentar, N_Osn_narah, U_PensFond, \
								U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, \
								U_VidrahProfspilka, U_InshiVidrahuvannya, O_VsogoNaRuky, V_PensFond, V_SocStrah, \
								V_Fond_zainat, V_NeshchVyp, V_Inshi, OznakaDohodu \
					FROM operacii \
					WHERE id="+query->value(0).toString()+" LIMIT 1");
		int row = ui.tableWidget_dodatkovi_dohody->rowCount();
		ui.tableWidget_dodatkovi_dohody->setRowCount(row+1);
		query->seek(0);
		populateOtherDohodyTableRow(row, query->record());
		calcOtherNarahOperaciiTableSum();
	}
	else{
		qDebug() << "Помилка запису нової стрічки додаткового доходу.\nПричина: "+query->lastError().text()+
				"\nSQL запит: "+query->lastQuery();;
	}
	delete query;
	salaryPopulateSumNarahTable();
	
	connect(ui.tableWidget_dodatkovi_dohody, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveOtherDohodyRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_del_other_dohid_clicked()
{
	if	( ! QMessageBox::question(
                this,
                codec->toUnicode("!!! Видалення стрічки !!!"),
                codec->toUnicode("Ви дійсно бажаєте видалити сирічку нарахування?"),
                codec->toUnicode("Так"),
				codec->toUnicode("Ні-ні-ні"),
                QString(), 0, 1)
		){
		QSqlQuery *query = new QSqlQuery();
		int curRow = ui.tableWidget_dodatkovi_dohody->currentRow();
		if (query->exec("DELETE FROM operacii WHERE id = "+ui.tableWidget_dodatkovi_dohody->item(curRow,0)->text())){
			ui.tableWidget_dodatkovi_dohody->removeRow(curRow);
			curRow--;
			if (curRow<0)
				curRow = 0;
			ui.tableWidget_dodatkovi_dohody->setCurrentCell(curRow, 1);
		}
		delete query;
		salaryPopulateSumNarahTable();
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryPopulateSumNarahTable()
{
	disconnect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
	
	clearSumNarahTable();
	//Заповнення колонок, що не портебують підрахунку
	QSqlQuery query("SELECT month(CurDate), id, ComentarForOplata, Vh_saldo, \
							O_Inshi, O_Shtraf, O_Alimenty, O_ZaPoperMis, O_Premiya, O_Avans, O_Oplacheno\
					FROM operacii \
					WHERE Npr_id = "+ui.spinBox_id->text()+" \
							and OznakaDohodu=1 and year(CurDate) = "+ui.spinBox_year_filtr->text());
	while (query.next()){
		int curMonth=query.value(0).toInt();
		
		populateNarahSumTableItem(curMonth-1, 0, query.value(1).toString());
		populateNarahSumTableItem(curMonth-1, 1, query.value(2).toString());
		populateNarahSumTableItem(curMonth-1, 2, uMToStr2(query.value(3).toDouble()));
		populateNarahSumTableItem(curMonth-1, 13, uMToStr2(query.value(4).toDouble()));
		populateNarahSumTableItem(curMonth-1, 14, uMToStr2(query.value(5).toDouble()));
		populateNarahSumTableItem(curMonth-1, 15, uMToStr2(query.value(6).toDouble()));
		populateNarahSumTableItem(curMonth-1, 16, uMToStr2(query.value(7).toDouble()));
		populateNarahSumTableItem(curMonth-1, 17, uMToStr2(query.value(8).toDouble()));
		populateNarahSumTableItem(curMonth-1, 18, uMToStr2(query.value(9).toDouble()));
		populateNarahSumTableItem(curMonth-1, 19, uMToStr2(query.value(10).toDouble()));
	}
	
	//Суми доходів з ОД=01 
	//Заповнення конок, що потребують сумування
	query.exec("SELECT month(CurDate), \
						N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_Indexaciya+N_VyplProfspilky+N_InshiNarahuvannya AS narahSum, \
						U_PensFond+U_EV_Likarn+U_EV_Vidpuskni+U_Fond_zainat+U_SocStrah+U_Prybutkov_podatok+ \
							U_VidrahProfspilka+U_InshiVidrahuvannya AS utrymSum, \
						V_PensFond+V_EV_Likarn+V_EV_Vidpuskni+V_SocStrah+V_Fond_zainat+V_NeshchVyp+V_Inshi AS vidrahSum \
					FROM operacii \
					WHERE OznakaDohodu=1 and Npr_id="+ui.spinBox_id->text()+" and year(CurDate)="+ui.spinBox_year_filtr->text());
	while (query.next()){
		int curMonth=query.value(0).toInt();
		populateNarahSumTableItem(curMonth-1, SUM_NARAH_NARAH_SUM_BEG_COLUMN, uMToStr2(query.value(1).toDouble()),0);
		populateNarahSumTableItem(curMonth-1, SUM_NARAH_UTRYM_SUM_BEG_COLUMN, uMToStr2(query.value(2).toDouble()),0);
		populateNarahSumTableItem(curMonth-1, SUM_NARAH_VIDRAH_SUM_BEG_COLUMN, uMToStr2(query.value(3).toDouble()),0);
	}
	
	//Суми інших доходів (ОД>01)
	for (int i=0; i<12; ++i)
		NaRukyOtherDohody[i]=0;
	query.exec("SELECT month(CurDate), \
						sum(N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
						N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
						N_Doplata+N_Indexaciya+N_VyplProfspilky+N_InshiNarahuvannya) AS narahSum, \
						sum(U_PensFond+U_Fond_zainat+U_SocStrah+U_Prybutkov_podatok+ \
						U_VidrahProfspilka+U_InshiVidrahuvannya) AS utrymSum, \
						sum(V_PensFond+V_SocStrah+V_Fond_zainat+V_NeshchVyp+V_Inshi) AS vidrahSum, \
						sum(O_VsogoNaRuky) AS naRukySum\
					FROM operacii \
					WHERE OznakaDohodu>1 and Npr_id="+ui.spinBox_id->text()+" and year(CurDate)="+ui.spinBox_year_filtr->text()+" \
					GROUP BY month(CurDate)");
	while (query.next()){
		int curMonth=query.value(0).toInt();

		populateNarahSumTableItem(curMonth-1, SUM_NARAH_NARAH_SUM_BEG_COLUMN+1, uMToStr2(query.value(1).toDouble()),0);
		populateNarahSumTableItem(curMonth-1, SUM_NARAH_UTRYM_SUM_BEG_COLUMN+1, uMToStr2(query.value(2).toDouble()),0);
		populateNarahSumTableItem(curMonth-1, SUM_NARAH_VIDRAH_SUM_BEG_COLUMN+1, uMToStr2(query.value(3).toDouble()),0);
		NaRukyOtherDohody[curMonth-1]=query.value(4).toDouble();
	}
	
	
	calcNarahSumTable();
	connect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::calcNarahSumTable()
{
	int row, col;
	double sum=0;
	for (row=0; row<12; ++row){
		//Підбиття сумарного нарахування
		//sum  = ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_NARAH_SUM_BEG_COLUMN)->data(Qt::DisplayRole).toDouble();
		//sum += ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_NARAH_SUM_BEG_COLUMN+1)->data(Qt::DisplayRole).toDouble();
		//ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_NARAH_SUM_END_COLUMN)->setText(uMToStr2(sum));
		
		//Підбиття сумарного утримання
		sum  = ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_UTRYM_SUM_BEG_COLUMN)->data(Qt::DisplayRole).toDouble();
		sum += ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_UTRYM_SUM_BEG_COLUMN+1)->data(Qt::DisplayRole).toDouble();
		ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_UTRYM_SUM_END_COLUMN)->setText(uMToStr2(sum));
		
		//Підбиття сумарного відрахування
		sum  = ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_VIDRAH_SUM_BEG_COLUMN)->data(Qt::DisplayRole).toDouble();
		sum += ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_VIDRAH_SUM_BEG_COLUMN+1)->data(Qt::DisplayRole).toDouble();
		ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_VIDRAH_SUM_END_COLUMN)->setText(uMToStr2(sum));
		
		//Підбиття суми на руки
		double saldoSum;
		saldoSum  = ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_VHSALDO_COLUMN)->data(Qt::DisplayRole).toDouble();
		sum = ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_NARAH_SUM_BEG_COLUMN)->data(Qt::DisplayRole).toDouble();
		sum -= ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_UTRYM_SUM_END_COLUMN)->data(Qt::DisplayRole).toDouble();	
		//Додавання "На руки" з доходів >01
		sum += NaRukyOtherDohody[row];
		ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_IN_MONTH_NARAH_COLUMN)->setText(uMToStr2(sum));
		ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_IN_MONTH_NARAH_WITH_SALDO_COLUMN)->setText(uMToStr2(saldoSum+sum));
		
		//Підбиття вихідного сальдо
		sum  = ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_IN_MONTH_NARAH_WITH_SALDO_COLUMN)->data(Qt::DisplayRole).toDouble();
		for (int i=SUM_NARAH_OPLATA_BEGIN_SUM_COLUMN; i<=SUM_NARAH_OPLATA_END_SUM_COLUMN; i++)
			sum -= ui.tableWidget_SumNarah_Oplata->item(row,i)->data(Qt::DisplayRole).toDouble();
		ui.tableWidget_SumNarah_Oplata->item(row,SUM_NARAH_VYHSALDO_COLUMN)->setText(uMToStr2(sum));
	}
	
	//Підбиття сум по колонках
	for(col = 2; col<ui.tableWidget_SumNarah_Oplata->columnCount()-1; col++)
	{
		sum = 0;
		for(int row = 0; row<ui.tableWidget_SumNarah_Oplata->rowCount()-1; row++)
			sum += ui.tableWidget_SumNarah_Oplata->item(row,col)->data(Qt::DisplayRole).toDouble();
		ui.tableWidget_SumNarah_Oplata->item(12, col)->setText(uMToStr2(sum));
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::populateNarahSumTableItem(int row, int col, QString str, Qt::ItemFlags editFlag)
{
	QTableWidgetItem *tableWidgetItem = ui.tableWidget_SumNarah_Oplata->item(row, col);
	tableWidgetItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | editFlag);
	tableWidgetItem->setText(str);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::setOperaciiTabWidgetGUI(int index)
{
	if (index==0){
		ui.label_month_filtr->setVisible(true);
		ui.spinBox_month_filtr->setVisible(true);
		ui.label_show_operacii_checkboxes->setVisible(false);
		ui.checkBox_Operacii_vidprac_godyny->setVisible(false);
		ui.checkBox_Operacii_indexaciya->setVisible(false);
		ui.checkBox_Operacii_narahovano->setVisible(false);
		ui.checkBox_Operacii_utrymannya->setVisible(false);
		ui.checkBox_Operacii_pislyapodatk_vidrah->setVisible(false);
		ui.checkBox_Operacii_vidrahuv_na_zarplatu->setVisible(false);
	}
	else if (index==1){
		ui.label_month_filtr->setVisible(false);
		ui.spinBox_month_filtr->setVisible(false);
		ui.label_show_operacii_checkboxes->setVisible(true);
		ui.checkBox_Operacii_vidprac_godyny->setVisible(true);
		ui.checkBox_Operacii_indexaciya->setVisible(true);
		ui.checkBox_Operacii_narahovano->setVisible(true);
		ui.checkBox_Operacii_utrymannya->setVisible(true);
		ui.checkBox_Operacii_pislyapodatk_vidrah->setVisible(false);
		ui.checkBox_Operacii_vidrahuv_na_zarplatu->setVisible(true);
	}
	else if (index==2){
		ui.label_month_filtr->setVisible(true);
		ui.spinBox_month_filtr->setVisible(true);
		ui.label_show_operacii_checkboxes->setVisible(true);
		ui.checkBox_Operacii_vidprac_godyny->setVisible(true);
		ui.checkBox_Operacii_indexaciya->setVisible(false);
		ui.checkBox_Operacii_narahovano->setVisible(false);
		ui.checkBox_Operacii_utrymannya->setVisible(true);
		ui.checkBox_Operacii_pislyapodatk_vidrah->setVisible(false);
		ui.checkBox_Operacii_vidrahuv_na_zarplatu->setVisible(true);
	}
	else if (index==3){
		ui.label_month_filtr->setVisible(false);
		ui.spinBox_month_filtr->setVisible(false);
		ui.label_show_operacii_checkboxes->setVisible(true);
		ui.checkBox_Operacii_vidprac_godyny->setVisible(true);
		ui.checkBox_Operacii_indexaciya->setVisible(false);
		ui.checkBox_Operacii_narahovano->setVisible(true);
		ui.checkBox_Operacii_utrymannya->setVisible(true);
		ui.checkBox_Operacii_pislyapodatk_vidrah->setVisible(true);
		ui.checkBox_Operacii_vidrahuv_na_zarplatu->setVisible(true);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryBeforeChangeDefaultOplata(QSqlRecord & record)
{
	//if (!record.isNull("Pererah_date"))
	//	d_Perevid_Date = record.value("Pererah_date").toDate();
	
	//record.setValue("id", nextLichylnyk("oplata"));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::clearOperaciiTable()
{
	for (int col=0; col<ui.tableWidget_Operacii->columnCount(); col++)
		for (int row=0; row<ui.tableWidget_Operacii->rowCount(); row++){
			QTableWidgetItem *tableWidgetItem = ui.tableWidget_Operacii->item(row, col);
			tableWidgetItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			//tableWidgetItem->setFlags(0);
			tableWidgetItem->setText("-");
		}
	for (int row=0; row<ui.tableWidget_Operacii->rowCount(); row++){
		ui.tableWidget_Operacii->item(row, 1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget_Operacii->item(row, 3)->setData(Qt::UserRole, false);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::clearSumNarahTable()
{
	for (int col=0; col<ui.tableWidget_SumNarah_Oplata->columnCount(); col++)
		for (int row=0; row<ui.tableWidget_SumNarah_Oplata->rowCount(); row++){
			QTableWidgetItem *tableWidgetItem = ui.tableWidget_SumNarah_Oplata->item(row, col);
			tableWidgetItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			tableWidgetItem->setText("-");
		}
	for (int row=0; row<ui.tableWidget_SumNarah_Oplata->rowCount(); row++)
		ui.tableWidget_SumNarah_Oplata->item(row, 1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::salaryFiltrComboboxIndexChanged(int index)
{
	disconnect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int))); //????'???????
	disconnect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), this, SLOT(salaryFiltrComboboxIndexChanged(int)));
	
	ui.comboBox_poshuk_osoba_id->clearEditText();
	
	connect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int))); //????'???????
	connect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), this, SLOT(salaryFiltrComboboxIndexChanged(int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::populateSalaryForm( int seek_query )
{
	salaryDisConnectEditres(); //обов'зкова
	disconnect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(populateSalaryForm(int)));
	
	if ( (new_record==1) && (record_edited==1) ){ // novyi zapys, yakyi bulo redagovano
		salaryFormRecordEndEditing();
		salaryFormInsertRecord();
		execSalarySelectQuery();
		new_record = 0;
	}
	else if ( record_edited==1 ){ // zmina zapysu
		int id = ui.spinBox_id->value();
		salaryFormRecordEndEditing();
		salaryFormUpdateRecord();
		execSalarySelectQuery();
		int indexNpr = nprList.indexOf(id)+1;
		if (indexNpr == 0)
			indexNpr = 1;
		seek_query = indexNpr;
		ui.spinBox_navigator->setValue(seek_query);
		enableNavigationButtons(seek_query);
	}
	else if ( (new_record==1) && (record_edited==0) ){
		execSalarySelectQuery();
		enableNavigationButtons(seek_query);
		ui.pushButton_new->setEnabled( TRUE );
		new_record = 0;
	}
	salaryDisConnectEditres();
	
	nprSelectQuery->seek( seek_query-1 );
	xRecord = nprSelectQuery->record();
	//Заповнення форми
	ui.spinBox_id->setValue(xRecord.value("id").toInt());
	ui.lineEdit_Prizv->setText(xRecord.value("Prizv").toString());
	ui.lineEdit_Imya->setText(xRecord.value("Imia").toString());
	ui.lineEdit_Batk->setText(xRecord.value("Pobatk").toString());
	ui.comboBox_gender->setCurrentIndex(ui.comboBox_gender->findData(xRecord.value("Gender").toInt(), Qt::UserRole));
	if (xRecord.value("Data_nar").toDate().isValid())
		ui.dateEdit_BirthDate->setDate(xRecord.value("Data_nar").toDate());
	else
		ui.dateEdit_BirthDate->setDate(ui.dateEdit_BirthDate->minimumDate());
	ui.comboBox_Posada->setCurrentIndex(ui.comboBox_Posada->findData(xRecord.value("Posada_id").toInt(), Qt::UserRole));
	ui.comboBox_typ_roboty->setCurrentIndex(xRecord.value("TypRoboty").toInt());
	ui.comboBox_typ_oplaty->setCurrentIndex(xRecord.value("TypOplaty").toInt());
	ui.lineEdit_IdKod->setText(xRecord.value("Iden_kod").toString());
	ui.lineEdit_Tel->setText(xRecord.value("Tel").toString());
	ui.spinBox_rozryad->setValue(xRecord.value("Rozryad").toInt());
	ui.doubleSpinBox_Stavka->setValue(xRecord.value("Stavka").toDouble());
	ui.comboBox_pidrozdil->setCurrentIndex(ui.comboBox_pidrozdil->findData(xRecord.value("Pidrozdil_id").toInt(), Qt::UserRole));
	ui.spinBox_DityNum->setValue(xRecord.value("Num_dit").toInt());
	ui.spinBox_OznPilgy->setValue(xRecord.value("Ozn_pilgy").toInt());
	if (xRecord.value("Data_pruin_na_rob").toDate().isValid())
		ui.dateEdit_PryinDate->setDate(xRecord.value("Data_pruin_na_rob").toDate());
	else
		ui.dateEdit_PryinDate->setDate(date);
	ui.doubleSpinBox_KlasnistVodiya->setValue(xRecord.value("Klasnist_vodiya").toDouble());
	ui.doubleSpinBox_doplata->setValue(xRecord.value("VidsDoplaty").toDouble());
	ui.doubleSpinBox_alimenty->setValue(xRecord.value("Alimenty").toDouble());
	ui.checkBox_Pensioner->setChecked(xRecord.value("Pensioner").toBool());
	ui.spinBox_buh_rahunok->setValue(xRecord.value("Buh_rahunok").toInt());
	ui.checkBox_ChlenProfspilky->setChecked(xRecord.value("ChlenProfspilky").toBool());
	ui.checkBox_Invalid->setChecked(xRecord.value("Invalid").toBool());
	ui.checkBox_WorkerWorks->setChecked(xRecord.value("Pracuye").toBool());
    
	salaryPopulateLikarnyaniTable();
	salaryPopulateVidpuskniTable();
	salaryPopulateOperaciiTable();
	salaryPopulateOtherDohodyTable();
	salaryPopulateSumNarahTable();
	
	//обов'зкові
	connect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(populateSalaryForm(int)));
	salaryConnectEditres();	//обов'зкова
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::verifyStatusWorker()
{
	/*if(!((userAccess == 2) or (userAccess == 3)))
		ui.tableWidget_Operacii->setEditTriggers(QAbstractItemView::NoEditTriggers);
	*/
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::saveMainNarahRecord(QTableWidgetItem* item)
{
	disconnect(ui.tableWidget_Operacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveMainNarahRecord(QTableWidgetItem*)));
	disconnect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
	
	int id = item->tableWidget()->item(item->row(),0)->data(Qt::DisplayRole).toInt();
	QString str = item->data(Qt::DisplayRole).toString().replace(',','.');
	QSqlQuery query("UPDATE operacii \
							SET "+mainNarahColNamesList.at(item->column())+" = '"+str+"' \
							WHERE id = "+QString::number(id)+" LIMIT 1");
	query.exec("SELECT "+mainNarahColNamesList.at(item->column())+" FROM operacii WHERE id = "+QString::number(id)+" LIMIT 1");
	query.seek(0);
	if (item->column()==2)
		item->setText(query.value(0).toString());
	else if ((item->column()==INDEXACIYA_BEGIN_COLUMN) || (item->column()==INDEXACIYA_BEGIN_COLUMN+1))
		item->setText(QString::number(query.value(0).toDouble(), 'f', 1).replace('.',','));
	else
		item->setText(uMToStr2(query.value(0).toDouble()));
	
	calcMainNarahOperaciiTableSum();
	salaryPopulateSumNarahTable();
	
	connect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
	connect(ui.tableWidget_Operacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveMainNarahRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::saveOtherDohodyRecord(QTableWidgetItem* item)
{
	disconnect(ui.tableWidget_dodatkovi_dohody, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveOtherDohodyRecord(QTableWidgetItem*)));
	disconnect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
	
	int id = item->tableWidget()->item(item->row(),0)->data(Qt::DisplayRole).toInt();
	QString str = item->data(Qt::DisplayRole).toString().replace(',','.');
	QSqlQuery query;
	
	if (!query.exec("UPDATE operacii \
				SET "+otherNarahColNamesList.at(item->column())+" = '"+str+"' \
				WHERE id = "+QString::number(id)+" LIMIT 1"))
		QMessageBox::critical( 0, "Помилка запису", 
								"Програма не змогла змінити запис.\nПричина: "+query.lastError().text(), 
								QMessageBox::Ok, 
								QMessageBox::Ok );
	query.exec("SELECT "+otherNarahColNamesList.at(item->column())+" FROM operacii WHERE id = "+QString::number(id)+" LIMIT 1");
	query.seek(0);
	if (item->column()==1 || item->column()==2)
		item->setText(query.value(0).toString());
	else
		item->setText(uMToStr2(query.value(0).toDouble()));
	
	calcOtherNarahOperaciiTableSum();
	salaryPopulateSumNarahTable();
	
	connect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
	connect(ui.tableWidget_dodatkovi_dohody, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveOtherDohodyRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::saveNarahSumRecord(QTableWidgetItem* item)
{
	disconnect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
	
	int id = item->tableWidget()->item(item->row(),0)->data(Qt::DisplayRole).toInt();
	QString str = item->data(Qt::DisplayRole).toString().replace(',','.');
	QSqlQuery query("UPDATE operacii \
							SET "+narahSumColNamesList.at(item->column())+" = '"+str+"' \
							WHERE id = "+QString::number(id)+" LIMIT 1");
	query.exec("SELECT "+narahSumColNamesList.at(item->column())+" FROM operacii WHERE id = "+QString::number(id)+" LIMIT 1");
	query.seek(0);
	if (item->column()==2)
		item->setText(query.value(0).toString());
	else
		item->setText(uMToStr2(query.value(0).toDouble()));
	
	calcMainNarahOperaciiTableSum();
	salaryPopulateSumNarahTable();
	
	connect(ui.tableWidget_SumNarah_Oplata, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveNarahSumRecord(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::enableNavigationButtons(int index)
{
	ui.pushButton_first->setEnabled ( TRUE );
	ui.pushButton_previous->setEnabled ( TRUE );
	ui.pushButton_next->setEnabled ( TRUE );
	ui.pushButton_last->setEnabled ( TRUE );
	if ( index < 2 ){
		ui.pushButton_first->setEnabled ( FALSE );
		ui.pushButton_previous->setEnabled ( FALSE );
	}
	if ( index >= ui.spinBox_navigator->maximum() ){
		ui.pushButton_next->setEnabled ( FALSE );
		ui.pushButton_last->setEnabled ( FALSE );
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::tableOperaciiVidpracGodyny()
{
	bool ext = !ui.checkBox_Operacii_vidprac_godyny->isChecked();
	ui.tableWidget_Operacii->setColumnHidden ( 1, ext);
	ui.tableWidget_Operacii->setColumnHidden ( 2, ext);
	
	ui.tableWidget_dodatkovi_dohody->setColumnHidden( 2, ext);
	
	ui.tableWidget_SumNarah_Oplata->setColumnHidden( 1, ext);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::tableOperaciiNarahovanoView()
{
	bool ext = !ui.checkBox_Operacii_narahovano->isChecked();
	for (int col=NARAH_BEGIN_COLUMN; col<=NARAH_END_COLUMN; col++)
		ui.tableWidget_Operacii->setColumnHidden ( col, ext);
	
	for (int col=SUM_NARAH_NARAH_SUM_BEG_COLUMN; col<=SUM_NARAH_NARAH_SUM_END_COLUMN; col++)
		ui.tableWidget_SumNarah_Oplata->setColumnHidden ( col, ext);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::tableOperaciiUtrymannyaView()
{
	bool ext = !ui.checkBox_Operacii_utrymannya->isChecked();
	for (int col=NARAH_SUM_COLUMN+1; col<=UTRYM_SUM_COLUMN; col++)
		ui.tableWidget_Operacii->setColumnHidden ( col, ext);
	
	for (int col=OTHER_DOHID_NARAH_SUM_COLUMN+1; col<=OTHER_DOHID_UTRYM_SUM_COLUMN; col++)
		ui.tableWidget_dodatkovi_dohody->setColumnHidden ( col, ext);
	
	for (int col=SUM_NARAH_UTRYM_SUM_BEG_COLUMN; col<=SUM_NARAH_UTRYM_SUM_END_COLUMN; col++)
		ui.tableWidget_SumNarah_Oplata->setColumnHidden ( col, ext);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::tableOperaciiOplataView()
{
	bool ext = !ui.checkBox_Operacii_pislyapodatk_vidrah->isChecked();
	
	for (int col=SUM_NARAH_OPLATA_BEGIN_SUM_COLUMN; col<=SUM_NARAH_OPLATA_END_SUM_COLUMN; col++)
		ui.tableWidget_SumNarah_Oplata->setColumnHidden ( col, ext);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::tableOperaciiVidrahuvannyaZarplatyView()
{
	bool ext = !ui.checkBox_Operacii_vidrahuv_na_zarplatu->isChecked();
	for (int col=UTRYM_SUM_COLUMN+1; col<=VIDRAH_SUM_COLUMN; col++)
		ui.tableWidget_Operacii->setColumnHidden ( col, ext);
	
	for (int col=OTHER_DOHID_NA_RUKY_COLUMN+1; col<=OTHER_DOHID_VIDRAH_SUM_COLUMN; col++)
		ui.tableWidget_dodatkovi_dohody->setColumnHidden ( col, ext);
	
	for (int col=SUM_NARAH_VIDRAH_SUM_BEG_COLUMN; col<=SUM_NARAH_VIDRAH_SUM_END_COLUMN; col++)
		ui.tableWidget_SumNarah_Oplata->setColumnHidden ( col, ext);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::tableOperaciiIndexaciyaView()
{
	bool ext = !ui.checkBox_Operacii_indexaciya->isChecked();
	for (int col=INDEXACIYA_BEGIN_COLUMN; col<=INDEXACIYA_END_COLUMN; col++)
		ui.tableWidget_Operacii->setColumnHidden ( col, ext);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::populateTryfyPracivnykivTable()
{
	disconnect(ui.tableWidget_taryfy, SIGNAL(itemChanged(QTableWidgetItem *)), 
				this, SLOT(taryfyTableItemChanged(QTableWidgetItem *)));
	
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) FROM taryfni_sitky");
	query->seek(0);
	int numRows = query->value(0).toInt();
	ui.tableWidget_taryfy->setRowCount(numRows);
	query->exec("SELECT * FROM taryfni_sitky");
	for (int i=0; i<numRows; i++){
		query->next();
		ui.tableWidget_taryfy->setItem(i,0,new QTableWidgetItem(query->value(0).toString()));
		ui.tableWidget_taryfy->setItem(i,1,new QTableWidgetItem(query->value(1).toString()));
		ui.tableWidget_taryfy->setItem(i,2,new QTableWidgetItem(query->value(2).toString()));
	}
	delete query;
	
	ui.tableWidget_taryfy->resizeRowsToContents();
	connect(ui.tableWidget_taryfy, SIGNAL(itemChanged(QTableWidgetItem *)), 
				this, SLOT(taryfyTableItemChanged(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_insertTaryf_clicked()
{
	disconnect(ui.tableWidget_taryfy, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(taryfyTableItemChanged(QTableWidgetItem *)));
	
	QSqlQuery *query = new QSqlQuery();
	if (query->exec("INSERT INTO taryfni_sitky (Posada, Taryf) VALUES ('', 1000)")){
		query->exec("SELECT last_insert_id()");
		query->seek(0);
		ui.tableWidget_taryfy->insertRow(ui.tableWidget_taryfy->rowCount());
		int rowNum = ui.tableWidget_taryfy->rowCount()-1;
		ui.tableWidget_taryfy->setItem(rowNum, 0, new QTableWidgetItem(query->value(0).toString()));
		ui.tableWidget_taryfy->item(rowNum, 0)->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
		ui.tableWidget_taryfy->setItem(rowNum, 1, new QTableWidgetItem(""));
		ui.tableWidget_taryfy->item(rowNum, 1)->setTextAlignment( Qt::AlignVCenter | Qt::AlignLeft );
		ui.tableWidget_taryfy->setItem(rowNum, 2, new QTableWidgetItem("1000"));
		ui.tableWidget_taryfy->item(rowNum, 2)->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
		ui.tableWidget_taryfy->editItem(ui.tableWidget_taryfy->item(rowNum, 1));
		ui.tableWidget_taryfy->setCurrentCell(rowNum, 1);
		ui.tableWidget_taryfy->resizeRowToContents(rowNum);
	}
	delete query;
	populateNprComboBoxes();
	connect(ui.tableWidget_taryfy, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(taryfyTableItemChanged(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::pushButton_delTaryf_clicked()
{
	if	( ! QMessageBox::question(
                this,
                codec->toUnicode("!!! Видалення стрічки !!!"),
                codec->toUnicode("Ви дійсно бажаєте видалити сирічку?"),
                codec->toUnicode("Так"),
				codec->toUnicode("Ні-ні-ні"),
                QString(), 0, 1)
		){
		QSqlQuery *query = new QSqlQuery();
	
		int curRow = ui.tableWidget_taryfy->currentRow();
		if (query->exec("DELETE FROM taryfni_sitky WHERE id="+ui.tableWidget_taryfy->item(curRow,0)->text())){
			ui.tableWidget_taryfy->removeRow(curRow);
			curRow--;
			if (curRow<0)
				curRow = 0;
			ui.tableWidget_taryfy->setCurrentCell(curRow, 1);
		}
		populateNprComboBoxes();
		delete query;
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::taryfyTableItemChanged(QTableWidgetItem *item)
{
	disconnect(ui.tableWidget_taryfy, SIGNAL(itemChanged(QTableWidgetItem *)), 
				this, SLOT(taryfyTableItemChanged(QTableWidgetItem *)));
	
	QSqlQuery query;
	int rowNum = ui.tableWidget_taryfy->currentRow();
	query.exec("UPDATE taryfni_sitky \
				SET Posada="+sqlStr(ui.tableWidget_taryfy->item(rowNum,1)->text())+", \
					Taryf="+sqlStr(ui.tableWidget_taryfy->item(rowNum,2)->data(Qt::EditRole).toDouble())+" \
				WHERE id="+ui.tableWidget_taryfy->item(rowNum,0)->text());
	query.exec("SELECT Posada, Taryf FROM taryfni_sitky WHERE id="+ui.tableWidget_taryfy->item(rowNum,0)->text());
	query.next();
	ui.tableWidget_taryfy->item(rowNum,1)->setText(query.value(0).toString());
	ui.tableWidget_taryfy->item(rowNum,2)->setText(query.value(1).toString());
	populateNprComboBoxes();
	
	connect(ui.tableWidget_taryfy, SIGNAL(itemChanged(QTableWidgetItem *)), 
				this, SLOT(taryfyTableItemChanged(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::curDateChanged(const QDate & xDate)
{
	workDate = xDate;
	ui.spinBox_year_filtr->setValue(workDate.year());
	ui.spinBox_month_filtr->setValue(workDate.month());
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::showWorker(int nprId)
{
	int pos = nprList.indexOf(nprId)+1;
	if (pos>0){
		ui.spinBox_navigator->setValue(pos);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
UPeopleSalaryMainWindow::~UPeopleSalaryMainWindow()
{
	//Збереження вигляду GUI в іні-файл
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	
	QSettings *settings = new QSettings("./salary.ini", QSettings::IniFormat);
	settings->beginGroup ("GUI_View");
	settings->setValue("chBoxOperaciiVidpracyovaniGodynyDni",ui.checkBox_Operacii_vidprac_godyny->isChecked());
	settings->setValue("chBoxOperaciiIndexaciya",ui.checkBox_Operacii_indexaciya->isChecked());
	settings->setValue("chBoxOperaciiTableNarahovano",ui.checkBox_Operacii_narahovano->isChecked());
	settings->setValue("chBoxOperaciiTableUtrymannya",ui.checkBox_Operacii_utrymannya->isChecked());
	settings->setValue("chBoxOperaciiTablePislapodatkvidrah",ui.checkBox_Operacii_pislyapodatk_vidrah->isChecked());
	settings->setValue("chBoxOperaciiTableVidrahuvannyaNaZarplatu",ui.checkBox_Operacii_vidrahuv_na_zarplatu->isChecked());
	settings->setValue("chBoxShowOnlyWorkWorkers",ui.checkBox_show_only_work_workers->isChecked());
	settings->setValue("workDate",ui.dateEdit_curDate->date());
	settings->setValue("tabWidget_operaciiCurPage", ui.tabWidget_operacii->currentIndex());
	delete settings;
	
	emit mainWindowClosed();
}
//--------------------utech--------------------utech--------------------utech--------------------
Dovidnyk::Dovidnyk(const QString& icon, const QString& name,vudDovidnuka vud_)
{
	vud = vud_;
	//setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(name);
	setWindowIcon(QIcon(icon));
	resize(550,600);
	tableWidget = new QTableWidget(this);
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	addRecPbtn = new QPushButton(QIcon(":/crystalProject/pictures/crystal-project-actions-PNG/edit_add-256.png"),"Додати запис");
	delRecPbtn = new QPushButton(QIcon(":/crystalProject/pictures/crystal-project-actions-PNG/edit_remove-256.png"),"Видалити запис");
	closePbtn = new QPushButton(QIcon(":/crystalProject/pictures/crystal-project-actions-PNG/cancel-256.png"),"Закрити");
	QVBoxLayout *vlayout = new QVBoxLayout(this); 
	vlayout->setContentsMargins(2, 2, 2, 2);
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addWidget(addRecPbtn);
	hlayout->addWidget(delRecPbtn);
	hlayout->addStretch(8);
	hlayout->addWidget(closePbtn);
	vlayout->addWidget(tableWidget);
	vlayout->addLayout(hlayout);
	if(vud == GrypuKorust){
		populateData("grupa",QStringList()<<"id"<<"Назва групи користувачів");
		tableWidget->hideColumn(2);
	}
	else if(vud == BjydzNormu)
	{
		populateData("budjetni_normu",QStringList()<<"id"<<"Група"<<"Опис"<<"Значення", "ORDER BY groupName, Opus");
		tableWidget->setItemDelegateForColumn(3, new UDoubleSpinBoxDelegate(-1000000, 1000000, 2, tableWidget));
		tableWidget->hideColumn(4);
	}
	else if(vud == VuslygaRokiv){
		populateData("vusluga_rokiv",QStringList()<<"id"<<"Стаж, до X років"<<"Відсотки", "ORDER BY Befour_year");
		tableWidget->setItemDelegateForColumn(1, new USpinBoxDelegate(0, 100, tableWidget));
		tableWidget->setItemDelegateForColumn(2, new UDoubleSpinBoxDelegate(0, 100, 2, tableWidget));
	}
	
	tableWidget->setCurrentCell(0,1);
	curRowChanged();
	
	setLayout(vlayout);
	connect(addRecPbtn,SIGNAL(clicked()),this,SLOT(insertRec()));
	connect(delRecPbtn,SIGNAL(clicked()),this,SLOT(deleteRec()));
	connect(tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(verifyRec(QTableWidgetItem*)));
	connect(tableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(curRowChanged()));
	connect(closePbtn,SIGNAL(clicked()),this,SLOT(close()));	
}
//--------------------utech--------------------utech--------------------utech--------------------
void Dovidnyk::insertRec()
{
	disconnect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
	
	QSqlQuery query;
	int row = tableWidget->rowCount();
	if (vud == BjydzNormu){
		if (!query.exec("INSERT INTO budjetni_normu (GroupName, Opus, Znach, ReadOnly) VALUES ('-','-','0',false)"))
			return;
		query.exec("SELECT last_insert_id()");
		query.seek(0);
		query.exec("SELECT * FROM budjetni_normu WHERE id="+query.value(0).toString());
		if (query.seek(0)){
			tableWidget->setRowCount(row+1);
			tableWidget->setItem(row,0,new QTableWidgetItem(query.value(0).toString()));
			tableWidget->setItem(row,1,new QTableWidgetItem(query.value(1).toString()));
			tableWidget->setItem(row,2,new QTableWidgetItem(query.value(2).toString()));
			tableWidget->setItem(row,3,new QTableWidgetItem(query.value(3).toString()));
			tableWidget->setItem(row,4,new QTableWidgetItem(query.value(4).toString()));
			tableWidget->item(row, 1)->setBackground(QBrush(greenColor));
			tableWidget->item(row, 2)->setBackground(QBrush(greenColor));
			tableWidget->item(row, 3)->setBackground(QBrush(greenColor));
			tableWidget->item(row, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			tableWidget->setCurrentCell(row,1);
			tableWidget->editItem(tableWidget->item(row,1));
		}
		else
			QMessageBox::critical(this,
				"!!! Невдале створення стрічки !!!",
				"Невдале створення стрічки. Програма не змогла додати запис до бази даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
	}
	else if (vud == GrypuKorust){
		if (!query.exec("INSERT INTO Grupa (GrupaName, ReadOnly) VALUES ('-',false)"))
			return;
		query.exec("SELECT last_insert_id()");
		query.seek(0);
		query.exec("SELECT * FROM Grupa WHERE id="+query.value(0).toString());
		if (query.seek(0)){
			int row = tableWidget->rowCount();
			tableWidget->insertRow(row);
			tableWidget->setItem(row,0,new QTableWidgetItem(query.value(0).toString()));
			tableWidget->setItem(row,1,new QTableWidgetItem(query.value(1).toString()));
			tableWidget->setItem(row,2,new QTableWidgetItem(query.value(2).toString()));
			tableWidget->item(row, 1)->setBackground(QBrush(greenColor));
			tableWidget->item(row, 1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			tableWidget->setCurrentCell(row,1);
			tableWidget->editItem(tableWidget->item(row,1));
		}
		else
			QMessageBox::critical(this,
				"!!! Невдале створення стрічки !!!",
				"Невдале створення стрічки. Програма не змогла додати запис до бази даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
	}
	else if(vud == VuslygaRokiv)
	{
		if (!query.exec("INSERT INTO vusluga_rokiv (Befour_year, Vidsotky) VALUES (50,0)"))
			return;
		query.exec("SELECT last_insert_id()");
		query.seek(0);
		query.exec("SELECT * FROM budjetni_normu WHERE id="+query.value(0).toString());
		if (query.seek(0)){
			int row = tableWidget->rowCount();
			tableWidget->insertRow(row);
			tableWidget->setItem(row,0,new QTableWidgetItem(query.value(0).toString()));
			tableWidget->setItem(row,1,new QTableWidgetItem(query.value(1).toString()));
			tableWidget->setItem(row,2,new QTableWidgetItem(query.value(2).toString()));
			tableWidget->item(row, 1)->setBackground(QBrush(greenColor));
			tableWidget->item(row, 1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			tableWidget->item(row, 2)->setBackground(QBrush(greenColor));
			tableWidget->item(row, 2)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			tableWidget->setCurrentCell(row,1);
			tableWidget->editItem(tableWidget->item(row,1));
		}
		else
			QMessageBox::critical(this,
				"!!! Невдале створення стрічки !!!",
				"Невдале створення стрічки. Програма не змогла додати запис до бази даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
	}
	tableWidget->resizeRowToContents(row);
	
	connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
} 
//--------------------utech--------------------utech--------------------utech--------------------
void Dovidnyk::saveItem(QTableWidgetItem* item)
{
	disconnect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
	
	QSqlQuery query;
	int row = item->row();
	int id = tableWidget->item(row, 0)->data(Qt::DisplayRole).toInt();
	if (vud == BjydzNormu){
		if (!query.exec("UPDATE budjetni_normu \
						SET GroupName="+mysqlStr(tableWidget->item(row, 1)->data(Qt::DisplayRole).toString())+", \
							Opus="+mysqlStr(tableWidget->item(row, 2)->data(Qt::DisplayRole).toString())+", \
							Znach="+mysqlStr(tableWidget->item(row, 3)->data(Qt::DisplayRole).toDouble())+" \
						WHERE id="+mysqlStr(id))){
			QMessageBox::critical(this,
				"!!! Невдала зміна стрічки !!!",
				"Невдала зміна стрічки. Програма не змогла змінити запис в базі даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
		}
		query.exec("SELECT * FROM budjetni_normu WHERE id="+mysqlStr(id));
		query.seek(0);
		tableWidget->item(row,1)->setText(query.value(1).toString());
		tableWidget->item(row,2)->setText(query.value(2).toString());
		tableWidget->item(row,3)->setText(query.value(3).toString());
	}
	else if (vud == GrypuKorust){
		if (query.exec("UPDATE grupa \
						SET GrupaName="+mysqlStr(tableWidget->item(row, 1)->data(Qt::DisplayRole).toString())+" \
						WHERE id="+mysqlStr(id))){
			query.exec("SELECT * FROM grupa WHERE id="+mysqlStr(id));
			query.seek(0);
			tableWidget->item(row,1)->setText(query.value(1).toString());
		}
		else	
			QMessageBox::critical(this,
				"!!! Невдала зміна стрічки !!!",
				"Невдала зміна стрічки. Програма не змогла змінити запис в базі даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
	}
	else if (vud == VuslygaRokiv){
		if (!query.exec("UPDATE vusluga_rokiv \
						SET Befour_year="+mysqlStr(tableWidget->item(row, 1)->data(Qt::DisplayRole).toInt())+", \
						Vidsotky="+mysqlStr(tableWidget->item(row, 2)->data(Qt::DisplayRole).toDouble())+" \
						WHERE id="+mysqlStr(id))){
			QMessageBox::critical(this,
				"!!! Невдала зміна стрічки !!!",
				"Невдала зміна стрічки. Програма не змогла змінити запис в базі даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
		}
		query.exec("SELECT * FROM vusluga_rokiv WHERE id="+mysqlStr(id));
		query.next();
		tableWidget->item(row,1)->setText(query.value(1).toString());
		tableWidget->item(row,2)->setText(query.value(2).toString());
	}
	connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void Dovidnyk::deleteRec()
{
	bool control = tableWidget->item(tableWidget->currentRow(),tableWidget->columnCount()-1)->data(Qt::DisplayRole).toBool();
	int id = tableWidget->item(tableWidget->currentItem()->row(), 0)->data(Qt::DisplayRole).toInt();
	QSqlQuery query;
	if(vud == GrypuKorust)
	{
		if(!control){
			if (query.exec("DELETE FROM grupa WHERE id="+mysqlStr(id)))
				tableWidget->removeRow(tableWidget->currentItem()->row());
			else
				QMessageBox::critical(this,
				"!!! Помилка видалення запису !!!",
				"Програма не змогла видалити запис з бази даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
		}
		else{
			QMessageBox::critical(this,
				"!!! Неможливість видалення запису !!!",
				"Ця група користувачів є базовою, тому її неможливо видалити.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
		}
	}
	else if(vud == BjydzNormu)
	{
		if(!control){
			if (query.exec("DELETE FROM budjetni_normu WHERE id="+mysqlStr(id)))
				tableWidget->removeRow(tableWidget->currentItem()->row());
			else
				QMessageBox::critical(this,
				"!!! Помилка видалення запису !!!",
				"Програма не змогла видалити запис з бази даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
		}
		else{
			QMessageBox::critical(this,
				"!!! Неможливість видалення запису !!!",
				"Ця бюджетна норма є базовою, тому її неможливо видалити.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
		}
	}
	else if(vud == VuslygaRokiv)
	{
		if (query.exec("DELETE FROM vusluga_rokiv WHERE id="+mysqlStr(id)))
				tableWidget->removeRow(tableWidget->currentItem()->row());
			else
				QMessageBox::critical(this,
				"!!! Помилка видалення запису !!!",
				"Програма не змогла видалити запис з бази даних.",
				QMessageBox::Ok,
				QMessageBox::NoButton);
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void Dovidnyk::verifyRec(QTableWidgetItem* item)
{
	disconnect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
	
	bool control = tableWidget->item(item->row(),tableWidget->columnCount()-1)->data(Qt::DisplayRole).toBool();
	if (vud == BjydzNormu){
		if (item->column()==1 || item->column()==2)
			item->setFlags((control) ? item->flags()& ~Qt::ItemIsEditable : item->flags()|Qt::ItemIsEditable);
	}
	else if (vud == GrypuKorust){
			item->setFlags((control) ? item->flags()& ~Qt::ItemIsEditable : item->flags()|Qt::ItemIsEditable);
	}
	
	connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void Dovidnyk::curRowChanged()
{
	delRecPbtn->setEnabled(!tableWidget->item(tableWidget->currentRow(), tableWidget->columnCount()-1)->data(Qt::DisplayRole).toBool());
}
//--------------------utech--------------------utech--------------------utech--------------------
void Dovidnyk::populateData(const QString& tableName, const QStringList& headers, QString orderStr)
{
	disconnect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
	
	QSqlQuery query;
	query.exec("SELECT count(*) FROM "+tableName);
	query.seek(0);
	numRows = query.value(0).toInt();
	query.exec("SELECT * FROM "+tableName+" "+orderStr);
	numCols = query.record().count();
	tableWidget->setColumnCount(numCols);
	tableWidget->setRowCount(numRows);
	
	for(int row = 0; row < numRows ; row++){
		query.next();
		for(int col = 0; col < numCols; col++){
			tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString().replace('.',',')));
			tableWidget->item(row, col)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			if (vud == BjydzNormu) {
				if (col==1 || col==2) {
					tableWidget->item(row, col)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
				}
			}
		}
	}
	//Зафарбовування кольором комірок, які можна редагувати
	if (vud == BjydzNormu){
		tableWidget->setColumnWidth(1,80);
		tableWidget->setColumnWidth(2,350);
		tableWidget->setColumnWidth(3,70);
		for (int row = 0; row < numRows; row++){
			if (tableWidget->item(row, 4)->data(Qt::DisplayRole).toBool()){
				tableWidget->item(row, 1)->setBackground(QBrush(redColor));
				tableWidget->item(row, 2)->setBackground(QBrush(redColor));
			}
			else{
				tableWidget->item(row, 1)->setBackground(QBrush(greenColor));
				tableWidget->item(row, 2)->setBackground(QBrush(greenColor));
			}
			tableWidget->item(row, 3)->setBackground(QBrush(greenColor));
		}
	}
	else if (vud == GrypuKorust){
		for (int row = 0; row < numRows; row++){
			if (tableWidget->item(row, 2)->data(Qt::DisplayRole).toBool())
				tableWidget->item(row, 1)->setBackground(QBrush(redColor));
			else
				tableWidget->item(row, 1)->setBackground(QBrush(greenColor));
			tableWidget->item(row, 1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		}
	}
	
	tableWidget->hideColumn(0); // don't show the ID
	tableWidget->setHorizontalHeaderLabels(headers);
	tableWidget->resizeRowsToContents();
	
	connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(saveItem(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UIndexaciyaInfoDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
	QStyleOptionViewItemV3 opt = setOptions(index, option);
	drawBackground(painter, opt, index);
	if (index.data(Qt::UserRole).toBool()){
		QStyleOptionViewItemV3 opt2 = opt;
		opt2.decorationPosition = QStyleOptionViewItem::Top;
		opt2.displayAlignment = Qt::AlignLeft;
		drawDisplay(painter, opt2, opt2.rect, "базовий");
	}
	double val=index.data(Qt::EditRole).toDouble();
	if (val>0)
		drawDisplay(painter, opt, opt.rect, QString::number(val,'f',1).replace('.',','));
	else
		drawDisplay(painter, opt, opt.rect, "-");
	drawFocus(painter, opt, opt.rect);
}
//--------------------utech--------------------utech--------------------utech--------------------
