//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу головного вікна програми нарахування зарплати
//
// Створений програмістами Компанії "Утех" 17.07.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UPEOPLESALARYMAINWINDOW_H
#define UPEOPLESALARYMAINWINDOW_H
//--------------------utech--------------------utech--------------------utech--------------------
//Дохід 01
#define TABEL_INFO_COLUMN 1
#define INDEXACIYA_BEGIN_COLUMN 3
#define INDEXACIYA_END_COLUMN 6
#define NARAH_BEGIN_COLUMN 7
#define NARAH_END_COLUMN 20
#define NARAH_SUM_COLUMN 21
#define UTRYM_SUM_COLUMN 30
#define VIDRAH_SUM_COLUMN 36
//Соцпільга не бере участі у сумі утримань
#define SOC_PILGA_COLUMN 26

//Дохід >01
#define OTHER_DOHID_NARAH_SUM_COLUMN 3
#define OTHER_DOHID_UTRYM_SUM_COLUMN 11
#define OTHER_DOHID_NA_RUKY_COLUMN 12
#define OTHER_DOHID_VIDRAH_SUM_COLUMN 18
//Соцпільга не бере участі у сумі утримань
#define OTHER_DOHID_SOC_PILGA_COLUMN 7

//Сумарне нарахування
#define SUM_NARAH_VHSALDO_COLUMN 2
#define SUM_NARAH_NARAH_SUM_BEG_COLUMN 3
#define SUM_NARAH_NARAH_SUM_END_COLUMN 4
#define SUM_NARAH_UTRYM_SUM_BEG_COLUMN 5
#define SUM_NARAH_UTRYM_SUM_END_COLUMN 7
#define SUM_NARAH_VIDRAH_SUM_BEG_COLUMN 8
#define SUM_NARAH_VIDRAH_SUM_END_COLUMN 10
#define SUM_NARAH_IN_MONTH_NARAH_COLUMN 11
#define SUM_NARAH_IN_MONTH_NARAH_WITH_SALDO_COLUMN 12
#define SUM_NARAH_OPLATA_BEGIN_SUM_COLUMN 13
#define SUM_NARAH_OPLATA_END_SUM_COLUMN 19
#define SUM_NARAH_VYHSALDO_COLUMN 20
//--------------------utech--------------------utech--------------------utech--------------------

#include <QMainWindow>
#include <QtSql>
#include <QHeaderView>
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QTableView>
#include <QHash>

#include "ui_UPeopleSalaryMainWindow.h"

#include "../additional/UComboBoxSQLRelation.h"

//--------------------utech--------------------utech--------------------utech--------------------
class Browser;
class Dovidnyk;
enum vudDovidnuka {GrypuKorust,BjydzNormu,VuslygaRokiv};
//--------------------utech--------------------utech--------------------utech--------------------
class UPeopleSalaryMainWindow : public QMainWindow
{
  Q_OBJECT

  public:
		UPeopleSalaryMainWindow(QWidget *parent = 0);
		~UPeopleSalaryMainWindow();
  public slots:
		void execSalarySelectQuery();
		void populateNprComboBoxes();
		void checkBox_show_only_work_workers_stateChanged(int state);
		void salaryConnectEditres();
		void salaryDisConnectEditres();
		void salaryFormRecordEditing();
		void salaryFormRecordEndEditing();
		void salaryFormUpdateRecord();
		void salaryFormInsertRecord();
		void salaryFormFirst();
		void salaryFormPrevious();
		void salaryFormNext();
		void salaryFormLast();
		void salaryFormNew();
		void salaryFormDeleteRec();
		void salaryButtonSaveClicked();
		void salaryButtonCancelClicked();
		
		void salaryPopulateLikarnyaniTable();
		void pushButton_insert_Likarnyani_clicked();
		void pushButton_del_Likarnyani_clicked();
		void saveLikarnyaniRecord(QTableWidgetItem*);
		void calcLikarnyaniTable();
		
		void salaryPopulateVidpuskniTable();
		void pushButton_insert_Vidpuskni_clicked();
		void pushButton_del_Vidpuskni_clicked();
		void saveVidpuskniRecord(QTableWidgetItem*);
		void calcVidpuskniTable();
		
