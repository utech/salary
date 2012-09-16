/****************************************************************************
**
** Created: Roman Mulyk 17.08.2006
**
****************************************************************************/

#ifndef UUSERDELEGATE_H
#define UUSERDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QHash>
#include <QDoubleSpinBox>
#include <QString>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QComboBox>

//----------interfaces-------------
class UUserDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    UUserDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint( QPainter *,const QStyleOptionViewItem& ,const QModelIndex&) const;
    mutable QHash<int,QString> listPereviznyky;
private:
    mutable bool isEdited;
};
//-----------------------------------
#endif
