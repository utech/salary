/****************************************************************************
**
** Created: Roman Mulyk 18.09.2006
**
****************************************************************************/

#include <QSqlQuery>

#include "UUserDelegate.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UUserDelegate::UUserDelegate(QObject *parent):QItemDelegate(parent)
{
	QSqlQuery query;
	query.exec("SELECT id, SELECT CONCAT_WS(' ',Prizv,Imia,Pobatk)AS name FROM npr");
	while (query.next())
		listPereviznyky[query.value(0).toInt()] = query.value(1).toString();
	isEdited = false;
}
//==========================
QWidget *UUserDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit* editor = new QLineEdit(parent);    
    editor->installEventFilter(const_cast<UUserDelegate*>(this));
    return editor;
}
//==========================
void UUserDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //QTextCodec* codec = QTextCodec::codecForName("cp1251"); 
    QString value;
    QLineEdit *t = static_cast<QLineEdit*>(editor);
    listPereviznyky.clear();
   	QSqlQuery query;
   	query.exec("SELECT id, SELECT CONCAT_WS(' ',Prizv,Imia,Pobatk)AS name FROM npr");
		while (query.next())
			listPereviznyky[query.value(0).toInt()] = query.value(1).toString();
   	value = listPereviznyky.value(index.model()->data(index, Qt::DisplayRole).toInt());
		t->setText(value);
  	isEdited = true;
}
//===========================
void UUserDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *t = static_cast<QLineEdit*>(editor);
    QString value = t->text();
    int id = 0;
    id = listPereviznyky.key(value);
    model->setData(index, id);
}
//================================
void UUserDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
//=============================
void UUserDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const 
{
	QStyleOptionViewItem opt = option; 
	QString text;
	if(!isEdited)
		text = index.data().toString();
	else
		text = listPereviznyky.value(index.model()->data(index, Qt::DisplayRole).toInt());
	drawDisplay(painter, opt, opt.rect, text);
  drawFocus(painter, opt, opt.rect);
}
