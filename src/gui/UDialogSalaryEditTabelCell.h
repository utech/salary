//--------------------utech--------------------utech--------------------utech--------------------
//
// Створений програмістами Компанії "Утех" 24.03.2010р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDialogSalaryEditTabelCell_H
#define UDialogSalaryEditTabelCell_H

#include <QDialog>
#include <QDate>
#include <QtDebug>
#include <QTableWidgetItem>

#include "ui_UDialogSalaryEditTabelCell.h"
//--------------------utech--------------------utech--------------------utech--------------------
class UDialogSalaryEditTabelCell : public QDialog
{
	Q_OBJECT

  public:
	UDialogSalaryEditTabelCell(int nprId, QDate dt, QTableWidgetItem* item, QWidget *parent = 0);
		
	~UDialogSalaryEditTabelCell();
  public slots:
	void pushButton_accept_clicked();
	void checkRobTime();
  private:
	Ui::UDialogSalaryEditTabelCell ui;
		
	QDate _dt;
	int _nprId;
	QTableWidgetItem *_item;
};
//--------------------utech--------------------utech--------------------utech--------------------
#endif
