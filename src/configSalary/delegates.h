#ifndef __DELEGATES_H__
#define __DELEGATES_H__

#include <QItemDelegate>
#include <QHash>

//===========================
class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
  ComboBoxDelegate(QObject *parent = 0);
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const; 
  mutable QHash<int,QString> yesNoHash;
};

#endif // __DELEGATES_H__
