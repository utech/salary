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
void UPrintDocs::print_onePlatijka(UPlatijkyInfo p)
{
	printform = new UPrintForm(0, "������� ���������",true);
	printform->document()->clear();
	
	QTextCursor cellCursor(printform->document());
	cellCursor.insertFragment(createPlatijkaFragment(p));
	
	printform->printer()->setDocName("������� ���������");
	printform->printer()->setPageMargins ( 5, 5, 5, 1, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
QTextDocumentFragment UPrintDocs::createPlatijkaFragment(UPlatijkyInfo p, QTextFormat::PageBreakFlag pageBrake)
{
	QTextDocument * doc = new QTextDocument();
	QTextCursor cursor(doc), cellCursor;
	QTextBlockFormat blockFormat;
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table;

	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_bu;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignBottom);
	textCharFormat.setFontPointSize( 12 );
	textCharFormat_bold.setFontPointSize( 12 );
	textCharFormat_bu = textCharFormat_bold;
	textCharFormat_bu.setFontUnderline(true);
	
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.setBlockFormat( blockFormat );
	
	//��������� ������� ��� ������� 0410001
	constraints << QTextLength(QTextLength::FixedLength, 100);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setBorder(1);
	tableFormat.setBorderBrush(QBrush(Qt::black));
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(2);
	tableFormat.setAlignment(Qt::AlignRight);
	table = cursor.insertTable(1, 1, tableFormat);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	cell = table->cellAt(0, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignCenter );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("0410001",textCharFormat);
	cursor.movePosition(QTextCursor::End);
	
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertText("���Ҳ���  ��������� � "+p.platijkaNum, textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertText("�� \""+QString::number(p.date.day())+"\" "+monthlist.at(p.date.month()-1)+" "+QString::number(p.date.year())+"�.", textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	cursor.insertText("�������� ������", textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	cursor.insertText("\"____\"___________"+QString::number(p.date.year())+"�.", textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//��������� ������� ��� ������� ���������� �������
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 110);
	constraints << QTextLength(QTextLength::FixedLength, 180);
	constraints << QTextLength(QTextLength::FixedLength, 80);
	constraints << QTextLength(QTextLength::FixedLength, 20);
	constraints << QTextLength(QTextLength::FixedLength, 130);
	constraints << QTextLength(QTextLength::FixedLength, 110);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setBorder(0);
	tableFormat.setBorderBrush(QBrush(Qt::lightGray));
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(0);
	tableFormat.setAlignment(Qt::AlignLeft);
	table = cursor.insertTable(9, 7, tableFormat);
	UPopulateCell tableCell(table);
	
	tableCell.setBlockMargin(1, 0, 1, 0);
	
	textCharFormat.setFontPointSize( 11 );
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat_bu.setFontPointSize( 11 );
	//�������
	table->mergeCells(0,0,1,7);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(0,0,"�������: ", textCharFormat);
	tableCell.insertText(p.platnyk, textCharFormat_bold);
	
	//��� ������
	table->mergeCells(1,2,1,5);
	tableCell.set(1,0,"���: ", textCharFormat);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.setFrameFormat(1, Qt::black, QTextFrameFormat::BorderStyle_Solid);
	tableCell.setWithFrame(1,1,p.platnykEdrpou, textCharFormat_bold);
	
	//���� ��������
	table->mergeCells(2,0,2,3);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(2,0,"���� ��������\n", textCharFormat);
	tableCell.insertText(p.platnykBank, textCharFormat_bu);
	
	//��� �����
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(2,3,"��� �����", textCharFormat);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.setWithFrame(3,3,p.platnykMFO, textCharFormat_bold);
	
	//����� ���. �
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(2,5,"����� ���. �", textCharFormat);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.setWithFrame(3,5,p.platnykRahunok, textCharFormat_bold);
	
	//����
	table->mergeCells(3,6,6,1);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(2,6,"����", textCharFormat);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignBottom);
	tableCell.setWithFrame(3,6,"\n\n\n"+m_to_str_2(p.suma)+"\n\n\n", textCharFormat_bold);
	
	//���������
	table->mergeCells(5,0,1,6);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(5,0,"���������: ", textCharFormat);
	tableCell.insertText(p.oderjuvach, textCharFormat_bold);
	
	//��� ������
	table->mergeCells(6,2,1,4);
	tableCell.set(6,0,"���: ", textCharFormat);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.setWithFrame(6,1,p.oderjuvachEdrpou, textCharFormat_bold);
	
	//���� ����������
	table->mergeCells(7,0,2,3);
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(7,0,"���� ����������\n", textCharFormat);
	tableCell.insertText(p.oderjuvachBank, textCharFormat_bu);
	
	//��� �����
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(7,3,"��� �����", textCharFormat);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.setWithFrame(8,3,p.oderjuvachMFO, textCharFormat_bold);
	
	//����� ���. �
	tableCell.setAlignment(Qt::AlignLeft);
	tableCell.set(7,5,"������ ���. �", textCharFormat);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.setWithFrame(8,5,p.oderjuvachRahunok, textCharFormat_bold);
	
	cursor.movePosition(QTextCursor::End);
	
	cursor.insertBlock(blockFormat);
	
	//���� �������
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertText("���� �������: ", textCharFormat);
	cursor.insertText(numberToString(m_to_str_2(p.suma)), textCharFormat_bold);
	 
	//����������� �������:
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertText("����������� �������: ", textCharFormat);
	cursor.insertText(p.pryznachennyaPlateju, textCharFormat_bold);
	
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//��������� ������� ��� ������ � �������
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 100);
	constraints << QTextLength(QTextLength::FixedLength, 110);
	constraints << QTextLength(QTextLength::FixedLength, 220);
	constraints << QTextLength(QTextLength::FixedLength, 300);

	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setBorder(0);
	tableFormat.setBorderBrush(QBrush(Qt::white));
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setAlignment(Qt::AlignLeft);
	table = cursor.insertTable(1, 4, tableFormat);
	tableCell.setTextTable(table);
	
	tableCell.setBlockMargin(1, 0, 1, 0);
	
	//̳��� �������
	tableCell.setAlignment(Qt::AlignCenter);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	tableCell.set(0,0,"�.�.\n", textCharFormat);
	
	//ϳ����
	tableCell.setAlignment(Qt::AlignRight);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignBottom);
	tableCell.set(0,1,"ϳ�����\n\n", textCharFormat);
	
	//˳���� ��� ������
	tableCell.setAlignment(Qt::AlignLeft);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignBottom);
	tableCell.set(0,2,"____________________\n\n____________________", textCharFormat);
	
	//��������� ������
	tableCell.setAlignment(Qt::AlignCenter);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignBottom);
	tableCell.set(0,3,"��������� ������\n\n�____�___________200_ �.\n\nϳ���� �����", textCharFormat);
	
	QTextDocumentFragment fragment(doc);
	
	cursor.movePosition(QTextCursor::End);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertHtml("<hr>");
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	cursor.insertFragment(fragment);
	QTextDocumentFragment resFragment(doc);
	
	//��������� ����� ��� ���� ������� � ��������� ���� �������
	doc->clear();
	constraints.clear();
	constraints << QTextLength(QTextLength::PercentageLength, 100);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setBorder(0);
	tableFormat.setBorderBrush(QBrush(Qt::white));
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(0);
	tableFormat.setAlignment(Qt::AlignLeft);
	tableFormat.setPageBreakPolicy(pageBrake);
	table = cursor.insertTable(1, 1, tableFormat);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	cell = table->cellAt(0, 0);
	cell.setFormat(textCharFormat);
	cursor = cell.firstCursorPosition();
	cursor.insertFragment(resFragment);
	
	QTextDocumentFragment resultFragment(doc);
	
	delete doc;
	return resultFragment;
}
//--------------------utech--------------------utech--------------------utech--------------------
QTextDocumentFragment UPrintDocs::createYearCardFragment(int worker_id, short year)
{
	QTextDocument * doc = new QTextDocument();
	
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_italic;
	textCharFormat_italic.setFontItalic(true);
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_italic.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat.setFontPointSize( 9 );
	textCharFormat_bold.setFontPointSize( 9 );
	textCharFormat_italic.setFontPointSize( 9 );
	QTextCursor cursor(doc), cellCursor;
	
	QSqlQuery query;
	
	query.exec("SELECT npr.Prizv, npr.Imia, npr.Pobatk, taryfni_sitky.Posada, npr.id \
				FROM npr \
				LEFT JOIN taryfni_sitky ON npr.Posada_id=taryfni_sitky.id \
				WHERE npr.id="+sqlStr(worker_id));
	query.next();
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("������ ����������:  ", textCharFormat);
	cursor.insertText("[ "+query.value(4).toString()+" ] "+query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+", "+query.value(3).toString()+" ", textCharFormat_bold);
	cursor.insertBlock( blockFormat );
	
	char rowCount = 12;
	char colCount = 19;
	
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table=NULL;
	UPopulateCell tableCell(table);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 25);
	constraints << QTextLength(QTextLength::FixedLength, 25);
	constraints << QTextLength(QTextLength::FixedLength, 65);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 65);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	constraints << QTextLength(QTextLength::FixedLength, 60);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.5);
	tableFormat.setBorderBrush(QBrush(Qt::black)); 
	tableFormat.setHeaderRowCount(1);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	table = cursor.insertTable(rowCount+2, colCount, tableFormat);
	tableCell.setTextTable(table);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"̳-����", textCharFormat);
	tableCell.set(0,1,"��", textCharFormat);
	tableCell.set(0,2,"���.\n�����.", textCharFormat);
	tableCell.set(0,3,"�������", textCharFormat);
	tableCell.set(0,4,"˳����.", textCharFormat);
	tableCell.set(0,5,"³�����", textCharFormat);
	tableCell.set(0,6,"�����.", textCharFormat);
	tableCell.set(0,7,"ͳ��", textCharFormat);
	tableCell.set(0,8,"��������.", textCharFormat);
	tableCell.set(0,9,"�����.\n���.", textCharFormat);
	tableCell.set(0,10,"�����", textCharFormat);
	tableCell.set(0,11,"�����.\n���.", textCharFormat);
	tableCell.set(0,12,"�������", textCharFormat);
	tableCell.set(0,13,"������.", textCharFormat);
	tableCell.set(0,14,"����.\n������.", textCharFormat);
	tableCell.set(0,15,"��.\n�����.", textCharFormat);
	tableCell.set(0,16,"������", textCharFormat);
	tableCell.set(0,17,"�.����.\n����.", textCharFormat);
	tableCell.set(0,18,"�.����.\n����.", textCharFormat);
	
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	int row=1;
	int workerMonthRowCount;
	int od;
	double sumNarahOd;
	double dVal;
	double colSum[colCount];
	for (int i=0; i<colCount; ++i)
		colSum[i] = 0;
	for (int month=1;month<13;++month){
		workerMonthRowCount = 0;
		
		//"̳����"
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,0,month, textCharFormat);
		
		query.exec("SELECT OznakaDohodu, N_Osn_narah, N_VuslRokiv, N_Likarn, N_Vidpuskni, N_Svatkovi_dni, \
						N_Nichni, N_Nadurochni, N_Klasn_vodiya, N_Premia, N_MaterDopomoga, \
						N_Doplata, N_Indexaciya, N_VyplProfspilky, N_InshiNarahuvannya, VydTabelyaInMonth, \
						U_PensFond, V_PensFond \
					FROM operacii \
					WHERE Npr_id="+mysqlStr(worker_id)+" \
						and year(CurDate)="+QString::number(year)+" \
						and month(CurDate)="+QString::number(month)+" \
					ORDER BY OznakaDohodu");
		while (query.next()){
			if ((++workerMonthRowCount) > 1)
				table->insertRows(row, 1);
			
			od = query.value(0).toInt();
			if (od == 1){ //���������� ������ ��� ��=1
				//"��"
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,1,query.value(0).toString().rightJustified(2,'0'), textCharFormat);
				//"���.\n�����."
				tableCell.setAlignment(Qt::AlignRight);
				dVal = query.value(1).toDouble();
				sumNarahOd = dVal;
				colSum[2] += dVal;
				tableCell.setDash(row,2,dVal, textCharFormat);
				//"�������"
				dVal = query.value(2).toDouble();
				sumNarahOd += dVal;
				colSum[3] += dVal;
				tableCell.setDash(row,3,dVal, textCharFormat);
				//"˳����."
				dVal = query.value(3).toDouble();
				sumNarahOd += dVal;
				colSum[4] += dVal;
				tableCell.setDash(row,4,dVal, textCharFormat);
				//"³�����"
				dVal = query.value(4).toDouble();
				sumNarahOd += dVal;
				colSum[5] += dVal;
				tableCell.setDash(row,5,dVal, textCharFormat);
				//"�����."
				dVal = query.value(5).toDouble();
				sumNarahOd += dVal;
				colSum[6] += dVal;
				tableCell.setDash(row,6,dVal, textCharFormat);
				//"ͳ��"
				dVal = query.value(6).toDouble();
				sumNarahOd += dVal;
				colSum[7] += dVal;
				tableCell.setDash(row,7,dVal, textCharFormat);
				//"��������."
				dVal = query.value(7).toDouble();
				sumNarahOd += dVal;
				colSum[8] += dVal;
				tableCell.setDash(row,8,dVal, textCharFormat);
				//"�����.\n���."
				dVal = query.value(8).toDouble();
				sumNarahOd += dVal;
				colSum[9] += dVal;
				tableCell.setDash(row,9,dVal, textCharFormat);
				//"�����"
				dVal = query.value(9).toDouble();
				sumNarahOd += dVal;
				colSum[10] += dVal;
				tableCell.setDash(row,10,dVal, textCharFormat);
				//"�����.\n���."
				dVal = query.value(10).toDouble();
				sumNarahOd += dVal;
				colSum[11] += dVal;
				tableCell.setDash(row,11,dVal, textCharFormat);
				//"�������"
				dVal = query.value(11).toDouble();
				sumNarahOd += dVal;
				colSum[12] += dVal;
				tableCell.setDash(row,12,dVal, textCharFormat);
				//"������."
				dVal = query.value(12).toDouble();
				sumNarahOd += dVal;
				colSum[13] += dVal;
				tableCell.setDash(row,13,dVal, textCharFormat);
				//"����.\n������."
				dVal = query.value(13).toDouble();
				sumNarahOd += dVal;
				colSum[14] += dVal;
				tableCell.setDash(row,14,dVal, textCharFormat);
				//"��.\n�����."
				dVal = query.value(14).toDouble();
				sumNarahOd += dVal;
				colSum[15] += dVal;
				tableCell.setDash(row,15,dVal, textCharFormat);
				//"������"
				colSum[16] += sumNarahOd;
				tableCell.setDash(row,16,sumNarahOd, textCharFormat);
				//"�.����.\n����."
				dVal = query.value(16).toDouble();
				colSum[17] += dVal;
				tableCell.setDash(row,17,dVal, textCharFormat);
				//"�.����.\n����."
				dVal = query.value(17).toDouble();
				colSum[18] += dVal;
				tableCell.setDash(row,18,dVal, textCharFormat);
			}
			else if (od > 1){ //���������� ������ ��� ��>1
				//"��"
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,1,query.value(0).toString().rightJustified(2,'0'), textCharFormat_italic);
				//"���.\n�����."
				tableCell.setAlignment(Qt::AlignRight);
				dVal = query.value(1).toDouble();
				//colSum[2] += dVal;
				tableCell.setDash(row,2,dVal, textCharFormat_italic);
				//"�������"
				tableCell.set(row,3,"-", textCharFormat_italic);
				//"˳����."
				tableCell.set(row,4,"-", textCharFormat_italic);
				//"³�����"
				tableCell.set(row,5,"-", textCharFormat_italic);
				//"�����."
				tableCell.set(row,6,"-", textCharFormat_italic);
				//"ͳ��"
				tableCell.set(row,7,"-", textCharFormat_italic);
				//"��������."
				tableCell.set(row,8,"-", textCharFormat_italic);
				//"�����.\n���."
				tableCell.set(row,9,"-", textCharFormat_italic);
				//"�����"
				tableCell.set(row,10,"-", textCharFormat_italic);
				//"�����.\n���."
				tableCell.set(row,11,"-", textCharFormat_italic);
				//"�������"
				tableCell.set(row,12,"-", textCharFormat_italic);
				//"������."
				tableCell.set(row,13,"-", textCharFormat_italic);
				//"����.\n������."
				tableCell.set(row,14,"-", textCharFormat_italic);
				//"��.\n�����."
				tableCell.set(row,15,"-", textCharFormat_italic);
				//"������"
				dVal = query.value(1).toDouble();
				//colSum[16] += dVal;
				tableCell.setDash(row,16,dVal, textCharFormat_italic);
				//"�.����.\n����."
				dVal = query.value(16).toDouble();
				colSum[17] += dVal;
				tableCell.setDash(row,17,dVal, textCharFormat_italic);
				//"�.����.\n����."
				dVal = query.value(17).toDouble();
				colSum[18] += dVal;
				tableCell.setDash(row,18,dVal, textCharFormat_italic);
			}
			
			++row;
		}
		if (!workerMonthRowCount){ //���������� ������ ��� ��������� ����������� � �����
			//"���.\n�����."
			tableCell.setAlignment(Qt::AlignRight);
			tableCell.set(row,2,"-", textCharFormat);
			//"�������"
			tableCell.set(row,3,"-", textCharFormat);
			//"˳����."
			tableCell.set(row,4,"-", textCharFormat);
			//"³�����"
			tableCell.set(row,5,"-", textCharFormat);
			//"�����."
			tableCell.set(row,6,"-", textCharFormat);
			//"ͳ��"
			tableCell.set(row,7,"-", textCharFormat);
			//"��������."
			tableCell.set(row,8,"-", textCharFormat);
			//"�����.\n���."
			tableCell.set(row,9,"-", textCharFormat);
			//"�����"
			tableCell.set(row,10,"-", textCharFormat);
			//"�����.\n���."
			tableCell.set(row,11,"-", textCharFormat);
			//"�������"
			tableCell.set(row,12,"-", textCharFormat);
			//"������."
			tableCell.set(row,13,"-", textCharFormat);
			//"����.\n������."
			tableCell.set(row,14,"-", textCharFormat);
			//"��.\n�����."
			tableCell.set(row,15,"-", textCharFormat);
			//"������"
			tableCell.set(row,16,"-", textCharFormat);
			//"�.����.\n����."
			tableCell.set(row,17,"-", textCharFormat);
			//"�.����.\n����."
			tableCell.set(row,18,"-", textCharFormat);
			
			workerMonthRowCount = 1;
			
			++row;
		}
		
		if (workerMonthRowCount>1) //��'������� ������ ���������� �����
			table->mergeCells( row-workerMonthRowCount, 0, workerMonthRowCount, 1);
	}
	
	//��������� �������
	table->mergeCells(row,0,1,2);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(row,0,"������", textCharFormat);
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(row,2,colSum[2], textCharFormat);
	tableCell.set(row,3,colSum[3], textCharFormat);
	tableCell.set(row,4,colSum[4], textCharFormat);
	tableCell.set(row,5,colSum[5], textCharFormat);
	tableCell.set(row,6,colSum[6], textCharFormat);
	tableCell.set(row,7,colSum[7], textCharFormat);
	tableCell.set(row,8,colSum[8], textCharFormat);
	tableCell.set(row,9,colSum[9], textCharFormat);
	tableCell.set(row,10,colSum[10], textCharFormat);
	tableCell.set(row,11,colSum[11], textCharFormat);
	tableCell.set(row,12,colSum[12], textCharFormat);
	tableCell.set(row,13,colSum[13], textCharFormat);
	tableCell.set(row,14,colSum[14], textCharFormat);
	tableCell.set(row,15,colSum[15], textCharFormat);
	tableCell.set(row,16,colSum[16], textCharFormat);
	tableCell.set(row,17,colSum[17], textCharFormat);
	tableCell.set(row,18,colSum[18], textCharFormat);
	
	QTextDocumentFragment fragment(doc);
	delete doc;
	return fragment;
}
//--------------------utech--------------------utech--------------------utech--------------------
QTextDocumentFragment UPrintDocs::createYearUtrymannyaCardFragment(int worker_id, short year)
{
	QTextDocument * doc = new QTextDocument();
	
	//����� ���������
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_italic;
	textCharFormat_italic.setFontItalic(true);
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_italic.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat.setFontPointSize( 9 );
	textCharFormat_bold.setFontPointSize( 9 );
	textCharFormat_italic.setFontPointSize( 9 );
	QTextCursor cursor(doc), cellCursor;
	
	QSqlQuery query;
	
	query.exec("SELECT npr.Prizv, npr.Imia, npr.Pobatk, taryfni_sitky.Posada, npr.id \
				FROM npr \
				LEFT JOIN taryfni_sitky ON npr.Posada_id=taryfni_sitky.id \
				WHERE npr.id="+sqlStr(worker_id));
	query.next();
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("������ ����������:  ", textCharFormat);
	cursor.insertText("[ "+query.value(4).toString()+" ] "+query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+", "
						+query.value(3).toString()+" ", textCharFormat_bold);
	cursor.insertBlock( blockFormat );
	
	char rowCount = 12;
	char colCount = 17;
	
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextTable *table=NULL;
	UPopulateCell tableCell(table);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 25);
	constraints << QTextLength(QTextLength::FixedLength, 25);
	constraints << QTextLength(QTextLength::FixedLength, 65);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 55);
	constraints << QTextLength(QTextLength::FixedLength, 65);
	
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(1);
	tableFormat.setBorder(0.5);
	tableFormat.setBorderBrush(QBrush(Qt::black)); 
	tableFormat.setHeaderRowCount(1);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	table = cursor.insertTable(rowCount+2, colCount, tableFormat);
	tableCell.setTextTable(table);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"̳-����", textCharFormat);
	tableCell.set(0,1,"��", textCharFormat);
	tableCell.set(0,2,"�����.", textCharFormat);
	tableCell.set(0,3,"�.����.\n����.", textCharFormat);
	tableCell.set(0,4,"�.����.\n����.", textCharFormat);
	tableCell.set(0,5,"�.����.\n��������.", textCharFormat);
	tableCell.set(0,6,"�.���.\n�����", textCharFormat);
	tableCell.set(0,7,"�.�������.", textCharFormat);
	tableCell.set(0,8,"�.������.", textCharFormat);
	tableCell.set(0,9,"�.����", textCharFormat);
	tableCell.set(0,10,"�.�����", textCharFormat);
	tableCell.set(0,11,"�.����.\n����.", textCharFormat);
	tableCell.set(0,12,"�.����.\n����.", textCharFormat);
	tableCell.set(0,13,"�.����.\n��������.", textCharFormat);
	tableCell.set(0,14,"�.���.\n���.", textCharFormat);
	tableCell.set(0,15,"�.����.", textCharFormat);
	tableCell.set(0,16,"�.�����", textCharFormat);
	
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	
	int row=1;
	int workerMonthRowCount;
	int od;
	double sumU, sumV;
	double dVal;
	double colSum[colCount];
	for (int i=0; i<colCount; ++i)
		colSum[i] = 0;
	for (int month=1;month<13;++month){
		workerMonthRowCount = 0;
		
		//"̳����"
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,0,month, textCharFormat);
		
		query.exec("SELECT OznakaDohodu, N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni\
						+N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+\
						+N_Doplata+N_Indexaciya+N_VyplProfspilky+N_InshiNarahuvannya, \
						U_PensFond, U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, \
						U_VidrahProfspilka, U_InshiVidrahuvannya, \
						V_PensFond, V_Fond_zainat, V_SocStrah, V_NeshchVyp, V_Inshi \
					FROM operacii \
					WHERE Npr_id="+mysqlStr(worker_id)+" \
						and year(CurDate)="+QString::number(year)+" \
						and month(CurDate)="+QString::number(month)+" \
					ORDER BY OznakaDohodu");
		while (query.next()){
			if ((++workerMonthRowCount) > 1)
				table->insertRows(row, 1);
			
			od = query.value(0).toInt();

			//"��"
			tableCell.setAlignment(Qt::AlignCenter);
			tableCell.set(row,1,query.value(0).toString().rightJustified(2,'0'), textCharFormat);
			//"�����."
			tableCell.setAlignment(Qt::AlignRight);
			dVal = query.value(1).toDouble();
			colSum[2] += dVal;
			tableCell.setDash(row,2,dVal, textCharFormat);
			//"�.����.\n����."
			dVal = query.value(2).toDouble();
			sumU = dVal;
			colSum[3] += dVal;
			tableCell.setDash(row,3,dVal, textCharFormat);
			//"�.����.\n����."
			dVal = query.value(3).toDouble();
			sumU += dVal;
			colSum[4] += dVal;
			tableCell.setDash(row,4,dVal, textCharFormat);
			//"�.����.\n��������."
			dVal = query.value(4).toDouble();
			sumU += dVal;
			colSum[5] += dVal;
			tableCell.setDash(row,5,dVal, textCharFormat);
			//"�.���.\n�����"
			dVal = query.value(6).toDouble() > 0.009 ? query.value(5).toDouble() : 0; //���� ����������� ������� 0, �� � �������� ����
			colSum[6] += dVal;
			tableCell.setDash(row,6,dVal, textCharFormat);
			//"�.�������."
			dVal = query.value(6).toDouble();
			sumU += dVal;
			colSum[7] += dVal;
			tableCell.setDash(row,7,dVal, textCharFormat);
			//"�.������."
			dVal = query.value(7).toDouble();
			sumU += dVal;
			colSum[8] += dVal;
			tableCell.setDash(row,8,dVal, textCharFormat);
			//"�.����"
			dVal = query.value(8).toDouble();
			sumU += dVal;
			colSum[9] += dVal;
			tableCell.setDash(row,9,dVal, textCharFormat);
			//"�.�����"
			colSum[10] += sumU;
			tableCell.setDash(row,10,sumU, textCharFormat);
			//"�.����.\n����."
			dVal = query.value(9).toDouble();
			sumV = dVal;
			colSum[11] += dVal;
			tableCell.setDash(row,11,dVal, textCharFormat);
			//"�.����.\n����."
			dVal = query.value(10).toDouble();
			sumV += dVal;
			colSum[12] += dVal;
			tableCell.setDash(row,12,dVal, textCharFormat);
			//"�.����.\n��������."
			dVal = query.value(11).toDouble();
			sumV += dVal;
			colSum[13] += dVal;
			tableCell.setDash(row,13,dVal, textCharFormat);
			//"�.���.\n���."
			dVal = query.value(12).toDouble();
			sumV += dVal;
			colSum[14] += dVal;
			tableCell.setDash(row,14,dVal, textCharFormat);
			//"�.����."
			dVal = query.value(13).toDouble();
			sumV += dVal;
			colSum[15] += dVal;
			tableCell.setDash(row,15,dVal, textCharFormat);
			//"�.�����"
			colSum[16] += sumV;
			tableCell.setDash(row,16,sumV, textCharFormat);
			
			++row;
		}
		if (!workerMonthRowCount){ //���������� ������ ��� ��������� ����������� � �����
			//"���.\n�����."
			tableCell.setAlignment(Qt::AlignRight);
			tableCell.set(row,1,"-", textCharFormat);
			tableCell.set(row,2,"-", textCharFormat);
			tableCell.set(row,3,"-", textCharFormat);
			tableCell.set(row,4,"-", textCharFormat);
			tableCell.set(row,5,"-", textCharFormat);
			tableCell.set(row,6,"-", textCharFormat);
			tableCell.set(row,7,"-", textCharFormat);
			tableCell.set(row,8,"-", textCharFormat);
			tableCell.set(row,9,"-", textCharFormat);
			tableCell.set(row,10,"-", textCharFormat);
			tableCell.set(row,11,"-", textCharFormat);
			tableCell.set(row,12,"-", textCharFormat);
			tableCell.set(row,13,"-", textCharFormat);
			tableCell.set(row,14,"-", textCharFormat);
			tableCell.set(row,15,"-", textCharFormat);
			tableCell.set(row,16,"-", textCharFormat);
			
			workerMonthRowCount = 1;
			
			++row;
		}
		
		if (workerMonthRowCount>1) //��'������� ������ ���������� �����
			table->mergeCells( row-workerMonthRowCount, 0, workerMonthRowCount, 1);
	}
	
	//��������� �������
	table->mergeCells(row,0,1,2);
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(row,0,"������", textCharFormat);
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.setDash(row,2,colSum[2], textCharFormat);
	tableCell.setDash(row,3,colSum[3], textCharFormat);
	tableCell.setDash(row,4,colSum[4], textCharFormat);
	tableCell.setDash(row,5,colSum[5], textCharFormat);
	tableCell.setDash(row,6,colSum[6], textCharFormat);
	tableCell.setDash(row,7,colSum[7], textCharFormat);
	tableCell.setDash(row,8,colSum[8], textCharFormat);
	tableCell.setDash(row,9,colSum[9], textCharFormat);
	tableCell.setDash(row,10,colSum[10], textCharFormat);
	tableCell.setDash(row,11,colSum[11], textCharFormat);
	tableCell.setDash(row,12,colSum[12], textCharFormat);
	tableCell.setDash(row,13,colSum[13], textCharFormat);
	tableCell.setDash(row,14,colSum[14], textCharFormat);
	tableCell.setDash(row,15,colSum[15], textCharFormat);
	tableCell.setDash(row,16,colSum[16], textCharFormat);
	
	QTextDocumentFragment fragment(doc);
	delete doc;
	return fragment;
}
//--------------------utech--------------------utech--------------------utech--------------------
