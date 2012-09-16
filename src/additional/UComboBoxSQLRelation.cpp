//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу SQL комбобокса
//
// Створений програмістами Компанії "Утех" 28.02.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UComboBoxSQLRelation.h"

//--------------------utech--------------------utech--------------------utech--------------------
UComboBoxSQLRelation::UComboBoxSQLRelation(QComboBox *combo, QString relTable, QString idCol, QString dataCol, QObject *parent)
    : QObject(parent)
{
	_comboBox = combo;
	relarionalTable = relTable;
	idColumn = idCol;
	dataColumn = dataCol;
	
	populateComboBox();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UComboBoxSQLRelation::populateComboBox()
{
	_comboBox->blockSignals(true);
	int curId = currentId();
	relarionalHash.clear();
	_comboBox->clear();
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT "+idColumn+", "+dataColumn+" FROM "+relarionalTable+" ORDER BY "+dataColumn);
	relarionalHash.insert(0, 0);
	_comboBox->addItem("");
	while (query->next()){
		relarionalHash.insert(query->at()+1, query->value(0).toInt());
		_comboBox->addItem(query->value(1).toString());
	}
	setCurrentId(curId);
	_comboBox->blockSignals(false);
	if (curId != currentId())
		currentIdChanged(); 
	delete query;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UComboBoxSQLRelation::setCurrentId(int id)
{
	_comboBox->setCurrentIndex(relarionalHash.key(id,0));
}
//--------------------utech--------------------utech--------------------utech--------------------
int UComboBoxSQLRelation::currentId()
{
	return relarionalHash.value(_comboBox->currentIndex(),0);
}
//--------------------utech--------------------utech--------------------utech--------------------
void UComboBoxSQLRelation::setComboBox(QComboBox *relComboBox)
{
	_comboBox = relComboBox;
}
//--------------------utech--------------------utech--------------------utech--------------------
QComboBox *UComboBoxSQLRelation::comboBox()
{
	return _comboBox;
}
//--------------------utech--------------------utech--------------------utech--------------------
