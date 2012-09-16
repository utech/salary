//--------------------utech--------------------utech--------------------utech--------------------
//
// Implamantation of UDialogYMFilter class.
// Created by Roman Mulyk 12.09.2006
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGYMFILTER_H
#define UDIALOGYMFILTER_H

#include <QDialog>
#include <QDate>
#include <QtDebug>

#include "ui_UDialogYMFilter.h"
//--------------------utech--------------------utech--------------------utech--------------------
enum UYMDialogType{
	UMonthYearDialog,
	UKvartalYearDialog
};

//--------------------utech--------------------utech--------------------utech--------------------
class UDialogYMFilter : public QDialog
{
        Q_OBJECT

    public:
        UDialogYMFilter(QWidget *parent = 0, QString title="", short year=2006, short month=10, short dialogId=UMonthYearDialog);
		
		int get_Month();
		int get_Year();
		
		~UDialogYMFilter();
    public slots:


    private:
		Ui::UDialogYMFilter ui;
		
		QDate date;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
