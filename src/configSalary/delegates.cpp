#include "delegates.h"
#include <QComboBox>
#include <QModelIndex>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent):QItemDelegate(parent)
{
	yesNoHash[0] = "ЭГ"; 	
	yesNoHash[1] = "връ";
}
//==========================
QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
	QComboBox *editor = new QComboBox(parent);
  editor->insertItem(0,"ЭГ");
  editor->insertItem(1,"връ");
  editor->installEventFilter(const_cast<ComboBoxDelegate*>(this));
  return editor;
}
//==========================
void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  int value = index.model()->data(index, Qt::DisplayRole).toInt();
  QComboBox *combo = static_cast<QComboBox*>(editor);
  combo->setCurrentIndex(value);
}
//===========================
void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *combo = static_cast<QComboBox*>(editor);
  int value = combo->currentIndex();
  model->setData(index, value);
}
//================================
void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
//=============================
void ComboBoxDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const 
{
	QStyleOptionViewItem opt = option;
	QString text; 
  text = yesNoHash.value(index.model()->data(index, Qt::DisplayRole).toInt());
  drawDisplay(painter, opt, opt.rect, text);
  drawFocus(painter, opt, opt.rect);
}
