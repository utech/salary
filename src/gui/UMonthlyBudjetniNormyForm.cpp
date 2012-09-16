//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна довідника робочих днів
//
// Створений програмістами Компанії "Утех" 19.02.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UMonthlyBudjetniNormyForm.h"

#include <USqlAccessible>
//--------------------utech--------------------utech--------------------utech--------------------
UMonthlyBudjetniNormyForm::UMonthlyBudjetniNormyForm(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	
	ui.tableWidget->setColumnHidden(0,true);
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->resizeRowsToContents();
	//ui.tableWidget->setItemDelegateForColumn(1, new USpinBoxDelegate(1,31, ui.tableWidget));
	//ui.tableWidget->setItemDelegateForColumn(2, new USpinBoxDelegate(0, 300, ui.tableWidget));
	
	connect(ui.spinBox_year_filtr, SIGNAL(valueChanged (int)), this, SLOT(populateWorkDaysTableWidget()));
	connect(ui.pushButton_reload, SIGNAL(clicked()), this, SLOT(populateWorkDaysTableWidget()));
	connect(ui.pushButton_closeForm, SIGNAL(clicked()), this, SLOT(close()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMonthlyBudjetniNormyForm::populateWorkDaysTableWidget()
{
	disconnect(ui.tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveWorkDays(QTableWidgetItem *)));
	
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT month(CurDate), id, WorkDaysInMonth, WorkHoursInMonth, MinZarplata, MinProjytkovyi, IndexSpojyvCin \
				FROM budjetni_normy_monthly \
				WHERE year(CurDate)="+QVariant(ui.spinBox_year_filtr->value()).toString());
	for (int i=0; i<12; i++){
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem("-"));
		QTableWidgetItem *item = new QTableWidgetItem("-");
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget->setItem(i, 1, item);
		item = new QTableWidgetItem("-");
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget->setItem(i, 2, item);
		item = new QTableWidgetItem("-");
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget->setItem(i, 3, item);
		item = new QTableWidgetItem("-");
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget->setItem(i, 4, item);
		item = new QTableWidgetItem("-");
		item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.tableWidget->setItem(i, 5, item);
	}
	while (query->next()){
		int row = query->value(0).toInt()-1;
		ui.tableWidget->item(row, 0)->setText(query->value(1).toString());
		ui.tableWidget->item(row, 1)->setText(query->value(2).toString());
		ui.tableWidget->item(row, 2)->setText(query->value(3).toString());
		ui.tableWidget->item(row, 3)->setText(QString::number(query->value(4).toDouble(),'f', 2).replace('.',','));
		ui.tableWidget->item(row, 4)->setText(QString::number(query->value(5).toDouble(),'f', 2).replace('.',','));
		ui.tableWidget->item(row, 5)->setText(QString::number(query->value(6).toDouble(),'f', 1).replace('.',','));
	}
	delete query;
	
	connect(ui.tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveWorkDays(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMonthlyBudjetniNormyForm::setFiltrYear(int year)
{
	ui.spinBox_year_filtr->setValue(year);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UMonthlyBudjetniNormyForm::saveWorkDays(QTableWidgetItem *item)
{
	disconnect(ui.tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveWorkDays(QTableWidgetItem *)));
	
	QSqlQuery *query = new QSqlQuery();
	int row = item->row();
	int id = ui.tableWidget->item(row,0)->text().toInt();
	QDate namDaysDate(ui.spinBox_year_filtr->value(), row+1, 1);
	if (id>0){
		query->exec("UPDATE budjetni_normy_monthly \
					SET WorkDaysInMonth="+mysqlStr(ui.tableWidget->item(row,1)->text().replace(',','.'))+", \
						WorkHoursInMonth="+mysqlStr(ui.tableWidget->item(row,2)->text().replace(',','.'))+", \
						MinZarplata="+mysqlStr(ui.tableWidget->item(row,3)->text().replace(',','.'))+", \
						MinProjytkovyi="+mysqlStr(ui.tableWidget->item(row,4)->text().replace(',','.'))+", \
						IndexSpojyvCin="+mysqlStr(ui.tableWidget->item(row,5)->text().replace(',','.'))+" \
					WHERE id="+mysqlStr(id));
	}
	else{
		query->exec("INSERT INTO budjetni_normy_monthly \
						(CurDate, WorkDaysInMonth, \
						WorkHoursInMonth, MinZarplata, \
						MinProjytkovyi, IndexSpojyvCin) \
					VALUES ("+mysqlStr(QDate(ui.spinBox_year_filtr->value(), row+1, 1).toString("yyyy-MM-dd"))+","+mysqlStr(ui.tableWidget->item(row,1)->data(Qt::DisplayRole).toInt())+", \
						"+mysqlStr(ui.tableWidget->item(row,2)->data(Qt::DisplayRole).toInt())+","+mysqlStr(ui.tableWidget->item(row,3)->text().replace(',','.'))+", \
						"+mysqlStr(ui.tableWidget->item(row,4)->text().replace(',','.'))+","+mysqlStr(ui.tableWidget->item(row,5)->text().replace(',','.'))+")");
		query->exec("SELECT last_insert_id()");
		query->seek(0);
		id = query->value(0).toInt();
		ui.tableWidget->item(row, 0)->setText(QString::number(id));
	}
	
	//Перечитування змінених даних з таблиці
	query->exec("SELECT WorkDaysInMonth, WorkHoursInMonth, MinZarplata, MinProjytkovyi, IndexSpojyvCin \
				FROM budjetni_normy_monthly \
				WHERE id="+mysqlStr(id));
	if (query->seek(0)){
		ui.tableWidget->item(row, 1)->setText(query->value(0).toString());
		ui.tableWidget->item(row, 2)->setText(query->value(1).toString());
		ui.tableWidget->item(row, 3)->setText(QString::number(query->value(2).toDouble(),'f', 2).replace('.',','));
		ui.tableWidget->item(row, 4)->setText(QString::number(query->value(3).toDouble(),'f', 2).replace('.',','));
		ui.tableWidget->item(row, 5)->setText(QString::number(query->value(4).toDouble(),'f', 1).replace('.',','));
	}
	else{
		ui.tableWidget->item(row, 1)->setText("0");
		ui.tableWidget->item(row, 2)->setText("0");
		ui.tableWidget->item(row, 3)->setText("0");
		ui.tableWidget->item(row, 4)->setText("0");
		ui.tableWidget->item(row, 5)->setText("0");
	}
	delete query;
	
	connect(ui.tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveWorkDays(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
UMonthlyBudjetniNormyForm::~UMonthlyBudjetniNormyForm()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
