//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу делегата для таблиці тарифних сіток програми 
//  нарахування зарплати
//
// Створений програмістами Компанії "Утех" 28.02.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UTARYFDELEGATE_H
#define UTARYFDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QString>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QComboBox>

#include <UMath>

//--------------------utech--------------------utech--------------------utech--------------------
class UTaryfDelegate : public QItemDelegate
{	
   Q_OBJECT
public:
		UTaryfDelegate( QObject *parent = 0);
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const;
private slots:

private:
		int durationColumn_1;
		int durationColumn_2;
};      
//--------------------utech--------------------utech--------------------utech--------------------
#endif
