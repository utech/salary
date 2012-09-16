//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ��������� ����� ��������� ���� �������� ����������� ��������
//
// ��������� ������������ ������ "����" 17.07.2007�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UPeopleSalaryMainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFileDialog>
#include <QDesktopServices>

#include <QLocale>

#include "UDialogIndexaciyaType.h"

#include "../additional/UDialogYMFilter.h"
#include "../engine/USalaryScript.h"
#include "UDialogOptions.h"
#include "UDialogFirmInfo.h"
#include "UWidgetPrintPlatijky.h"
#include "../export/UZvit1df.h"
#include "UPrintDocs.h"
#include "UMonthlyBudjetniNormyForm.h"
#include "UDialogSalaryPidrozdily.h"
#include "UDialogSalarySvyatkoviDni.h"
#include "UFormSalaryEditTabelWidget.h"
#include "USalary.h"
#include "UPFUMonthPersonDbf.h"
#include "UDialogChoosePidrozdil_salary.h"
#include "UFormSalaryTabelSumWidget.h"
#include "UPFUEdynyiVnesokDbf.h"
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_debuger_activated()
{
	emit requestDebugWindow();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_tabel_activated()
{
	UFormSalaryTabelSumWidget *f = new UFormSalaryTabelSumWidget(ui.dateEdit_curDate->date(), 
											ui.checkBox_show_only_work_workers->isChecked());
	f->show();
	connect(this, SIGNAL(closeLoginSignal()), f, SLOT(close()));
	connect(this, SIGNAL(mainWindowClosed()), f, SLOT(close()));
	connect(this, SIGNAL(mainWindowClosed()), f, SIGNAL(mainWindowClosed()));
	connect(f, SIGNAL(currentWorkerChanged(int)), this, SLOT(showWorker(int)));
	connect(ui.checkBox_show_only_work_workers, SIGNAL(stateChanged(int)), f, SLOT(showOnlyWorkWorkersChanged(int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_fullTabel_activated()
{
	UFormSalaryEditTabelWidget *f = new UFormSalaryEditTabelWidget();
	connect(this, SIGNAL(closeLoginSignal()), f, SLOT(close()));
	connect(this, SIGNAL(mainWindowClosed()), f, SLOT(close()));
	connect(this, SIGNAL(mainWindowClosed()), f, SIGNAL(mainWindowClosed()));
	connect(f, SIGNAL(currentWorkerChanged(int)), this, SLOT(showWorker(int)));
	f->showMaximized();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_indexaciyaBaseMonth_activated()
{
	UDialogIndexaciyaType *d = new UDialogIndexaciyaType(workDate.year(), ui.checkBox_show_only_work_workers->isChecked());
	d->exec();
	delete d;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_posady_i_taryfy_activated()
{
	ui.dockWidget_taryfy_na_zarplatu->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_pidrozdily_activated()
{
	UDialogSalaryPidrozdily d;
	d.exec();
	populateNprComboBoxes();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_svyatkoviDni_activated()
{
	UDialogSalarySvyatkoviDni d;
	d.exec();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_infAboutFirm_activated()
{
	UDialogFirmInfo * firmInfo = new UDialogFirmInfo();
	firmInfo->exec();
	delete firmInfo;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_BudgetNorm_activated()
{
	dovidnyk = new Dovidnyk(":/pictures/pictures/education_economy_section.png","������� \"������� �����\"",BjydzNormu);
	dovidnyk->exec();
	delete dovidnyk;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_MeritOfYears_activated()
{
	dovidnyk = new Dovidnyk(":/pictures/pictures/bookmarks.png","������� \"������� ����\"",VuslygaRokiv);
	dovidnyk->exec();
	delete dovidnyk;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::actionMonthlyBudjetniNormy_activated()
{
	UMonthlyBudjetniNormyForm *bnForm = new UMonthlyBudjetniNormyForm();
	bnForm->setFiltrYear(workDate.year());
	bnForm->populateWorkDaysTableWidget();
	bnForm->exec();
	delete bnForm;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_OsnovneNarah_activated()
{
	bool done_ok=true;
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"������� �����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		done_ok = script->zdiysnOsnNarahAllWorkers(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1));
		delete script;
		salaryPopulateOperaciiTable();
		salaryPopulateOtherDohodyTable();
		salaryPopulateSumNarahTable();
		
		if (done_ok)
			QMessageBox::information(this,"!!! �������� ����������� !!!","���������� ������� ��� �������.");
		else
			QMessageBox::critical(this,"!!! �������� ����������� !!!","����������� ������� � ���������.");
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_NarahSumyUtrIVidr_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"��������� �������� � ���������� ��� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		bool done_ok = script->zdiysnDodatkNarahAllWorkers(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1));
		delete script;
		salaryPopulateOperaciiTable();
		salaryPopulateOtherDohodyTable();
		salaryPopulateSumNarahTable();
		if (done_ok)
			QMessageBox::information(this,
					codec->toUnicode("!!! �������� ����������� �������� � ���������� !!!"),
					codec->toUnicode("���������� �������� � ���������� ������� ��� �������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
		else
				QMessageBox::critical(this,
					codec->toUnicode("!!! �������� ����������� �������� � ���������� !!!"),
					codec->toUnicode("����������� �������� � ���������� ������� � ���������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_calcOplataInMonth_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"��������� ��������� ������ ��� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		bool done_ok = script->zdiysnObrahunkuPokaznOplaty(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1));
		delete script;
		salaryPopulateSumNarahTable();
		if (done_ok)
			QMessageBox::information(this,
					codec->toUnicode("!!! �������� ����������� ��������� ������ !!!"),
					codec->toUnicode("���������� ��������� ������ ������� ��� �������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
		else
				QMessageBox::critical(this,
					codec->toUnicode("!!! �������� ����������� ��������� ������ !!!"),
					codec->toUnicode("����������� ��������� ������ ������� � ���������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_calcVhSaldoInMonth_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"��������� �������� ������ ��� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		bool done_ok = script->zdiysnObrahunkuVhidnogoSaldo(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1));
		delete script;
		salaryPopulateSumNarahTable();
		if (done_ok)
			QMessageBox::information(this,
					codec->toUnicode("!!! �������� ��������� �������� ������ !!!"),
					codec->toUnicode("��������� �������� ������ ��������� ��� �������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
		else
				QMessageBox::critical(this,
					codec->toUnicode("!!! �������� ��������� �������� ������ !!!"),
					codec->toUnicode("��������� �������� ������ ��������� � ���������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_OsnovneNarahDlyaPotochnogoPrac_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"������� ����������� ��� ��������� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		bool done_ok = script->zdiysnOsnNarahOneWorker(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1), ui.spinBox_id->value());
		delete script;
		salaryPopulateOperaciiTable();
		salaryPopulateOtherDohodyTable();
		salaryPopulateSumNarahTable();
		if (done_ok)
			QMessageBox::information(this,
					codec->toUnicode("!!! �������� ����������� !!!"),
					codec->toUnicode("���������� ������� ��� �������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
		else
				QMessageBox::critical(this,
					codec->toUnicode("!!! �������� ����������� !!!"),
					codec->toUnicode("����������� ������� � ���������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_NarahSumyUtrIVidrCurWorker_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"��������� �������� � ���������� ��� ��������� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		bool done_ok = script->zdiysnDodatkNarahOneWorker(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1), ui.spinBox_id->value());
		delete script;
		salaryPopulateOperaciiTable();
		salaryPopulateOtherDohodyTable();
		salaryPopulateSumNarahTable();
		if (done_ok)
			QMessageBox::information(this,
					codec->toUnicode("!!! �������� ����������� �������� � ���������� !!!"),
					codec->toUnicode("���������� �������� � ���������� ������� ��� �������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
		else
				QMessageBox::critical(this,
					codec->toUnicode("!!! �������� ����������� �������� � ���������� !!!"),
					codec->toUnicode("����������� �������� � ���������� ������� � ���������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_calcOplataInMonthForCurWorker_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"��������� ��������� ������ ��� ��������� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		bool done_ok = script->zdiysnObrahunkuPokaznOplatyOneWorker(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1), ui.spinBox_id->value());
		delete script;
		salaryPopulateSumNarahTable();
		if (done_ok)
			QMessageBox::information(this,
					codec->toUnicode("!!! �������� ��������� ��������� ������ !!!"),
					codec->toUnicode("��������� ��������� ������ ��� ���������\n���������� ��������� ��� �������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
		else
				QMessageBox::critical(this,
					codec->toUnicode("!!! �������� ��������� ��������� ������ !!!"),
					codec->toUnicode("��������� ��������� ������ ��� ���������\n���������� ��������� � ���������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_calcVhSaldoInMonthForCurWorker_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"��������� ��������� ������ ��� ��������� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		USalaryScript *script = new USalaryScript();
		bool done_ok = script->zdiysnObrahunkuVhidnogoSaldoOneWorker(QDate(filterDialog->get_Year(),filterDialog->get_Month(),1), ui.spinBox_id->value());
		delete script;
		salaryPopulateSumNarahTable();		
		if (done_ok)
			QMessageBox::information(this,
					codec->toUnicode("!!! �������� ��������� �������� ������ !!!"),
					codec->toUnicode("��������� �������� ������ ��� ���������\n���������� ��������� ��� �������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
		else
				QMessageBox::critical(this,
					codec->toUnicode("!!! �������� ��������� �������� ������ !!!"),
					codec->toUnicode("��������� ��������� �������� ������ ��� ���������\n���������� ��������� � ���������."),
					QMessageBox::Ok,
					QMessageBox::NoButton);
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_perelik_pracivnykiv_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UPrintDocs *printDocs = new UPrintDocs();
	printDocs->perelik_pracivnykiv(pidrozdilId, ui.checkBox_show_only_work_workers->isChecked());
	connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
	connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
	delete printDocs;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_vidomist_narah_zarplaty_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ������� ����������� ��������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_narahuvannya_za_misac(filterDialog->get_Year(),filterDialog->get_Month(), 
							pidrozdilId, ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_month_cards_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� �������� �� �������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_worker_cards(filterDialog->get_Year(),filterDialog->get_Month(), pidrozdilId, ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_month_card_for_current_worker_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ������ �� ������� ��� ��������� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_cur_worker_card(filterDialog->get_Year(),filterDialog->get_Month(), ui.spinBox_id->value());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_year_cards_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ����� ������ �� �������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_year_cards(filterDialog->get_Year(),pidrozdilId,ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_year_card_for_current_worker_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ���� ������ �� ������� ��� ��������� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_year_cur_worker_card(filterDialog->get_Year(),ui.spinBox_id->value());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_year_utrymannya_cards_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ����� ������ �� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_year_utrymannya_cards(filterDialog->get_Year(),pidrozdilId,ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_year_utrymannya_card_for_current_worker_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ���� ������ �� ���������� ��� ��������� ����������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_year_utrymannya_cur_worker_card(filterDialog->get_Year(),ui.spinBox_id->value());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_utrymannya_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ����������� �������� �� ��������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_vidomist_utrymannya(filterDialog->get_Year(),filterDialog->get_Month(), pidrozdilId, ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_print_vidrahuvannya_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ����������� ���������� �� ��������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_vidomist_vidrahuvannya(filterDialog->get_Year(),filterDialog->get_Month(), pidrozdilId, ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_printOplataZaborgovanist_activated()
{
	short pidrozdilId = 0;
	QSqlQuery query("SELECT count(*) FROM pidrozdily_salary");
	query.next();
	if (query.value(0).toInt()>1){
		UDialogChoosePidrozdil_salary d;
		if (!d.exec())
			return;
		pidrozdilId = d.pidrozdilId();
	}
	
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ����������� ���������� �� ��������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_vidomist_oplaty_i_zaborgovanosti(filterDialog->get_Year(),filterDialog->get_Month(), pidrozdilId, ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_printPlatijky_activated()
{
	UWidgetPrintPlatijky *widgetPrintPlatijky = new UWidgetPrintPlatijky(workDate);
	widgetPrintPlatijky->setAttribute(Qt::WA_DeleteOnClose, true);
	widgetPrintPlatijky->show();
	connect(this, SIGNAL(closeLoginSignal()), widgetPrintPlatijky, SLOT(close()));
	connect(this, SIGNAL(mainWindowClosed()), widgetPrintPlatijky, SLOT(close()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_printVidomistNarahuvannyaIVidrahuvannya_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"���� ����������� ���������� �� ��������", workDate.year(), workDate.month());
	if (filterDialog->exec()==1){
		UPrintDocs *printDocs = new UPrintDocs();
		printDocs->print_vidomist_narah_i_vidrah_by_buhrahunok(filterDialog->get_Year(),filterDialog->get_Month(), ui.checkBox_show_only_work_workers->isChecked());
		connect(this, SIGNAL(closeLoginSignal()), printDocs->printform, SLOT(close()));
		connect(this, SIGNAL(mainWindowClosed()), printDocs->printform, SLOT(close()));
		delete printDocs;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_createZvit1df_activated()
{
	UDialogYMFilter *filterDialog = new UDialogYMFilter(0,"��������� ���� 1df ��� �������� \"��������� �������\"", workDate.year(), workDate.month(), UKvartalYearDialog);
	if (filterDialog->exec()==1){
		UZvit1df * zv = new UZvit1df();
		if (zv->import1dfToFile(filterDialog->get_Month(), filterDialog->get_Year(), date))
			QMessageBox::information(0,"������� ����","��� 1df ������ �����������.");
		else
			QMessageBox::critical(0,"������� ����","������� ���� 1df ��������� � ���������.");
		delete zv;
	}
	delete filterDialog;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_misyachnaPersonifikaciya_activated()
{
	UDialogYMFilter filterDialog(0,"���� ����� ��� ���������� �������� �������������", workDate.year(), 
							workDate.month(), UMonthYearDialog);
	
	if (filterDialog.exec()==1){
		QString dirPath = QFileDialog::getExistingDirectory(this,"������ ����� ��� ��������. ������, ��� �� ����� ���� ������.", 
						QDesktopServices::storageLocation(QDesktopServices::DesktopLocation), QFileDialog::ShowDirsOnly );
		QDir dir(dirPath);
		if (!dir.exists())
			return;
		UPFUMonthPersonDbf zv;
		if (zv.createElZvit(dirPath, filterDialog.get_Year(), filterDialog.get_Month()))
			QMessageBox::information(0,"������� ����","��� \"�������� �������������\" ������ �����������.");
		else
			QMessageBox::critical(0,"������� ����","������� ���� \"�������� �������������\" ��������� � ���������.");
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_elZvitEdynyiVnesok_activated()
{
	UDialogYMFilter filterDialog(0,"���� ����� ��� ���������� ���� �� ������� ������", workDate.year(), 
							workDate.month(), UMonthYearDialog);
	
	if (filterDialog.exec()==1){
		QString dirPath = QFileDialog::getExistingDirectory(this,"������ ����� ��� ��������. ������, ��� �� ����� ���� ������.", 
						QDesktopServices::storageLocation(QDesktopServices::DesktopLocation), QFileDialog::ShowDirsOnly );
		QDir dir(dirPath);
		if (!dir.exists())
			return;
		UPFUEdynyiVnesokDbf zv;
		if (zv.createElZvit(dirPath, filterDialog.get_Year(), filterDialog.get_Month()))
			QMessageBox::information(0,"������� ����","��� \"������ ������\" ������ �����������.");
		else
			QMessageBox::critical(0,"������� ����","������� ���� \"������ ������\" ��������� � ���������.");
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_gui_options_activated()
{
	UDialogOptions *dOptions = new UDialogOptions();
	dOptions->exec();
	delete dOptions;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPeopleSalaryMainWindow::action_about_activated()
{
	QFile *aboutFile = new QFile(QCoreApplication::applicationDirPath()+"/../doc/gui_about_el-zar.html");
	aboutFile->open(QFile::ReadOnly);
	QTextStream *aboutText = new QTextStream(aboutFile);
	QMessageBox::about (this,codec->toUnicode("��� ��������"),"<CENTER><B>�������� \"���-���\" ����� "+USalary::version()+"</B></CENTER><BR>"+aboutText->readAll());
	delete aboutFile;
	delete aboutText;
}
//--------------------utech--------------------utech--------------------utech--------------------
