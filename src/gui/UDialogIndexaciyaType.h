//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу вікна налаштування базавого місяця індексації
//
// Створений програмістами Компанії "Утех" 25.03.2009р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIALOGINDEXACIYATYPE_H
#define UDIALOGINDEXACIYATYPE_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QSqlQuery>
#include <QItemDelegate>
#include <QComboBox>
#include <QStringList>

#include <UMath>
#include "../additional/UDifFunc.h"

#include "ui_UDialogIndexaciyaType.h"
//--------------------utech--------------------utech--------------------utech--------------------

//--------------------utech--------------------utech--------------------utech--------------------
class UDialogIndexaciyaType : public QDialog
{
  Q_OBJECT

  public:
		UDialogIndexaciyaType(short year, bool showOnlyWorkedWorkers, QWidget *parent = 0);
		~UDialogIndexaciyaType();
		
  public slots:
		void populateIndexTable();
		void saveData(QTableWidgetItem * item);
		
		void action_basingScriptDefinition_activated();
		void action_baseMonth_activated();
		void action_noBaseMonth_activated();
		void action_basingScriptDefinitionAll_activated();
		void action_baseMonthAll_activated();
		void action_noBaseMonthAll_activated();
  signals:
	  private:
		Ui::UDialogIndexaciyaType ui;
		bool onlyWorked;
};
//--------------------utech--------------------utech--------------------utech--------------------
class UIndexaciyaTypeDelegate: public QItemDelegate
{
	Q_OBJECT
  public:
		UIndexaciyaTypeDelegate(QObject *parent = 0);
		void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const;
  private:
	QStringList strList;
	short strCount;
}; 
//--------------------utech--------------------utech--------------------utech--------------------
#endif
