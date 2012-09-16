//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "QtDebug"

#include "UWidgetPrintPlatijky.h"
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UWidgetPrintPlatijky::UWidgetPrintPlatijky(QDate cDate=QDate(), QWidget *parent)
        : QWidget(parent)
{
	ui.setupUi(this);
	
	QDate date=QDate::currentDate();
	ui.dateEdit_platijkaDate->setDate(date);
	ui.dateEdit_pererahunokZaMisyac->setDate(cDate);
	
	ui.tableWidget_organizacii->setColumnHidden(0, true);
	ui.tableWidget_organizacii->setColumnHidden(9, true);
	ui.tableWidget_organizacii->setColumnWidth(1, 50);
	ui.tableWidget_organizacii->setColumnWidth(2, 300);
	ui.tableWidget_organizacii->setColumnWidth(3, 70);
	ui.tableWidget_organizacii->setColumnWidth(4, 300);
	ui.tableWidget_organizacii->setColumnWidth(5, 60);
	ui.tableWidget_organizacii->setColumnWidth(6, 200);
	ui.tableWidget_organizacii->setColumnWidth(7, 60);
	ui.tableWidget_organizacii->setColumnWidth(8, 100);
	populateTable(true);
	
	connect(ui.pushButton_selectAll, SIGNAL(clicked ()), this, SLOT(pushButton_selectAll_clicked()));
	connect(ui.pushButton_deselectAll, SIGNAL(clicked ()), this, SLOT(pushButton_deselectAll_clicked()));
	connect(ui.pushButton_addRow, SIGNAL(clicked ()), this, SLOT(pushButton_addRow_clicked()));
	connect(ui.pushButton_deleteRow, SIGNAL(clicked ()), this, SLOT(pushButton_deleteRow_clicked()));
	connect(ui.pushButton_readSumyOplat, SIGNAL(clicked ()), this, SLOT(pushButton_readSumyOplat_clicked()));
	connect(ui.pushButton_createCurPlatijka, SIGNAL(clicked ()), this, SLOT(pushButton_createCurPlatijka_clicked()));
	connect(ui.pushButton_createSelectedPlatijky, SIGNAL(clicked ()), this, SLOT(pushButton_createSelectedPlatijky_clicked()));
	
	connect(ui.tableWidget_organizacii->selectionModel(), SIGNAL(currentRowChanged (const QModelIndex &,const QModelIndex &)), this, SLOT(tableWidget_currentRowChanged()));
	
	pushButton_readSumyOplat_clicked();
	tableWidget_currentRowChanged();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::pushButton_selectAll_clicked()
{
	int rowCount = ui.tableWidget_organizacii->rowCount();
	for(int row=0;row<rowCount; ++row)
		if (QCheckBox *chBox = qobject_cast<QCheckBox *>(ui.tableWidget_organizacii->cellWidget(row, 1)))
			chBox->setChecked(true);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::pushButton_deselectAll_clicked()
{
	int rowCount = ui.tableWidget_organizacii->rowCount();
	for(int row=0;row<rowCount; ++row)
		if (QCheckBox *chBox = qobject_cast<QCheckBox *>(ui.tableWidget_organizacii->cellWidget(row, 1)))
			chBox->setChecked(false);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::tableWidget_organizacii_itemChanged(QTableWidgetItem *item)
{
	disconnect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
	
	QString colName;
	int col = item->column();
	int id = item->tableWidget()->item(item->row(), 0)->data(Qt::DisplayRole).toInt();
	switch (col){
		case 2: colName="Pryznach"; break;
		case 4: colName="NazvaOrg"; break;
		case 5: colName="Edrpoy"; break;
		case 6: colName="Bank"; break;
		case 7: colName="MFO"; break;
		case 8: colName="BankRahunok"; break;
		default: colName="";
	}
	QSqlQuery query;
	query.exec("UPDATE platijkyInfo SET "+colName+"="+mysqlStr(item->text())+" WHERE id="+mysqlStr(id));
	query.exec("SELECT "+colName+" FROM platijkyInfo WHERE id="+mysqlStr(id)+" LIMIT 1");
	query.seek(0);
	item->setText(query.value(0).toString());
	
	connect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::tableWidget_currentRowChanged()
{
	if (ui.tableWidget_organizacii->item(ui.tableWidget_organizacii->currentRow(), ui.tableWidget_organizacii->columnCount()-1)->data(Qt::DisplayRole).toInt() != 1)
		ui.pushButton_deleteRow->setEnabled(true);
	else
		ui.pushButton_deleteRow->setEnabled(false);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::pushButton_addRow_clicked()
{
	disconnect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
	disconnect(ui.tableWidget_organizacii->selectionModel(), SIGNAL(currentRowChanged (const QModelIndex &,const QModelIndex &)), this, SLOT(tableWidget_currentRowChanged()));
	
	QSqlQuery query;
	query.exec("INSERT INTO platijkyInfo (NazvaOrg, Pryznach) VALUES ('Нова організація','Нове призначення')");
	query.exec("SELECT last_insert_id()");
	query.seek(0);
	int id = query.value(0).toInt();
	int row = ui.tableWidget_organizacii->rowCount();
	query.exec("SELECT * FROM platijkyInfo WHERE id="+mysqlStr(id)+" LIMIT 1");
	if(query.seek(0)){
		ui.tableWidget_organizacii->insertRow(row);
		populateRow(row, query.record());
	}
	QCheckBox *chBox = new QCheckBox("Друк", ui.tableWidget_organizacii);
	ui.tableWidget_organizacii->setCellWidget(row,1, chBox);
	chBox->setCheckable(true);
	chBox->setChecked(true);
	chBox->setFocusPolicy( Qt::NoFocus );
	ui.tableWidget_organizacii->resizeRowsToContents();
	ui.tableWidget_organizacii->setCurrentCell(row, 2);
	ui.tableWidget_organizacii->editItem(ui.tableWidget_organizacii->item(row,2));
	
	tableWidget_currentRowChanged();
	
	connect(ui.tableWidget_organizacii->selectionModel(), SIGNAL(currentRowChanged (const QModelIndex &,const QModelIndex &)), this, SLOT(tableWidget_currentRowChanged()));
	connect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::pushButton_deleteRow_clicked()
{
	disconnect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
	
	if (ui.tableWidget_organizacii->item(ui.tableWidget_organizacii->currentRow(), ui.tableWidget_organizacii->columnCount()-1)->data(Qt::DisplayRole).toInt() != 1){
		if (! QMessageBox::question(
                this,
                "!!! Видалення стрічки !!!",
                "Ви дійсно бажаєте видалити сирічку?",
                "Так",
				"Ні-ні-ні",
                QString(), 0, 1)){
			int id = ui.tableWidget_organizacii->item(ui.tableWidget_organizacii->currentRow(), 0)->data(Qt::DisplayRole).toInt(); 
			QSqlQuery query;
			if (query.exec("DELETE FROM platijkyInfo WHERE id="+mysqlStr(id)+" LIMIT 1")){
				ui.tableWidget_organizacii->removeRow(ui.tableWidget_organizacii->currentRow());
			}
		}
	}
	else{
		QMessageBox::information(0,"Заборонено видаляти стрічку","Цю стрічку заборонено видаляти");
	}
	connect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::pushButton_readSumyOplat_clicked()
{
	disconnect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
	
	QSqlQuery query;
	query.exec("SELECT sum(U_PensFond), sum(U_Fond_zainat), sum(U_SocStrah), sum(U_Prybutkov_podatok), \
						sum(V_PensFond), sum(V_SocStrah), sum(V_Fond_zainat), sum(V_NeshchVyp) \
				FROM operacii \
				WHERE year(CurDate)="+mysqlStr(ui.dateEdit_pererahunokZaMisyac->date().year())+" \
					and month(CurDate)="+mysqlStr(ui.dateEdit_pererahunokZaMisyac->date().month()));
	query.seek(0);
	populateItem(0, 3, m_to_str_2(query.value(0).toDouble()));
	populateItem(1, 3, m_to_str_2(query.value(1).toDouble()));
	populateItem(2, 3, m_to_str_2(query.value(2).toDouble()));
	populateItem(3, 3, m_to_str_2(query.value(3).toDouble()));
	populateItem(4, 3, m_to_str_2(query.value(4).toDouble()));
	populateItem(5, 3, m_to_str_2(query.value(5).toDouble()));
	populateItem(6, 3, m_to_str_2(query.value(6).toDouble()));
	populateItem(7, 3, m_to_str_2(query.value(7).toDouble()));
	
	connect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::pushButton_createCurPlatijka_clicked()
{
	int row = ui.tableWidget_organizacii->currentRow();
	UPlatijkyInfo p;
	p.date = ui.dateEdit_platijkaDate->date();
	p.oderjuvach = ui.tableWidget_organizacii->item(row, 4)->text();
	p.oderjuvachEdrpou = ui.tableWidget_organizacii->item(row, 5)->text();
	p.oderjuvachBank = ui.tableWidget_organizacii->item(row, 6)->text();
	p.oderjuvachMFO = ui.tableWidget_organizacii->item(row, 7)->text();
	p.oderjuvachRahunok = ui.tableWidget_organizacii->item(row, 8)->text();
	p.pryznachennyaPlateju = ui.tableWidget_organizacii->item(row, 2)->text();
	p.suma = ui.tableWidget_organizacii->item(row, 3)->data(Qt::DisplayRole).toDouble();
	p.platnyk = OrganizShortName();
	p.platnykEdrpou = OrganizEDRPOU();
	p.platnykBank = OrganizBank();
	p.platnykMFO = OrganizBankMFO();
	p.platnykRahunok = OrganizBankRahunok();
	p.platijkaNum = ui.spinBox_beginPlatijkaNumber->text();
	
	printDocs = new UPrintDocs();
	printDocs->print_onePlatijka(p);
	connect(this, SIGNAL(closeDockWindows()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::pushButton_createSelectedPlatijky_clicked()
{
	int rowCount = ui.tableWidget_organizacii->rowCount();
	printDocs = new UPrintDocs();
	QTextDocument *doc = new QTextDocument;
	QTextCursor cursor(doc);
	
	UPlatijkyInfo p;
	p.date = ui.dateEdit_platijkaDate->date();
	p.platnyk = OrganizShortName();
	p.platnykEdrpou = OrganizEDRPOU();
	p.platnykBank = OrganizBank();
	p.platnykMFO = OrganizBankMFO();
	p.platnykRahunok = OrganizBankRahunok();
	p.platijkaNum = ui.spinBox_beginPlatijkaNumber->text();
	QList<int> rowsList;
	int row;
	for (row=0; row<rowCount; ++row){
		if (QCheckBox *chBox = qobject_cast<QCheckBox *>(ui.tableWidget_organizacii->cellWidget(row, 1))){
			if (chBox->isChecked())
				rowsList << row;
		}
	}
	rowCount = rowsList.size();
	int curRow=0;
	foreach(row, rowsList){
		curRow++;
		p.oderjuvach = ui.tableWidget_organizacii->item(row, 4)->text();
		p.oderjuvachEdrpou = ui.tableWidget_organizacii->item(row, 5)->text();
		p.oderjuvachBank = ui.tableWidget_organizacii->item(row, 6)->text();
		p.oderjuvachMFO = ui.tableWidget_organizacii->item(row, 7)->text();
		p.oderjuvachRahunok = ui.tableWidget_organizacii->item(row, 8)->text();
		p.pryznachennyaPlateju = ui.tableWidget_organizacii->item(row, 2)->text();
		p.suma = ui.tableWidget_organizacii->item(row, 3)->data(Qt::DisplayRole).toDouble();
		
		QTextFormat::PageBreakFlag bf;
		if (curRow == rowCount)
			bf = QTextFormat::PageBreak_Auto;
		else
			bf = QTextFormat::PageBreak_AlwaysAfter;
		cursor.insertFragment(printDocs->createPlatijkaFragment(p, bf));
	}
	QTextDocumentFragment fragment(doc);
	delete doc;
	printDocs->printFragment(fragment);
	printDocs->printform->printer()->setPageMargins( 5, 5, 5, 1, QPrinter::Millimeter );
	connect(this, SIGNAL(closeDockWindows()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::populateTable(bool allChecked)
{
	disconnect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
	int row;
	QSqlQuery *query = new QSqlQuery;
	query->exec("SELECT count(*) FROM platijkyInfo");
	query->seek(0);
	int rowCount = query->value(0).toInt();
	ui.tableWidget_organizacii->setRowCount(rowCount);
	query->exec("SELECT * FROM platijkyInfo ORDER BY id");
	for(row=0; row<rowCount && query->next(); ++row){
		populateRow(row, query->record());
	}
	QCheckBox *chBox;
	for(row=0;row<rowCount; ++row){
		if (!(chBox = qobject_cast<QCheckBox *>(ui.tableWidget_organizacii->cellWidget(row, 1)))){
			chBox = new QCheckBox("Друк", ui.tableWidget_organizacii);
			ui.tableWidget_organizacii->setCellWidget(row,1, chBox);
			if (allChecked){
				chBox->setCheckable(true);
				chBox->setChecked(true);
				chBox->setFocusPolicy( Qt::NoFocus );
			}
		}
	}
	if (allChecked && (ui.tableWidget_organizacii->rowCount()!=0)){
		ui.tableWidget_organizacii->setCurrentCell(0, 1);
		ui.tableWidget_organizacii->scrollToItem(ui.tableWidget_organizacii->item(0,0), QAbstractItemView::PositionAtTop);
	}
	ui.tableWidget_organizacii->resizeRowsToContents();
	delete query;
	connect(ui.tableWidget_organizacii, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableWidget_organizacii_itemChanged(QTableWidgetItem*)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::populateRow(int row, QSqlRecord rec)
{
	populateItem(row, 0, rec.value("id").toString());
	populateItem(row, 2, rec.value("Pryznach").toString());
	populateItem(row, 3, "0,00");
	populateItem(row, 4, rec.value("NazvaOrg").toString());
	populateItem(row, 5, rec.value("Edrpoy").toString());
	populateItem(row, 6, rec.value("Bank").toString());
	populateItem(row, 7, rec.value("MFO").toString());
	populateItem(row, 8, rec.value("BankRahunok").toString());
	if (rec.value("ReadOnly").toBool())
		populateItem(row, 9, "1");
	else
		populateItem(row, 9, "0");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UWidgetPrintPlatijky::populateItem(int row, int col, QString str, Qt::ItemFlags editFlag)
{
	QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(str);
	tableWidgetItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | editFlag);
	if (col == 2)
		tableWidgetItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	else if (col == 3)
		tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	else if (col == 4)
		tableWidgetItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	else if (col == 5)
		tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	else if (col == 6)
		tableWidgetItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	else if (col == 7)
		tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	else if (col == 8)
		tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	else
		tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	ui.tableWidget_organizacii->setItem(row, col, tableWidgetItem);
}
//--------------------utech--------------------utech--------------------utech--------------------
UWidgetPrintPlatijky::~UWidgetPrintPlatijky()
{
	emit closeDockWindows();
}
//--------------------utech--------------------utech--------------------utech--------------------
