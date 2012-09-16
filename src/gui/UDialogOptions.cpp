//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу вікна налаштування програми
//
// Створений програмістами Компанії "Утех" 30.03.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include "UDialogOptions.h"

#include "QFileDialog"
#include "QtDebug"
//--------------------utech--------------------utech--------------------utech--------------------
UDialogOptions::UDialogOptions(QWidget *parent)
        : QDialog(parent)
{
	ui.setupUi(this);
	
	buttonGroup = new QButtonGroup();
	buttonGroup->addButton(ui.toolButton_tabelOptions,0);
	buttonGroup->addButton(ui.toolButton_narahuvannyaOptions,1);
	buttonGroup->addButton(ui.toolButton_utrymannyaOptions,2);
	buttonGroup->addButton(ui.toolButton_scriptsOptions,3);
	buttonGroup->addButton(ui.toolButton_opzOptions,4);
	
	ui.toolButton_tabelOptions->setChecked(true);
	ui.tabWidget_Options->setCurrentIndex(0);
	
	populateOptions();
		//Connectors
	connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(pushButton_accept_clicked()));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(pushButton_cancel_clicked()));
	connect(ui.pushButton_zastosuvaty, SIGNAL(clicked()), this, SLOT(pushButton_zastosuvaty_clicked()));
	connect(buttonGroup, SIGNAL(buttonClicked(int)), ui.tabWidget_Options, SLOT(setCurrentIndex(int)));
	connect(ui.tabWidget_Options, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_Options_currentChanged(int)));
	
	//ToolButton select directory
	connect(ui.toolButton_vhSaldoScriptPath, SIGNAL(clicked()), this, SLOT(toolButton_vhSaldoScriptPath_clicked()));
	connect(ui.toolButton_osnNarahScriptPath, SIGNAL(clicked()), this, SLOT(toolButton_osnNarahScriptPath_clicked()));
	connect(ui.toolButton_utrVidr01ScriptPath, SIGNAL(clicked()), this, SLOT(toolButton_utrVidr01ScriptPath_clicked()));
	connect(ui.toolButton_utrVidr02ScriptPath, SIGNAL(clicked()), this, SLOT(toolButton_utrVidr02ScriptPath_clicked()));
	connect(ui.toolButton_oplataScriptPath, SIGNAL(clicked()), this, SLOT(toolButton_oplataScriptPath_clicked()));
	connect(ui.toolButton_opzDirPath, SIGNAL(clicked()), this, SLOT(toolButton_opzDirPath_clicked()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::connectEditres()
{
	connect(ui.radioButton_fullTabel, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	connect(ui.lineEdit_vhSaldoScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	connect(ui.lineEdit_osnNarahScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	connect(ui.lineEdit_utrVidr01ScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	connect(ui.lineEdit_utrVidr02ScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	connect(ui.lineEdit_oplataScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	connect(ui.lineEdit_opzDirPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	connect(ui.comboBox_opzPidprId, SIGNAL(currentIndexChanged(int)), this, SLOT(optionsEdited()));
	
	connect(ui.radioButton_noIndexaciya, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	connect(ui.radioButton_indexesNoNarah, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	connect(ui.radioButton_doIndexaciya, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	connect(ui.radioButton_socPilgaTilkyZOsobKartky, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	connect(ui.radioButton_socpilgaPoNarahuvanni, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	
	//Перечитування переліку організацій з OPZ
	connect(ui.lineEdit_opzDirPath, SIGNAL(textChanged(const QString &)), this, SLOT(populateOpzOrganizList()));
	connect(ui.lineEdit_opzDirPath, SIGNAL(textChanged(const QString &)), this, SLOT(populateOpzVersion()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::disconnectEditres()
{
	disconnect(ui.radioButton_fullTabel, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	disconnect(ui.lineEdit_vhSaldoScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	disconnect(ui.lineEdit_osnNarahScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	disconnect(ui.lineEdit_utrVidr01ScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	disconnect(ui.lineEdit_utrVidr02ScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	disconnect(ui.lineEdit_oplataScriptPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	disconnect(ui.lineEdit_opzDirPath, SIGNAL(textChanged(const QString &)), this, SLOT(optionsEdited()));
	disconnect(ui.comboBox_opzPidprId, SIGNAL(currentIndexChanged(int)), this, SLOT(optionsEdited()));
	
	disconnect(ui.radioButton_noIndexaciya, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	disconnect(ui.radioButton_indexesNoNarah, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	disconnect(ui.radioButton_doIndexaciya, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	disconnect(ui.radioButton_socPilgaTilkyZOsobKartky, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	disconnect(ui.radioButton_socpilgaPoNarahuvanni, SIGNAL(toggled(bool)), this, SLOT(optionsEdited()));
	
	//Перечитування переліку організацій з OPZ
	disconnect(ui.lineEdit_opzDirPath, SIGNAL(textChanged(const QString &)), this, SLOT(populateOpzOrganizList()));
	disconnect(ui.lineEdit_opzDirPath, SIGNAL(textChanged(const QString &)), this, SLOT(populateOpzVersion()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::populateOptions()
{
	disconnectEditres();
	int iVal;
	
	ui.pushButton_zastosuvaty->setEnabled(false);
		//Вид табеля
	if (getOption(1).toInt() == 0)
		ui.radioButton_fullTabel->setChecked(true);
	else
		ui.radioButton_SumTabel->setChecked(true);
		//Нарахування
	iVal = getOption(7).toInt();
	if (iVal == 0)
		ui.radioButton_noIndexaciya->setChecked(true);
	else if (iVal == 1)
		ui.radioButton_indexesNoNarah->setChecked(true);
	else if (iVal == 2)
		ui.radioButton_doIndexaciya->setChecked(true);
	else
		ui.radioButton_noIndexaciya->setChecked(true);
		//Утримання
	iVal = getOption(8).toInt();
	if (iVal == 0)
		ui.radioButton_socPilgaTilkyZOsobKartky->setChecked(true);
	else if (iVal == 1)
		ui.radioButton_socpilgaPoNarahuvanni->setChecked(true);
	else
		ui.radioButton_socpilgaPoNarahuvanni->setChecked(true);
		//Шляхи до файлів скриптів
	ui.lineEdit_vhSaldoScriptPath->setText(getOption(2));
	ui.lineEdit_osnNarahScriptPath->setText(getOption(3));
	ui.lineEdit_utrVidr01ScriptPath->setText(getOption(4));
	ui.lineEdit_utrVidr02ScriptPath->setText(getOption(5));
	ui.lineEdit_oplataScriptPath->setText(getOption(6));
		//Шлях до OPZ
	ui.lineEdit_opzDirPath->setText(getOption(100));
	populateOpzOrganizList();
	populateOpzVersion();
	
	optEdited = false;
	connectEditres();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::saveOptions()
{
	//Вид табеля
	if (ui.radioButton_fullTabel->isChecked())
		saveOption(1,0);
	else
		saveOption(1,1);
	//Нарахування
	if (ui.radioButton_noIndexaciya->isChecked())
		saveOption(7,0);
	else if (ui.radioButton_indexesNoNarah->isChecked())
		saveOption(7,1);
	else
		saveOption(7,2);
	//Утримання
	if (ui.radioButton_socPilgaTilkyZOsobKartky->isChecked())
		saveOption(8,0);
	else
		saveOption(8,1);
	//Шляхи до файлів скриптів
	saveOption(2,ui.lineEdit_vhSaldoScriptPath->text());
	saveOption(3,ui.lineEdit_osnNarahScriptPath->text());
	saveOption(4,ui.lineEdit_utrVidr01ScriptPath->text());
	saveOption(5,ui.lineEdit_utrVidr02ScriptPath->text());
	saveOption(6,ui.lineEdit_oplataScriptPath->text());
		//Налаштування OPZ
	saveOption(100,ui.lineEdit_opzDirPath->text());
	saveOption(101,ui.comboBox_opzPidprId->currentIndex());

	populateOptions();
}
//--------------------utech--------------------utech--------------------utech--------------------

void UDialogOptions::pushButton_accept_clicked()
{
	if (optEdited){
		saveOptions();
		emit requestReloadSettings();
	}
	accept();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::pushButton_cancel_clicked()
{
	reject();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::pushButton_zastosuvaty_clicked()
{
	saveOptions();
	emit requestReloadSettings();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::optionsEdited()
{
	ui.pushButton_zastosuvaty->setEnabled(true);
	optEdited = true;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::tabWidget_Options_currentChanged(int index)
{
	disconnect(buttonGroup, SIGNAL(buttonClicked(int)), ui.tabWidget_Options, SLOT(setCurrentIndex(int)));
	
	buttonGroup->button(index)->setChecked(true);
	
	connect(buttonGroup, SIGNAL(buttonClicked(int)), ui.tabWidget_Options, SLOT(setCurrentIndex(int)));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::populateOpzOrganizList()
{
	int orgIndex = getOption(101).toInt();
	QStringList orgsList = getOpzOrgsList(ui.lineEdit_opzDirPath->text());
	ui.comboBox_opzPidprId->clear();
	ui.comboBox_opzPidprId->addItem("");
	foreach (QString str, orgsList)
		ui.comboBox_opzPidprId->addItem(str);
	if (orgIndex<=orgsList.size())
		ui.comboBox_opzPidprId->setCurrentIndex(orgIndex);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::populateOpzVersion()
{
	ui.label_programVersionValue->setText(opzVersion(ui.lineEdit_opzDirPath->text()));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::toolButton_vhSaldoScriptPath_clicked()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QString curPath = ui.lineEdit_vhSaldoScriptPath->text();
	if (curPath.size() == 0 || !QFile::exists(curPath))
		curPath = "../script/";
	QString fileName = QFileDialog::getOpenFileName(this,
				"Пошук файла скрипта обрахунку вхідного сальдо", curPath, "Файли скриптів (*.js *.tis)");
	if (fileName.size() == 0)
		return;
	QDir dir(QCoreApplication::applicationDirPath());
	ui.lineEdit_vhSaldoScriptPath->setText(dir.relativeFilePath(fileName));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::toolButton_osnNarahScriptPath_clicked()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QString curPath = ui.lineEdit_osnNarahScriptPath->text();
	if (curPath.size() == 0 || !QFile::exists(curPath))
		curPath = "../script/";
	QString fileName = QFileDialog::getOpenFileName(this,
				"Пошук файла скрипта основного нарахування", curPath, "Файли скриптів (*.js *.tis)");
	if (fileName.size() == 0)
		return;
	QDir dir(QCoreApplication::applicationDirPath());
	ui.lineEdit_osnNarahScriptPath->setText(dir.relativeFilePath(fileName));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::toolButton_utrVidr01ScriptPath_clicked()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QString curPath = ui.lineEdit_utrVidr01ScriptPath->text();
	if (curPath.size() == 0 || !QFile::exists(curPath))
		curPath = "../script/";
	QString fileName = QFileDialog::getOpenFileName(this,
				"Пошук файла скрипта утр. і відрах. ОД=01", curPath, "Файли скриптів (*.js *.tis)");
	if (fileName.size() == 0)
		return;
	QDir dir(QCoreApplication::applicationDirPath());
	ui.lineEdit_utrVidr01ScriptPath->setText(dir.relativeFilePath(fileName));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::toolButton_utrVidr02ScriptPath_clicked()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QString curPath = ui.lineEdit_utrVidr02ScriptPath->text();
	if (curPath.size() == 0 || !QFile::exists(curPath))
		curPath = "../script/";
	QString fileName = QFileDialog::getOpenFileName(this,
				"Пошук файла скрипта утр. і відрах. ОД>01", curPath, "Файли скриптів (*.js *.tis)");
	if (fileName.size() == 0)
		return;
	QDir dir(QCoreApplication::applicationDirPath());
	ui.lineEdit_utrVidr02ScriptPath->setText(dir.relativeFilePath(fileName));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::toolButton_oplataScriptPath_clicked()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	QString curPath = ui.lineEdit_oplataScriptPath->text();
	if (curPath.size() == 0 || !QFile::exists(curPath))
		curPath = "../script/";
	QString fileName = QFileDialog::getOpenFileName(this,
				"Пошук файла скрипта обрахунку оплати", curPath, "Файли скриптів (*.js *.tis)");
	if (fileName.size() == 0)
		return;
	QDir dir(QCoreApplication::applicationDirPath());
	ui.lineEdit_oplataScriptPath->setText(dir.relativeFilePath(fileName));
}
//--------------------utech--------------------utech--------------------utech--------------------
void UDialogOptions::toolButton_opzDirPath_clicked()
{
	QString curPath = ui.lineEdit_opzDirPath->text();
	if (curPath.size() == 0 || !QFile::exists(curPath))
		curPath = OPZ_DEFAULT_PATH;
	QDir::setCurrent(OPZ_DEFAULT_PATH);
	QString dirName = QFileDialog::getExistingDirectory(this,
				"Пошук папки, у якій встановлено програму \"Податкова звітність\"", curPath, 
				QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dirName.size() == 0)
		return;
	ui.lineEdit_opzDirPath->setText(dirName);
}
//--------------------utech--------------------utech--------------------utech--------------------
UDialogOptions::~UDialogOptions()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
