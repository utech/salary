//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна налаштування базавого місяця індексації
//
// Створений програмістами Компанії "Утех" 25.03.2009р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UDialogIndexaciyaType.h"

#include <QAbstractItemModel>
#include <USqlAccessible>
#include <QtDebug>

#include <QMessageBox>
//--------------------utech--------------------utech--------------------utech--------------------
UDialogIndexaciyaType::UDialogIndexaciyaType(short year, bool showOnlyWorkedWorkers, QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	ui.tableWidget_indexType->addAction(ui.action_basingScriptDefinition);
	ui.tableWidget_indexType->addAction(ui.action_baseMonth);
	ui.tableWidget_indexType->addAction(ui.action_noBaseMonth);
	ui.tableWidget_indexType->addAction(ui.action_basingScriptDefinitionAll);
	ui.tableWidget_indexType->addAction(ui.action_baseMonthAll);
	ui.tableWidget_indexType->addAction(ui.action_noBaseMonthAll);
	ui.tableWidget_indexType->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	onlyWorked = showOnlyWorkedWorkers;
	ui.spinBox_curYear->setValue(year);
	
	ui.tableWidget_indexType->setColumnHidden(0, true);
	
	ui.tableWidget_indexType->setColumnWidth(1, 60);
	ui.tableWidget_indexType->setColumnWidth(2, 60);
	ui.tableWidget_indexType->setColumnWidth(3, 60);
	ui.tableWidget_indexType->setColumnWidth(4, 60);
	ui.tableWidget_indexType->setColumnWidth(5, 60);
	ui.tableWidget_indexType->setColumnWidth(6, 60);
	ui.tableWidget_indexType->setColumnWidth(7, 60);
	ui.tableWidget_indexType->setColumnWidth(8, 60);
	ui.tableWidget_indexType->setColumnWidth(9, 60);
	ui.tableWidget_indexType->setColumnWidth(10, 60);
	ui.tableWidget_indexType->setColumnWidth(11, 60);
	ui.tableWidget_indexType->setColumnWidth(12, 60);
	
	ui.tableWidget_indexType->setItemDelegateForColumn( 1, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 2, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 3, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 4, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 5, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 6, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 7, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 8, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn( 9, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn(10, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn(11, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	ui.tableWidget_indexType->setItemDelegateForColumn(12, new UIndexaciyaTypeDelegate(ui.tableWidget_indexType));
	
	populateIndexTable();
	
	connect(ui.toolButton_prevYear, SIGNAL(clicked ()), ui.spinBox_curYear, SLOT(stepDown()));
	connect(ui.toolButton_nextYear, SIGNAL(clicked ()), ui.spinBox_curYear, SLOT(stepUp()));
	connect(ui.spinBox_curYear, SIGNAL(valueChanged(int)), this, SLOT(populateIndexTable()));
	
	connect(ui.action_basingScriptDefinition, SIGNAL(activated()), this, SLOT(action_basingScriptDefinition_activated()));
	connect(ui.action_baseMonth, SIGNAL(activated()), this, SLOT(action_baseMonth_activated()));
	connect(ui.action_noBaseMonth, SIGNAL(activated()), this, SLOT(action_noBaseMonth_activated()));
	connect(ui.action_basingScriptDefinitionAll, SIGNAL(activated()), this, SLOT(action_basingScriptDefinitionAll_activated()));
	connect(ui.action_baseMonthAll, SIGNAL(activated()), this, SLOT(action_baseMonthAll_activated()));
	connect(ui.action_noBaseMonthAll, SIGNAL(activated()), this, SLOT(action_noBaseMonthAll_activated()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::populateIndexTable()
{
	disconnect(ui.tableWidget_indexType, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveData(QTableWidgetItem *)));
	
	QString filtrStr="";
	QAbstractItemModel *model = ui.tableWidget_indexType->model();
	if (onlyWorked){
		filtrStr=" WHERE Pracuye=true ";
	}
	QSqlQuery q, nprQuery;
	int rowCount=0;
	nprQuery.exec("SELECT count(*) FROM npr "+filtrStr);
	nprQuery.next();
	rowCount = nprQuery.value(0).toInt();
	ui.tableWidget_indexType->setRowCount(rowCount);
	nprQuery.exec("SELECT id, Prizv, Imia, Pobatk FROM npr "+filtrStr+" ORDER BY Prizv, Imia, Pobatk");
	QStringList headerList;
	int row=0;
	
	while (nprQuery.next()){
		model->setData( model->index(row, 0), nprQuery.value(0).toInt(), Qt::EditRole );
		headerList << nprQuery.value(1).toString() + " " + nprQuery.value(2).toString() + " " + nprQuery.value(3).toString();
		for (int i=0; i<12; i++)
			model->setData( model->index(row, i+1), 0, Qt::EditRole );
		q.exec("SELECT month(CurDate), Status FROM statusIndexaciiPracivnykiv \
				WHERE Npr_id="+nprQuery.value(0).toString()+" and year(CurDate)="+sqlStr(ui.spinBox_curYear->value()));
		while (q.next())
			model->setData( model->index(row, q.value(0).toInt()), q.value(1).toInt(), Qt::EditRole );
		row++;
	}
	
	ui.tableWidget_indexType->setVerticalHeaderLabels(headerList);
	ui.tableWidget_indexType->resizeRowsToContents();
	
	connect(ui.tableWidget_indexType, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveData(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::saveData(QTableWidgetItem * item)
{
	disconnect(ui.tableWidget_indexType, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveData(QTableWidgetItem *)));
	
	int row = item->row();
	int column = item->column();
	QString filtrString = " WHERE Npr_id="+mysqlStr(ui.tableWidget_indexType->item(row,0)->data(Qt::EditRole).toInt())+" \
					and year(CurDate)="+mysqlStr(ui.spinBox_curYear->value())+" \
					and month(CurDate)="+mysqlStr(column);
	QSqlQuery query;
	query.exec("SELECT count(*) FROM statusIndexaciiPracivnykiv "+filtrString);
	if (query.seek(0) && (query.value(0).toInt()>0)){
		query.exec("UPDATE statusIndexaciiPracivnykiv SET Status="+mysqlStr(item->data(Qt::EditRole).toInt()) + filtrString);
		qDebug() << "update: " << query.lastQuery();
	}
	else{
		query.exec("INSERT INTO statusIndexaciiPracivnykiv (Npr_id, CurDate, Status) VALUES \
					( "+mysqlStr(ui.tableWidget_indexType->item(row,0)->data(Qt::EditRole).toInt())+", \
					"+mysqlStr(QDate(ui.spinBox_curYear->value(),column,1))+", \
					"+mysqlStr(item->data(Qt::EditRole).toInt())+")");
		qDebug() << "insert: " << query.lastQuery();
	}
	query.exec("SELECT Status FROM statusIndexaciiPracivnykiv "+filtrString );
	query.seek(0);
	item->setData(Qt::EditRole, query.value(0).toInt());
	
	connect(ui.tableWidget_indexType, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(saveData(QTableWidgetItem *)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::action_basingScriptDefinition_activated()
{
	if (ui.tableWidget_indexType->rowCount() == 0)
		return;
	
	QTableWidgetItem *item = ui.tableWidget_indexType->currentItem();
	item->setData(Qt::EditRole, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::action_baseMonth_activated()
{
	if (ui.tableWidget_indexType->rowCount() == 0)
		return;
	
	QTableWidgetItem *item = ui.tableWidget_indexType->currentItem();
	item->setData(Qt::EditRole, 1);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::action_noBaseMonth_activated()
{
	if (ui.tableWidget_indexType->rowCount() == 0)
		return;
	
	QTableWidgetItem *item = ui.tableWidget_indexType->currentItem();
	item->setData(Qt::EditRole, 2);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::action_basingScriptDefinitionAll_activated()
{
	if (ui.tableWidget_indexType->rowCount() == 0)
		return;
	
	QTableWidgetItem *item = ui.tableWidget_indexType->currentItem();
	int col = item->column();
	for (int row=0; row<ui.tableWidget_indexType->rowCount(); row++)
		ui.tableWidget_indexType->item(row, col)->setData(Qt::EditRole, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::action_baseMonthAll_activated()
{
	if (ui.tableWidget_indexType->rowCount() == 0)
		return;
	
	QTableWidgetItem *item = ui.tableWidget_indexType->currentItem();
	int col = item->column();
	for (int row=0; row<ui.tableWidget_indexType->rowCount(); row++)
		ui.tableWidget_indexType->item(row, col)->setData(Qt::EditRole, 1);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogIndexaciyaType::action_noBaseMonthAll_activated()
{
	if (ui.tableWidget_indexType->rowCount() == 0)
		return;
	
	QTableWidgetItem *item = ui.tableWidget_indexType->currentItem();
	int col = item->column();
	for (int row=0; row<ui.tableWidget_indexType->rowCount(); row++)
		ui.tableWidget_indexType->item(row, col)->setData(Qt::EditRole, 2);
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogIndexaciyaType::~UDialogIndexaciyaType()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
//--------------------utech--------------------utech--------------------utech--------------------
UIndexaciyaTypeDelegate::UIndexaciyaTypeDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	strList << "Скрипт" << "Базов." << "Звич.";
	strCount = strList.size();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UIndexaciyaTypeDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
	QStyleOptionViewItemV3 opt = setOptions(index, option);
	drawBackground(painter, opt, index);
	int val=index.data(Qt::EditRole).toInt();
	if (!(val < strCount))
		val = 0;
	drawDisplay(painter, opt, opt.rect, strList[val]);
	drawFocus(painter, opt, opt.rect);
}
//--------------------utech--------------------utech--------------------utech--------------------
QWidget *UIndexaciyaTypeDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    QComboBox *cBox = new QComboBox(parent);
	cBox->addItems(strList);
		
    connect(cBox, SIGNAL(editingFinished()),
                this, SLOT(QItemDelegate::commitAndCloseEditor()));
    return cBox;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UIndexaciyaTypeDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
	int num = index.model()->data(index, Qt::EditRole).toInt();
    QComboBox *cBox = qobject_cast<QComboBox *>(editor);
    cBox->setCurrentIndex(num);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UIndexaciyaTypeDelegate::setModelData(QWidget *editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QComboBox *cBox = qobject_cast<QComboBox *>(editor);
    int num = cBox->currentIndex();
    model->setData(index, num, Qt::EditRole);
}
//--------------------utech--------------------utech--------------------utech--------------------
