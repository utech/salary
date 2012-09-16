//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ��������� ����� ���� ���������� ��� ����������
//
// ��������� ������������ ������ "����" 30.03.2008�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include <QtDebug>

#include "UDialogSalaryPidrozdily.h"
#include <UStandardDelegate>

//--------------------utech--------------------utech--------------------utech--------------------
UDialogSalaryPidrozdily::UDialogSalaryPidrozdily(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	
	ui.tableWidget_pidrozdily->setColumnCount(4);
	twExt = new UTableWidgetSqlExt(ui.tableWidget_pidrozdily, "pidrozdily_salary");
	twExt->setSortingEnabled(true);
	
	ui.tableWidget_pidrozdily->setHorizontalHeaderLabels( QStringList() 
															<< "id"
															<< "��"
															<< "�����"
															<< "��� ������");
	twExt->colNamesList << "Id"
						<< "Id"
						<< "Name"
						<< "TabelType";
	twExt->addColAlignment("Id", Qt::AlignVCenter | Qt::AlignRight);
	twExt->addColAlignment("Name", Qt::AlignVCenter | Qt::AlignLeft);
	twExt->addColAlignment("TabelType", Qt::AlignVCenter | Qt::AlignLeft);
	ui.tableWidget_pidrozdily->setItemDelegateForColumn(1, new USpinBoxDelegate(0, 100000, ui.tableWidget_pidrozdily));
	
	QSqlQuery query;
	UIntStringList delegateList;
		//������� ��� ������
	delegateList.clear();
	delegateList.insert(0, "����.����.�������");
	delegateList.insert(1, "������ ������");
	delegateList.insert(2, "ϳ�����. ������");
	twExt->setItemDelegateForColumn("TabelType", 
				new UIntStringListComboDelegate(ui.tableWidget_pidrozdily, delegateList));
	
	ui.tableWidget_pidrozdily->setColumnHidden(0, true);
	twExt->setColumnWidth("id", 50);
	twExt->setColumnWidth("Name", 250);
	twExt->setColumnWidth("TabelType", 100);
	twExt->populateTable();
	
	connect(ui.pushButton_create, SIGNAL(clicked()), this, SLOT(pushButton_create_clicked()));
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(pushButton_delete_clicked()));
	connect(ui.pushButton_reload, SIGNAL(clicked()), twExt, SLOT(populateTable()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogSalaryPidrozdily::pushButton_create_clicked()
{
	if (!twExt->addRow())
		QMessageBox::critical(0,"������� ��������� ������","����� ��������� ����� �� �������� �� ��������");
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogSalaryPidrozdily::pushButton_delete_clicked()
{
	if	( QMessageBox::question(
                this,
                "!!! ��������� ������(-��) !!!",
                "�� ����� ������ �������� ������(-�)?",
                "���",
				"ͳ-�-�",
                QString(), 0, 1)
		)
		return;
	if (!twExt->deleteSelectedRows())
		QMessageBox::critical(0,"������� ��������� ������(-��)","������(-�) �� ��������");
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogSalaryPidrozdily::~UDialogSalaryPidrozdily()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
