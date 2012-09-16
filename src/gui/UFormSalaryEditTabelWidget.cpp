//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна налаштування програми
//
// Створений програмістами Компанії "Утех" 23.03.2010р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UFormSalaryEditTabelWidget.h"
#include "UDialogSalaryEditTabelCell.h"
#include <UTableWidgetSqlExt>
#include <USqlAccessible>

#include <QtDebug>

#include "UPrintDocs.h"

//--------------------utech--------------------utech--------------------utech--------------------
UFormSalaryEditTabelWidget::UFormSalaryEditTabelWidget(QWidget *parent)
        : QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	
	ui.tableWidget_tabel->addAction(ui.action_reloadTabel);
	ui.tableWidget_tabel->addAction(ui.action_printTabel);
	ui.tableWidget_tabel->setContextMenuPolicy(Qt::ActionsContextMenu);
	ui.tableWidget_tabel->setItemDelegate(new USalaryTabelDayDelegate(ui.tableWidget_tabel));
	
	//Заповнення комбо вибору підрозділу
	QSqlQuery query;
	query.exec("SELECT id, Name FROM pidrozdily_salary ORDER BY Name");
	//ui.comboBox_pidrozdil->addItem("--- УСІ ---", 0);
	while (query.next())
		ui.comboBox_pidrozdil->addItem(query.value(1).toString(), query.value(0));
	ui.comboBox_pidrozdil->addItem("--- Без підрозділу ---", 0);
	
	new UIntEditingComboBox(ui.comboBox_nprFilter);
	
	ui.dateEdit_tabelDate->setDate(QDate::currentDate());
	
	//QMessageBox::information(0, "Мітка", "Мітка");
	populateTabel();
	
	connect(ui.dateEdit_tabelDate, SIGNAL(dateChanged(const QDate &)), this,SLOT(populateTabel()));
	connect(ui.comboBox_pidrozdil, SIGNAL(currentIndexChanged(int)), this,SLOT(populateTabel()));
	connect(ui.pushButton_reloadTabel, SIGNAL(clicked()), this,SLOT(populateTabel()));
	connect(ui.action_reloadTabel, SIGNAL(activated()), this,SLOT(populateTabel()));
	connect(ui.pushButton_findCell, SIGNAL(clicked()), this,SLOT(pushButton_findCell_clicked()));
	connect(ui.tableWidget_tabel, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), 
			this,SLOT(tableWidget_tabel_itemDoubleClicked(QTableWidgetItem *)));
	connect(ui.action_printTabel, SIGNAL(activated()), this,SLOT(printTabel()));
	connect(ui.pushButton_printTabel, SIGNAL(clicked()), this,SLOT(printTabel()));
}
//--------------------utech--------------------utech--------------------utech--------------------
UFormSalaryEditTabelWidget::~UFormSalaryEditTabelWidget()
{
	emit mainWindowClosed();
}
//--------------------utech--------------------utech--------------------utech--------------------
inline int UFormSalaryEditTabelWidget::currentPidrozdil()
{
	return ui.comboBox_pidrozdil->itemData(ui.comboBox_pidrozdil->currentIndex(), Qt::UserRole).toInt();
}
//--------------------utech--------------------utech--------------------utech--------------------
//Заповнює поля таблиці табеля з таблиці табеля бази даних 
void UFormSalaryEditTabelWidget::populateTabel()
{
	disconnect(ui.tableWidget_tabel->selectionModel(), SIGNAL(currentRowChanged( const QModelIndex &, const QModelIndex &)), 
				this, SLOT(sumTabelCurRowChanged( const QModelIndex &, const QModelIndex &)));
	
	QTableWidgetItem *item;
	QSqlQuery query;
	QString str;
	QDate begMonthDate(ui.dateEdit_tabelDate->date().year(), ui.dateEdit_tabelDate->date().month(),1), endMonthDate;
	endMonthDate.setDate(begMonthDate.year(), begMonthDate.month(), begMonthDate.daysInMonth());
	ui.dateEdit_dateFilter->setDate(begMonthDate);
	
	QString pidrozdilFilter = currentPidrozdil()?QString("Pidrozdil_id=")+sqlStr(currentPidrozdil()):
							QString(" Pidrozdil_id not in (SELECT id FROM pidrozdily_salary) ");
	
	//Заповнення комбо пошуку працівника
	ui.comboBox_nprFilter->clear();
	ui.comboBox_nprFilter->addItem("",0);
	query.exec("SELECT id, Prizv, Imia, Pobatk FROM npr \
				WHERE "+pidrozdilFilter+" \
				ORDER BY Prizv, Imia, Pobatk");
	while (query.next())
		ui.comboBox_nprFilter->addItem("["+query.value(0).toString()+"] "+query.value(1).toString()+" "+
					query.value(2).toString()+" "+query.value(3).toString(), query.value(0));
	
	//створення переліку індексів святкових днів
	QSet<int> svIndex;
	query.exec("SELECT CDate FROM svyatkovidni \
				WHERE CDate BETWEEN date("+sqlStr(begMonthDate)+") \
					and date("+sqlStr(endMonthDate)+")");
	while (query.next())
		svIndex << begMonthDate.daysTo(query.value(0).toDate());
	
	// Заповнення вертикального заголовка
	query.exec("SELECT count(*) FROM npr WHERE "+pidrozdilFilter);
	query.next();
	int rowCount = query.value(0).toInt(), row;
	ui.tableWidget_tabel->setRowCount(rowCount);
	query.exec("SELECT id, Prizv, Imia, Pobatk FROM npr \
				WHERE "+pidrozdilFilter+" \
				ORDER BY Prizv, Imia, Pobatk");
	for (row=0; row<rowCount && query.next(); row++){
		str = query.value(1).toString();
		if (query.value(2).toString().size()>0){
			str += QString(" ") + query.value(2).toString().at(0) + QString(".");
			if (query.value(3).toString().size()>0)
				str += QString(" ") + query.value(3).toString().at(0) + QString(".");
		}
		str += " ["+query.value(0).toString()+"]";
		item = new QTableWidgetItem();
		item->setData(Qt::DisplayRole, str);
		item->setData(Qt::UserRole, query.value(0));
		ui.tableWidget_tabel->setVerticalHeaderItem(row, item);
	}
	
	// Заповнення горизонтального заголовка
	ui.tableWidget_tabel->setColumnCount(begMonthDate.daysTo(endMonthDate)+1);
	int col=0;
	for (QDate xDate=begMonthDate; xDate<=endMonthDate; xDate=xDate.addDays(1)){
		item = new QTableWidgetItem();
		item->setData(Qt::DisplayRole, xDate.toString("dd.MM,ddd"));
		item->setData(Qt::UserRole, xDate);
		ui.tableWidget_tabel->setHorizontalHeaderItem(col, item);
		col++;
	}
	
	//створення усіх QTableWidgetItem
	for (row=0; row<rowCount; row++)
		for (col=0; col<ui.tableWidget_tabel->columnCount(); col++){
			item = new QTableWidgetItem();
			if (svIndex.contains(col))
				item->setData(Qt::BackgroundRole, QBrush(QColor("#e6ffa8")));
			ui.tableWidget_tabel->setItem(row,col, item);
			
		}
	
	USalaryTabelDay tabDay;
	QVariant variant;
	for (row=0; row<rowCount; row++){
		query.exec("SELECT Cur_Date, Pracivn_status_ID, God_zag, God_nichni, God_sv FROM tabel \
					WHERE Npr_id="+sqlStr(ui.tableWidget_tabel->verticalHeaderItem(row)->data(Qt::UserRole).toInt())+" \
						and (Cur_Date BETWEEN date("+sqlStr(begMonthDate)+") \
							AND date("+sqlStr(endMonthDate)+"))");
		while (query.next()){
			
			col = begMonthDate.daysTo(query.value(0).toDate());
			tabDay.workType = query.value(1).toInt();
			tabDay.rob = query.value(2).toTime();
			tabDay.nich = query.value(3).toTime();
			tabDay.sv = query.value(4).toTime();
			item = ui.tableWidget_tabel->item(row, col);
			variant.setValue(tabDay);
			item->setData(Qt::EditRole, variant);
		}
	}
	
	ui.tableWidget_tabel->resizeColumnsToContents();
	ui.tableWidget_tabel->resizeRowsToContents();
	
	connect(ui.tableWidget_tabel->selectionModel(), SIGNAL(currentRowChanged( const QModelIndex &, const QModelIndex &)), 
				this, SLOT(sumTabelCurRowChanged( const QModelIndex &, const QModelIndex &)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryEditTabelWidget::tableWidget_tabel_itemDoubleClicked(QTableWidgetItem *item)
{
	UDialogSalaryEditTabelCell d(ui.tableWidget_tabel->verticalHeaderItem(item->row())->data(Qt::UserRole).toInt(),
								ui.tableWidget_tabel->horizontalHeaderItem(item->column())->data(Qt::UserRole).toDate(),
								item,
								0);
	d.exec();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryEditTabelWidget::pushButton_findCell_clicked()
{
	QDate begMonthDate(ui.dateEdit_tabelDate->date().year(), ui.dateEdit_tabelDate->date().month(),1), endMonthDate;
	endMonthDate.setDate(begMonthDate.year(), begMonthDate.month(), begMonthDate.daysInMonth());
	
	int vId = ui.comboBox_nprFilter->itemData(ui.comboBox_nprFilter->currentIndex(), Qt::UserRole).toInt();
	ui.comboBox_nprFilter->setCurrentIndex(0);
	int col = begMonthDate.daysTo(ui.dateEdit_dateFilter->date());
	if (col < 0)
		col = 0;
	if (col > ui.tableWidget_tabel->columnCount()-1)
		col = ui.tableWidget_tabel->columnCount()-1;
	
	for (int row=0; row<ui.tableWidget_tabel->rowCount(); row++){
		if (ui.tableWidget_tabel->verticalHeaderItem(row)->data(Qt::UserRole).toInt() == vId){
			QTableWidgetItem *item = ui.tableWidget_tabel->item(row, col);
			ui.tableWidget_tabel->setCurrentItem(item);
			ui.tableWidget_tabel->scrollToItem(item);
			ui.tableWidget_tabel->setFocus();
			return;
		}
	}
	QMessageBox::information(0,"Пошук завершено","Програмі не вдалося знайти працівника з табельним номером "+QString::number(vId));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryEditTabelWidget::printTabel()
{
	UPrintDocs *printDocs = new UPrintDocs();
	printDocs->print_tabel_for_pidrozdil(ui.dateEdit_tabelDate->date().year(),ui.dateEdit_tabelDate->date().month(),
										currentPidrozdil());
	connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UFormSalaryEditTabelWidget::sumTabelCurRowChanged( const QModelIndex & curIndex, const QModelIndex & prevIndex)
{
	if (ui.tableWidget_tabel->rowCount() == 0)
		return;
	emit currentWorkerChanged(ui.tableWidget_tabel->verticalHeaderItem(curIndex.row())->data(Qt::UserRole).toInt());
}
//--------------------utech--------------------utech--------------------utech--------------------

/*
Малювання комірки таблиці табеля. Схема комірки в файлі tabelTableCell_1.dia,
який знаходиться в кореневій папці dia
*/
#include <QFont>

USalaryTabelDayDelegate::USalaryTabelDayDelegate(QObject *parent)
	: QItemDelegate(parent), mergin1(4), mergin2(2), spacingV(0), spacingH(1)
{
	font1.setFamily("Impact");
	font1.setPointSize(11);
	font2.setFamily("Arial");
	font2.setPointSize(9);
	
	QFontMetrics fm1(font1), fm2(font2);
	
	width1 = fm1.width("O");
	height1 = fm1.height();
	width2 = fm2.width("0");
	height2 = fm2.height();
	
	QSqlQuery query("SELECT id, Status_char, Status_color FROM pracivn_status");
	QColor color;
	int id;
	while (query.next()){
		id = query.value(0).toInt();
		color = QColor(query.value(2).toString());
		tabelChars.insert(id, query.value(1).toString());
		tabelColors.insert(id, color.isValid()?color:QColor(Qt::darkGray));
	}
}

void USalaryTabelDayDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt = setOptions(index, option);
	QRect xRect;
	USalaryTabelDay d = index.data(Qt::EditRole).value<USalaryTabelDay>();
	
	drawBackground(painter, opt, index);
	
	//Малювання букви
	painter->setPen(tabelColors.value(d.workType));
	painter->setFont(font1);
	xRect.setRect(opt.rect.x()+mergin1, opt.rect.y()+mergin1, width1*2, height1);
	painter->drawText(xRect, Qt::AlignLeft | Qt::AlignTop, tabelChars.value(d.workType));
	
	//Малювання стрічок часу
	if ((opt.state & QStyle::State_Selected) != QStyle::State_Selected)
		painter->setPen(opt.palette.text().color());
	else
		painter->setPen(opt.palette.highlightedText().color());
	
	painter->setFont(font2);
	xRect.setRect(opt.rect.x()+mergin1+width1*2+spacingH, opt.rect.y()+mergin2, width2*5, height2);
	painter->drawText(xRect, Qt::AlignRight | Qt::AlignTop, (d.rob>QTime(0,0)?d.rob.toString("h:mm"):"-"));
	
	xRect.setRect(opt.rect.x()+mergin1+width1*2+spacingH, opt.rect.y()+mergin2+height2+spacingV, width2*5, height2);
	painter->drawText(xRect, Qt::AlignRight | Qt::AlignTop, (d.nich>QTime(0,0)?d.nich.toString("h:mm"):"-"));
	
	xRect.setRect(opt.rect.x()+mergin1+width1*2+spacingH, opt.rect.y()+mergin2+2*height2+2*spacingV, width2*5, height2);
	painter->drawText(xRect, Qt::AlignRight | Qt::AlignTop, (d.sv>QTime(0,0)?d.sv.toString("h:mm"):"-"));
	
	//drawDisplay(painter, opt, opt.rect, str);
	drawFocus(painter, opt, opt.rect);
}

QSize USalaryTabelDayDelegate::sizeHint(const QStyleOptionViewItem & option,const QModelIndex & index) const
{
	return QSize(mergin1+width1*2+spacingH+5*width2+mergin2, 2*mergin2+3*height2+2*spacingV);
}
