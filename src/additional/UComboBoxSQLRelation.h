//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу SQL комбобокса
//
// Створений програмістами Компанії "Утех" 28.02.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UCOMBOBOXSQLRELATION_H
#define UCOMBOBOXSQLRELATION_H

#include <QObject>
#include <QString>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QComboBox>
#include <QSqlQuery>
#include <QHash>

//--------------------utech--------------------utech--------------------utech--------------------
class UComboBoxSQLRelation : public QObject
{	
   Q_OBJECT
public:
		UComboBoxSQLRelation(QComboBox *combo=0, QString relTable="", QString idCol="", QString dataCol="", QObject *parent=0);
        int currentId();
		void setComboBox(QComboBox *relComboBox);
		QComboBox *comboBox();
public slots:
		void populateComboBox();
		void setCurrentId(int id);
	signals:
		void currentIdChanged();
private:
		QComboBox *_comboBox;
		QString relarionalTable;
		QString idColumn;
		QString dataColumn;
		QHash <int,int> relarionalHash;
};      
//--------------------utech--------------------utech--------------------utech--------------------
#endif
