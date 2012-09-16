//--------------------utech--------------------utech--------------------utech--------------------
//
// Створений програмістами Компанії "Утех" 24.03.2010р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UDialogSalaryEditTabelCell.h"
#include "UFormSalaryEditTabelWidget.h"
#include <USqlAccessible>
#include <QSqlQuery>

//--------------------utech--------------------utech--------------------utech--------------------
UDialogSalaryEditTabelCell::UDialogSalaryEditTabelCell(int nprId, QDate dt, QTableWidgetItem *item, QWidget *parent)
        : QDialog(parent), _dt(dt), _nprId(nprId), _item(item)
{
    ui.setupUi(this);
	
	QSqlQuery query;
	query.exec("SELECT id, Status_name FROM pracivn_status ORDER BY id");
	ui.comboBox_workStatus->addItem("", 0);
	while (query.next())
		ui.comboBox_workStatus->addItem(query.value(1).toString(), query.value(0));
	
	query.exec("SELECT Prizv, Imia, Pobatk FROM npr WHERE id="+sqlStr(_nprId));
	query.next();
	ui.lineEdit_worker->setText("["+QString::number(_nprId)+"] "+query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString());
	ui.lineEdit_workDate->setText(_dt.toString("dd.MM.yyyy, dddd"));
	
	USalaryTabelDay tabelDay = item->data(Qt::EditRole).value<USalaryTabelDay>();
	ui.comboBox_workStatus->setCurrentIndex(ui.comboBox_workStatus->findData (tabelDay.workType,Qt::UserRole));
	ui.timeEdit_rob->setTime(tabelDay.rob);
	ui.timeEdit_nich->setTime(tabelDay.nich);
	ui.timeEdit_sv->setTime(tabelDay.sv);
	
	connect(ui.pushButton_accept, SIGNAL(clicked()), this,SLOT(pushButton_accept_clicked()));
	connect(ui.pushButton_reject, SIGNAL(clicked()), this,SLOT(reject()));
	
	connect(ui.timeEdit_nich, SIGNAL(editingFinished()), this,SLOT(checkRobTime()));
	connect(ui.timeEdit_sv, SIGNAL(editingFinished()), this,SLOT(checkRobTime()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogSalaryEditTabelCell::pushButton_accept_clicked()
{
	QSqlQuery query;
	query.exec("SELECT id FROM tabel WHERE Npr_ID="+sqlStr(_nprId)+" and Cur_date="+sqlStr(_dt));
	if (!query.next()){
		query.exec("INSERT INTO tabel (Npr_ID, Cur_date) VALUES ("+sqlStr(_nprId)+","+sqlStr(_dt)+")");
		query.exec("SELECT last_insert_id()");
		query.next();
	}
	int id = query.value(0).toInt();
	query.exec("UPDATE tabel SET \
					Pracivn_status_ID = "+sqlStr(ui.comboBox_workStatus->itemData(ui.comboBox_workStatus->currentIndex(), Qt::UserRole).toInt())+", \
					God_zag = "+sqlStr(ui.timeEdit_rob->time())+", \
					God_nichni = "+sqlStr(ui.timeEdit_nich->time())+", \
					God_sv = "+sqlStr(ui.timeEdit_sv->time())+" \
				WHERE id="+sqlStr(id));
	query.exec("SELECT Pracivn_status_ID, God_zag, God_nichni, God_sv FROM tabel WHERE id="+sqlStr(id));
	query.next();
	USalaryTabelDay tabelDay;
	tabelDay.workType = query.value(0).toInt();
	tabelDay.rob = query.value(1).toTime();
	tabelDay.nich = query.value(2).toTime();
	tabelDay.sv = query.value(3).toTime();
	QVariant var;
	var.setValue(tabelDay);
	_item->setData(Qt::EditRole, var);
	accept();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogSalaryEditTabelCell::checkRobTime()
{
	if (ui.timeEdit_rob->time() < ui.timeEdit_nich->time())
		ui.timeEdit_rob->setTime(ui.timeEdit_nich->time());
	if (ui.timeEdit_rob->time() < ui.timeEdit_sv->time())
		ui.timeEdit_rob->setTime(ui.timeEdit_sv->time());	
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogSalaryEditTabelCell::~UDialogSalaryEditTabelCell()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
