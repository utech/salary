//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна налаштування програми
//
// Створений програмістами Компанії "Утех" 29.04.2010р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UFormSalaryTabelSumWidget.h"
#include <UTableWidgetSqlExt>
#include <USqlAccessible>
#include <UStandardDelegate>

#include <QtDebug>

#include "UPrintDocs.h"

//--------------------utech--------------------utech--------------------utech--------------------
UFormSalaryTabelSumWidget::UFormSalaryTabelSumWidget(QDate dt, bool showWorkedWorkers, QWidget *parent)
        : QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	showOnlyWorkWorkers = showWorkedWorkers;
	
	ui.tableWidget_tabel_sum->hideColumn(0);
	ui.tableWidget_tabel_sum->hideColumn(1);
	ui.tableWidget_tabel_sum->setItemDelegateForColumn(2,new USpinBoxDelegate(0,31,ui.tableWidget_tabel_sum));
	ui.tableWidget_tabel_sum->setItemDelegateForColumn(3,new USpinBoxDelegate(0,31,ui.tableWidget_tabel_sum));
	ui.tableWidget_tabel_sum->setItemDelegateForColumn(4,new USpinBoxDelegate(0,31,ui.tableWidget_tabel_sum));
	ui.tableWidget_tabel_sum->setItemDelegateForColumn(5,new USpinBoxDelegate(0,400,ui.tableWidget_tabel_sum));
	ui.tableWidget_tabel_sum->setItemDelegateForColumn(6,new USpinBoxDelegate(0,400,ui.tableWidget_tabel_sum));
	ui.tableWidget_tabel_sum->setItemDelegateForColumn(7,new USpinBoxDelegate(0,400,ui.tableWidget_tabel_sum));
	
	ui.dateEdit_tabelMonthFilter->setDate( dt );
	populateSumTabel();
	
	QSqlQuery query;
	ui.comboBox_pidrozdil->addItem(" --- Все підприємство ---", 0);
	query.exec("SELECT id, Name FROM pidrozdily_salary ORDER BY Name");
	while (query.next())
		ui.comboBox_pidrozdil->addItem(query.value(1).toString(), query.value(0));
	ui.comboBox_pidrozdil->addItem(" --- Загублені працівники ---", -1);
	
	connect(ui.dateEdit_tabelMonthFilter, SIGNAL(dateChanged(const QDate &)), this, SLOT(populateSumTabel()));
	connect(ui.comboBox_pidrozdil, SIGNAL(currentIndexChanged(int)), this, SLOT(populateSumTabel()));
}
//--------------------utech--------------------utech--------------------utech--------------------
UFormSalaryTabelSumWidget::~UFormSalaryTabelSumWidget()
{
	emit mainWindowClosed();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryTabelSumWidget::populateSumTabel()
{
	disconnect(ui.tableWidget_tabel_sum, SIGNAL(cellChanged (int,int)), this, SLOT(updateSumTabelSQLRows(int, int)));
	disconnect(ui.tableWidget_tabel_sum->selectionModel(), SIGNAL(currentRowChanged( const QModelIndex &, const QModelIndex &)), this, SLOT(sumTabelCurRowChanged( const QModelIndex &, const QModelIndex &)));
	
	QString str, tempStr;
	QSqlQuery *query = new QSqlQuery();
	int pidrozdilId = ui.comboBox_pidrozdil->itemData(ui.comboBox_pidrozdil->currentIndex(),Qt::UserRole).toInt();
	if (pidrozdilId > 0){ //Вивід працівників конкретного підрозділу
		query->exec("SELECT count(*) FROM npr WHERE Pidrozdil_id="+sqlStr(pidrozdilId)+" "
						+(showOnlyWorkWorkers?QString(" and npr.Pracuye=true"):QString("")));
	}
	else if (pidrozdilId == 0){ //Вивід усіх працівників підприємста
		query->exec("SELECT count(*) FROM npr"
						+(showOnlyWorkWorkers?QString(" WHERE npr.Pracuye=true"):QString("")));
	}
	else{ // Вивід працівників, що загубились
		query->exec("SELECT count(*) FROM npr WHERE Pidrozdil_id not in (SELECT id FROM pidrozdily_salary) "
					+(showOnlyWorkWorkers?QString(" and Pracuye=true"):QString("")));
	}
	query->seek(0);
	int rowCount = query->value(0).toInt();
	ui.tableWidget_tabel_sum->setRowCount(rowCount);
	for (int row=0; row<ui.tableWidget_tabel_sum->rowCount(); row++)
		for (int col=0; col<ui.tableWidget_tabel_sum->columnCount(); col++){
			QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
			tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			//tableWidgetItem->setBackgroundColor(itemColor);
			tableWidgetItem->setText("-");
			ui.tableWidget_tabel_sum->setItem(row, col, tableWidgetItem);
		}
	QAbstractItemModel * model = ui.tableWidget_tabel_sum->model();
	QStringList nprList;
	if (pidrozdilId > 0) { //Вивід працівників конкретного підрозділу
		query->exec("SELECT tabel_sum_this.id, tabel_sum_this.Npr_ID, Work_days_sum, Likarn_days_sum, \
					Vidpusk_days_sum, God_zag_sum, God_nichni_sum, \
					God_sv_sum, npr.Prizv, npr.Imia, npr.Pobatk, npr.id, tabel_sum_this.Pryinyato, tabel_sum_this.Zvilneno \
				FROM npr \
				LEFT JOIN (SELECT * FROM tabel_sum \
							WHERE year(tabel_sum.Cur_date)="+sqlStr(ui.dateEdit_tabelMonthFilter->date().year())+" \
								and month(tabel_sum.Cur_date)="+sqlStr(ui.dateEdit_tabelMonthFilter->date().month())+") AS tabel_sum_this \
					ON tabel_sum_this.Npr_ID=npr.id \
				WHERE npr.Pidrozdil_id="+sqlStr(pidrozdilId)+" "
						+(showOnlyWorkWorkers?QString(" and npr.Pracuye=true"):QString(""))+" \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk");
	}
	else if (pidrozdilId == 0){ //Вивід усіх працівників підприємста
		query->exec("SELECT tabel_sum_this.id, tabel_sum_this.Npr_ID, Work_days_sum, Likarn_days_sum, \
					Vidpusk_days_sum, God_zag_sum, God_nichni_sum, \
					God_sv_sum, npr.Prizv, npr.Imia, npr.Pobatk, npr.id, tabel_sum_this.Pryinyato, tabel_sum_this.Zvilneno \
				FROM npr \
				LEFT JOIN (SELECT * FROM tabel_sum \
							WHERE year(tabel_sum.Cur_date)="+sqlStr(ui.dateEdit_tabelMonthFilter->date().year())+" \
								and month(tabel_sum.Cur_date)="+sqlStr(ui.dateEdit_tabelMonthFilter->date().month())+") AS tabel_sum_this \
					ON tabel_sum_this.Npr_ID=npr.id \
				"+(showOnlyWorkWorkers?QString(" WHERE npr.Pracuye=true"):QString(""))+" \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk");
	}
	else{ // Вивід працівників, що загубились
		query->exec("SELECT tabel_sum_this.id, tabel_sum_this.Npr_ID, Work_days_sum, Likarn_days_sum, \
					Vidpusk_days_sum, God_zag_sum, God_nichni_sum, \
					God_sv_sum, npr.Prizv, npr.Imia, npr.Pobatk, npr.id, tabel_sum_this.Pryinyato, tabel_sum_this.Zvilneno \
				FROM npr \
				LEFT JOIN (SELECT * FROM tabel_sum \
							WHERE year(tabel_sum.Cur_date)="+sqlStr(ui.dateEdit_tabelMonthFilter->date().year())+" \
								and month(tabel_sum.Cur_date)="+sqlStr(ui.dateEdit_tabelMonthFilter->date().month())+") AS tabel_sum_this \
					ON tabel_sum_this.Npr_ID=npr.id \
				WHERE Pidrozdil_id not in (SELECT id FROM pidrozdily_salary) "
					+(showOnlyWorkWorkers?QString(" and Pracuye=true"):QString(""))+" \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk");
	}
	
	for (int row=0; (row<rowCount) && query->next(); row++){
		if (query->value(0).toInt()>0){
			int val;
			model->setData(model->index(row,0), query->value(0).toInt(), Qt::DisplayRole);
			model->setData(model->index(row,1), query->value(1).toInt(), Qt::DisplayRole);
			model->setData(model->index(row,2), query->value(2).toInt(), Qt::DisplayRole);
			model->setData(model->index(row,3), query->value(3).toInt(), Qt::DisplayRole);
			model->setData(model->index(row,4), query->value(4).toInt(), Qt::DisplayRole);
			model->setData(model->index(row,5), query->value(5).toDouble(), Qt::DisplayRole);
			model->setData(model->index(row,6), query->value(6).toDouble(), Qt::DisplayRole);
			model->setData(model->index(row,7), query->value(7).toDouble(), Qt::DisplayRole);
			val = query->value(12).toInt();
			if (val > 0)
				model->setData(model->index(row,8), val, Qt::DisplayRole);
			else
				model->setData(model->index(row,8), "-", Qt::DisplayRole);
			val = query->value(13).toInt();
			if (val > 0)
				model->setData(model->index(row,9), val, Qt::DisplayRole);
			else
				model->setData(model->index(row,9), "-", Qt::DisplayRole);
			}
		else
			model->setData(model->index(row,1), query->value(11).toInt(), Qt::DisplayRole);
		str.clear();
		tempStr =  "[ "+query->value(11).toString()+" ] ";
		tempStr += query->value(8).toString();
		if (tempStr.length()>0)
			str += tempStr + " ";
		else
			str += "- ";
		tempStr = query->value(9).toString();
		if (tempStr.length()>0)
			str += tempStr.at(0) + QString(".");
		else
			str += "-.";
		tempStr = query->value(10).toString();
		if (tempStr.length()>0)
			str += tempStr.at(0) + QString(".");
		else
			str += "-.";
		nprList << str;
	}
	ui.tableWidget_tabel_sum->resizeColumnsToContents();
	ui.tableWidget_tabel_sum->resizeRowsToContents();
	if (ui.tableWidget_tabel_sum->rowCount()>0)
		sumTabelCurRowChanged( ui.tableWidget_tabel_sum->currentIndex(), QModelIndex());
	
	ui.tableWidget_tabel_sum->setVerticalHeaderLabels(nprList);
	delete query;
	
	connect(ui.tableWidget_tabel_sum->selectionModel(), SIGNAL(currentRowChanged( const QModelIndex &, const QModelIndex &)), this, SLOT(sumTabelCurRowChanged( const QModelIndex &, const QModelIndex &)));
	connect(ui.tableWidget_tabel_sum, SIGNAL(cellChanged (int,int)), this, SLOT(updateSumTabelSQLRows(int, int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryTabelSumWidget::sumTabelCurRowChanged( const QModelIndex & curIndex, const QModelIndex & prevIndex)
{
	QAbstractItemModel * model = ui.tableWidget_tabel_sum->model();
	emit currentWorkerChanged(model->data(model->index(curIndex.row(),1)).toInt());
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryTabelSumWidget::updateSumTabelSQLRows(int row, int column)
{
	disconnect(ui.tableWidget_tabel_sum, SIGNAL(cellChanged (int,int)), this, SLOT(updateSumTabelSQLRows(int, int)));
	
	int id;
	QAbstractItemModel * model = ui.tableWidget_tabel_sum->model();
	int npr_id = model->data(model->index(row,1)).toInt();
	short year = ui.dateEdit_tabelMonthFilter->date().year();
	short month = ui.dateEdit_tabelMonthFilter->date().month();
	QSqlQuery query;
	query.exec("SELECT id FROM tabel_sum \
				WHERE year(Cur_date)="+sqlStr(year)+" \
					and month(Cur_date)="+sqlStr(month)+" \
					and Npr_id="+sqlStr(npr_id));
	
	if (!query.next()){ //insert query
		query.exec("INSERT INTO tabel_sum (Cur_date, Npr_ID) VALUES ("+sqlStr(QDate(year, month, 1))+","+sqlStr(npr_id)+")");
		query.exec("SELECT last_insert_id()");
		query.next();
	}
	id = query.value(0).toInt();
	query.exec("UPDATE tabel_sum SET Work_days_sum="+sqlStr(model->data(model->index(row,2)).toInt())+", \
									Likarn_days_sum="+sqlStr(model->data(model->index(row,3)).toInt())+", \
									Vidpusk_days_sum="+sqlStr(model->data(model->index(row,4)).toInt())+", \
									God_zag_sum="+uMToStr2Krapka(model->data(model->index(row,5)).toDouble())+", \
									God_nichni_sum="+uMToStr2Krapka(model->data(model->index(row,6)).toDouble())+", \
									God_sv_sum="+uMToStr2Krapka(model->data(model->index(row,7)).toDouble())+", \
									Pryinyato="+uMToStr2Krapka(model->data(model->index(row,8)).toDouble())+", \
									Zvilneno="+uMToStr2Krapka(model->data(model->index(row,9)).toDouble())+
						" WHERE id="+sqlStr(id));
	query.exec("SELECT id, Npr_ID, Work_days_sum, Likarn_days_sum, Vidpusk_days_sum, \
						God_zag_sum, God_nichni_sum, God_sv_sum, Pryinyato, Zvilneno  \
				FROM tabel_sum \
				WHERE id="+QString::number(id));
	query.next();
	model->setData(model->index(row,0), query.value(0).toInt(), Qt::DisplayRole);
	model->setData(model->index(row,1), query.value(1).toInt(), Qt::DisplayRole);
	model->setData(model->index(row,2), query.value(2).toInt(), Qt::DisplayRole);
	model->setData(model->index(row,3), query.value(3).toInt(), Qt::DisplayRole);
	model->setData(model->index(row,4), query.value(4).toInt(), Qt::DisplayRole);
	model->setData(model->index(row,5), query.value(5).toDouble(), Qt::DisplayRole);
	model->setData(model->index(row,6), query.value(6).toDouble(), Qt::DisplayRole);
	model->setData(model->index(row,7), query.value(7).toDouble(), Qt::DisplayRole);
	int val = query.value(8).toInt();
	if (val > 0)
		model->setData(model->index(row,8), val, Qt::DisplayRole);
	else
		model->setData(model->index(row,8), "-", Qt::DisplayRole);
	val = query.value(9).toInt();
	if (val > 0)
		model->setData(model->index(row,9), val, Qt::DisplayRole);
	else
		model->setData(model->index(row,9), "-", Qt::DisplayRole);
	connect(ui.tableWidget_tabel_sum, SIGNAL(cellChanged (int,int)), this, SLOT(updateSumTabelSQLRows(int, int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryTabelSumWidget::showOnlyWorkWorkersChanged(int state)
{
	if (state == Qt::Checked)
		showOnlyWorkWorkers = true;
	else
		showOnlyWorkWorkers = false;
	populateSumTabel();
}
//--------------------utech--------------------utech--------------------utech--------------------
