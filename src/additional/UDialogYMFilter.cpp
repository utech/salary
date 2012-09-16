//------------------------------------------------------------
//
// Implamantation of UDialogYMFilter class.
// Created by Roman Mulyk 12.09.2006
//
//------------------------------------------------------------

#include "UDialogYMFilter.h"

//------------------------------------------------------------
UDialogYMFilter::UDialogYMFilter(QWidget *parent, QString title, short year, short month, short dialogId)
        : QDialog(parent)
{
    ui.setupUi(this);

	date = QDate::currentDate();
	
	if (date.month() == 12)
		ui.spinBox_year->setMaximum(date.year()+1);
	else
		ui.spinBox_year->setMaximum(date.year());
	
	if (dialogId == UMonthYearDialog){
		ui.label_month->setText("Ì³ñÿöü");
		ui.spinBox_month->setMinimum(1);
		ui.spinBox_month->setMaximum(12);
		ui.spinBox_month->setValue(month);
	}
	else if (dialogId == UKvartalYearDialog){
		ui.label_month->setText("Êâàðòàë");
		ui.spinBox_month->setMinimum(1);
		ui.spinBox_month->setMaximum(4);
		ui.spinBox_month->setValue(month/3);
	}
	
	ui.spinBox_year->setValue(year);
	
	ui.groupBox->parentWidget()->setWindowTitle( title);
	ui.groupBox->parentWidget()->setWindowFlags ( ui.groupBox->parentWidget()->windowFlags() | Qt::WindowStaysOnTopHint );
}
//------------------------------------------------------------
int UDialogYMFilter::get_Month()
{
	return ui.spinBox_month->value();
}
//------------------------------------------------------------
int UDialogYMFilter::get_Year()
{
	return ui.spinBox_year->value();
}
//------------------------------------------------------------
UDialogYMFilter::~UDialogYMFilter()
{

}
//------------------------------------------------------------
