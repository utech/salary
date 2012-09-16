//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна інформації про підприємство
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "QtDebug"
#include "UStandardDelegate"

#include "UDialogSalarySvyatkoviDni.h"

//--------------------utech--------------------utech--------------------utech--------------------
UDialogSalarySvyatkoviDni::UDialogSalarySvyatkoviDni(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	ui.spinBox_yearFilter->setValue(QDate::currentDate().year());
	
	ui.tableWidget_svyatkoviDni->setColumnCount(3);
	twExt = new UTableWidgetSqlExt(ui.tableWidget_svyatkoviDni, "svyatkovidni");
	twExt->setSortingEnabled(true);
	
	ui.tableWidget_svyatkoviDni->setHorizontalHeaderLabels( QStringList() 
															<< "id"
															<< "Свято"
															<< "Дата");
	twExt->colNamesList << "Id"
						<< "SvyatoName"
						<< "CDate";
	twExt->addColAlignment("SvyatoName", Qt::AlignVCenter | Qt::AlignLeft);
	twExt->addColAlignment("CDate", Qt::AlignVCenter | Qt::AlignRight);
	twExt->setItemDelegateForColumn("CDate", new UDateEditDelegate(QDate(2000,01,01), QDate(2010,12,31), ui.tableWidget_svyatkoviDni));
	ui.tableWidget_svyatkoviDni->setColumnHidden(0, true);
	twExt->setColumnWidth("SvyatoName", 250);
	twExt->setColumnWidth("CDate", 80);
	setFilterAndPopulate();
	
	connect(ui.pushButton_create, SIGNAL(clicked()), this, SLOT(pushButton_create_clicked()));
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(pushButton_delete_clicked()));
	connect(ui.pushButton_reload, SIGNAL(clicked()), this, SLOT(setFilterAndPopulate()));
	connect(ui.spinBox_yearFilter, SIGNAL(valueChanged(int)), this, SLOT(setFilterAndPopulate()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogSalarySvyatkoviDni::setFilterAndPopulate()
{
	twExt->setSqlFilter("year(CDate)="+sqlStr(ui.spinBox_yearFilter->value()));
	twExt->populateTable();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogSalarySvyatkoviDni::pushButton_create_clicked()
{
	if (!twExt->addRow())
		QMessageBox::critical(0,"Помилка створення запису","Запис шляхового листа не створено не створено");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogSalarySvyatkoviDni::pushButton_delete_clicked()
{
	if	( QMessageBox::question(
                this,
                "!!! Видалення стрічки(-ок) !!!",
                "Ви дійсно бажаєте видалити стрічку(-и)?",
                "Так",
				"Ні-ні-ні",
                QString(), 0, 1)
		)
		return;
	if (!twExt->deleteSelectedRows())
		QMessageBox::critical(0,"Помикла видалення стрічки(-ок)","Стрічку(-и) не видалено");
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogSalarySvyatkoviDni::~UDialogSalarySvyatkoviDni()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
