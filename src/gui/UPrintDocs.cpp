//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ��������� ����� UPrintDocs �������� ����������� ��������
//
// ��������� ������������ ������ "����" 17.07.2007�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UPrintDocs.h"

#include <UPopulateTextTableCells>
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UPrintDocs::UPrintDocs()
{
	monthlist 	<< "�����"
				<< "�����"
				<< "��������"
				<< "������"
				<< "�������"
				<< "�������"
				<< "������"
				<< "�������"
				<< "��������"
				<< "�������"
				<< "��������"
				<< "�������";
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::printFragment(QTextDocumentFragment fragment, QString docName, QPrinter::Orientation printerOrientation)
{
	printform = new UPrintForm(0, docName);
	printform->printer()->setOrientation( printerOrientation );
	
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	cursor.insertFragment(fragment);
	
	printform->printer()->setDocName(docName);
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::perelik_pracivnykiv(short pidrozdilId, bool pracFilter)
{
	QString str;
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) FROM npr"
					+(pracFilter?QString(" WHERE Pracuye=true"):QString(""))
					+(pidrozdilId>0?QString(" and Pidrozdil_id=")+QString::number(pidrozdilId):QString(""))
				);
	query->next();
	int numRows;
	UPostupForm *postupForm = new UPostupForm(0, query->value(0).toInt() );
	postupForm->show();
	
	printform = new UPrintForm(0, "������ ����������");
	printform->printer()->setOrientation( QPrinter::Landscape );
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	//����� ���������
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 8 );
	cursor.insertText("�������� �������� ��������� \"������\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 11 );
	cursor.insertText("��²���� ���ֲ���ʲ� " + OrganizShortName(), textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 6 );
	cursor.insertText(" ", textCharFormat);
	
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table;
	
	QSqlQuery pQuery;
	pQuery.exec("SELECT id, Name FROM pidrozdily_salary "
					+(pidrozdilId>0?QString("WHERE id=")+QString::number(pidrozdilId):QString(""))
					+" ORDER BY Name");
	bool firstPidrozdil = true;
	while (pQuery.next()) {
		cursor.movePosition(QTextCursor::End);
		blockFormat.setAlignment( Qt::AlignLeft );
		if (!firstPidrozdil)
			blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
		firstPidrozdil = false;
		
		cursor.insertBlock(blockFormat);
		blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_Auto);
		cursor.insertBlock(blockFormat);
		textCharFormat_bold.setFontPointSize(10);
		cursor.insertText("	ϳ������: "+pQuery.value(1).toString(),textCharFormat_bold);
		
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 140);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
		constraints << QTextLength(QTextLength::FixedLength, 40);
	
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(1);
		tableFormat.setHeaderRowCount(1);
		tableFormat.setBorder(0.5);
		
		QString nprFilter;
		nprFilter = " WHERE npr.Pidrozdil_id="+pQuery.value(0).toString()+" ";
		if (pracFilter)
			nprFilter += " and npr.Pracuye=true ";
		
		query->exec("SELECT count(*) \
				FROM npr \
				LEFT JOIN taryfni_sitky \
					ON taryfni_sitky.id=npr.Posada_id "+nprFilter);
		query->next();
		numRows = query->value(0).toInt();
		
		table = cursor.insertTable(numRows+1, 16, tableFormat);
		//���������� ����� �������
		textCharFormat.setFontPointSize( 9 );
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		textCharFormat_bold.setFontPointSize( 9 );
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		fnt = textCharFormat.font();
		fnt.setItalic(true);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(true);
		textCharFormat_bold.setFont(fnt);
		
		blockFormat.setBottomMargin(0);
		blockFormat.setTopMargin(0);
	
		cell = table->cellAt(0, 0);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("� �/�",textCharFormat);
	
		cell = table->cellAt(0, 1);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("� � �",textCharFormat_bold);
	
		cell = table->cellAt(0, 2);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("�.���.",textCharFormat);
	
		cell = table->cellAt(0, 3);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("�������.���",textCharFormat);
	
		cell = table->cellAt(0, 4);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("�.�����.",textCharFormat);
	
		cell = table->cellAt(0, 5);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("������",textCharFormat);
	
		cell = table->cellAt(0, 6);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("���./���.",textCharFormat);
	
		cell = table->cellAt(0, 7);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("�����",textCharFormat);
	
		cell = table->cellAt(0, 8);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("���.",textCharFormat);
	
		cell = table->cellAt(0, 9);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("����.",textCharFormat);
	
		cell = table->cellAt(0, 10);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("��",textCharFormat);
	
		cell = table->cellAt(0, 11);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("��",textCharFormat);
	
		cell = table->cellAt(0, 12);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("����.",textCharFormat);
	
		cell = table->cellAt(0, 13);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("���.",textCharFormat);
		
		cell = table->cellAt(0, 14);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("��/����/���",textCharFormat);
		
		cell = table->cellAt(0, 15);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("����.",textCharFormat);
	
		//���������� �������
		int workPracivnykyCount = 0,
		workShtatCount = 0,
		workDogovirCount = 0,
		workOPCount = 0,
		workKVCount = 0,
		workDoplataCount = 0,
		workAlimentyCount = 0,
		workProfspilkaCount = 0,
		workPensioneryCount = 0,
		workInvalidyCount = 0;
	
		fnt = textCharFormat.font();
		fnt.setItalic(false);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(false);
		textCharFormat_bold.setFont(fnt);
		
		query->exec("SELECT npr.id, Prizv, Imia, Pobatk, Data_nar, taryfni_sitky.Posada, \
						TypRoboty, TypOplaty, Iden_kod, Tel, Rozryad, Stavka, \
						Num_dit, Ozn_pilgy, Data_pruin_na_rob, Klasnist_vodiya, \
						VidsDoplaty, Alimenty, Pensioner, Buh_rahunok, \
						ChlenProfspilky, Invalid, Pracuye \
				FROM npr \
				LEFT JOIN taryfni_sitky \
					ON taryfni_sitky.id=npr.Posada_id "+nprFilter+"\
				ORDER BY Prizv, Imia, Pobatk");
		int row;
		bool lastWork=true;
		if (query->seek(0))
			lastWork = query->value(22).toBool();
		query->seek(-1);
		
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
		
		for (row=1; (row<=numRows) && query->next(); row++){
			bool isWork = query->value(22).toBool();
			if (lastWork!=isWork){
				fnt = textCharFormat.font();
				fnt.setItalic(!isWork);
				textCharFormat.setFont(fnt);
				fnt = textCharFormat_bold.font();
				fnt.setItalic(!isWork);
				textCharFormat_bold.setFont(fnt);
				lastWork=isWork;
			}
		
			//� �/�
			cell = table->cellAt(row, 0);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(QString::number(row),textCharFormat);
			//� � � (��)
			cell = table->cellAt(row, 1);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(1).toString()+" "+query->value(2).toString()+" "+
									query->value(3).toString()+" ("+query->value(0).toString()+")",textCharFormat_bold);
			//�.���.
			cell = table->cellAt(row, 2);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(4).toDate().toString("dd.MM.yyyy"),textCharFormat);
			//�������.���
			cell = table->cellAt(row, 3);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(8).toString(),textCharFormat);
			//�.�����.
			cell = table->cellAt(row, 4);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(14).toDate().toString("dd.MM.yyyy"),textCharFormat);
			//������
			cell = table->cellAt(row, 5);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(5).toString(),textCharFormat);
			//���./���.
			cell = table->cellAt(row, 6);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(6).toInt()==0){
				str = "��/";
				if (isWork)
					workShtatCount++;
			}
			else{
				str = "���/";
				if (isWork)
					workDogovirCount++;
			}
			if (query->value(7).toInt()==0)
				str += "��";
			else
				str += "���";
			cellCursor.insertText(str,textCharFormat);
			//�����
			cell = table->cellAt(row, 7);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(m_to_str_2(query->value(11).toDouble()),textCharFormat);
			//���.
			cell = table->cellAt(row, 8);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(9).toString(),textCharFormat);
			//����.
			cell = table->cellAt(row, 9);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(7).toInt()==1)
				str = query->value(10).toString();
			else
				str = "-";
			cellCursor.insertText(str,textCharFormat);
			//��
			cell = table->cellAt(row, 10);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(13).toDouble()!=0){
				str = query->value(13).toString();
				if (isWork)
					workOPCount++;
			}
			else
				str = "-";
			cellCursor.insertText(str,textCharFormat);
			//��
			cell = table->cellAt(row, 11);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(15).toDouble()!=0){
				str = query->value(15).toString();
				if (isWork)
					workKVCount++;
			}
			else
				str = "-";
			cellCursor.insertText(str,textCharFormat);
			//����.
			cell = table->cellAt(row, 12);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(16).toDouble()!=0){
				str = query->value(16).toString();
				if (isWork)
					workDoplataCount++;
			}
			else
				str = "-";
			cellCursor.insertText(str,textCharFormat);
			//���.
			cell = table->cellAt(row, 13);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(17).toDouble()!=0){
				str = query->value(17).toString();
				if (isWork)
					workAlimentyCount++;
			}
			else
				str = "-";
			cellCursor.insertText(str,textCharFormat);
			//��/����/���
			cell = table->cellAt(row, 14);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(20).toBool()){
				str = "+/";
				if (isWork)
					workProfspilkaCount++;
			}
			else
				str = "-/";
			if (query->value(18).toBool()){
				str += "+/";
				if (isWork)
					workPensioneryCount++;
			}
			else
				str += "-/";
			if (query->value(21).toBool()){
				str += "+";
				if (isWork)
					workInvalidyCount++;
			}
			else
				str += "-";
			cellCursor.insertText(str,textCharFormat);
			//����.
			cell = table->cellAt(row, 15);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (isWork){
				str = "���";
				workPracivnykyCount++;
			}
			else
				str = "ͳ";
			cellCursor.insertText(str,textCharFormat);
		}
	
		//����� ���������� ����������
		fnt = textCharFormat.font();
		fnt.setItalic(false);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(false);
		textCharFormat_bold.setFont(fnt);
		
		cursor.movePosition(QTextCursor::End);
		blockFormat.setAlignment( Qt::AlignLeft );
		cursor.insertBlock(blockFormat);
		cursor.insertBlock(blockFormat);
		//������ ���������� � ���
		cursor.insertBlock(blockFormat);
		textCharFormat.setFontPointSize( 10 );
		cursor.insertText("1. ������ ���������� � ���			"+QString::number(numRows), textCharFormat);
		//������ ���������
		cursor.insertBlock(blockFormat);
		cursor.insertText("2. ������ ���������				"+QString::number(workPracivnykyCount), textCharFormat);
		//� �.�. � ����
		cursor.insertBlock(blockFormat);
		cursor.insertText("	� �.�. � ����				"+QString::number(workShtatCount), textCharFormat);
		//� �.�. �� ��������
		cursor.insertBlock(blockFormat);
		cursor.insertText("	� �.�. �� ��������			"+QString::number(workDogovirCount), textCharFormat);
		//��������� �����
		cursor.insertBlock(blockFormat);
		cursor.insertText("3.����������, �� ��������� ��������� �����		"+QString::number(workOPCount), textCharFormat);
		//�������� ����
		cursor.insertBlock(blockFormat);
		cursor.insertText("4.����������, �� ��������� ������� \"�������� ����\"	"+QString::number(workKVCount), textCharFormat);
		//�������
		cursor.insertBlock(blockFormat);
		cursor.insertText("5.����������, �� ��������� �������			"+QString::number(workDoplataCount), textCharFormat);
		//�������
		cursor.insertBlock(blockFormat);
		cursor.insertText("6.����������, �� ��������� �������			"+QString::number(workAlimentyCount), textCharFormat);
		//���������
		cursor.insertBlock(blockFormat);
		cursor.insertText("7.����������, �� � ������� ���������			"+QString::number(workProfspilkaCount), textCharFormat);
		//���������
		cursor.insertBlock(blockFormat);
		cursor.insertText("8.���������					"+QString::number(workPensioneryCount), textCharFormat);
		//�������
		cursor.insertBlock(blockFormat);
		cursor.insertText("9.�������					"+QString::number(workInvalidyCount), textCharFormat);
	}
	
	delete postupForm;
	delete query;
	
	printform->printer()->setDocName("������ ���������� "+OrganizShortName());
	printform->printer()->setPageMargins ( 10, 10, 10, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_narahuvannya_za_misac(int year, int month, short pidrozdilId, bool pracFilter)
{
	QSqlQuery *nprQuery = new QSqlQuery();
	QSqlQuery *query = new QSqlQuery();
	QSqlRecord record;
	query->exec("SELECT count(*) FROM operacii \
				WHERE year(CurDate)="+QString::number(year)+" \
					and month(CurDate)="+QString::number(month)
					+(pracFilter?QString(" and (operacii.Pracuye=true or operacii.OznakaDohodu>1) "):QString(""))
					+(pidrozdilId>0?QString(" and operacii.Pidrozdil_id=")+QString::number(pidrozdilId):QString(""))
				);
	query->seek(0);
	int numRows = query->value(0).toInt();
	UPostupForm *postupForm = new UPostupForm(0, numRows );
	postupForm->show();
	
	printform = new UPrintForm(0, "����������� �� " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� ����������� �������� �� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("�� ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("�."), textCharFormat);
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table;
	UPopulateCell tableCell(NULL);
	
	QSqlQuery pQuery;
	pQuery.exec("SELECT id, Name FROM pidrozdily_salary "
					+(pidrozdilId>0?QString("WHERE id=")+QString::number(pidrozdilId):QString(""))
					+" ORDER BY Name");
	bool firstPidrozdil = true;
	while (pQuery.next()) {
		cursor.movePosition(QTextCursor::End);
		blockFormat.setAlignment( Qt::AlignLeft );
		if (!firstPidrozdil)
			blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
		firstPidrozdil = false;
		
		cursor.insertBlock(blockFormat);
		blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_Auto);
		cursor.insertBlock(blockFormat);
		textCharFormat_bold.setFontPointSize(10);
		cursor.insertText("	ϳ������: "+pQuery.value(1).toString(),textCharFormat_bold);
	
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 130);
		constraints << QTextLength(QTextLength::FixedLength, 50);
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 51);
		constraints << QTextLength(QTextLength::FixedLength, 60);
	
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(1);
		tableFormat.setHeaderRowCount(1);
		tableFormat.setBorder(0.5);
		
		QString operaciiPidrozdFilter;
		operaciiPidrozdFilter += " and operacii.Pidrozdil_id="+pQuery.value(0).toString()+" ";
		if (pracFilter)
			operaciiPidrozdFilter += " and (operacii.Pracuye=true or operacii.OznakaDohodu>1) ";
		
		nprQuery->exec("SELECT count(*) \
					FROM operacii \
					WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+operaciiPidrozdFilter);
		nprQuery->next();
		numRows = nprQuery->value(0).toInt();
		
		table = cursor.insertTable(numRows+3, 19, tableFormat);
		tableCell.setTextTable(table);
	
		QString lStr="\n-------\n";
		//���������� ����� �������
		textCharFormat.setFontPointSize( 9 );
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		textCharFormat_bold.setFontPointSize( 9 );
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		fnt = textCharFormat.font();
		fnt.setItalic(true);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(true);
		textCharFormat_bold.setFont(fnt);
	
		blockFormat.setBottomMargin(0);
		blockFormat.setTopMargin(0);
	
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(0,0,"� �/�", textCharFormat);
		tableCell.set(0,1,"� � �", textCharFormat);
		tableCell.set(0,2,"������", textCharFormat);
		tableCell.set(0,3,"��", textCharFormat);
		tableCell.set(0,4,"���.\n�����.", textCharFormat);
		tableCell.set(0,5,"�������", textCharFormat);
		tableCell.set(0,6,"˳����.", textCharFormat);
		tableCell.set(0,7,"³�����", textCharFormat);
		tableCell.set(0,8,"�����.", textCharFormat);
		tableCell.set(0,9,"ͳ��", textCharFormat);
		tableCell.set(0,10,"��������.", textCharFormat);
		tableCell.set(0,11,"�����.\n���.", textCharFormat);
		tableCell.set(0,12,"�����", textCharFormat);
		tableCell.set(0,13,"�����.\n���.", textCharFormat);
		tableCell.set(0,14,"�������", textCharFormat);
	
		tableCell.set(0,15,"������.", textCharFormat);
	
		tableCell.set(0,16,"����.\n������.", textCharFormat);
		tableCell.set(0,17,"��.\n�����.", textCharFormat);
		tableCell.set(0,18,"������", textCharFormat);

		//���������� �������
		fnt = textCharFormat.font();
		fnt.setItalic(false);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(false);
		textCharFormat_bold.setFont(fnt);

		double N_Osn_narah_Sum=0, N_VuslRokiv_Sum=0, N_Likarn_Sum=0, N_Vidpuskni_Sum=0, N_Svatkovi_dni_Sum=0, 
				N_Nichni_Sum=0, N_Nadurochni_Sum=0, N_Klasn_vodiya_Sum=0, N_Premia_Sum=0, N_MaterDopomoga_Sum=0, 
				N_Doplata_Sum=0, N_Indexaciya_Sum=0, N_VyplProfspilky_Sum=0, N_InshiNarahuvannya_Sum=0;
		double allWorkersAllNarahSum=0;
		double N_Osn_Narah_Sum_Other_Dohid=0;
		double workerNarahSum=0;
		double val;
		
		nprQuery->exec("SELECT Npr_id, count(*), npr.Prizv, npr.Imia, npr.Pobatk, npr.TypOplaty \
					FROM operacii \
					LEFT JOIN npr ON npr.id=operacii.Npr_ID \
					WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+operaciiPidrozdFilter+" \
					GROUP BY Npr_id \
					ORDER BY npr.Prizv, npr.Imia, npr.Pobatk");
		int row;
		int workersCounter = 0;
	
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
		for (row=1; (row<=numRows) && (nprQuery->next());){
			int operCount = nprQuery->value(1).toInt();
			query->exec("SELECT OznakaDohodu, N_Osn_narah, N_VuslRokiv, N_Likarn, N_Vidpuskni, N_Svatkovi_dni, \
						N_Nichni, N_Nadurochni, N_Klasn_vodiya, N_Premia, N_MaterDopomoga, \
						N_Doplata, N_Indexaciya, N_VyplProfspilky, N_InshiNarahuvannya, VydTabelyaInMonth \
					FROM operacii \
					WHERE Npr_id="+mysqlStr(nprQuery->value(0).toInt())+" \
						and year(CurDate)="+QString::number(year)+" \
						and month(CurDate)="+QString::number(month)+" \
					ORDER BY OznakaDohodu \
					LIMIT "+mysqlStr(operCount));
			
			if (operCount > 1){
				table->mergeCells(row,0,operCount,1);
				table->mergeCells(row,1,operCount,1);
				table->mergeCells(row,2,operCount,1);
			}
		
			bool firstWorkerRow = true;
			while (query->next()){
				record = query->record();
				short oznakaDohodu = record.value("OznakaDohodu").toInt();
				workerNarahSum = 0;
				
				//� �/�
				tableCell.setAlignment(Qt::AlignCenter);
				if (oznakaDohodu == 1)
					tableCell.set(row,0,QString::number(++workersCounter), textCharFormat);
			
				if (firstWorkerRow){
					//� � �
					tableCell.setAlignment(Qt::AlignLeft);
					tableCell.set(row,1,nprQuery->value(2).toString().toUpper() +" "+nprQuery->value(3).toString() +" "+nprQuery->value(4).toString(), textCharFormat);
			
					//������
					tableCell.setAlignment(Qt::AlignCenter);
					tableCell.set(row,2,tabelResString(nprQuery->value(0).toInt(), year, month, record.value("VydTabelyaInMonth").toInt(), nprQuery->value(5).toInt()), textCharFormat);
				
					firstWorkerRow = false;
				}
			
				//������ ������
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,3,oznakaDohodu, textCharFormat);
			
				//������� �����������
				tableCell.setAlignment(Qt::AlignRight);
				val = query->value(1).toDouble();
				N_Osn_narah_Sum += val;
				if (oznakaDohodu > 1)
					N_Osn_Narah_Sum_Other_Dohid += val;
				workerNarahSum += val;
				tableCell.set(row,4,val, textCharFormat);
			
				if (oznakaDohodu == 1){
					//������� ����
					val = query->value(2).toDouble();
					N_VuslRokiv_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,5,val, textCharFormat);
					else
						tableCell.set(row,5,"-", textCharFormat);
					//"˳����."
					val = query->value(3).toDouble();
					N_Likarn_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,6,val, textCharFormat);
					else
						tableCell.set(row,6,"-", textCharFormat);
					//"³�����"
					val = query->value(4).toDouble();
					N_Vidpuskni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,7,val, textCharFormat);
					else
						tableCell.set(row,7,"-", textCharFormat);
					//"�����."
					val = query->value(5).toDouble();
					N_Svatkovi_dni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,8,val, textCharFormat);
					else
						tableCell.set(row,8,"-", textCharFormat);
					//"ͳ��"
					val = query->value(6).toDouble();
					N_Nichni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,9,val, textCharFormat);
					else
						tableCell.set(row,9,"-", textCharFormat);
					//"��������."
					val = query->value(7).toDouble();
					N_Nadurochni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,10,val, textCharFormat);
					else
						tableCell.set(row,10,"-", textCharFormat);
					//"�����.\n���."
					val = query->value(8).toDouble();
					N_Klasn_vodiya_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,11,val, textCharFormat);
					else
						tableCell.set(row,11,"-", textCharFormat);
					//"�����"
					val = query->value(9).toDouble();
					N_Premia_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,12,val, textCharFormat);
					else
						tableCell.set(row,12,"-", textCharFormat);
					//"�����.\n���."
					val = query->value(10).toDouble();
					N_MaterDopomoga_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,13,val, textCharFormat);
					else
						tableCell.set(row,13,"-", textCharFormat);
					//"�������"
					val = query->value(11).toDouble();
					N_Doplata_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,14,val, textCharFormat);
					else
						tableCell.set(row,14,"-", textCharFormat);
					//"����������"
					val = query->value(12).toDouble();
					N_Indexaciya_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,15,val, textCharFormat);
					else
						tableCell.set(row,15,"-", textCharFormat);	
					//"����.\n������."
					val = query->value(13).toDouble();
					N_VyplProfspilky_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,16,val, textCharFormat);
					else
						tableCell.set(row,16,"-", textCharFormat);
					//"��.\n�����."
					val = query->value(14).toDouble();
					N_InshiNarahuvannya_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,17,val, textCharFormat);
					else
						tableCell.set(row,17,"-", textCharFormat);
					//"������"
					allWorkersAllNarahSum += workerNarahSum;
					tableCell.set(row,18,workerNarahSum, textCharFormat);
				}
				else{
					//������� ����
					tableCell.set(row,5,"-", textCharFormat);
					//"˳����."
					tableCell.set(row,6,"-", textCharFormat);	
					//"³�����"
					tableCell.set(row,7,"-", textCharFormat);
					//"�����."
					tableCell.set(row,8,"-", textCharFormat);
					//"ͳ��"
					tableCell.set(row,9,"-", textCharFormat);
					//"��������."
					tableCell.set(row,10,"-", textCharFormat);
					//"�����.\n���."
					tableCell.set(row,11,"-", textCharFormat);
					//"�����"
					tableCell.set(row,12,"-", textCharFormat);
					//"�����.\n���."
					tableCell.set(row,13,"-", textCharFormat);
					//"�������"
					tableCell.set(row,14,"-", textCharFormat);
					//"����������"
					tableCell.set(row,15,"-", textCharFormat);
					//"����.\n������."
					tableCell.set(row,16,"-", textCharFormat);
					//"��.\n�����."
					tableCell.set(row,17,"-", textCharFormat);
					//"������"
					allWorkersAllNarahSum += workerNarahSum;
					tableCell.set(row,18,workerNarahSum, textCharFormat);
				}
			
				row++;
				postupForm->inc_pos();
			}
		}
			//� ���� ���� � �� = 01 ----------------------------
		table->mergeCells(row,0,1,2);
		
		//������ � �.�. �������. � �� = 01"
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"�������. � �� = 01", textCharFormat);
		
		//������ ������
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,3,"1", textCharFormat);
		
		//������� �����������
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,4,N_Osn_narah_Sum-N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
		
		//������� ����
		if (N_VuslRokiv_Sum != 0)
			tableCell.set(row,5,N_VuslRokiv_Sum, textCharFormat);
		else
			tableCell.set(row,5,"-", textCharFormat);
		//"˳����."
		if (N_Likarn_Sum != 0)
			tableCell.set(row,6,N_Likarn_Sum, textCharFormat);
		else
			tableCell.set(row,6,"-", textCharFormat);
		//"³�����"
		if (N_Vidpuskni_Sum != 0)
			tableCell.set(row,7,N_Vidpuskni_Sum, textCharFormat);
		else
			tableCell.set(row,7,"-", textCharFormat);
		//"�����."
		if (N_Svatkovi_dni_Sum != 0)
			tableCell.set(row,8,N_Svatkovi_dni_Sum, textCharFormat);
		else
			tableCell.set(row,8,"-", textCharFormat);
		//"ͳ��"
		if (N_Nichni_Sum != 0)
			tableCell.set(row,9,N_Nichni_Sum, textCharFormat);
		else
			tableCell.set(row,9,"-", textCharFormat);
		//"��������."
		if (N_Nadurochni_Sum != 0)
			tableCell.set(row,10,N_Nadurochni_Sum, textCharFormat);
		else
			tableCell.set(row,10,"-", textCharFormat);
		//"�����.\n���."
		if (N_Klasn_vodiya_Sum != 0)
			tableCell.set(row,11,N_Klasn_vodiya_Sum, textCharFormat);
		else
			tableCell.set(row,11,"-", textCharFormat);
		//"�����"
		if (N_Premia_Sum != 0)
			tableCell.set(row,12,N_Premia_Sum, textCharFormat);
		else
			tableCell.set(row,12,"-", textCharFormat);
		//"�����.\n���."
		if (N_MaterDopomoga_Sum != 0)
			tableCell.set(row,13,N_MaterDopomoga_Sum, textCharFormat);
		else
			tableCell.set(row,13,"-", textCharFormat);
		//"�������"
		if (N_Doplata_Sum != 0)
			tableCell.set(row,14,N_Doplata_Sum, textCharFormat);
		else
			tableCell.set(row,14,"-", textCharFormat);
		//"����������"
		if (N_Doplata_Sum != 0)
			tableCell.set(row,15,N_Indexaciya_Sum, textCharFormat);
		else
			tableCell.set(row,15,"-", textCharFormat);
		//"����.\n������."
		if (N_VyplProfspilky_Sum != 0)
			tableCell.set(row,16,N_VyplProfspilky_Sum, textCharFormat);
		else
			tableCell.set(row,16,"-", textCharFormat);
		//"��.\n�����."
		if (N_InshiNarahuvannya_Sum != 0)
			tableCell.set(row,17,N_InshiNarahuvannya_Sum, textCharFormat);
		else
			tableCell.set(row,17,"-", textCharFormat);
		//"������"
		tableCell.set(row,18,allWorkersAllNarahSum-N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
		
		//� ���� ���� � �� > 01 ----------------------------
		row++;
		table->mergeCells(row,0,1,2);
	
		//������ � �.�. �������. � �� = 01"
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"�������. � �� > 01", textCharFormat);
	
		//������ ������
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,3,">1", textCharFormat);
	
		//������� �����������
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,4,N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
	
		//������� ����
		tableCell.set(row,5,"-", textCharFormat);
		//"˳����."
		tableCell.set(row,6,"-", textCharFormat);
		//"³�����"
		tableCell.set(row,7,"-", textCharFormat);
		//"�����."
		tableCell.set(row,8,"-", textCharFormat);
		//"ͳ��"
		tableCell.set(row,9,"-", textCharFormat);
		//"��������."
		tableCell.set(row,10,"-", textCharFormat);
		//"�����.\n���."
		tableCell.set(row,11,"-", textCharFormat);
		//"�����"
		tableCell.set(row,12,"-", textCharFormat);
		//"�����.\n���."
		tableCell.set(row,13,"-", textCharFormat);
		//"�������"
		tableCell.set(row,14,"-", textCharFormat);
		//"����������"
		tableCell.set(row,15,"-", textCharFormat);
		//"����.\n������."
		tableCell.set(row,16,"-", textCharFormat);
		//"��.\n�����."
		tableCell.set(row,17,"-", textCharFormat);
		//"������"
		tableCell.set(row,18,N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
	}
	
	delete nprQuery;
	delete query;
	delete postupForm;
	printform->printer()->setDocName("����������� �� "+QString::number(month)+"."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_worker_cards(int year, int month, short pidrozdilId, bool pracFilter)
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) FROM operacii \
				WHERE OznakaDohodu=1 \
					and year(CurDate)="+QString::number(year)+" \
					and month(CurDate)="+QString::number(month)
					+(pracFilter?QString(" and operacii.Pracuye=true "):QString(" "))
					+(pidrozdilId>0?QString(" and operacii.Pidrozdil_id=")+QString::number(pidrozdilId):QString(" "))
				);
	query->next();
	int numWorkers = query->value(0).toInt();
	int numRows = (numWorkers+1)/2;
	UPostupForm *postupForm = new UPostupForm(0, numWorkers );
	postupForm->show();
	
	printform = new UPrintForm(0, "������ �� ������� �� " + monthlist[month-1], true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	printform->document()->clear();
	QTextCursor cellCursor(printform->document());
	
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	constraints << QTextLength(QTextLength::FixedLength, 500);
	constraints << QTextLength(QTextLength::FixedLength, 500);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(15);
	tableFormat.setCellPadding(0);
	tableFormat.setBorder(0.5);
	QTextTable *table = cellCursor.insertTable(numRows, 2, tableFormat);
	
	int row=0, col=0;
	query->exec("SELECT operacii.Npr_id FROM operacii \
				LEFT JOIN npr ON operacii.Npr_id=npr.id \
				WHERE OznakaDohodu=1 \
					and year(CurDate)="+QString::number(year)+" \
					and month(CurDate)="+QString::number(month)
					+(pracFilter?QString(" and operacii.Pracuye=true "):QString(" "))
					+(pidrozdilId>0?QString(" and operacii.Pidrozdil_id=")+QString::number(pidrozdilId):QString(" ") 
				+" ORDER BY npr.Prizv, npr.Imia, npr.Pobatk")
				);
	while (query->next() && !(row>numRows)){
		cell = table->cellAt(row, col);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignRight );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertFragment(createWorkerCardFragment(query->value(0).toInt(), year, month));
		cellCursor.insertBlock(blockFormat);
		postupForm->inc_pos();
		++col;
		if (col==2){
			col=0;
			++row;
		}
	}
	
	delete query;
	delete postupForm;
	printform->printer()->setDocName("������ �� ������� ���������� �� "+QString::number(month)+"."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 0.5, 0.5, 0.5, 0.5, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_cur_worker_card(int year, int month, int workerId)
{
	printform = new UPrintForm(0, "������ �� ������� �� " + monthlist[month-1],true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cellCursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	constraints << QTextLength(QTextLength::FixedLength, 500);
	constraints << QTextLength(QTextLength::FixedLength, 500);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(15);
	tableFormat.setCellPadding(0);
	tableFormat.setBorder(0);
	QTextTable *table = cellCursor.insertTable(1, 1, tableFormat);
	
	cell = table->cellAt(0, 0);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertFragment(createWorkerCardFragment(workerId, year, month));
	
	printform->printer()->setDocName("������ �� ������� �� "+QString::number(month)+"."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 0.5, 0.5, 0.5, 0.5, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_year_cards(int year, short pidrozdilId, bool pracFilter)
{
	QSqlQuery query;
	if (pracFilter)
		query.exec("SELECT count(*) FROM (SELECT Npr_id FROM operacii \
							WHERE year(CurDate)="+sqlStr(year)
								+(pidrozdilId>0?QString(" and Pidrozdil_id=")+QString::number(pidrozdilId):QString(""))
							+" GROUP BY Npr_id) AS nprCnt");
	else
		query.exec("SELECT count(*) FROM npr"
					+(pidrozdilId>0?QString(" WHERE Pidrozdil_id=")+QString::number(pidrozdilId):QString("")));
	query.next();
	UPostupForm * postup = new UPostupForm(0, query.value(0).toInt());
	postup->show();
	
	printform = new UPrintForm(0, "г�� ������ �� ������� �� " + QString::number(year) + " ��",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	
	
	if (pracFilter)
		query.exec("SELECT oper.Npr_id FROM \
						(SELECT Npr_id FROM operacii \
							WHERE year(CurDate)="+sqlStr(year)
								+(pidrozdilId>0?QString(" and Pidrozdil_id=")+QString::number(pidrozdilId):QString(" "))
							+" GROUP BY Npr_id) AS oper \
					LEFT JOIN npr ON npr.id=oper.Npr_id \
					ORDER BY Prizv, Imia, Pobatk");
	else
		query.exec("SELECT id FROM npr "
					+(pidrozdilId>0?QString(" WHERE Pidrozdil_id=")+QString::number(pidrozdilId):QString(" "))
					+" ORDER BY Prizv, Imia, Pobatk");
	
	while (query.next()){
		cursor.insertFragment(createYearCardFragment(query.value(0).toInt(), year));
		cursor.insertBlock(blockFormat);
		cursor.insertBlock(blockFormat);
		postup->incPos();
	}
	
	delete postup;
	
	printform->printer()->setDocName("г�� ������ �� ������� �� "+QString::number(year) + " ��."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_year_cur_worker_card(int year, int workerId)
{
	printform = new UPrintForm(0, "г��� ������ �� ������� �� " + QString::number(year) + " ��",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertFragment(createYearCardFragment(workerId, year));
	
	printform->printer()->setDocName("г��� ������ �� ������� �� "+QString::number(year) + " ��."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_year_utrymannya_cards(int year, short pidrozdilId, bool pracFilter)
{
	QSqlQuery query;
	if (pracFilter)
		query.exec("SELECT count(*) FROM (SELECT Npr_id FROM operacii \
							WHERE year(CurDate)="+sqlStr(year)
								+(pidrozdilId>0?QString(" and Pidrozdil_id=")+QString::number(pidrozdilId):QString(""))
							+" GROUP BY Npr_id) AS nprCnt");
	else
		query.exec("SELECT count(*) FROM npr"
					+(pidrozdilId>0?QString(" WHERE Pidrozdil_id=")+QString::number(pidrozdilId):QString("")));
	query.next();
	UPostupForm * postup = new UPostupForm(0, query.value(0).toInt());
	postup->show();
	
	printform = new UPrintForm(0, "г�� ������ �� ������� �� " + QString::number(year) + " ��",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	
	
	if (pracFilter)
		query.exec("SELECT oper.Npr_id FROM \
						(SELECT Npr_id FROM operacii \
							WHERE year(CurDate)="+sqlStr(year)
								+(pidrozdilId>0?QString(" and Pidrozdil_id=")+QString::number(pidrozdilId):QString(" "))
							+" GROUP BY Npr_id) AS oper \
					LEFT JOIN npr ON npr.id=oper.Npr_id \
					ORDER BY Prizv, Imia, Pobatk");
	else
		query.exec("SELECT id FROM npr "
					+(pidrozdilId>0?QString(" WHERE Pidrozdil_id=")+QString::number(pidrozdilId):QString(" "))
					+" ORDER BY Prizv, Imia, Pobatk");
	
	while (query.next()){
		cursor.insertFragment(createYearUtrymannyaCardFragment(query.value(0).toInt(), year));
		cursor.insertBlock(blockFormat);
		cursor.insertBlock(blockFormat);
		postup->incPos();
	}
	
	delete postup;
	
	printform->printer()->setDocName("г�� ������ �� ������� �� "+QString::number(year) + " ��."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_year_utrymannya_cur_worker_card(int year, int workerId)
{
	printform = new UPrintForm(0, "г��� ������ �� ������� �� " + QString::number(year) + " ��",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertFragment(createYearUtrymannyaCardFragment(workerId, year));
	
	printform->printer()->setDocName("г��� ������ �� ������� �� "+QString::number(year) + " ��."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_vidomist_utrymannya(int year, int month, short pidrozdilId, bool pracFilter)
{
	QSqlQuery *nprQuery = new QSqlQuery();
	QSqlQuery *query = new QSqlQuery();
	QSqlQuery *tQuery = new QSqlQuery();
	QSqlRecord record;
	query->exec("SELECT count(*) FROM operacii \
				WHERE year(CurDate)="+QString::number(year)+" \
					and month(CurDate)="+QString::number(month)
					+(pracFilter?QString(" and (operacii.Pracuye=true or operacii.OznakaDohodu>1) "):QString(""))
					+(pidrozdilId>0?QString(" and operacii.Pidrozdil_id=")+QString::number(pidrozdilId):QString(""))
				);
	query->next();
	int numRows = query->value(0).toInt();
	UPostupForm *postupForm = new UPostupForm(0, numRows );
	postupForm->show();
	
	printform = new UPrintForm(0, "³������ ����������� �������� �� " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� ����������� �������� �� �������� �� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("�� ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("�."), textCharFormat);
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table;
	UPopulateCell tableCell(NULL);
	
	QSqlQuery pQuery;
	pQuery.exec("SELECT id, Name FROM pidrozdily_salary "
					+(pidrozdilId>0?QString("WHERE id=")+QString::number(pidrozdilId):QString(""))
					+" ORDER BY Name");
	bool firstPidrozdil = true;
	while (pQuery.next()) {
		cursor.movePosition(QTextCursor::End);
		blockFormat.setAlignment( Qt::AlignLeft );
		if (!firstPidrozdil)
			blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
		firstPidrozdil = false;
		
		cursor.insertBlock(blockFormat);
		blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_Auto);
		cursor.insertBlock(blockFormat);
		textCharFormat_bold.setFontPointSize(10);
		cursor.insertText("	ϳ������: "+pQuery.value(1).toString(),textCharFormat_bold);
	
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 250);
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(1);
		tableFormat.setBorder(0.5);
		tableFormat.setHeaderRowCount(1);
	
		QString operaciiPidrozdFilter;
		operaciiPidrozdFilter += " and operacii.Pidrozdil_id="+pQuery.value(0).toString()+" ";
		if (pracFilter)
			operaciiPidrozdFilter += " and (operacii.Pracuye=true or operacii.OznakaDohodu>1) ";
		
		nprQuery->exec("SELECT count(*) \
					FROM operacii \
					WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+operaciiPidrozdFilter);
		qDebug() << nprQuery->lastQuery();
		nprQuery->next();
		numRows = nprQuery->value(0).toInt();
		table = cursor.insertTable(numRows+2, 14, tableFormat);
		tableCell.setTextTable(table);
	
		QString lStr="\n-------\n";
		//���������� ����� �������
		textCharFormat.setFontPointSize( 9 );
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		textCharFormat_bold.setFontPointSize( 9 );
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		fnt = textCharFormat.font();
		fnt.setItalic(true);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(true);
		textCharFormat_bold.setFont(fnt);
	
		blockFormat.setBottomMargin(0);
		blockFormat.setTopMargin(0);
	
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(0,0,"� �/�", textCharFormat);
		tableCell.set(0,1,"� � �", textCharFormat);
		tableCell.set(0,2,"��", textCharFormat);
		tableCell.set(0,3,"���� �����.", textCharFormat);
		tableCell.set(0,4,"����.����", textCharFormat);
		tableCell.set(0,5,"����.\n����.", textCharFormat);
		tableCell.set(0,6,"��������", textCharFormat);
		tableCell.set(0,7,"��������", textCharFormat);
		tableCell.set(0,8,"�������.���.", textCharFormat);
		tableCell.set(0,9,"���.������.", textCharFormat);
		tableCell.set(0,10,"����", textCharFormat);
		tableCell.set(0,11,"������\n���.��.", textCharFormat);
		tableCell.set(0,12,"������\n�� ����", textCharFormat);
		tableCell.set(0,13,"������\n���.����.", textCharFormat);

		//���������� �������
		fnt = textCharFormat.font();
		fnt.setItalic(false);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(false);
		textCharFormat_bold.setFont(fnt);

		double U_PensFond_Sum=0, U_Fond_zainat_Sum=0, U_SocStrah_Sum=0, U_SocPilga_Sum=0, U_Prybutkov_podatok_Sum=0, 
				U_VidrahProfspilka_Sum=0, U_InshiVidrahuvannya_Sum=0;
		double workerUtrymSum=0;
		double allWorkersNaRukySum=0;
		double allNarahSum=0;
		double allUtrymSum=0;
		double curODNarahSum=0, curODUtrymSum=0;
		double val;
		double naRukyNoOdMore01;
	
		nprQuery->exec("SELECT Npr_id, count(*), npr.Prizv, npr.Imia, npr.Pobatk \
					FROM operacii \
					LEFT JOIN npr ON npr.id=operacii.Npr_ID \
					WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+operaciiPidrozdFilter+" \
					GROUP BY npr.Prizv, npr.Imia, npr.Pobatk");
		qDebug() << nprQuery->lastQuery();
		int row;
		int workersCounter = 0;
	
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
		for (row=1; (row<=numRows) && (nprQuery->next());){
			int operCount = nprQuery->value(1).toInt();
			workerUtrymSum = 0;
			
			query->exec("SELECT OznakaDohodu, U_PensFond+U_EV_Likarn+U_EV_Vidpuskni, U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, \
						U_VidrahProfspilka, U_InshiVidrahuvannya, O_VsogoNaRuky \
					FROM operacii \
					WHERE Npr_id="+mysqlStr(nprQuery->value(0).toInt())+" \
						and year(CurDate)="+QString::number(year)+" \
						and month(CurDate)="+QString::number(month)+" \
					ORDER BY OznakaDohodu \
					LIMIT "+mysqlStr(operCount));
			qDebug() << query->lastQuery();
		
			if (operCount > 1){
				table->mergeCells(row,0,operCount,1);
				table->mergeCells(row,1,operCount,1);
				table->mergeCells(row,12,operCount,1);
				table->mergeCells(row,13,operCount,1);
			}
		
			bool firstWorkerRow = true;
			int curOdRow=0;
			naRukyNoOdMore01 = 0;
			while (query->next()){
				++curOdRow;
				record = query->record();
				short oznakaDohodu = record.value("OznakaDohodu").toInt();
				curODUtrymSum = 0;
			
				//� �/�
				tableCell.setAlignment(Qt::AlignCenter);
				if (oznakaDohodu == 1)
					tableCell.set(row,0,QString::number(++workersCounter), textCharFormat);
			
				if (firstWorkerRow){
					//� � �
					tableCell.setAlignment(Qt::AlignLeft);
					tableCell.set(row,1,nprQuery->value(2).toString().toUpper() +" "+nprQuery->value(3).toString() +" "+nprQuery->value(4).toString(), textCharFormat);

					firstWorkerRow = false;
				}
			
				//������ ������
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,2,oznakaDohodu, textCharFormat);
			
				//"���� �����."
				tableCell.setAlignment(Qt::AlignRight);
				tQuery->exec("SELECT sum(N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_VyplProfspilky+N_Indexaciya+N_InshiNarahuvannya) AS narahSum \
						FROM operacii \
						WHERE OznakaDohodu="+mysqlStr(oznakaDohodu)+" and Npr_id="+mysqlStr(nprQuery->value(0).toInt())+" \
						and year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month));
				tQuery->seek(0);
				val = tQuery->value(0).toDouble();
				curODNarahSum = val;
				if (oznakaDohodu == 1)
					allNarahSum += val;
				tableCell.setDash(row,3,val, textCharFormat);
			
				//"����.����"
				val = query->value(1).toDouble();
				U_PensFond_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,4,val, textCharFormat);
			
				//"����.\n����."
				val = query->value(2).toDouble();
				U_Fond_zainat_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,5,val, textCharFormat);
			
				//"��������"
				val = query->value(3).toDouble();
				U_SocStrah_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,6,val, textCharFormat);
			
				//"��������"
				val = query->value(4).toDouble();
				U_SocPilga_Sum += val;
				tableCell.setDash(row,7,val, textCharFormat);
			
				//"�������.���."
				val = query->value(5).toDouble();
				U_Prybutkov_podatok_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,8,val, textCharFormat);
			
				//"���.������."
				val = query->value(6).toDouble();
				curODUtrymSum += val;
				U_VidrahProfspilka_Sum += val;
				tableCell.setDash(row,9,val, textCharFormat);
				
				//"����"
				val = query->value(7).toDouble();
				U_InshiVidrahuvannya_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,10,val, textCharFormat);
			
				//"������\n���.��."
				val = curODUtrymSum;
				workerUtrymSum += curODUtrymSum;
				allUtrymSum += curODUtrymSum;
				tableCell.setDash(row,11,val, textCharFormat);
			
				//"������\n�� ����"
				if (oznakaDohodu == 1){
					val = curODNarahSum - curODUtrymSum;
					naRukyNoOdMore01 += curODNarahSum - curODUtrymSum;
				}
				else{
					tQuery->exec("SELECT sum(O_VsogoNaRuky) FROM operacii \
							WHERE OznakaDohodu="+mysqlStr(oznakaDohodu)+" \
								and Npr_id="+mysqlStr(nprQuery->value(0).toInt())+" \
								and year(CurDate)="+mysqlStr(year)+" \
								and month(CurDate)="+mysqlStr(month));
					tQuery->seek(0);
					val = tQuery->value(0).toDouble();
					
					naRukyNoOdMore01 += val - curODUtrymSum;
				}
			
				if (curOdRow == operCount){
					//"�� ����"
					val = naRukyNoOdMore01;
					allWorkersNaRukySum += val;
					tableCell.setDash(row-operCount+1,12,val, textCharFormat);
				}
			
				if (curOdRow == operCount){
					//"������\n���.����."
					val = workerUtrymSum;
					tableCell.setDash(row-operCount+1,13,val, textCharFormat);
				}
			
				row++;
				postupForm->inc_pos();
			}
		}
		//���������� ������ "������"
		table->mergeCells(row,0,1,2);
	
		//������
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"������", textCharFormat);
	
		//������ ������
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,2,"-", textCharFormat);
	
		//"���� �����."
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.setDash(row,3,allNarahSum, textCharFormat);
			
		//"����.����"
		tableCell.setDash(row,4,U_PensFond_Sum, textCharFormat);
	
		//"����.\n����."
		tableCell.setDash(row,5,U_Fond_zainat_Sum, textCharFormat);
			
		//"��������"
		tableCell.setDash(row,6,U_SocStrah_Sum, textCharFormat);
			
		//"��������"
		tableCell.setDash(row,7,U_SocPilga_Sum, textCharFormat);
			
		//"�������.���."
		tableCell.setDash(row,8,U_Prybutkov_podatok_Sum, textCharFormat);
			
		//"���.������."
		tableCell.setDash(row,9,U_VidrahProfspilka_Sum, textCharFormat);
			
		//"����"
		tableCell.setDash(row,10,U_InshiVidrahuvannya_Sum, textCharFormat);
			
		//"������\n���.��."
		tableCell.setDash(row,11,allUtrymSum, textCharFormat);
	
		//"������\n�� ����"
		tableCell.setDash(row,12,allWorkersNaRukySum, textCharFormat);
			
		//"������\n���.����."
		tableCell.setDash(row,13,allUtrymSum, textCharFormat);
	
	}
	
	delete nprQuery;
	delete query;
	delete tQuery;
	delete postupForm;
	printform->printer()->setDocName("��������� �� "+QString::number(month)+"."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 8, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_vidomist_vidrahuvannya(int year, int month, short pidrozdilId, bool pracFilter)
{
	QSqlQuery *nprQuery = new QSqlQuery();
	QSqlQuery *query = new QSqlQuery();
	QSqlQuery *tQuery = new QSqlQuery();
	QSqlRecord record;
	query->exec("SELECT count(*) FROM operacii \
				WHERE year(CurDate)="+QString::number(year)+" \
					and month(CurDate)="+QString::number(month)
					+(pracFilter?QString(" and (operacii.Pracuye=true or operacii.OznakaDohodu>1) "):QString(""))
					+(pidrozdilId>0?QString(" and operacii.Pidrozdil_id=")+QString::number(pidrozdilId):QString(""))
				);
	query->seek(0);
	int numRows = query->value(0).toInt();
	UPostupForm *postupForm = new UPostupForm(0, numRows );
	postupForm->show();
	
	printform = new UPrintForm(0, "³������ ���������� �� �������� �� " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� ²��������� �� �������� �� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("�� ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("�."), textCharFormat);
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table;
	UPopulateCell tableCell(NULL);
	
	QSqlQuery pQuery;
	pQuery.exec("SELECT id, Name FROM pidrozdily_salary "
					+(pidrozdilId>0?QString("WHERE id=")+QString::number(pidrozdilId):QString(""))
					+" ORDER BY Name");
	bool firstPidrozdil = true;
	while (pQuery.next()) {
		cursor.movePosition(QTextCursor::End);
		blockFormat.setAlignment( Qt::AlignLeft );
		if (!firstPidrozdil)
			blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
		firstPidrozdil = false;
		
		cursor.insertBlock(blockFormat);
		blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_Auto);
		cursor.insertBlock(blockFormat);
		textCharFormat_bold.setFontPointSize(10);
		cursor.insertText("	ϳ������: "+pQuery.value(1).toString(),textCharFormat_bold);
	
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 250);
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		constraints << QTextLength(QTextLength::FixedLength, 60);
		
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(1);
		tableFormat.setHeaderRowCount(1);
		tableFormat.setBorder(0.5);
	
		QString operaciiPidrozdFilter;
		operaciiPidrozdFilter += " and operacii.Pidrozdil_id="+pQuery.value(0).toString()+" ";
		if (pracFilter)
			operaciiPidrozdFilter += " and (operacii.Pracuye=true or operacii.OznakaDohodu>1) ";
		
		nprQuery->exec("SELECT count(*) \
					FROM operacii \
					WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+operaciiPidrozdFilter);
		nprQuery->next();
		numRows = nprQuery->value(0).toInt();
		table = cursor.insertTable(numRows+2, 11, tableFormat);
		tableCell.setTextTable(table);
	
		QString lStr="\n-------\n";
		//���������� ����� �������
		textCharFormat.setFontPointSize( 9 );
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		textCharFormat_bold.setFontPointSize( 9 );
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		fnt = textCharFormat.font();
		fnt.setItalic(true);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(true);
		textCharFormat_bold.setFont(fnt);
	
		blockFormat.setBottomMargin(0);
		blockFormat.setTopMargin(0);
	
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(0,0,"� �/�", textCharFormat);
		tableCell.set(0,1,"� � �", textCharFormat);
		tableCell.set(0,2,"��", textCharFormat);
		tableCell.set(0,3,"���� �����.", textCharFormat);
		tableCell.set(0,4,"����.����", textCharFormat);
		tableCell.set(0,5,"��������", textCharFormat);
		tableCell.set(0,6,"����.\n����.", textCharFormat);
		tableCell.set(0,7,"���.���.", textCharFormat);
		tableCell.set(0,8,"����.", textCharFormat);
		tableCell.set(0,9,"���� ²��.��", textCharFormat);
		tableCell.set(0,10,"������ ²�� ����.", textCharFormat);
	
		//���������� �������
		fnt = textCharFormat.font();
		fnt.setItalic(false);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(false);
		textCharFormat_bold.setFont(fnt);

		double V_PensFond_Sum=0, V_SocStrah_Sum=0, V_Fond_zainat_Sum=0, V_NeshchVyp_Sum=0, V_Inshi_Sum=0;
		double workerVidrahSum;
		double allNarahSum=0;
		double allVidrahSum=0;
		double curODVidrahSum=0;
		double val;
	
		nprQuery->exec("SELECT Npr_id, count(*), npr.Prizv, npr.Imia, npr.Pobatk \
					FROM operacii \
					LEFT JOIN npr ON npr.id=operacii.Npr_ID \
					WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+operaciiPidrozdFilter+" \
					GROUP BY Npr_id \
					ORDER BY npr.Prizv, npr.Imia, npr.Pobatk");
		int row;
		int workersCounter = 0;
	
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
		for (row=1; (row<=numRows) && (nprQuery->next());){
			int operCount = nprQuery->value(1).toInt();
			workerVidrahSum = 0;
		
			query->exec("SELECT OznakaDohodu, V_PensFond+V_EV_Likarn+V_EV_Vidpuskni, V_SocStrah, V_Fond_zainat, V_NeshchVyp, V_Inshi \
					FROM operacii \
					WHERE Npr_id="+mysqlStr(nprQuery->value(0).toInt())+" \
						and year(CurDate)="+QString::number(year)+" \
						and month(CurDate)="+QString::number(month)+" \
					ORDER BY OznakaDohodu \
					LIMIT "+mysqlStr(operCount));
		
			if (operCount > 1){
				table->mergeCells(row,0,operCount,1);
				table->mergeCells(row,1,operCount,1);
				table->mergeCells(row,10,operCount,1);
			}
		
			bool firstWorkerRow = true;
			int curOdRow=0;
			while (query->next()){
				++curOdRow;
				record = query->record();
				short oznakaDohodu = record.value("OznakaDohodu").toInt();
				curODVidrahSum = 0;
			
				//� �/�
				tableCell.setAlignment(Qt::AlignCenter);
				if (oznakaDohodu == 1)
					tableCell.set(row,0,QString::number(++workersCounter), textCharFormat);
			
				if (firstWorkerRow){
					//� � �
					tableCell.setAlignment(Qt::AlignLeft);
					tableCell.set(row,1,nprQuery->value(2).toString().toUpper() +" "+nprQuery->value(3).toString() +" "+nprQuery->value(4).toString(), textCharFormat);

					firstWorkerRow = false;
				}
			
				//������ ������
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,2,oznakaDohodu, textCharFormat);
			
				//"���� �����."
				tableCell.setAlignment(Qt::AlignRight);
				tQuery->exec("SELECT sum(N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_VyplProfspilky+N_Indexaciya+N_InshiNarahuvannya) AS narahSum \
						FROM operacii \
						WHERE OznakaDohodu="+mysqlStr(oznakaDohodu)+" and Npr_id="+mysqlStr(nprQuery->value(0).toInt())+" \
						and year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month));
				tQuery->seek(0);
				val = tQuery->value(0).toDouble();
				allNarahSum += val;
				if (val != 0)
					tableCell.set(row,3,val, textCharFormat);
				else
					tableCell.set(row,3,"-", textCharFormat);
			
				//"����.����"
				val = query->value(1).toDouble();
				V_PensFond_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,4,val, textCharFormat);
				else
					tableCell.set(row,4,"-", textCharFormat);
			
				//"��������"
				val = query->value(2).toDouble();
				V_SocStrah_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,5,val, textCharFormat);
				else
					tableCell.set(row,5,"-", textCharFormat);
			
				//"����.\n����."
				val = query->value(3).toDouble();
				V_Fond_zainat_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,6,val, textCharFormat);
				else
					tableCell.set(row,6,"-", textCharFormat);
			
				//"���.���."
				val = query->value(4).toDouble();
				V_NeshchVyp_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,7,val, textCharFormat);
				else
					tableCell.set(row,7,"-", textCharFormat);
			
				//"����."
				val = query->value(5).toDouble();
				V_Inshi_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,8,val, textCharFormat);
				else
					tableCell.set(row,8,"-", textCharFormat);
			
				//"���� ²��.��"
				workerVidrahSum += curODVidrahSum;
				if (curODVidrahSum != 0)
					tableCell.set(row,9,curODVidrahSum, textCharFormat);
				else
					tableCell.set(row,9,"-", textCharFormat);
			
				if (curOdRow == operCount){
					//"������ ²�� ����."
					val = workerVidrahSum;
					allVidrahSum += workerVidrahSum;
					if (val != 0)
						tableCell.set(row-operCount+1,10,val, textCharFormat);
					else
						tableCell.set(row-operCount+1,10,"-", textCharFormat);
				}
			
				row++;
				postupForm->inc_pos();
			}
		}
		//���������� ������ "������"
		table->mergeCells(row,0,1,2);
	
		//������
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"������", textCharFormat);
	
		//������ ������
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,2,"-", textCharFormat);
	
		//"���� �����."
		tableCell.setAlignment(Qt::AlignRight);
		if (allNarahSum != 0)
			tableCell.set(row,3,allNarahSum, textCharFormat);
		else
			tableCell.set(row,3,"-", textCharFormat);
			
		//"����.����"
		if (V_PensFond_Sum != 0)
			tableCell.set(row,4,V_PensFond_Sum, textCharFormat);
		else
			tableCell.set(row,4,"-", textCharFormat);
	
		//"��������"
		if (V_SocStrah_Sum != 0)
			tableCell.set(row,5,V_SocStrah_Sum, textCharFormat);
		else
			tableCell.set(row,5,"-", textCharFormat);
			
		//"����.\n����."
		if (V_Fond_zainat_Sum != 0)
			tableCell.set(row,6,V_Fond_zainat_Sum, textCharFormat);
		else
			tableCell.set(row,6,"-", textCharFormat);
			
		//������ �������
		if (V_NeshchVyp_Sum != 0)
			tableCell.set(row,7,V_NeshchVyp_Sum, textCharFormat);
		else
			tableCell.set(row,7,"-", textCharFormat);
			
		//����
		if (V_Inshi_Sum != 0)
			tableCell.set(row,8,V_Inshi_Sum, textCharFormat);
			else
		tableCell.set(row,8,"-", textCharFormat);
			
		//������
		if (allVidrahSum != 0){
			tableCell.set(row,9,allVidrahSum, textCharFormat);
			tableCell.set(row,10,allVidrahSum, textCharFormat);
		}
		else{
			tableCell.set(row,9,"-", textCharFormat);
			tableCell.set(row,10,"-", textCharFormat);
		}
	}
	
	delete nprQuery;
	delete query;
	delete tQuery;
	delete postupForm;
	printform->printer()->setDocName("³���������� �� "+QString::number(month)+"."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 10, 5, 4, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_vidomist_oplaty_i_zaborgovanosti(int year, int month, short pidrozdilId, bool pracFilter)
{
	QSqlQuery nprQuery;
	QSqlQuery query;
	query.exec("SELECT count(*) FROM npr"
				+(pracFilter?QString(" WHERE Pracuye=true"):QString(""))
				+(pidrozdilId>0?QString(" and Pidrozdil_id=")+QString::number(pidrozdilId):QString(""))
				);
	query.seek(0);
	int numRows = query.value(0).toInt();
	UPostupForm *postupForm = new UPostupForm(0, numRows );
	postupForm->show();
	
	printform = new UPrintForm(0, "³������ ������/������������� �� " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� ������/������������Ҳ �� ������Ҳ �� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("�� ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("�."), textCharFormat);
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table;
	UPopulateCell tableCell(NULL);
	
	QSqlQuery pQuery;
	pQuery.exec("SELECT id, Name FROM pidrozdily_salary "
					+(pidrozdilId>0?QString("WHERE id=")+QString::number(pidrozdilId):QString(""))
					+" ORDER BY Name");
	bool firstPidrozdil = true;
	while (pQuery.next()) {
		cursor.movePosition(QTextCursor::End);
		blockFormat.setAlignment( Qt::AlignLeft );
		if (!firstPidrozdil)
			blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
		firstPidrozdil = false;
		
		cursor.insertBlock(blockFormat);
		blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_Auto);
		cursor.insertBlock(blockFormat);
		textCharFormat_bold.setFontPointSize(10);
		cursor.insertText("	ϳ������: "+pQuery.value(1).toString(),textCharFormat_bold);
	
		constraints.clear();
		constraints << QTextLength(QTextLength::FixedLength, 30);
		constraints << QTextLength(QTextLength::FixedLength, 120);
		constraints << QTextLength(QTextLength::FixedLength, 70);
		constraints << QTextLength(QTextLength::FixedLength, 70);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 70);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 52);
		constraints << QTextLength(QTextLength::FixedLength, 70);
		constraints << QTextLength(QTextLength::FixedLength, 70);
		
		tableFormat.setColumnWidthConstraints(constraints);
		tableFormat.setCellSpacing(0);
		tableFormat.setCellPadding(0.5);
		tableFormat.setBorder(0.5);
		tableFormat.setHeaderRowCount(1);
		QString nprFilter;
		nprFilter = " WHERE Pidrozdil_id="+pQuery.value(0).toString()+" ";
		if (pracFilter)
			nprFilter += " and Pracuye=true ";
		
		query.exec("SELECT count(*) \
					FROM npr \
					"+nprFilter);
		query.next();
		numRows = query.value(0).toInt();
		table = cursor.insertTable(numRows+2, 17, tableFormat);
		tableCell.setTextTable(table);
	
		QString lStr="\n-------\n";
		//���������� ����� �������
		textCharFormat.setFontPointSize( 9 );
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		textCharFormat_bold.setFontPointSize( 9 );
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
		fnt = textCharFormat.font();
		fnt.setItalic(true);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(true);
		textCharFormat_bold.setFont(fnt);
	
		blockFormat.setBottomMargin(0);
		blockFormat.setTopMargin(0);
	
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(0,0,"� �/�", textCharFormat);
		tableCell.set(0,1,"� � �", textCharFormat);
		tableCell.set(0,2,"��.\n������.", textCharFormat);
		tableCell.set(0,3,"���.\n��=1.", textCharFormat);
		tableCell.set(0,4,"���.\n��>1.", textCharFormat);
		tableCell.set(0,5,"���.\n��=1", textCharFormat);
		tableCell.set(0,6,"���.\n��>1", textCharFormat);
		tableCell.set(0,7,"����\n���", textCharFormat);
		tableCell.set(0,8,"��\n����", textCharFormat);
		tableCell.set(0,9,"���� ���.", textCharFormat);
		tableCell.set(0,10,"�����", textCharFormat);
		tableCell.set(0,11,"���.", textCharFormat);
	
		tableCell.set(0,12,"�����.\n��.", textCharFormat);
		tableCell.set(0,13,"�����.", textCharFormat);
	
		tableCell.set(0,14,"�����", textCharFormat);
		tableCell.set(0,15,"������", textCharFormat);
		tableCell.set(0,16,"���.\n������.", textCharFormat);
	
		//���������� �������
		fnt = textCharFormat.font();
		fnt.setItalic(false);
		textCharFormat.setFont(fnt);
		fnt = textCharFormat_bold.font();
		fnt.setItalic(false);
		textCharFormat_bold.setFont(fnt);

		double WorkerVhSaldoSum=0, WorkerNarahSum=0, WorkerUtrymSum=0,
				WorkerVyhSaldoSum=0, WorkerNarahOD01Sum=0, WorkerUtrym01Sum=0, WorkerNaRukyOtherDohid=0;
		double allUtrymSum=0, allVyhSaldo=0, allVhSaldo=0;
		double allNarah01Sum=0, allNarah02Sum=0, allUtrym01Sum=0, allUtrym02Sum=0;
		double allInshiSum=0, allShtrafSum=0, allAlimentySum=0, allPoperMonthSum=0, allPremiyaSum=0, allAvansSum=0, allVydanoSum=0;
		double allNaRukySum=0;
		double val;
	
		nprQuery.exec("SELECT id, Prizv, Imia, Pobatk \
					FROM npr \
					"+nprFilter+" \
					ORDER BY Prizv, Imia, Pobatk");
		int row;
	
		textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
		textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
		for (row=1; (row<=numRows) && (nprQuery.next());++row){
			int nprId = nprQuery.value(0).toInt();
			WorkerVyhSaldoSum = 0;
		
			//� �/�
			tableCell.setAlignment(Qt::AlignCenter);
			tableCell.set(row,0,QString::number(row), textCharFormat);
			
			//� � �
			tableCell.setAlignment(Qt::AlignLeft);
			tableCell.set(row,1,nprQuery.value(1).toString().toUpper() +" "+nprQuery.value(2).toString(), textCharFormat);
		
			query.exec("SELECT Vh_saldo, O_Inshi, O_Shtraf, O_Alimenty, O_ZaPoperMis, O_Premiya, O_Avans, O_Oplacheno \
					FROM operacii \
					WHERE Npr_id="+mysqlStr(nprId)+" and \
							OznakaDohodu=1 and \
							year(CurDate)="+mysqlStr(year)+" and \
							month(CurDate)="+mysqlStr(month)+" LIMIT 1");
			query.seek(0);
		
			//"��.\n������."
			tableCell.setAlignment(Qt::AlignRight);
			val = query.value(0).toDouble();
			WorkerVhSaldoSum = val;
			allVhSaldo += val;
			tableCell.set(row,2,val, textCharFormat);
		
			//"����."
			val = query.value(1).toDouble();
			WorkerVyhSaldoSum -= val;
			allInshiSum += val;
			tableCell.setDash(row,9,val, textCharFormat);
			
			//"�����"
			val = query.value(2).toDouble();
			WorkerVyhSaldoSum -= val;
			allShtrafSum += val;
			tableCell.setDash(row,10,val, textCharFormat);
		
			//"���."
			val = query.value(3).toDouble();
			WorkerVyhSaldoSum -= val;
			allAlimentySum +=val;
			tableCell.setDash(row,11,val, textCharFormat);
		
			//"�� ����� ��."
			val = query.value(4).toDouble();
			WorkerVyhSaldoSum -= val;
			allPoperMonthSum +=val;
			tableCell.setDash(row,12,val, textCharFormat);
		
			//"�����"
			val = query.value(5).toDouble();
			WorkerVyhSaldoSum -= val;
			allPremiyaSum +=val;
			tableCell.setDash(row,13,val, textCharFormat);
			
			//"�����"
			val = query.value(6).toDouble();
			WorkerVyhSaldoSum -= val;
			allAvansSum += val;
			tableCell.setDash(row,14,val, textCharFormat);
		
			//"������"
			val = query.value(7).toDouble();
			WorkerVyhSaldoSum -= val;
			allVydanoSum += val;
			tableCell.setDash(row,15,val, textCharFormat);
		
			query.exec("SELECT N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_VyplProfspilky+N_Indexaciya+N_InshiNarahuvannya AS narahSum, \
							U_PensFond+U_EV_Likarn+U_EV_Vidpuskni+U_Fond_zainat+U_SocStrah+U_Prybutkov_podatok+ \
							U_VidrahProfspilka+U_InshiVidrahuvannya AS utrymSum \
					FROM operacii \
					WHERE OznakaDohodu=1 and \
						Npr_id="+mysqlStr(nprId)+" and \
						year(CurDate)="+mysqlStr(year)+" and \
						month(CurDate)="+mysqlStr(month));
			query.seek(0);
		
			//"���.\n��=1."
			val = query.value(0).toDouble();
			WorkerNarahSum = val;
			WorkerNarahOD01Sum = val;
			allNarah01Sum += val;
			tableCell.setDash(row,3,val, textCharFormat);
		
			//���.\n��=1
			val = query.value(1).toDouble();
			WorkerUtrymSum = val;
			allUtrymSum += val;
			WorkerUtrym01Sum = val;
			allUtrym01Sum += val;
			tableCell.setDash(row,5,val, textCharFormat);
		
			query.exec("SELECT sum(N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_VyplProfspilky+N_InshiNarahuvannya) AS narahSum, \
						sum(U_PensFond+U_EV_Likarn+U_EV_Vidpuskni+U_Fond_zainat+U_SocStrah+U_Prybutkov_podatok+ \
							U_VidrahProfspilka+U_InshiVidrahuvannya) AS utrymSum, \
						sum(O_VsogoNaRuky) AS naRukySum\
					FROM operacii \
					WHERE OznakaDohodu>1 and \
						Npr_id="+mysqlStr(nprId)+" and \
						year(CurDate)="+mysqlStr(year)+" and \
						month(CurDate)="+mysqlStr(month));
			query.seek(0);
			WorkerNaRukyOtherDohid = query.value(2).toDouble();
		
			//"���.\n��>1."
			val = query.value(0).toDouble();
			//WorkerNarahSum += val;
			allNarah02Sum += val;
			tableCell.setDash(row,4,val, textCharFormat);
		
			//���.\n��>1
			val = query.value(1).toDouble();
			WorkerUtrymSum += val;
			allUtrymSum += val;
			allUtrym02Sum += val;
			tableCell.setDash(row,6,val, textCharFormat);
		
			//"����\n���"
			val = WorkerUtrymSum;
			tableCell.setDash(row,7,val, textCharFormat);
		
			//�� ����
			val = WorkerNarahOD01Sum + WorkerNaRukyOtherDohid - WorkerUtrymSum;
			allNaRukySum += val;
			tableCell.setDash(row,8,val, textCharFormat);
		
			//������� ������
			val = WorkerVyhSaldoSum += WorkerVhSaldoSum + WorkerNarahOD01Sum + WorkerNaRukyOtherDohid - WorkerUtrymSum;
			allVyhSaldo += val;
			tableCell.set(row,16,val, textCharFormat);
		
			postupForm->inc_pos();
		}
		//���������� ������ "������"
		table->mergeCells(row,0,1,2);
	
		//������
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"������", textCharFormat);
	
		tableCell.setDash(row,2,allVhSaldo, textCharFormat);
		tableCell.setDash(row,3,allNarah01Sum, textCharFormat);
		tableCell.setDash(row,4,allNarah02Sum, textCharFormat);
		tableCell.setDash(row,5,allUtrym01Sum, textCharFormat);
		tableCell.setDash(row,6,allUtrym02Sum, textCharFormat);
		tableCell.setDash(row,7,allUtrymSum, textCharFormat);
		tableCell.setDash(row,8,allNaRukySum, textCharFormat);
		tableCell.setDash(row,9,allInshiSum, textCharFormat);
		tableCell.setDash(row,10,allShtrafSum, textCharFormat);
		tableCell.setDash(row,11,allAlimentySum, textCharFormat);
		tableCell.setDash(row,12,allPoperMonthSum, textCharFormat);
		tableCell.setDash(row,13,allPremiyaSum, textCharFormat);
		tableCell.setDash(row,14,allAvansSum, textCharFormat);
		tableCell.setDash(row,15,allVydanoSum, textCharFormat);
		tableCell.setDash(row,16,allVyhSaldo, textCharFormat);
	}
	
	delete postupForm;
	printform->printer()->setDocName("������/������������� �� "+QString::number(month)+"."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 8, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_vidomist_narah_i_vidrah_by_buhrahunok(int year, int month, bool pracFilter)
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) FROM (SELECT Buh_rahunok FROM operacii \
				WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+(pracFilter?QString(" and (operacii.Pracuye=true or operacii.OznakaDohodu>1) "):QString(""))+" and OznakaDohodu=1\
				GROUP BY Buh_rahunok) AS oper");
	query->seek(0);
	int numRows = query->value(0).toInt();
	UPostupForm *postupForm = new UPostupForm(0, numRows * 2 );
	postupForm->show();
	
	printform = new UPrintForm(0, "����������� � ����������� � ����� ���������� �� " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� ����������� � ���вǲ ��������ʲ�\n�� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("�� ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("�."), textCharFormat);
	//��������� �������
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	constraints << QTextLength(QTextLength::FixedLength, 35);
	constraints << QTextLength(QTextLength::FixedLength, 35);
	constraints << QTextLength(QTextLength::FixedLength, 35);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setHeaderRowCount(1);
	tableFormat.setBorder(0.5);
	QTextTable *table = cursor.insertTable(numRows+2, 18, tableFormat);
	UPopulateCell tableCell(table);
	
	//���������� ����� �������
	textCharFormat.setFontPointSize( 9 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 9 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	fnt = textCharFormat.font();
	fnt.setItalic(true);
	textCharFormat.setFont(fnt);
	fnt = textCharFormat_bold.font();
	fnt.setItalic(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	tableCell.setAlignment(Qt::AlignCenter);
	
	tableCell.set(0,0,"� �/�", textCharFormat);
	tableCell.set(0,1,"���. ���.", textCharFormat);
	tableCell.set(0,2,"�-���", textCharFormat);
	tableCell.set(0,3,"���.\n�����.", textCharFormat);
	tableCell.set(0,4,"�������", textCharFormat);
	tableCell.set(0,5,"˳����.", textCharFormat);
	tableCell.set(0,6,"³�����", textCharFormat);
	tableCell.set(0,7,"�����.", textCharFormat);
	tableCell.set(0,8,"ͳ��", textCharFormat);
	tableCell.set(0,9,"��������.", textCharFormat);
	tableCell.set(0,10,"�����.\n���.", textCharFormat);
	tableCell.set(0,11,"�����", textCharFormat);
	tableCell.set(0,12,"�����.\n���.", textCharFormat);
	tableCell.set(0,13,"�������", textCharFormat);
	tableCell.set(0,14,"������.", textCharFormat);
	tableCell.set(0,15,"����.\n������.", textCharFormat);
	tableCell.set(0,16,"��.\n�����.", textCharFormat);
	tableCell.set(0,17,"������\n�����.", textCharFormat);
	
	//���������� �������
	fnt = textCharFormat.font();
	fnt.setItalic(false);
	textCharFormat.setFont(fnt);
	fnt = textCharFormat_bold.font();
	fnt.setItalic(false);
	textCharFormat_bold.setFont(fnt);

	double N_Osn_narah_Sum=0, N_VuslRokiv_Sum=0, N_Likarn_Sum=0, N_Vidpuskni_Sum=0, N_Svatkovi_dni_Sum=0, 
			N_Nichni_Sum=0, N_Nadurochni_Sum=0, N_Klasn_vodiya_Sum=0, N_Premia_Sum=0, N_MaterDopomoga_Sum=0, 
			N_Doplata_Sum=0, N_Index_Sum=0, N_VyplProfspilky_Sum=0, N_InshiNarahuvannya_Sum=0;
	double curNarahSum;
	double allNarahSum=0;
	int pracCount=0;
	double dVal;
	int iVal;
	
	query->exec("SELECT Buh_rahunok, count(*), sum(N_Osn_narah), sum(N_VuslRokiv), sum(N_Likarn), sum(N_Vidpuskni), \
						sum(N_Svatkovi_dni), sum(N_Nichni), sum(N_Nadurochni), sum(N_Klasn_vodiya), sum(N_Premia), \
						sum(N_MaterDopomoga), sum(N_Doplata), sum(N_Indexaciya), sum(N_VyplProfspilky), sum(N_InshiNarahuvannya), \
						sum(V_PensFond+V_EV_Likarn+V_EV_Vidpuskni), sum(V_SocStrah), sum(V_Fond_zainat), sum(V_NeshchVyp), sum(V_Inshi) \
					FROM operacii \
					WHERE year(CurDate)="+mysqlStr(year)+" \
							and month(CurDate)="+mysqlStr(month)+(pracFilter?QString(" and (operacii.Pracuye=true or operacii.OznakaDohodu>1) "):QString(""))+"  and OznakaDohodu=1 \
					GROUP BY Buh_rahunok \
					ORDER BY Buh_rahunok");
	int row;
	
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	for (row=1; (row<=numRows) && (query->next()); row++){
		tableCell.setAlignment(Qt::AlignRight);
		//"� �/�"
		tableCell.set(row,0,row, textCharFormat);
		//"���. ���."
		tableCell.setDash(row,1,query->value(0).toInt(), textCharFormat);
		//"�-���"
		iVal = query->value(1).toInt();
		pracCount += iVal;
		tableCell.set(row,2,iVal, textCharFormat);
		
		//"���.\n�����."
		dVal = query->value(2).toDouble();
		curNarahSum = dVal;
		N_Osn_narah_Sum += dVal;
		tableCell.setDash(row,3,dVal, textCharFormat);
		//"�������"
		dVal = query->value(3).toDouble();
		curNarahSum += dVal;
		N_VuslRokiv_Sum += dVal;
		tableCell.setDash(row,4,dVal, textCharFormat);
		//"˳����."
		dVal = query->value(4).toDouble();
		curNarahSum += dVal;
		N_Likarn_Sum += dVal;
		tableCell.setDash(row,5,dVal, textCharFormat);
		//"³�����"
		dVal = query->value(5).toDouble();
		curNarahSum += dVal;
		N_Vidpuskni_Sum += dVal;
		tableCell.setDash(row,6,dVal, textCharFormat);
		//"�����."
		dVal = query->value(6).toDouble();
		curNarahSum += dVal;
		N_Svatkovi_dni_Sum += dVal;
		tableCell.setDash(row,7,dVal, textCharFormat);
		//"ͳ��"
		dVal = query->value(7).toDouble();
		curNarahSum += dVal;
		N_Nichni_Sum += dVal;
		tableCell.setDash(row,8,dVal, textCharFormat);
		//"��������."
		dVal = query->value(8).toDouble();
		curNarahSum += dVal;
		N_Nadurochni_Sum += dVal;
		tableCell.setDash(row,9,dVal, textCharFormat);
		//"�����.\n���."
		dVal = query->value(9).toDouble();
		curNarahSum += dVal;
		N_Klasn_vodiya_Sum += dVal;
		tableCell.setDash(row,10,dVal, textCharFormat);
		//"�����"
		dVal = query->value(10).toDouble();
		curNarahSum += dVal;
		N_Premia_Sum += dVal;
		tableCell.setDash(row,11,dVal, textCharFormat);
		//"�����.\n���."
		dVal = query->value(11).toDouble();
		curNarahSum += dVal;
		N_MaterDopomoga_Sum += dVal;
		tableCell.setDash(row,12,dVal, textCharFormat);
		//"�������"
		dVal = query->value(12).toDouble();
		curNarahSum += dVal;
		N_Doplata_Sum += dVal;
		tableCell.setDash(row,13,dVal, textCharFormat);
		//"����������"
		dVal = query->value(13).toDouble();
		curNarahSum += dVal;
		N_Index_Sum += dVal;
		tableCell.setDash(row,14,dVal, textCharFormat);
		//"����.\n������."
		dVal = query->value(14).toDouble();
		curNarahSum += dVal;
		N_VyplProfspilky_Sum += dVal;
		tableCell.setDash(row,15,dVal, textCharFormat);
		//"��.\n�����."
		dVal = query->value(15).toDouble();
		curNarahSum += dVal;
		N_InshiNarahuvannya_Sum += dVal;
		tableCell.setDash(row,16,dVal, textCharFormat);
		//"������.\n�����."
		allNarahSum += curNarahSum;
		tableCell.setDash(row,17,curNarahSum, textCharFormat);
		
		postupForm->inc_pos();
	}
	
	//���������� ������ "������"
	table->mergeCells(row,0,1,2);
	
	//������
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(row,0,"������", textCharFormat);
	
	//"�-���"
	tableCell.set(row,2,pracCount, textCharFormat);
		
	//"���.\n�����."
	tableCell.setDash(row,3,N_Osn_narah_Sum, textCharFormat);
	//"�������"
	tableCell.setDash(row,4,N_VuslRokiv_Sum, textCharFormat);
	//"˳����."
	tableCell.setDash(row,5,N_Likarn_Sum, textCharFormat);
	//"³�����"
	tableCell.setDash(row,6,N_Vidpuskni_Sum, textCharFormat);
	//"�����."
	tableCell.setDash(row,7,N_Svatkovi_dni_Sum, textCharFormat);
	//"ͳ��"
	tableCell.setDash(row,8,N_Nichni_Sum, textCharFormat);
	//"��������."
	tableCell.setDash(row,9,N_Nadurochni_Sum, textCharFormat);
	//"�����.\n���."
	tableCell.setDash(row,10,N_Klasn_vodiya_Sum, textCharFormat);
	//"�����"
	tableCell.setDash(row,11,N_Premia_Sum, textCharFormat);
	//"�����.\n���."
	tableCell.setDash(row,12,N_MaterDopomoga_Sum, textCharFormat);
	//"�������"
	tableCell.setDash(row,13,N_Doplata_Sum, textCharFormat);
	//"����������"
	tableCell.setDash(row,14,N_Index_Sum, textCharFormat);
	//"����.\n������."
	tableCell.setDash(row,15,N_VyplProfspilky_Sum, textCharFormat);
	//"��.\n�����."
	tableCell.setDash(row,16,N_InshiNarahuvannya_Sum, textCharFormat);
	//"������.\n�����."
	tableCell.setDash(row,17,allNarahSum, textCharFormat);
		
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� ²���������� � ���вǲ ��������ʲ�\n�� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("�� ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("�."), textCharFormat);
	//��������� �������
	constraints.clear();;
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 30);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	constraints << QTextLength(QTextLength::FixedLength, 70);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setHeaderRowCount(1);
	table = cursor.insertTable(numRows+2, 9, tableFormat);
	tableCell.setTextTable(table);
	
	//���������� ����� �������
	textCharFormat.setFontPointSize( 9 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 9 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	fnt = textCharFormat.font();
	fnt.setItalic(true);
	textCharFormat.setFont(fnt);
	fnt = textCharFormat_bold.font();
	fnt.setItalic(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	tableCell.setAlignment(Qt::AlignCenter);
	
	tableCell.set(0,0,"� �/�", textCharFormat);
	tableCell.set(0,1,"���. ���.", textCharFormat);
	tableCell.set(0,2,"�-���", textCharFormat);
	tableCell.set(0,3,"����.����", textCharFormat);
	tableCell.set(0,4,"��������", textCharFormat);
	tableCell.set(0,5,"����.\n����.", textCharFormat);
	tableCell.set(0,6,"���.���.", textCharFormat);
	tableCell.set(0,7,"����.", textCharFormat);
	tableCell.set(0,8,"������\n���.", textCharFormat);
	
	//���������� �������
	fnt = textCharFormat.font();
	fnt.setItalic(false);
	textCharFormat.setFont(fnt);
	fnt = textCharFormat_bold.font();
	fnt.setItalic(false);
	textCharFormat_bold.setFont(fnt);

	double V_PensFond=0, V_SocStrah=0, V_Fond_zainat=0, V_NeshchVyp=0, V_Inshi=0;
	double curVidrahSum;
	double allVidrahSum=0;
	pracCount = 0;
	
	query->exec("SELECT Buh_rahunok, count(*), sum(N_Osn_narah), sum(N_VuslRokiv), sum(N_Likarn), sum(N_Vidpuskni), \
						sum(N_Svatkovi_dni), sum(N_Nichni), sum(N_Nadurochni), sum(N_Klasn_vodiya), sum(N_Premia), \
						sum(N_MaterDopomoga), sum(N_Doplata), sum(N_Indexaciya), sum(N_VyplProfspilky), sum(N_InshiNarahuvannya), \
						sum(V_PensFond+V_EV_Likarn+V_EV_Vidpuskni), sum(V_SocStrah), sum(V_Fond_zainat), sum(V_NeshchVyp), sum(V_Inshi) \
					FROM operacii WHERE year(CurDate)="+mysqlStr(year)+" \
						and month(CurDate)="+mysqlStr(month)+" \
						"+(pracFilter?QString(" and (operacii.Pracuye=true or operacii.OznakaDohodu>1) "):QString(""))+" \
					GROUP BY Buh_rahunok \
					ORDER BY Buh_rahunok");
	for (row=1; (row<=numRows) && (query->next()); row++){
		tableCell.setAlignment(Qt::AlignRight);
		//"� �/�"
		tableCell.set(row,0,row, textCharFormat);
		//"���. ���."
		tableCell.setDash(row,1,query->value(0).toInt(), textCharFormat);
		//"�-���"
		iVal = query->value(1).toInt();
		pracCount += iVal;
		tableCell.set(row,2,iVal, textCharFormat);
		
		//"����.����"
		dVal = query->value(16).toDouble();
		curVidrahSum = dVal;
		V_PensFond += dVal;
		tableCell.setDash(row,3,dVal, textCharFormat);
		//"��������"
		dVal = query->value(17).toDouble();
		curVidrahSum += dVal;
		V_SocStrah += dVal;
		tableCell.setDash(row,4,dVal, textCharFormat);
		//"����.\n����."
		dVal = query->value(18).toDouble();
		curVidrahSum += dVal;
		V_Fond_zainat += dVal;
		tableCell.setDash(row,5,dVal, textCharFormat);
		//"���.���."
		dVal = query->value(19).toDouble();
		curVidrahSum += dVal;
		V_NeshchVyp += dVal;
		tableCell.setDash(row,6,dVal, textCharFormat);
		//"����."
		dVal = query->value(20).toDouble();
		curVidrahSum += dVal;
		V_Inshi += dVal;
		tableCell.setDash(row,7,dVal, textCharFormat);
		//"������ �����������"
		allVidrahSum += curVidrahSum;
		tableCell.setDash(row,8,curVidrahSum, textCharFormat);
			
		postupForm->inc_pos();
	}
	
	//���������� ������ "������"
	table->mergeCells(row,0,1,2);
	
	//������
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(row,0,"������", textCharFormat);
	
	//"�-���"
	tableCell.set(row,2,pracCount, textCharFormat);
	
	//"����.����"
	tableCell.setDash(row,3,V_PensFond, textCharFormat);
	//"��������"
	tableCell.setDash(row,4,V_SocStrah, textCharFormat);
	//"����.\n����."
	tableCell.setDash(row,5,V_Fond_zainat, textCharFormat);
	//"���.���."
	tableCell.setDash(row,6,V_NeshchVyp, textCharFormat);
	//"����."
	tableCell.setDash(row,7,V_Inshi, textCharFormat);
	//"������ �����������"
	tableCell.setDash(row,8,allVidrahSum, textCharFormat);
	
	
	// ���� ������� �������� ������� ������
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� �������� ������� ������ � ���вǲ ²����ʲ�\n�� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	//��������� �������
	constraints.clear();;
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 120);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setHeaderRowCount(1);
	table = cursor.insertTable(2, 3, tableFormat);
	tableCell.setTextTable(table);
	
	//���������� ����� �������
	textCharFormat.setFontPointSize( 9 );
	textCharFormat_bold.setFontPointSize( 9 );
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"³������, %", textCharFormat);
	tableCell.set(0,1,"�-��� ����.", textCharFormat);
	tableCell.set(0,2,"����, ���.", textCharFormat);
	
	row = 1;
	int workersCount = 0;
	double salSum = 0;
	tableCell.setAlignment(Qt::AlignRight);
	query->exec("SELECT Vids_U_PensFond, count(*), sum(U_PensFond) FROM \
				(SELECT U_PensFond, Vids_U_PensFond FROM operacii \
				WHERE year(CurDate)="+sqlStr(year)+" and month(CurDate)="+sqlStr(month)+" and U_PensFond<>0 \
				UNION ALL \
				SELECT U_EV_Likarn, Vids_U_EV_Likarn FROM operacii \
				WHERE year(CurDate)="+sqlStr(year)+" and month(CurDate)="+sqlStr(month)+" and U_EV_Likarn<>0) AS tEv \
				GROUP BY Vids_U_PensFond");
	while (query->next()) {
		table->insertRows(row, 1);
		
		tableCell.set(row,0,uMToStr2(query->value(0).toDouble()), textCharFormat);
		iVal = query->value(1).toInt();
		workersCount += iVal;
		tableCell.set(row,1,iVal, textCharFormat);
		dVal = query->value(2).toDouble();
		salSum += dVal;
		tableCell.set(row,2,uMToStr2(dVal), textCharFormat);
		
		++row;
	}
	//ϳ������
	tableCell.set(row,0,"�����", textCharFormat_bold);
	tableCell.set(row,1,workersCount, textCharFormat_bold);
	tableCell.set(row,2,uMToStr2(salSum), textCharFormat_bold);
	
	
	// ���� ������� ���������� ������� ������
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("²��̲��� ���������� ������� ������ � ���вǲ ²����ʲ�\n�� " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	//��������� �������
	constraints.clear();;
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 120);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setHeaderRowCount(1);
	table = cursor.insertTable(2, 3, tableFormat);
	tableCell.setTextTable(table);
	
	//���������� ����� �������
	textCharFormat.setFontPointSize( 9 );
	textCharFormat_bold.setFontPointSize( 9 );
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"³������, %", textCharFormat);
	tableCell.set(0,1,"�-��� ����.", textCharFormat);
	tableCell.set(0,2,"����, ���.", textCharFormat);
	
	row = 1;
	workersCount = 0;
	salSum = 0;
	tableCell.setAlignment(Qt::AlignRight);
	query->exec("SELECT Vids_V_PensFond, count(*), sum(V_PensFond) FROM \
				(SELECT V_PensFond, Vids_V_PensFond FROM operacii \
				WHERE year(CurDate)="+sqlStr(year)+" and month(CurDate)="+sqlStr(month)+" and V_PensFond<>0 \
				UNION ALL \
				SELECT V_EV_Likarn, Vids_V_EV_Likarn FROM operacii \
				WHERE year(CurDate)="+sqlStr(year)+" and month(CurDate)="+sqlStr(month)+" and V_EV_Likarn<>0) AS tEv \
				GROUP BY Vids_V_PensFond");
	while (query->next()) {
		table->insertRows(row, 1);
		
		tableCell.set(row,0,uMToStr2(query->value(0).toDouble()), textCharFormat);
		iVal = query->value(1).toInt();
		workersCount += iVal;
		tableCell.set(row,1,iVal, textCharFormat);
		dVal = query->value(2).toDouble();
		salSum += dVal;
		tableCell.set(row,2,uMToStr2(dVal), textCharFormat);
		
		++row;
	}
	//ϳ������
	tableCell.set(row,0,"�����", textCharFormat_bold);
	tableCell.set(row,1,workersCount, textCharFormat_bold);
	tableCell.set(row,2,uMToStr2(salSum), textCharFormat_bold);
	
	
	delete postupForm;
	delete query;
	printform->printer()->setDocName("�����. � �����. � ����� ����������� �� "+QString::number(month)+"."+QString::number(year)+"�.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
QTextDocumentFragment UPrintDocs::createWorkerCardFragment(int worker_id, int year, int month)
{
	QTextDocument * doc = new QTextDocument();
	QSqlQuery * query = new QSqlQuery();
	QSqlQuery * tQuery = new QSqlQuery();
	
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	doc->clear();
	QTextCursor cursor(doc), cellCursor;
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText("C������� ��������� \"���-���\" "+QDate::currentDate().toString("dd.MM.yyyy")+"�. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 10 );
	cursor.insertText("ϳ���������: " + OrganizShortName(), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 10 );
	cursor.insertText("������ �� ������Ҳ �� " + monthlist.at(month-1).toUpper() + " " + QString::number(year)+"�.", textCharFormat_bold);
	
	query->exec("SELECT oper.Vh_saldo, oper.N_Osn_narah, oper.N_VuslRokiv, oper.N_Likarn, oper.N_Vidpuskni, \
						oper.N_Svatkovi_dni, oper.N_Nichni, oper.N_Nadurochni, oper.N_Klasn_vodiya, oper.N_Premia, \
						oper.N_MaterDopomoga, oper.N_Doplata, oper.N_VyplProfspilky, oper.N_InshiNarahuvannya, \
						'', oper.U_PensFond, oper.U_Fond_zainat, oper.U_SocStrah, oper.U_SocPilga, \
						oper.U_Prybutkov_podatok, oper.U_VidrahProfspilka, oper.U_InshiVidrahuvannya, \
						oper.O_ZaPoperMis, oper.O_Avans, oper.O_Inshi, oper.O_Shtraf, oper.O_Alimenty, \
						oper.O_Premiya, oper.O_Oplacheno, oper.TypRoboty, oper.TypOplaty, oper.Stavka, \
						oper.Ozn_pilgy, oper.Klasnist_vodiya, oper.VidsDoplaty, oper.VidsAlimentiv, \
						oper.Pensioner, oper.ChlenProfspilky, oper.Invalid, oper.Vids_U_Socstrah, \
						oper.Posada_id, oper.Rozryad, npr2.Prizv, npr2.Imia, npr2.Pobatk, npr2.Iden_kod, \
						taryfni_sitky.Posada, oper.VydTabelyaInMonth, oper.N_Indexaciya \
				FROM ((SELECT * FROM npr WHERE id="+QString::number(worker_id)+") AS npr2 \
				LEFT JOIN (SELECT * FROM operacii \
						WHERE OznakaDohodu=1 and Npr_id="+QString::number(worker_id)+" \
							and year(CurDate)="+QString::number(year)+" \
							and month(CurDate)="+QString::number(month)+") \
					AS oper ON oper.Npr_id=npr2.id) \
				LEFT JOIN taryfni_sitky ON oper.Posada_id=taryfni_sitky.id");
	query->seek(0);
	qDebug() << query->lastQuery();
	QString str, str2, str3;
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 9 );
	str = query->value(43).toString();
	if (str.size()>0)
		str = str.at(0);
	else
		str = "-";
	str2 = query->value(44).toString();
	if (str2.size()>0)
		str2 = str2.at(0);
	else
		str2 = "-";
	str3 = query->value(7).toString();
	if (str3.size()<5)
		str3 = "-";
	cursor.insertText("���������: " + query->value(42).toString().toUpper() + " " + str+"." + str2 + ". [" + str3 + "]", textCharFormat);
	
	//��������� ������� ��ղ��� �������ֲ�
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	constraints << QTextLength(QTextLength::FixedLength, 75);
	constraints << QTextLength(QTextLength::FixedLength, 185);
	constraints << QTextLength(QTextLength::FixedLength, 200);
	constraints << QTextLength(QTextLength::FixedLength, 60);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(0.6);
	QTextTable *table = cursor.insertTable(9, 4, tableFormat);
	
	//���������� ������� ��ղ��� �������ֲ�
	//---
	cell = table->cellAt(0, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������",textCharFormat);
	
	//---
	cell = table->cellAt(1, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("��� ������",textCharFormat);
	
	//---
	cell = table->cellAt(2, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������",textCharFormat);
	
	//---
	cell = table->cellAt(3, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�����",textCharFormat);
	
	//---
	cell = table->cellAt(4, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������",textCharFormat);
	
	//������
	cell = table->cellAt(0, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(query->value(46).toString(),textCharFormat);
	
	//��� ������
	cell = table->cellAt(1, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(29).toInt()==0)
		str = "����";
	else
		str = "������";
	cellCursor.insertText(str,textCharFormat);
	
	//������
	cell = table->cellAt(2, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(30).toInt()==0)
		str = "��������";
	else
		str = "���������";
	cellCursor.insertText(str,textCharFormat);
	
	//�����
	cell = table->cellAt(3, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(query->value(31).toString()+"���.",textCharFormat);
	
	//������
	cell = table->cellAt(4, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(30).toInt()==1)
		str = query->value(41).toString();
	else
		str = "-";
	cellCursor.insertText(str,textCharFormat);
	
	//---
	cell = table->cellAt(0, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������ �����",textCharFormat);
	
	//---
	cell = table->cellAt(1, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������� ����",textCharFormat);
	
	//---
	cell = table->cellAt(2, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������",textCharFormat);
	
	//---
	cell = table->cellAt(3, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������",textCharFormat);
	
	//---
	cell = table->cellAt(4, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���������",textCharFormat);
	
	//������ �����
	cell = table->cellAt(0, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(32).toInt()!=0)
		str = query->value(32).toString();
	else
		str = "-";
	cellCursor.insertText(str,textCharFormat);
	
	//�������� ����
	cell = table->cellAt(1, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(33).toDouble()!=0)
		str = query->value(33).toString();
	else
		str = "-";
	cellCursor.insertText(str,textCharFormat);
	
	//�������
	cell = table->cellAt(2, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(34).toDouble()!=0)
		str = query->value(34).toString();
	else
		str = "-";
	cellCursor.insertText(str,textCharFormat);
	
	//�������
	cell = table->cellAt(3, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(35).toDouble()!=0)
		str = query->value(35).toString();
	else
		str = "-";
	cellCursor.insertText(str,textCharFormat);
	
	//���������
	cell = table->cellAt(4, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(37).toBool())
		str = "���";
	else
		str = "-";
	cellCursor.insertText(str,textCharFormat);
	
	//��������� ������
	table->mergeCells(5,0,1,4);
	cell = table->cellAt(5, 0);
	cell.setFormat(textCharFormat_bold);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������",textCharFormat_bold);
	
	//---
	cell = table->cellAt(6, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(30).toInt() == 0)
		str = "������� ��";
	else
		str = "������� ���.";
	cellCursor.insertText(str,textCharFormat);
	
	//---
	cell = table->cellAt(7, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("ͳ�� ���.",textCharFormat);
	
	//---
	cell = table->cellAt(8, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������� ���.",textCharFormat);
	
	//������� ������/��
	cell = table->cellAt(6, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//��������� ������
		if (query->value(30).toInt() == 0)//�������� ������
			tQuery->exec("SELECT count(*) \
						FROM tabel \
						WHERE Pracivn_status_id=1 \
							and Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
		else//��������� ������
			tQuery->exec("SELECT sum(God_zag) \
						FROM tabel \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
	}
	else{//ϳ��������� ������
		if (query->value(30).toInt() == 0)//�������� ������
			tQuery->exec("SELECT Work_days_sum \
							FROM tabel_sum \
							WHERE Npr_id="+QString::number(worker_id)+" \
								and year(Cur_date)="+QString::number(year)+" \
								and month(Cur_date)="+QString::number(month));
		else//��������� ������
			tQuery->exec("SELECT God_zag_sum \
						FROM tabel_sum \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
	}
	tQuery->seek(0);
	cellCursor.insertText(tQuery->value(0).toString(),textCharFormat);
	
	//ͳ��
	cell = table->cellAt(7, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//��������� ������
		if (query->value(30).toInt() == 0)//�������� ������
			str = "-";
		else{//��������� ������
			tQuery->exec("SELECT sum(God_nichni) \
						FROM tabel \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
			tQuery->seek(0);
			str = tQuery->value(0).toString();
		}
	}
	else{//ϳ��������� ������
		if (query->value(30).toInt() == 0)//�������� ������
			str = "-";
		else{//��������� ������
			tQuery->exec("SELECT God_nichni_sum \
						FROM tabel_sum \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
			tQuery->seek(0);
			str = tQuery->value(0).toString();
		}
	}
	cellCursor.insertText(str,textCharFormat);
	
	//�������
	cell = table->cellAt(8, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//��������� ������
		if (query->value(30).toInt() == 0)//�������� ������
			str = "-";
		else{//��������� ������
			tQuery->exec("SELECT sum(God_sv) \
						FROM tabel \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
			tQuery->seek(0);
			str = tQuery->value(0).toString();
		}
	}
	else{//ϳ��������� ������
		if (query->value(30).toInt() == 0)//�������� ������
			str = "-";
		else{//��������� ������
			tQuery->exec("SELECT God_sv_sum \
						FROM tabel_sum \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
			tQuery->seek(0);
			str = tQuery->value(0).toString();
		}
	}
	cellCursor.insertText(str,textCharFormat);
	
	//---
	cell = table->cellAt(6, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("˳������ ��",textCharFormat);
	
	//---
	cell = table->cellAt(7, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("³������ ��",textCharFormat);
	
	//˳������
	cell = table->cellAt(6, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//��������� ������
		tQuery->exec("SELECT count(*) \
						FROM tabel \
						WHERE Pracivn_status_id=2 \
							and Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
		tQuery->seek(0);
		str = tQuery->value(0).toString();
	}
	else{//ϳ��������� ������
		tQuery->exec("SELECT Likarn_days_sum \
					FROM tabel_sum \
					WHERE Npr_id="+QString::number(worker_id)+" \
						and year(Cur_date)="+QString::number(year)+" \
						and month(Cur_date)="+QString::number(month));
		tQuery->seek(0);
		str = tQuery->value(0).toString();
	}
	cellCursor.insertText(str,textCharFormat);
	
	
	//³������
	cell = table->cellAt(7, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//��������� ������
		tQuery->exec("SELECT count(*) \
						FROM tabel \
						WHERE Pracivn_status_id=3 \
							and Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
		tQuery->seek(0);
		str = tQuery->value(0).toString();
	}
	else{//ϳ��������� ������
		tQuery->exec("SELECT vidpusk_days_sum \
					FROM tabel_sum \
					WHERE Npr_id="+QString::number(worker_id)+" \
						and year(Cur_date)="+QString::number(year)+" \
						and month(Cur_date)="+QString::number(month));
		tQuery->seek(0);
		str = tQuery->value(0).toString();
	}
	cellCursor.insertText(str,textCharFormat);
	
	cursor.movePosition(QTextCursor::End);
	//blockFormat.setAlignment( Qt::AlignCenter );
	//cursor.insertBlock(blockFormat);
	//cursor.insertText("�����ֲ�",textCharFormat_bold);
	
	//��������� ������� ��������
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 200);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 200);
	constraints << QTextLength(QTextLength::FixedLength, 60);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(0.6);
	table = cursor.insertTable(26, 4, tableFormat);
	
	table->mergeCells(1,0,1,2);
	table->mergeCells(1,2,1,2);
	
	//���������� ������� ��������
	cell = table->cellAt(0, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�����ֲ�",textCharFormat_bold);
	
	//---
	cell = table->cellAt(0, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������ ������",textCharFormat);
	
	//������ ������
	cell = table->cellAt(0, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	double vhSaldo = query->value(0).toDouble();
	cellCursor.insertText(m_to_str_2(query->value(0).toDouble()),textCharFormat);
	
	//---
	cell = table->cellAt(1, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�����������:",textCharFormat_bold);
	
	//---
	cell = table->cellAt(1, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���������:",textCharFormat_bold);
	
	//���������� ��������� �����������
	//---
	cell = table->cellAt(2, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������� �����������",textCharFormat);
	
	//---
	cell = table->cellAt(3, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������� ����",textCharFormat);
	
	//---
	cell = table->cellAt(4, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("˳������",textCharFormat);
	
	//---
	cell = table->cellAt(5, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("³������",textCharFormat);
	
	//---
	cell = table->cellAt(6, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������",textCharFormat);
	
	//---
	cell = table->cellAt(7, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("ͳ��",textCharFormat);
	
	//---
	cell = table->cellAt(8, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("��������",textCharFormat);
	
	//---
	cell = table->cellAt(9, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������� ����",textCharFormat);
	
	//---
	cell = table->cellAt(10, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�����",textCharFormat);
	
	//---
	cell = table->cellAt(11, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���������� ��������",textCharFormat);
	
	//---
	cell = table->cellAt(12, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������",textCharFormat);
	
	//---
	cell = table->cellAt(13, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������� ���������",textCharFormat);
	
	//---
	cell = table->cellAt(14, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� �����������",textCharFormat);
	
	//����������� ����������
	cell = table->cellAt(14, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���������� ��������",textCharFormat);
	
	//---
	cell = table->cellAt(15, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� ����������� � ��=01",textCharFormat_bold);
	
	//���������� ��������� �����������
	double narahSum=0;
	double val;
	//������� �����������
	cell = table->cellAt(2, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(1).toDouble();
	narahSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//������� ����
	cell = table->cellAt(3, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(2).toDouble();
	narahSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	
	//˳������
	cell = table->cellAt(4, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(3).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//³������
	cell = table->cellAt(5, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(4).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//�������
	cell = table->cellAt(6, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(5).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//ͳ��
	cell = table->cellAt(7, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(6).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//��������
	cell = table->cellAt(8, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(7).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//�������� ����
	cell = table->cellAt(9, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(8).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//�����
	cell = table->cellAt(10, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(9).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//���������� ��������
	cell = table->cellAt(11, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(10).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//�������
	cell = table->cellAt(12, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(11).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//������� ���������
	cell = table->cellAt(13, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(12).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//���� �����������
	cell = table->cellAt(14, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(13).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//���������� ��������
	cell = table->cellAt(14, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(48).toDouble();
	narahSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//������ ����������
	cell = table->cellAt(15, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(narahSum),textCharFormat_bold);
	
	//���������� ��������� ���������
	//---
	cell = table->cellAt(2, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������� ����",textCharFormat);
	
	//---
	cell = table->cellAt(3, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� ���������",textCharFormat);
	
	//---
	cell = table->cellAt(4, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� ���.�����������",textCharFormat);
	
	//---
	cell = table->cellAt(5, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("����������� �������",textCharFormat);
	
	//---
	cell = table->cellAt(6, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("� �.�. ��������� �����",textCharFormat);
	
	//---
	cell = table->cellAt(7, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("³����.�� ���������",textCharFormat);
	
	//---
	cell = table->cellAt(8, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� �����������",textCharFormat);
	
	//---
	cell = table->cellAt(15, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� �������� � �� = 01",textCharFormat);
	
	double utrymSum = 0;
	//�������� ����
	cell = table->cellAt(2, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(15).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//���� ���������
	cell = table->cellAt(3, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(16).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//���� ���.�����������
	cell = table->cellAt(4, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(17).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//����������� �������
	cell = table->cellAt(5, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(19).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//� �.�. ��������� �����
	cell = table->cellAt(6, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(18).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//³����.�� ���������
	cell = table->cellAt(7, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(37).toBool()){
		val = query->value(20).toDouble();
		utrymSum += val;
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	}
	else
		cellCursor.insertText("-",textCharFormat);	
	
	//���� �����������
	cell = table->cellAt(8, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(21).toDouble();
	utrymSum += val;
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//������ ��������
	cell = table->cellAt(15, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(utrymSum),textCharFormat_bold);
	
	//���������� ������ ����������� ����������� � ���������
	tQuery->exec("SELECT 	sum(N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
								N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
								N_Doplata+N_VyplProfspilky+N_InshiNarahuvannya) AS narahSum, \
							sum(U_PensFond+U_Fond_zainat+U_SocStrah+U_Prybutkov_podatok+ \
								U_VidrahProfspilka+U_InshiVidrahuvannya) AS utrymSum, \
							sum(O_VsogoNaRuky) \
				FROM operacii \
				WHERE OznakaDohodu>1 and Npr_id="+mysqlStr(worker_id)+" \
					and year(CurDate)="+mysqlStr(year)+" and month(CurDate)="+mysqlStr(month));
	
	tQuery->seek(0);
	double otherNarahSum = tQuery->value(0).toDouble();
	double otherUtrynSum = tQuery->value(1).toDouble();
	double otherNarahNaRuky = tQuery->value(2).toDouble();

	cell = table->cellAt(16, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� ����������� � �� > 01");
	
	//����������� � �� > 01
	cell = table->cellAt(16, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (otherNarahSum != 0)
		cellCursor.insertText(m_to_str_2(otherNarahSum),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	cell = table->cellAt(16, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("��������� � �� > 01");
	
	//table->mergeCells(17,0,1,3);
	cell = table->cellAt(17, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�� ���� � �� = 01");
	
	cell = table->cellAt(17, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�� ���� � �� > 01");
	
	cell = table->cellAt(18, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������ �� ���� �� �����.", textCharFormat_bold);
	
	cell = table->cellAt(18, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������ �����.", textCharFormat_bold);
	
	//��������� � �� > 01
	cell = table->cellAt(16, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (otherUtrynSum != 0)
		cellCursor.insertText(m_to_str_2(otherUtrynSum),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//�� ���� � �� = 01
	cell = table->cellAt(17, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (otherNarahNaRuky != 0)
		cellCursor.insertText(m_to_str_2(narahSum - utrymSum - otherUtrynSum),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//�� ���� � �� > 01
	cell = table->cellAt(17, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (otherNarahNaRuky != 0)
		cellCursor.insertText(m_to_str_2(otherNarahNaRuky),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//������ �� ���� � ��������� �����
	cell = table->cellAt(18, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(narahSum - utrymSum - otherUtrynSum + otherNarahNaRuky),textCharFormat_bold);
	
	//������ �����.
	cell = table->cellAt(18, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(utrymSum + otherUtrynSum),textCharFormat_bold);
	
	
	
	//�������
	table->mergeCells(19,0,1,4);
	
	//---
	cell = table->cellAt(19, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignCenter );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������",textCharFormat_bold);
	
	
	//������, �������������, �����, �������
	//---
	cell = table->cellAt(20, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�����",textCharFormat);
	
	//---
	cell = table->cellAt(21, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("��������� �� �����. ��.",textCharFormat);
	
	//---
	cell = table->cellAt(22, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�����. ���. � ��������� �����",textCharFormat);
	
	//---
	cell = table->cellAt(23, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	//cellCursor.insertText("�������������",textCharFormat);
	cellCursor.insertText("�����",textCharFormat);
	
	//---
	cell = table->cellAt(20, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�����",textCharFormat);
	
	//---
	cell = table->cellAt(21, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("�������",textCharFormat);
	
	//---
	cell = table->cellAt(22, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("���� �������",textCharFormat);
	
	//���������� ��������� �����, �������������, �����, �������
	//�����
	cell = table->cellAt(20, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(23).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//��������� �� �����. ��.
	cell = table->cellAt(21, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(22).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//�����. ���. � ��������� �����
	cell = table->cellAt(22, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(28).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//�����
	cell = table->cellAt(23, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(27).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//�����
	cell = table->cellAt(20, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(25).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//�������
	cell = table->cellAt(21, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(26).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//����
	cell = table->cellAt(22, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(24).toDouble();
	if ( val != 0)
		cellCursor.insertText(m_to_str_2(val),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//---
	cell = table->cellAt(24, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("��������� � ��������� �����",textCharFormat_bold);
	
	//---
	cell = table->cellAt(25, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("������� ������",textCharFormat);
	
	table->mergeCells(24,0,1,3);
	table->mergeCells(25,0,1,3);
	
	//��������� � ��������� �����
	cell = table->cellAt(24, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(22).toDouble() + query->value(23).toDouble() + query->value(24).toDouble() + query->value(25).toDouble() + 
			query->value(26).toDouble() + query->value(27).toDouble() + query->value(28).toDouble();
	double oplataSum = val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat_bold);
	
	//������� ������
	cell = table->cellAt(25, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(vhSaldo + narahSum + otherNarahNaRuky - utrymSum - otherUtrynSum - oplataSum),textCharFormat);
	
	QTextDocumentFragment fragment(doc);
	delete doc;
	delete query;
	delete tQuery;
	return fragment;
}
//--------------------utech--------------------utech--------------------utech--------------------
UPrintDocs::~UPrintDocs()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
