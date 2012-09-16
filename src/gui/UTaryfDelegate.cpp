//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл визначення класу делегата для таблиці тарифних сіток програми 
//  нарахування зарплати
//
// Створений програмістами Компанії "Утех" 28.02.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include <QSqlQuery>

#include "UTaryfDelegate.h"

//--------------------utech--------------------utech--------------------utech--------------------
UTaryfDelegate::UTaryfDelegate(QObject *parent)
    : QItemDelegate(parent)
{
	durationColumn_1 = 1;
	durationColumn_2 = 2;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UTaryfDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
	QStyleOptionViewItem myOption = option;
    if ( index.column() == durationColumn_2 ){
        QString text = m_to_str_2(index.model()->data(index, Qt::DisplayRole).toDouble());
        myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
	}
	else{
        QItemDelegate::paint(painter, option, index);
    }
}
//--------------------utech--------------------utech--------------------utech--------------------
QWidget *UTaryfDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    //години
    if (index.column() == durationColumn_2) {
        QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(parent);
		doubleSpinBox->setMinimum ( 0 );
		doubleSpinBox->setMaximum ( 100000 );
		doubleSpinBox->setDecimals(2);
		
        connect(doubleSpinBox, SIGNAL(editingFinished()),
                this, SLOT(QItemDelegate::commitAndCloseEditor()));
        return doubleSpinBox;
    }
	else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}
//--------------------utech--------------------utech--------------------utech--------------------
void UTaryfDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
	//години
    if (index.column() == durationColumn_2){
		double num = index.model()->data(index, Qt::DisplayRole).toDouble();
        QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox *>(editor);
        doubleSpinBox->setValue(num);
		doubleSpinBox->selectAll();
    }

	else {
        QItemDelegate::setEditorData(editor, index);
    }
}
//--------------------utech--------------------utech--------------------utech--------------------
void UTaryfDelegate::setModelData(QWidget *editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
	//години
    if (index.column() == durationColumn_2){
        QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox *>(editor);
        double num = doubleSpinBox->value();
        model->setData(index, num);
    }
	else {
        QItemDelegate::setModelData(editor, model, index);
    }
}
//--------------------utech--------------------utech--------------------utech--------------------