		void salaryPopulateOperaciiTable();
		void calcMainNarahOperaciiTableSum();
		void populateStandartTableItem(int row, int col, QString str, Qt::ItemFlags editFlag=Qt::ItemIsEditable);
		void salaryPopulateOtherDohodyTable();
		void populateOtherDohodyTableRow(int row, QSqlRecord rec);
		void populateOtherDohodyItem(int row, int col, QString str, Qt::ItemFlags editFlag=Qt::ItemIsEditable);
		void calcOtherNarahOperaciiTableSum();
		void pushButton_insert_other_dohid_clicked();
		void pushButton_del_other_dohid_clicked();
		void salaryPopulateSumNarahTable();
		void calcNarahSumTable();
		void populateNarahSumTableItem(int row, int col, QString str, Qt::ItemFlags editFlag=Qt::ItemIsEditable);
		void setOperaciiTabWidgetGUI(int index);
		void salaryBeforeChangeDefaultOplata(QSqlRecord & record);
		void clearOperaciiTable();
		void clearSumNarahTable();
		void salaryFiltrComboboxIndexChanged(int index);
		void populateSalaryForm( int seek_query );
		void verifyStatusWorker();
		void saveMainNarahRecord(QTableWidgetItem*);
		void saveOtherDohodyRecord(QTableWidgetItem* item);
		void saveNarahSumRecord(QTableWidgetItem* item);
		void enableNavigationButtons(int index);
		void tableOperaciiVidpracGodyny();
		void tableOperaciiNarahovanoView();
		void tableOperaciiUtrymannyaView();
		void tableOperaciiOplataView();
		void tableOperaciiVidrahuvannyaZarplatyView();
		void tableOperaciiIndexaciyaView();
		void populateTryfyPracivnykivTable();
		void pushButton_insertTaryf_clicked();
		void pushButton_delTaryf_clicked();
		void taryfyTableItemChanged(QTableWidgetItem *item);
		void curDateChanged(const QDate & xDate);
		void showWorker(int nprId);
		//action slots
		void action_debuger_activated();
		void action_tabel_activated();
		void action_fullTabel_activated();
		void action_indexaciyaBaseMonth_activated();
		void action_posady_i_taryfy_activated();
		void action_pidrozdily_activated();
		void action_svyatkoviDni_activated();
		void action_infAboutFirm_activated();
		void action_BudgetNorm_activated();
		void actionMonthlyBudjetniNormy_activated();
		void action_MeritOfYears_activated();
		void action_OsnovneNarah_activated();
		void action_NarahSumyUtrIVidr_activated();
		void action_calcOplataInMonth_activated();
		void action_calcVhSaldoInMonth_activated();
		void action_OsnovneNarahDlyaPotochnogoPrac_activated();
		void action_NarahSumyUtrIVidrCurWorker_activated();
		void action_calcOplataInMonthForCurWorker_activated();
		void action_calcVhSaldoInMonthForCurWorker_activated();
		void action_print_perelik_pracivnykiv_activated();
		void action_print_vidomist_narah_zarplaty_activated();
		void action_print_month_cards_activated();
		void action_print_month_card_for_current_worker_activated();
		void action_print_year_cards_activated();
		void action_print_year_card_for_current_worker_activated();
		void action_print_year_utrymannya_cards_activated();
		void action_print_year_utrymannya_card_for_current_worker_activated();
		void action_print_utrymannya_activated();
		void action_print_vidrahuvannya_activated();
		void action_printOplataZaborgovanist_activated();
		void action_printPlatijky_activated();
		void action_printVidomistNarahuvannyaIVidrahuvannya_activated();
		void action_createZvit1df_activated();
		void action_misyachnaPersonifikaciya_activated();
		void action_elZvitEdynyiVnesok_activated();
		void action_gui_options_activated();
		void action_about_activated();
	
	signals:
		void requestDebugWindow();
		void closeLoginSignal();
		void mainWindowClosed();
	
	  private:
		Ui::UPeopleSalaryMainWindow ui;
		Dovidnyk* dovidnyk;
		
		QSqlRecord xRecord;
		QSqlQuery *nprSelectQuery;
		QVector<int> nprList;
		QPalette *palette, *default_palette;
		QDate date, workDate;
		QTextCodec *codec;
		int record_edited; //0-record not edited, 1 - record edited
		int new_record; // 1 - new record< 0-not;
		unsigned char days_in_month[12];
		QHash<int,int> grupyHash;
		QStringList mainNarahColNamesList;
		QStringList otherNarahColNamesList;
		QStringList narahSumColNamesList;
		
		double NaRukyOtherDohody[12];
};
//--------------------utech--------------------utech--------------------utech--------------------
class Dovidnyk: public QDialog
{
	Q_OBJECT

public:
	Dovidnyk(const QString&, const QString&, vudDovidnuka);
	void populateData(const QString&, const QStringList&, QString orderStr="");
	
private slots:
	void insertRec();
	void deleteRec();
	void verifyRec(QTableWidgetItem* item);
	void saveItem(QTableWidgetItem*);
	void curRowChanged();
private:
	int numRows;
	int numCols;
	QTableWidget* tableWidget;
	QPushButton* addRecPbtn;
	QPushButton* delRecPbtn;
	QPushButton* closePbtn;
	vudDovidnuka vud;
};
//--------------------utech--------------------utech--------------------utech--------------------
class UIndexaciyaInfoDelegate: public QItemDelegate
{
	Q_OBJECT
  public:
		UIndexaciyaInfoDelegate(QObject *parent = 0): QItemDelegate(parent){};
		void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
}; 
//--------------------utech--------------------utech--------------------utech--------------------
#endif
