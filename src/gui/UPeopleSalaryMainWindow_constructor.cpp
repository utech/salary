//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу головного вікна програми нарахування зарплати
//
// Створений програмістами Компанії "Утех" 17.07.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UPeopleSalaryMainWindow.h"
#include "USalary.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLocale>

#include <UStandardDelegate>
#include <../additional/UDifFunc.h>

#include "../additional/UComboBoxSQLRelation.h"
#include "UTaryfDelegate.h"
//--------------------utech--------------------utech--------------------utech--------------------
UPeopleSalaryMainWindow::UPeopleSalaryMainWindow(QWidget *parent)
        : QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle("\"Ель-Зар\" - Електронна книжка по зарплаті. Версія "+USalary::version()+" :: "+OrganizShortName());
	setAttribute(Qt::WA_DeleteOnClose);
	
	ui.comboBox_gender->addItem("",0);
	ui.comboBox_gender->addItem("чол.",1);
	ui.comboBox_gender->addItem("жін.",2);
	
	palette = new QPalette();
	codec = QTextCodec::codecForName("cp1251");
	palette->setColor ( QPalette::Active, QPalette::WindowText, Qt::red );
	default_palette = new QPalette();
	*default_palette = ui.line->palette();
	date = QDate::currentDate();
	record_edited = 0;
	new_record = 0;
	
	nprSelectQuery = new QSqlQuery();
	
	//Налаштування GUI з іні-файлу
	QSettings *settings = new QSettings("./salary.ini", QSettings::IniFormat);
	ui.checkBox_Operacii_vidprac_godyny->setChecked(settings->value("GUI_View/chBoxOperaciiVidpracyovaniGodynyDni").toBool());
	ui.checkBox_Operacii_indexaciya->setChecked(settings->value("GUI_View/chBoxOperaciiIndexaciya").toBool());
	ui.checkBox_Operacii_narahovano->setChecked(settings->value("GUI_View/chBoxOperaciiTableNarahovano").toBool());
	ui.checkBox_Operacii_utrymannya->setChecked(settings->value("GUI_View/chBoxOperaciiTableUtrymannya").toBool());
	ui.checkBox_Operacii_pislyapodatk_vidrah->setChecked(settings->value("GUI_View/chBoxOperaciiTablePislapodatkvidrah").toBool());
	ui.checkBox_Operacii_vidrahuv_na_zarplatu->setChecked(settings->value("GUI_View/chBoxOperaciiTableVidrahuvannyaNaZarplatu").toBool());
	ui.checkBox_show_only_work_workers->setChecked(settings->value("GUI_View/chBoxShowOnlyWorkWorkers").toBool());
	ui.dateEdit_curDate->setDate(settings->value("GUI_View/workDate").toDate());
	ui.tabWidget_operacii->setCurrentIndex(settings->value("GUI_View/tabWidget_operaciiCurPage").toInt());
	delete settings;
	
	workDate = ui.dateEdit_curDate->date();
	ui.dateEdit_curDate->setMaximumDate(date);
	
		//initialize days_in_month array
	days_in_month[0]=31;
	days_in_month[1]=28;
	days_in_month[2]=31;
	days_in_month[3]=30;
	days_in_month[4]=31;
	days_in_month[5]=30;
	days_in_month[6]=31;
	days_in_month[7]=31;
	days_in_month[8]=30;
	days_in_month[9]=31;
	days_in_month[10]=30;
	days_in_month[11]=31;
	
		//Початкове налаштування таблиць
			//Таблиця лікарняні
	ui.tableWidget_Likarnyani->hideColumn(0);
	ui.tableWidget_Likarnyani->setItemDelegateForColumn(1,new USpinBoxDelegate(1,12,ui.tableWidget_Likarnyani));
	ui.tableWidget_Likarnyani->setItemDelegateForColumn(2,new USpinBoxDelegate(2000,2100,ui.tableWidget_Likarnyani));
	ui.tableWidget_Likarnyani->setItemDelegateForColumn(3,new UDoubleSpinBoxDelegate(0,100000,2,ui.tableWidget_Likarnyani));
	ui.tableWidget_Likarnyani->setItemDelegateForColumn(4,new USpinBoxDelegate(0,31,ui.tableWidget_Likarnyani));
	UIntStringList ISList;
	ISList.insert(1, "За рах.Роботодав.");
	ISList.insert(2, "За рах.ФСС");
	ui.tableWidget_Likarnyani->setItemDelegateForColumn(5,new UIntStringListComboDelegate(ui.tableWidget_Likarnyani, ISList));
	ui.tableWidget_Likarnyani->setColumnWidth(1, 70);
	ui.tableWidget_Likarnyani->setColumnWidth(2, 70);
	ui.tableWidget_Likarnyani->setColumnWidth(3, 70);
	ui.tableWidget_Likarnyani->setColumnWidth(4, 70);
	ui.tableWidget_Likarnyani->setColumnWidth(5, 110);
	ui.tableWidget_Likarnyani->setColumnWidth(6, 120);
	
			//Таблиця відпускні
	ui.tableWidget_Vidpuskni->hideColumn(0);
	ui.tableWidget_Vidpuskni->setItemDelegateForColumn(1,new USpinBoxDelegate(1,12,ui.tableWidget_Vidpuskni));
	ui.tableWidget_Vidpuskni->setItemDelegateForColumn(2,new USpinBoxDelegate(2000,2100,ui.tableWidget_Vidpuskni));
	ui.tableWidget_Vidpuskni->setItemDelegateForColumn(3,new UDoubleSpinBoxDelegate(0,100000,2,ui.tableWidget_Vidpuskni));
	ui.tableWidget_Vidpuskni->setItemDelegateForColumn(4,new USpinBoxDelegate(0,31,ui.tableWidget_Vidpuskni));
	ui.tableWidget_Vidpuskni->setColumnWidth(1, 70);
	ui.tableWidget_Vidpuskni->setColumnWidth(2, 70);
	ui.tableWidget_Vidpuskni->setColumnWidth(3, 70);
	ui.tableWidget_Vidpuskni->setColumnWidth(4, 70);
	ui.tableWidget_Vidpuskni->setColumnWidth(5, 120);
	
	ui.tableWidget_dodatkovi_dohody->hideColumn(0);
	setOperaciiTabWidgetGUI(ui.tabWidget_operacii->currentIndex());
	ui.dockWidget_taryfy_na_zarplatu->hide();
	ui.dockWidget_taryfy_na_zarplatu->setFloating(true);
	ui.dockWidget_taryfy_na_zarplatu->resize(540,500);
	ui.tableWidget_taryfy->hideColumn(0);
	ui.tableWidget_taryfy->setColumnWidth( 1, 400);
	ui.tableWidget_taryfy->setColumnWidth( 2, 80);
	ui.tableWidget_taryfy->setItemDelegate(new UTaryfDelegate(ui.tableWidget_taryfy));
	
	ui.line->setFrameShadow(QFrame::Plain);
	ui.line_2->setFrameShadow(QFrame::Plain);
	ui.line_3->setFrameShadow(QFrame::Plain);
	ui.line_4->setFrameShadow(QFrame::Plain);
	ui.spinBox_year_filtr->setValue(workDate.year());
	ui.spinBox_month_filtr->setValue(workDate.month());
		
	// set operacci table
	for (int col=0; col<ui.tableWidget_Operacii->columnCount(); col++){
		QColor itemColor;
		if (col>2 and col<=NARAH_SUM_COLUMN)
			itemColor=operaciiNarahColor;
		else if (col>NARAH_SUM_COLUMN and col<=UTRYM_SUM_COLUMN)
			itemColor=operaciiUtrymColor;
		else if (col>UTRYM_SUM_COLUMN and col<=VIDRAH_SUM_COLUMN)
			itemColor=operaciiVidrahColor;
		else
			itemColor.setRgb(255,255,255);
		Qt::Alignment align = Qt::AlignVCenter | (col==TABEL_INFO_COLUMN ? Qt::AlignLeft : Qt::AlignRight);
		for (int row=0; row<ui.tableWidget_Operacii->rowCount(); row++){
			QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
			tableWidgetItem->setTextAlignment(align);
			if (row!=12)
				tableWidgetItem->setBackgroundColor(itemColor);
			else
				tableWidgetItem->setBackgroundColor(itemColor.lighter(85));
			ui.tableWidget_Operacii->setItem(row, col, tableWidgetItem );
		}
	}
	ui.tableWidget_Operacii->setColumnWidth(TABEL_INFO_COLUMN, 230);
	ui.tableWidget_Operacii->setColumnWidth(2, 200);
	for (int col=3; col<ui.tableWidget_Operacii->columnCount(); col++)
		ui.tableWidget_Operacii->setColumnWidth(col, 100);
	ui.tableWidget_Operacii->resizeRowsToContents();
	ui.tableWidget_Operacii->verticalHeader()->resize(10, ui.tableWidget_Operacii->verticalHeader()->size().height());
	ui.tableWidget_Operacii->verticalHeader()->setDefaultAlignment ( Qt::AlignRight );
	ui.tableWidget_Operacii->setItemDelegateForColumn(3, new UIndexaciyaInfoDelegate(ui.tableWidget_Operacii));
	ui.tableWidget_Operacii->hideColumn(0);
	
	//set otherDohodyTable
	ui.tableWidget_dodatkovi_dohody->setColumnWidth(1, 50);
	ui.tableWidget_dodatkovi_dohody->setColumnWidth(2, 200);
	for (int col=3; col<ui.tableWidget_dodatkovi_dohody->columnCount(); col++)
		ui.tableWidget_dodatkovi_dohody->setColumnWidth(col, 100);
	
	// set tableWidget_SumNarah_Oplata
	for (int col=0; col<ui.tableWidget_SumNarah_Oplata->columnCount(); col++){
		for (int row=0; row<ui.tableWidget_SumNarah_Oplata->rowCount(); row++){
			QTableWidgetItem *tableWidgetItem = new QTableWidgetItem();
			tableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			if ( (col == SUM_NARAH_VHSALDO_COLUMN) || (col == SUM_NARAH_IN_MONTH_NARAH_COLUMN) || (col == SUM_NARAH_IN_MONTH_NARAH_WITH_SALDO_COLUMN))
				tableWidgetItem->setBackgroundColor(operaciiSaldoColor);
			else if ((col >= SUM_NARAH_NARAH_SUM_BEG_COLUMN) && (col <= SUM_NARAH_NARAH_SUM_END_COLUMN))
				tableWidgetItem->setBackgroundColor(operaciiNarahColor);
			else if ((col >= SUM_NARAH_UTRYM_SUM_BEG_COLUMN) && (col <= SUM_NARAH_UTRYM_SUM_END_COLUMN))
				tableWidgetItem->setBackgroundColor(operaciiUtrymColor);
			else if ((col >= SUM_NARAH_VIDRAH_SUM_BEG_COLUMN) && (col <= SUM_NARAH_VIDRAH_SUM_END_COLUMN))
				tableWidgetItem->setBackgroundColor(operaciiVidrahColor);
			else if (col >= SUM_NARAH_OPLATA_BEGIN_SUM_COLUMN && col <= SUM_NARAH_OPLATA_END_SUM_COLUMN)
				tableWidgetItem->setBackgroundColor(operaciiOplataColor);
			else if (col == SUM_NARAH_VYHSALDO_COLUMN)
				tableWidgetItem->setBackgroundColor(operaciiSaldoColor);
			ui.tableWidget_SumNarah_Oplata->setItem(row, col, tableWidgetItem );
		}
	}
	ui.tableWidget_SumNarah_Oplata->setColumnWidth(1, 200);
	for (int col=2; col<ui.tableWidget_SumNarah_Oplata->columnCount(); col++)
		ui.tableWidget_SumNarah_Oplata->setColumnWidth(col, 100);
	ui.tableWidget_SumNarah_Oplata->resizeRowsToContents();
	ui.tableWidget_SumNarah_Oplata->verticalHeader()->resize(10, ui.tableWidget_SumNarah_Oplata->verticalHeader()->size().height());
	ui.tableWidget_SumNarah_Oplata->verticalHeader()->setDefaultAlignment ( Qt::AlignRight );
	//ui.tableWidget_SumNarah_Oplata->setItemDelegate(new UDoubleSpinBoxDelegate(-100000,100000,2,ui.tableWidget_SumNarah_Oplata));
	ui.tableWidget_SumNarah_Oplata->hideColumn(0);
	ui.tableWidget_SumNarah_Oplata->hideColumn(5);
	
	//Налаштування видимих полів таблиці операцій
	tableOperaciiVidpracGodyny();
	tableOperaciiIndexaciyaView();
	tableOperaciiNarahovanoView();
	tableOperaciiUtrymannyaView();
	tableOperaciiOplataView();
	tableOperaciiVidrahuvannyaZarplatyView();
	
	//Створення ліста полів у таблицях операцій для запису у них змін
	mainNarahColNamesList 	<< ""
							<< ""
							<< "Comentar"
							<< "I_PromIndex"
							<< "I_Index"
							<< "I_ProindexovanaSuma"
							<< "I_SumaDlyaIndexacii"
							<< "N_Osn_narah"
							<< "N_VuslRokiv"
							<< "N_Likarn"
							<< "N_Vidpuskni"
							<< "N_Svatkovi_dni"
							<< "N_Nichni"
							<< "N_Nadurochni"
							<< "N_Klasn_vodiya"
							<< "N_Premia"
							<< "N_MaterDopomoga"
							<< "N_Doplata"
							<< "N_Indexaciya"
							<< "N_VyplProfspilky"
							<< "N_InshiNarahuvannya"
							<< ""
							<< "U_PensFond"
							<< "U_EV_Likarn"
							<< "U_Fond_zainat"
							<< "U_SocStrah"
							<< "U_SocPilga"
							<< "U_Prybutkov_podatok"
							<< "U_VidrahProfspilka"
							<< "U_InshiVidrahuvannya"
							<< ""
							<< "V_PensFond"
							<< "V_SocStrah"
							<< "V_Fond_zainat"
							<< "V_NeshchVyp"
							<< "V_Inshi"
							<< "";
	otherNarahColNamesList	<< ""
							<< "OznakaDohodu"
							<< "Comentar"
							<< "N_Osn_narah"
							<< "U_PensFond"
							<< "U_Fond_zainat"
							<< "U_SocStrah"
							<< "U_SocPilga"
							<< "U_Prybutkov_podatok"
							<< "U_VidrahProfspilka"
							<< "U_InshiVidrahuvannya"
							<< "O_VsogoNaRuky"
							<< "O_VsogoNaRuky"
							<< "V_PensFond"
							<< "V_SocStrah"
							<< "V_Fond_zainat"
							<< "V_NeshchVyp"
							<< "V_Inshi"
							<< "";
	narahSumColNamesList	<< ""
							<< "ComentarForOplata"
							<< "Vh_saldo"
							<< ""
							<< ""
							<< ""
							<< ""
							<< ""
							<< ""
							<< ""
							<< ""
							<< ""
							<< ""
							<< "O_Inshi"
							<< "O_Shtraf"
							<< "O_Alimenty"
							<< "O_ZaPoperMis"
							<< "O_Premiya"
							<< "O_Avans"
							<< "O_Oplacheno"
							<< "";
	//prepare first view record
	populateNprComboBoxes();
	execSalarySelectQuery();
	ui.spinBox_navigator->setValue(1); // 1 - bo pershyi zapys
	enableNavigationButtons(1); // 1 - bo pershyi zapys
	populateSalaryForm(1); // 1 - bo pershyi zapys
	populateTryfyPracivnykivTable();
	
	//action connectors
	connect(ui.action_closeProgram, SIGNAL(activated()), this, SLOT(close()));
	connect(ui.action_debuger, SIGNAL(activated()), this, SLOT(action_debuger_activated()));
	connect(ui.action_tabel, SIGNAL(activated()), this, SLOT(action_tabel_activated()));	
	connect(ui.action_fullTabel, SIGNAL(activated()), this, SLOT(action_fullTabel_activated()));	
	connect(ui.action_indexaciyaBaseMonth, SIGNAL(activated()), this, SLOT(action_indexaciyaBaseMonth_activated()));
	connect(ui.action_posady_i_taryfy, SIGNAL(activated()), this, SLOT(action_posady_i_taryfy_activated()));
	connect(ui.action_pidrozdily, SIGNAL(activated()), this, SLOT(action_pidrozdily_activated()));
	connect(ui.action_svyatkoviDni, SIGNAL(activated()), this, SLOT(action_svyatkoviDni_activated()));
	
	connect(ui.action_infAboutFirm, SIGNAL(activated()), this, SLOT(action_infAboutFirm_activated()));
	connect(ui.action_BudgetNorm, SIGNAL(activated()), this, SLOT(action_BudgetNorm_activated()));
	connect(ui.actionMonthlyBudjetniNormy, SIGNAL(activated()), this, SLOT(actionMonthlyBudjetniNormy_activated()));
	connect(ui.action_MeritOfYears, SIGNAL(activated()), this, SLOT(action_MeritOfYears_activated()));
	connect(ui.action_OsnovneNarah, SIGNAL(activated()), this, SLOT(action_OsnovneNarah_activated()));
	connect(ui.action_NarahSumyUtrIVidr, SIGNAL(activated()), this, SLOT(action_NarahSumyUtrIVidr_activated()));
	
	connect(ui.action_calcOplataInMonth, SIGNAL(activated()), this, SLOT(action_calcOplataInMonth_activated()));
	connect(ui.action_calcVhSaldoInMonth, SIGNAL(activated()), this, SLOT(action_calcVhSaldoInMonth_activated()));
	
	connect(ui.action_OsnovneNarahDlyaPotochnogoPrac, SIGNAL(activated()), this, SLOT(action_OsnovneNarahDlyaPotochnogoPrac_activated()));
	connect(ui.action_NarahSumyUtrIVidrCurWorker, SIGNAL(activated()), this, SLOT(action_NarahSumyUtrIVidrCurWorker_activated()));
	
	connect(ui.action_calcOplataInMonthForCurWorker, SIGNAL(activated()), this, SLOT(action_calcOplataInMonthForCurWorker_activated()));
	connect(ui.action_calcVhSaldoInMonthForCurWorker, SIGNAL(activated()), this, SLOT(action_calcVhSaldoInMonthForCurWorker_activated()));
	
	connect(ui.action_print_perelik_pracivnykiv, SIGNAL(activated()), this, SLOT(action_print_perelik_pracivnykiv_activated()));
	connect(ui.action_print_vidomist_narah_zarplaty, SIGNAL(activated()), this, SLOT(action_print_vidomist_narah_zarplaty_activated()));
	connect(ui.action_print_month_cards, SIGNAL(activated()), this, SLOT(action_print_month_cards_activated()));
	connect(ui.action_print_month_card_for_current_worker, SIGNAL(activated()), this, SLOT(action_print_month_card_for_current_worker_activated()));
	connect(ui.action_print_year_cards, SIGNAL(activated()), this, SLOT(action_print_year_cards_activated()));
	connect(ui.action_print_year_card_for_current_worker, SIGNAL(activated()), this, SLOT(action_print_year_card_for_current_worker_activated()));
	connect(ui.action_print_year_utrymannya_cards, SIGNAL(activated()), 
				this, SLOT(action_print_year_utrymannya_cards_activated()));
	connect(ui.action_print_year_utrymannya_card_for_current_worker, SIGNAL(activated()), 
				this, SLOT(action_print_year_utrymannya_card_for_current_worker_activated()));
	
	connect(ui.action_print_utrymannya, SIGNAL(activated()), this, SLOT(action_print_utrymannya_activated()));
	connect(ui.action_print_vidrahuvannya, SIGNAL(activated()), this, SLOT(action_print_vidrahuvannya_activated()));
	connect(ui.action_printOplataZaborgovanist, SIGNAL(activated()), this, SLOT(action_printOplataZaborgovanist_activated()));
	connect(ui.action_printPlatijky, SIGNAL(activated()), this, SLOT(action_printPlatijky_activated()));
	connect(ui.action_printVidomistNarahuvannyaIVidrahuvannya, SIGNAL(activated()), this, SLOT(action_printVidomistNarahuvannyaIVidrahuvannya_activated()));
	
	connect(ui.action_createZvit1df, SIGNAL(activated()), this, SLOT(action_createZvit1df_activated()));
	connect(ui.action_misyachnaPersonifikaciya, SIGNAL(activated()), this, SLOT(action_misyachnaPersonifikaciya_activated()));
	connect(ui.action_elZvitEdynyiVnesok, SIGNAL(activated()), this, SLOT(action_elZvitEdynyiVnesok_activated()));
	
	connect(ui.action_gui_options, SIGNAL(activated()), this, SLOT(action_gui_options_activated()));
	connect(ui.action_about, SIGNAL(activated()), this, SLOT(action_about_activated()));
	
	connect(ui.checkBox_show_only_work_workers, SIGNAL(stateChanged(int)), this, SLOT(checkBox_show_only_work_workers_stateChanged(int)));
	
	//connect(ui.tableWidget_Operacii, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(verifyStatusWorker()));
	connect(ui.spinBox_year_filtr, SIGNAL(valueChanged (int)), this, SLOT(salaryPopulateOperaciiTable()));
	connect(ui.spinBox_year_filtr, SIGNAL(valueChanged (int)), this, SLOT(salaryPopulateSumNarahTable()));
	
	connect(ui.spinBox_year_filtr, SIGNAL(valueChanged (int)), this, SLOT(salaryPopulateOtherDohodyTable()));
	connect(ui.spinBox_month_filtr, SIGNAL(valueChanged (int)), this, SLOT(salaryPopulateOtherDohodyTable()));
	connect(ui.spinBox_month_filtr, SIGNAL(valueChanged (int)), this, SLOT(salaryPopulateLikarnyaniTable()));
	connect(ui.spinBox_month_filtr, SIGNAL(valueChanged (int)), this, SLOT(salaryPopulateVidpuskniTable()));
	
	connect(ui.dateEdit_curDate, SIGNAL(dateChanged(const QDate &)), this, SLOT(curDateChanged(const QDate &)));
	
	connect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(populateSalaryForm(int))); //  populate salary form after spinbox_navigator val. ch.
	connect(ui.comboBox_Poshuk_PIB, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int))); //spinbox_navigator val. ch. after comboBox_poshuk_osoba_id val.ch.
	connect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), ui.spinBox_navigator, SLOT(setValue(int))); //spinbox_navigator val. ch. after comboBox_poshuk_osoba_id_rahunok val.ch.
	connect(ui.comboBox_poshuk_osoba_id, SIGNAL(currentIndexChanged (int)), this, SLOT(salaryFiltrComboboxIndexChanged(int))); //spinbox_navigator val. ch. after comboBox_poshuk_osoba_id_rahunok val.ch.
	connect(ui.pushButton_first, SIGNAL(clicked ()), this, SLOT(salaryFormFirst())); //first button --> spinBox_navigator
	connect(ui.pushButton_previous, SIGNAL(clicked ()), this, SLOT(salaryFormPrevious())); //previous button --> spinBox_navigator
	connect(ui.pushButton_next, SIGNAL(clicked ()), this, SLOT(salaryFormNext())); //next button --> spinBox_navigator
	connect(ui.pushButton_last, SIGNAL(clicked ()), this, SLOT(salaryFormLast())); //last button --> spinBox_navigator
	connect(ui.pushButton_new, SIGNAL(clicked ()), this, SLOT(salaryFormNew())); //new button --> new Record
	connect(ui.pushButton_delete, SIGNAL(clicked ()), this, SLOT(salaryFormDeleteRec()));
	
	connect(ui.spinBox_navigator, SIGNAL(valueChanged (int)), this, SLOT(enableNavigationButtons(int))); //enable/disable navigation buttons
	connect(ui.checkBox_Operacii_vidprac_godyny, SIGNAL(stateChanged(int)), this, SLOT(tableOperaciiVidpracGodyny()));
	connect(ui.checkBox_Operacii_narahovano, SIGNAL(stateChanged(int)), this, SLOT(tableOperaciiNarahovanoView()));
	connect(ui.checkBox_Operacii_utrymannya, SIGNAL(stateChanged(int)), this, SLOT(tableOperaciiUtrymannyaView()));
	connect(ui.checkBox_Operacii_pislyapodatk_vidrah, SIGNAL(stateChanged(int)), this, SLOT(tableOperaciiOplataView()));
	connect(ui.checkBox_Operacii_vidrahuv_na_zarplatu, SIGNAL(stateChanged(int)), this, SLOT(tableOperaciiVidrahuvannyaZarplatyView()));
	connect(ui.checkBox_Operacii_indexaciya, SIGNAL(stateChanged(int)), this, SLOT(tableOperaciiIndexaciyaView()));
	
	//Конектори таблиці Посади і тарифи
	connect(ui.pushButton_insertTaryf, SIGNAL(clicked ()), this, SLOT(pushButton_insertTaryf_clicked()));
	connect(ui.pushButton_delTaryf, SIGNAL(clicked ()), this, SLOT(pushButton_delTaryf_clicked()));
	
	connect(ui.pushButton_insert_Likarnyani, SIGNAL(clicked ()), this, SLOT(pushButton_insert_Likarnyani_clicked()));
	connect(ui.pushButton_del_Likarnyani, SIGNAL(clicked ()), this, SLOT(pushButton_del_Likarnyani_clicked()));
	
	connect(ui.pushButton_insert_Vidpuskni, SIGNAL(clicked ()), this, SLOT(pushButton_insert_Vidpuskni_clicked()));
	connect(ui.pushButton_del_Vidpuskni, SIGNAL(clicked ()), this, SLOT(pushButton_del_Vidpuskni_clicked()));
	
	connect(ui.pushButton_insert_other_dohid, SIGNAL(clicked ()), this, SLOT(pushButton_insert_other_dohid_clicked()));
	connect(ui.pushButton_del_other_dohid, SIGNAL(clicked ()), this, SLOT(pushButton_del_other_dohid_clicked()));
	
	connect(ui.pushButton_salary_form_save, SIGNAL(clicked ()), this, SLOT(salaryButtonSaveClicked()));
	connect(ui.pushButton_salary_form_abort, SIGNAL(clicked ()), this, SLOT(salaryButtonCancelClicked()));

	connect(ui.tabWidget_operacii, SIGNAL(currentChanged(int) ), this, SLOT(setOperaciiTabWidgetGUI(int))); 
}
//--------------------utech--------------------utech--------------------utech--------------------
