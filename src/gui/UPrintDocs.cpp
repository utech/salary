//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу UPrintDocs програми нарахування зарплати
//
// Створений програмістами Компанії "Утех" 17.07.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UPrintDocs.h"

#include <UPopulateTextTableCells>
#include <USqlAccessible>

//--------------------utech--------------------utech--------------------utech--------------------
UPrintDocs::UPrintDocs()
{
	monthlist 	<< "січень"
				<< "лютий"
				<< "березень"
				<< "квітень"
				<< "травень"
				<< "червень"
				<< "липень"
				<< "серпень"
				<< "вересень"
				<< "жовтень"
				<< "листопад"
				<< "грудень";
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
	
	printform = new UPrintForm(0, "Перелік працівників");
	printform->printer()->setOrientation( QPrinter::Landscape );
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	//Шапка документа
	printform->document()->clear();
	QTextCursor cursor(printform->document()), cellCursor;
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 8 );
	cursor.insertText("Документ створено програмою \"Ельзар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 11 );
	cursor.insertText("ДОВІДНИК ПРАЦІВНИКІВ " + OrganizShortName(), textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 6 );
	cursor.insertText(" ", textCharFormat);
	
	//Створення таблиці
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
		cursor.insertText("	Підрозділ: "+pQuery.value(1).toString(),textCharFormat_bold);
		
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
		//Заповнення шапки таблиці
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
		cellCursor.insertText("№ з/п",textCharFormat);
	
		cell = table->cellAt(0, 1);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("П І Б",textCharFormat_bold);
	
		cell = table->cellAt(0, 2);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Д.Нар.",textCharFormat);
	
		cell = table->cellAt(0, 3);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Ідентиф.код",textCharFormat);
	
		cell = table->cellAt(0, 4);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Д.прийн.",textCharFormat);
	
		cell = table->cellAt(0, 5);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Посада",textCharFormat);
	
		cell = table->cellAt(0, 6);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Роб./опл.",textCharFormat);
	
		cell = table->cellAt(0, 7);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Оклад",textCharFormat);
	
		cell = table->cellAt(0, 8);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Тел.",textCharFormat);
	
		cell = table->cellAt(0, 9);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Розр.",textCharFormat);
	
		cell = table->cellAt(0, 10);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("ОП",textCharFormat);
	
		cell = table->cellAt(0, 11);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("КВ",textCharFormat);
	
		cell = table->cellAt(0, 12);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Допл.",textCharFormat);
	
		cell = table->cellAt(0, 13);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Алім.",textCharFormat);
		
		cell = table->cellAt(0, 14);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Пр/пенс/інв",textCharFormat);
		
		cell = table->cellAt(0, 15);
		cell.setFormat(textCharFormat);
		cellCursor = cell.firstCursorPosition();
		blockFormat.setAlignment( Qt::AlignCenter );
		cellCursor.setBlockFormat( blockFormat );
		cellCursor.insertText("Прац.",textCharFormat);
	
		//Заповнення таблиці
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
		
			//№ з/п
			cell = table->cellAt(row, 0);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(QString::number(row),textCharFormat);
			//П І Б (ШД)
			cell = table->cellAt(row, 1);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(1).toString()+" "+query->value(2).toString()+" "+
									query->value(3).toString()+" ("+query->value(0).toString()+")",textCharFormat_bold);
			//Д.Нар.
			cell = table->cellAt(row, 2);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(4).toDate().toString("dd.MM.yyyy"),textCharFormat);
			//Ідентиф.код
			cell = table->cellAt(row, 3);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(8).toString(),textCharFormat);
			//Д.прийн.
			cell = table->cellAt(row, 4);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(14).toDate().toString("dd.MM.yyyy"),textCharFormat);
			//Посада
			cell = table->cellAt(row, 5);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(5).toString(),textCharFormat);
			//Роб./опл.
			cell = table->cellAt(row, 6);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (query->value(6).toInt()==0){
				str = "шт/";
				if (isWork)
					workShtatCount++;
			}
			else{
				str = "дог/";
				if (isWork)
					workDogovirCount++;
			}
			if (query->value(7).toInt()==0)
				str += "міс";
			else
				str += "год";
			cellCursor.insertText(str,textCharFormat);
			//Оклад
			cell = table->cellAt(row, 7);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(m_to_str_2(query->value(11).toDouble()),textCharFormat);
			//Тел.
			cell = table->cellAt(row, 8);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			cellCursor.insertText(query->value(9).toString(),textCharFormat);
			//Розр.
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
			//ОП
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
			//КВ
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
			//Допл.
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
			//Алім.
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
			//Пр/пенс/інв
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
			//Прац.
			cell = table->cellAt(row, 15);
			cell.setFormat(textCharFormat);
			cellCursor = cell.firstCursorPosition();
			blockFormat.setAlignment( Qt::AlignCenter );
			cellCursor.setBlockFormat( blockFormat );
			if (isWork){
				str = "Так";
				workPracivnykyCount++;
			}
			else
				str = "Ні";
			cellCursor.insertText(str,textCharFormat);
		}
	
		//Запис підсумкових результатів
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
		//Всього працівників у базі
		cursor.insertBlock(blockFormat);
		textCharFormat.setFontPointSize( 10 );
		cursor.insertText("1. Всього працівників у базі			"+QString::number(numRows), textCharFormat);
		//Всього працюючих
		cursor.insertBlock(blockFormat);
		cursor.insertText("2. Всього працюючих				"+QString::number(workPracivnykyCount), textCharFormat);
		//в т.ч. у штаті
		cursor.insertBlock(blockFormat);
		cursor.insertText("	в т.ч. у штаті				"+QString::number(workShtatCount), textCharFormat);
		//в т.ч. по договору
		cursor.insertBlock(blockFormat);
		cursor.insertText("	в т.ч. по договору			"+QString::number(workDogovirCount), textCharFormat);
		//Податкова пільга
		cursor.insertBlock(blockFormat);
		cursor.insertText("3.Працівники, які отримують податкову пільгу		"+QString::number(workOPCount), textCharFormat);
		//Класність водія
		cursor.insertBlock(blockFormat);
		cursor.insertText("4.Працівники, які отримують доплату \"Класність водія\"	"+QString::number(workKVCount), textCharFormat);
		//Доплата
		cursor.insertBlock(blockFormat);
		cursor.insertText("5.Працівники, які отримують доплату			"+QString::number(workDoplataCount), textCharFormat);
		//Аліменти
		cursor.insertBlock(blockFormat);
		cursor.insertText("6.Працівники, які сплачують аліменти			"+QString::number(workAlimentyCount), textCharFormat);
		//Профспілка
		cursor.insertBlock(blockFormat);
		cursor.insertText("7.Працівники, які є членами профспілки			"+QString::number(workProfspilkaCount), textCharFormat);
		//Пенсіонери
		cursor.insertBlock(blockFormat);
		cursor.insertText("8.Пенсіонери					"+QString::number(workPensioneryCount), textCharFormat);
		//Інваліди
		cursor.insertBlock(blockFormat);
		cursor.insertText("9.Інваліди					"+QString::number(workInvalidyCount), textCharFormat);
	}
	
	delete postupForm;
	delete query;
	
	printform->printer()->setDocName("Перелік працівників "+OrganizShortName());
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
	
	printform = new UPrintForm(0, "Нарахування за " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//Шапка документа
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
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ НАРАХУВАННЯ ЗАРПЛАТИ ПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("за ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("р."), textCharFormat);
	//Створення таблиці
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
		cursor.insertText("	Підрозділ: "+pQuery.value(1).toString(),textCharFormat_bold);
	
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
		//Заповнення шапки таблиці
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
		tableCell.set(0,0,"№ п/п", textCharFormat);
		tableCell.set(0,1,"П І Б", textCharFormat);
		tableCell.set(0,2,"Табель", textCharFormat);
		tableCell.set(0,3,"ОД", textCharFormat);
		tableCell.set(0,4,"Осн.\nнарах.", textCharFormat);
		tableCell.set(0,5,"Вислуга", textCharFormat);
		tableCell.set(0,6,"Лікарн.", textCharFormat);
		tableCell.set(0,7,"Відпуск", textCharFormat);
		tableCell.set(0,8,"Святк.", textCharFormat);
		tableCell.set(0,9,"Нічні", textCharFormat);
		tableCell.set(0,10,"Надурочн.", textCharFormat);
		tableCell.set(0,11,"Класн.\nвод.", textCharFormat);
		tableCell.set(0,12,"Премія", textCharFormat);
		tableCell.set(0,13,"Матер.\nдоп.", textCharFormat);
		tableCell.set(0,14,"Доплата", textCharFormat);
	
		tableCell.set(0,15,"Індекс.", textCharFormat);
	
		tableCell.set(0,16,"Випл.\nпрофсп.", textCharFormat);
		tableCell.set(0,17,"Ін.\nнарах.", textCharFormat);
		tableCell.set(0,18,"Всього", textCharFormat);

		//Заповнення таблиці
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
				
				//№ п/п
				tableCell.setAlignment(Qt::AlignCenter);
				if (oznakaDohodu == 1)
					tableCell.set(row,0,QString::number(++workersCounter), textCharFormat);
			
				if (firstWorkerRow){
					//П І Б
					tableCell.setAlignment(Qt::AlignLeft);
					tableCell.set(row,1,nprQuery->value(2).toString().toUpper() +" "+nprQuery->value(3).toString() +" "+nprQuery->value(4).toString(), textCharFormat);
			
					//Табель
					tableCell.setAlignment(Qt::AlignCenter);
					tableCell.set(row,2,tabelResString(nprQuery->value(0).toInt(), year, month, record.value("VydTabelyaInMonth").toInt(), nprQuery->value(5).toInt()), textCharFormat);
				
					firstWorkerRow = false;
				}
			
				//Ознака доходу
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,3,oznakaDohodu, textCharFormat);
			
				//Основне нарахування
				tableCell.setAlignment(Qt::AlignRight);
				val = query->value(1).toDouble();
				N_Osn_narah_Sum += val;
				if (oznakaDohodu > 1)
					N_Osn_Narah_Sum_Other_Dohid += val;
				workerNarahSum += val;
				tableCell.set(row,4,val, textCharFormat);
			
				if (oznakaDohodu == 1){
					//Вислуга років
					val = query->value(2).toDouble();
					N_VuslRokiv_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,5,val, textCharFormat);
					else
						tableCell.set(row,5,"-", textCharFormat);
					//"Лікарн."
					val = query->value(3).toDouble();
					N_Likarn_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,6,val, textCharFormat);
					else
						tableCell.set(row,6,"-", textCharFormat);
					//"Відпуск"
					val = query->value(4).toDouble();
					N_Vidpuskni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,7,val, textCharFormat);
					else
						tableCell.set(row,7,"-", textCharFormat);
					//"Святк."
					val = query->value(5).toDouble();
					N_Svatkovi_dni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,8,val, textCharFormat);
					else
						tableCell.set(row,8,"-", textCharFormat);
					//"Нічні"
					val = query->value(6).toDouble();
					N_Nichni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,9,val, textCharFormat);
					else
						tableCell.set(row,9,"-", textCharFormat);
					//"Надурочн."
					val = query->value(7).toDouble();
					N_Nadurochni_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,10,val, textCharFormat);
					else
						tableCell.set(row,10,"-", textCharFormat);
					//"Класн.\nвод."
					val = query->value(8).toDouble();
					N_Klasn_vodiya_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,11,val, textCharFormat);
					else
						tableCell.set(row,11,"-", textCharFormat);
					//"Премія"
					val = query->value(9).toDouble();
					N_Premia_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,12,val, textCharFormat);
					else
						tableCell.set(row,12,"-", textCharFormat);
					//"Матер.\nдоп."
					val = query->value(10).toDouble();
					N_MaterDopomoga_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,13,val, textCharFormat);
					else
						tableCell.set(row,13,"-", textCharFormat);
					//"Доплата"
					val = query->value(11).toDouble();
					N_Doplata_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,14,val, textCharFormat);
					else
						tableCell.set(row,14,"-", textCharFormat);
					//"Індексація"
					val = query->value(12).toDouble();
					N_Indexaciya_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,15,val, textCharFormat);
					else
						tableCell.set(row,15,"-", textCharFormat);	
					//"Випл.\nпрофсп."
					val = query->value(13).toDouble();
					N_VyplProfspilky_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,16,val, textCharFormat);
					else
						tableCell.set(row,16,"-", textCharFormat);
					//"Ін.\nнарах."
					val = query->value(14).toDouble();
					N_InshiNarahuvannya_Sum += val;
					workerNarahSum += val;
					if (val != 0)
						tableCell.set(row,17,val, textCharFormat);
					else
						tableCell.set(row,17,"-", textCharFormat);
					//"Всього"
					allWorkersAllNarahSum += workerNarahSum;
					tableCell.set(row,18,workerNarahSum, textCharFormat);
				}
				else{
					//Вислуга років
					tableCell.set(row,5,"-", textCharFormat);
					//"Лікарн."
					tableCell.set(row,6,"-", textCharFormat);	
					//"Відпуск"
					tableCell.set(row,7,"-", textCharFormat);
					//"Святк."
					tableCell.set(row,8,"-", textCharFormat);
					//"Нічні"
					tableCell.set(row,9,"-", textCharFormat);
					//"Надурочн."
					tableCell.set(row,10,"-", textCharFormat);
					//"Класн.\nвод."
					tableCell.set(row,11,"-", textCharFormat);
					//"Премія"
					tableCell.set(row,12,"-", textCharFormat);
					//"Матер.\nдоп."
					tableCell.set(row,13,"-", textCharFormat);
					//"Доплата"
					tableCell.set(row,14,"-", textCharFormat);
					//"Індексація"
					tableCell.set(row,15,"-", textCharFormat);
					//"Випл.\nпрофсп."
					tableCell.set(row,16,"-", textCharFormat);
					//"Ін.\nнарах."
					tableCell.set(row,17,"-", textCharFormat);
					//"Всього"
					allWorkersAllNarahSum += workerNarahSum;
					tableCell.set(row,18,workerNarahSum, textCharFormat);
				}
			
				row++;
				postupForm->inc_pos();
			}
		}
			//В тому числі з ОД = 01 ----------------------------
		table->mergeCells(row,0,1,2);
		
		//надпис в т.ч. нарахув. з ОД = 01"
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"Нарахув. з ОД = 01", textCharFormat);
		
		//Ознака доходу
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,3,"1", textCharFormat);
		
		//Основне нарахування
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,4,N_Osn_narah_Sum-N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
		
		//Вислуга років
		if (N_VuslRokiv_Sum != 0)
			tableCell.set(row,5,N_VuslRokiv_Sum, textCharFormat);
		else
			tableCell.set(row,5,"-", textCharFormat);
		//"Лікарн."
		if (N_Likarn_Sum != 0)
			tableCell.set(row,6,N_Likarn_Sum, textCharFormat);
		else
			tableCell.set(row,6,"-", textCharFormat);
		//"Відпуск"
		if (N_Vidpuskni_Sum != 0)
			tableCell.set(row,7,N_Vidpuskni_Sum, textCharFormat);
		else
			tableCell.set(row,7,"-", textCharFormat);
		//"Святк."
		if (N_Svatkovi_dni_Sum != 0)
			tableCell.set(row,8,N_Svatkovi_dni_Sum, textCharFormat);
		else
			tableCell.set(row,8,"-", textCharFormat);
		//"Нічні"
		if (N_Nichni_Sum != 0)
			tableCell.set(row,9,N_Nichni_Sum, textCharFormat);
		else
			tableCell.set(row,9,"-", textCharFormat);
		//"Надурочн."
		if (N_Nadurochni_Sum != 0)
			tableCell.set(row,10,N_Nadurochni_Sum, textCharFormat);
		else
			tableCell.set(row,10,"-", textCharFormat);
		//"Класн.\nвод."
		if (N_Klasn_vodiya_Sum != 0)
			tableCell.set(row,11,N_Klasn_vodiya_Sum, textCharFormat);
		else
			tableCell.set(row,11,"-", textCharFormat);
		//"Премія"
		if (N_Premia_Sum != 0)
			tableCell.set(row,12,N_Premia_Sum, textCharFormat);
		else
			tableCell.set(row,12,"-", textCharFormat);
		//"Матер.\nдоп."
		if (N_MaterDopomoga_Sum != 0)
			tableCell.set(row,13,N_MaterDopomoga_Sum, textCharFormat);
		else
			tableCell.set(row,13,"-", textCharFormat);
		//"Доплата"
		if (N_Doplata_Sum != 0)
			tableCell.set(row,14,N_Doplata_Sum, textCharFormat);
		else
			tableCell.set(row,14,"-", textCharFormat);
		//"Індексація"
		if (N_Doplata_Sum != 0)
			tableCell.set(row,15,N_Indexaciya_Sum, textCharFormat);
		else
			tableCell.set(row,15,"-", textCharFormat);
		//"Випл.\nпрофсп."
		if (N_VyplProfspilky_Sum != 0)
			tableCell.set(row,16,N_VyplProfspilky_Sum, textCharFormat);
		else
			tableCell.set(row,16,"-", textCharFormat);
		//"Ін.\nнарах."
		if (N_InshiNarahuvannya_Sum != 0)
			tableCell.set(row,17,N_InshiNarahuvannya_Sum, textCharFormat);
		else
			tableCell.set(row,17,"-", textCharFormat);
		//"Всього"
		tableCell.set(row,18,allWorkersAllNarahSum-N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
		
		//В тому числі з ОД > 01 ----------------------------
		row++;
		table->mergeCells(row,0,1,2);
	
		//надпис в т.ч. нарахув. з ОД = 01"
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"Нарахув. з ОД > 01", textCharFormat);
	
		//Ознака доходу
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,3,">1", textCharFormat);
	
		//Основне нарахування
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,4,N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
	
		//Вислуга років
		tableCell.set(row,5,"-", textCharFormat);
		//"Лікарн."
		tableCell.set(row,6,"-", textCharFormat);
		//"Відпуск"
		tableCell.set(row,7,"-", textCharFormat);
		//"Святк."
		tableCell.set(row,8,"-", textCharFormat);
		//"Нічні"
		tableCell.set(row,9,"-", textCharFormat);
		//"Надурочн."
		tableCell.set(row,10,"-", textCharFormat);
		//"Класн.\nвод."
		tableCell.set(row,11,"-", textCharFormat);
		//"Премія"
		tableCell.set(row,12,"-", textCharFormat);
		//"Матер.\nдоп."
		tableCell.set(row,13,"-", textCharFormat);
		//"Доплата"
		tableCell.set(row,14,"-", textCharFormat);
		//"Індексація"
		tableCell.set(row,15,"-", textCharFormat);
		//"Випл.\nпрофсп."
		tableCell.set(row,16,"-", textCharFormat);
		//"Ін.\nнарах."
		tableCell.set(row,17,"-", textCharFormat);
		//"Всього"
		tableCell.set(row,18,N_Osn_Narah_Sum_Other_Dohid, textCharFormat);
	}
	
	delete nprQuery;
	delete query;
	delete postupForm;
	printform->printer()->setDocName("Нарахування за "+QString::number(month)+"."+QString::number(year)+"р.");
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
	
	printform = new UPrintForm(0, "Картки по зарплаті за " + monthlist[month-1], true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	printform->document()->clear();
	QTextCursor cellCursor(printform->document());
	
	//Створення таблиці
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
	printform->printer()->setDocName("Картки по зарплаті працівників за "+QString::number(month)+"."+QString::number(year)+"р.");
	printform->printer()->setPageMargins ( 0.5, 0.5, 0.5, 0.5, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_cur_worker_card(int year, int month, int workerId)
{
	printform = new UPrintForm(0, "Картка по зарплаті за " + monthlist[month-1],true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cellCursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	
	//Створення таблиці
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
	
	printform->printer()->setDocName("Картка по зарплаті за "+QString::number(month)+"."+QString::number(year)+"р.");
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
	
	printform = new UPrintForm(0, "Річні картки по зарплаті за " + QString::number(year) + " рік",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
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
	
	printform->printer()->setDocName("Річні картки по зарплаті за "+QString::number(year) + " рік."+QString::number(year)+"р.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_year_cur_worker_card(int year, int workerId)
{
	printform = new UPrintForm(0, "Річна картка по зарплаті за " + QString::number(year) + " рік",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertFragment(createYearCardFragment(workerId, year));
	
	printform->printer()->setDocName("Річна картка по зарплаті за "+QString::number(year) + " рік."+QString::number(year)+"р.");
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
	
	printform = new UPrintForm(0, "Річні картки по зарплаті за " + QString::number(year) + " рік",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
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
	
	printform->printer()->setDocName("Річні картки по зарплаті за "+QString::number(year) + " рік."+QString::number(year)+"р.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPrintDocs::print_year_utrymannya_cur_worker_card(int year, int workerId)
{
	printform = new UPrintForm(0, "Річна картка по зарплаті за " + QString::number(year) + " рік",true);
	printform->printer()->setOrientation( QPrinter::Landscape );
	printform->document()->clear();
	QTextCursor cursor(printform->document());
	
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat;
	textCharFormat.setFontPointSize(8);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertFragment(createYearUtrymannyaCardFragment(workerId, year));
	
	printform->printer()->setDocName("Річна картка по зарплаті за "+QString::number(year) + " рік."+QString::number(year)+"р.");
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
	
	printform = new UPrintForm(0, "Відомість нарахованих утримань за " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//Шапка документа
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
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ НАРАХОВАНИХ УТРИМАНЬ ІЗ ЗАРПЛАТИ ПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("за ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("р."), textCharFormat);
	//Створення таблиці
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
		cursor.insertText("	Підрозділ: "+pQuery.value(1).toString(),textCharFormat_bold);
	
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
		//Заповнення шапки таблиці
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
		tableCell.set(0,0,"№ п/п", textCharFormat);
		tableCell.set(0,1,"П І Б", textCharFormat);
		tableCell.set(0,2,"ОД", textCharFormat);
		tableCell.set(0,3,"Сума Нарах.", textCharFormat);
		tableCell.set(0,4,"Пенс.фонд", textCharFormat);
		tableCell.set(0,5,"Фонд.\nзайн.", textCharFormat);
		tableCell.set(0,6,"Соцстрах", textCharFormat);
		tableCell.set(0,7,"Соцпільга", textCharFormat);
		tableCell.set(0,8,"Прибутк.под.", textCharFormat);
		tableCell.set(0,9,"Утр.профсп.", textCharFormat);
		tableCell.set(0,10,"Інші", textCharFormat);
		tableCell.set(0,11,"Всього\nУТР.ОД.", textCharFormat);
		tableCell.set(0,12,"Всього\nНА РУКИ", textCharFormat);
		tableCell.set(0,13,"Всього\nУТР.Прац.", textCharFormat);

		//Заповнення таблиці
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
			
				//№ п/п
				tableCell.setAlignment(Qt::AlignCenter);
				if (oznakaDohodu == 1)
					tableCell.set(row,0,QString::number(++workersCounter), textCharFormat);
			
				if (firstWorkerRow){
					//П І Б
					tableCell.setAlignment(Qt::AlignLeft);
					tableCell.set(row,1,nprQuery->value(2).toString().toUpper() +" "+nprQuery->value(3).toString() +" "+nprQuery->value(4).toString(), textCharFormat);

					firstWorkerRow = false;
				}
			
				//Ознака доходу
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,2,oznakaDohodu, textCharFormat);
			
				//"Сума Нарах."
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
			
				//"Пенс.фонд"
				val = query->value(1).toDouble();
				U_PensFond_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,4,val, textCharFormat);
			
				//"Фонд.\nзайн."
				val = query->value(2).toDouble();
				U_Fond_zainat_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,5,val, textCharFormat);
			
				//"Соцстрах"
				val = query->value(3).toDouble();
				U_SocStrah_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,6,val, textCharFormat);
			
				//"Соцпільга"
				val = query->value(4).toDouble();
				U_SocPilga_Sum += val;
				tableCell.setDash(row,7,val, textCharFormat);
			
				//"Прибутк.под."
				val = query->value(5).toDouble();
				U_Prybutkov_podatok_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,8,val, textCharFormat);
			
				//"Утр.профсп."
				val = query->value(6).toDouble();
				curODUtrymSum += val;
				U_VidrahProfspilka_Sum += val;
				tableCell.setDash(row,9,val, textCharFormat);
				
				//"Інші"
				val = query->value(7).toDouble();
				U_InshiVidrahuvannya_Sum += val;
				curODUtrymSum += val;
				tableCell.setDash(row,10,val, textCharFormat);
			
				//"Всього\nУТР.ОД."
				val = curODUtrymSum;
				workerUtrymSum += curODUtrymSum;
				allUtrymSum += curODUtrymSum;
				tableCell.setDash(row,11,val, textCharFormat);
			
				//"Всього\nНА РУКИ"
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
					//"На руки"
					val = naRukyNoOdMore01;
					allWorkersNaRukySum += val;
					tableCell.setDash(row-operCount+1,12,val, textCharFormat);
				}
			
				if (curOdRow == operCount){
					//"Всього\nУТР.Прац."
					val = workerUtrymSum;
					tableCell.setDash(row-operCount+1,13,val, textCharFormat);
				}
			
				row++;
				postupForm->inc_pos();
			}
		}
		//Заповнення стрічки "Всього"
		table->mergeCells(row,0,1,2);
	
		//Всього
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"Всього", textCharFormat);
	
		//Ознака доходу
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,2,"-", textCharFormat);
	
		//"Сума Нарах."
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.setDash(row,3,allNarahSum, textCharFormat);
			
		//"Пенс.фонд"
		tableCell.setDash(row,4,U_PensFond_Sum, textCharFormat);
	
		//"Фонд.\nзайн."
		tableCell.setDash(row,5,U_Fond_zainat_Sum, textCharFormat);
			
		//"Соцстрах"
		tableCell.setDash(row,6,U_SocStrah_Sum, textCharFormat);
			
		//"Соцпільга"
		tableCell.setDash(row,7,U_SocPilga_Sum, textCharFormat);
			
		//"Прибутк.под."
		tableCell.setDash(row,8,U_Prybutkov_podatok_Sum, textCharFormat);
			
		//"Утр.профсп."
		tableCell.setDash(row,9,U_VidrahProfspilka_Sum, textCharFormat);
			
		//"Інші"
		tableCell.setDash(row,10,U_InshiVidrahuvannya_Sum, textCharFormat);
			
		//"Всього\nУТР.ОД."
		tableCell.setDash(row,11,allUtrymSum, textCharFormat);
	
		//"Всього\nНА РУКИ"
		tableCell.setDash(row,12,allWorkersNaRukySum, textCharFormat);
			
		//"Всього\nУТР.Прац."
		tableCell.setDash(row,13,allUtrymSum, textCharFormat);
	
	}
	
	delete nprQuery;
	delete query;
	delete tQuery;
	delete postupForm;
	printform->printer()->setDocName("Утримання за "+QString::number(month)+"."+QString::number(year)+"р.");
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
	
	printform = new UPrintForm(0, "Відомість відрахувань на зарплату за " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//Шапка документа
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
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ ВІДРАХУВАНЬ НА ЗАРПЛАТУ ПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("за ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("р."), textCharFormat);
	//Створення таблиці
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
		cursor.insertText("	Підрозділ: "+pQuery.value(1).toString(),textCharFormat_bold);
	
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
		//Заповнення шапки таблиці
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
		tableCell.set(0,0,"№ п/п", textCharFormat);
		tableCell.set(0,1,"П І Б", textCharFormat);
		tableCell.set(0,2,"ОД", textCharFormat);
		tableCell.set(0,3,"Сума Нарах.", textCharFormat);
		tableCell.set(0,4,"Пенс.фонд", textCharFormat);
		tableCell.set(0,5,"Соцстрах", textCharFormat);
		tableCell.set(0,6,"Фонд.\nзайн.", textCharFormat);
		tableCell.set(0,7,"Нещ.вип.", textCharFormat);
		tableCell.set(0,8,"Інші.", textCharFormat);
		tableCell.set(0,9,"Сума ВІДР.ОД", textCharFormat);
		tableCell.set(0,10,"Всього ВІДР Прац.", textCharFormat);
	
		//Заповнення таблиці
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
			
				//№ п/п
				tableCell.setAlignment(Qt::AlignCenter);
				if (oznakaDohodu == 1)
					tableCell.set(row,0,QString::number(++workersCounter), textCharFormat);
			
				if (firstWorkerRow){
					//П І Б
					tableCell.setAlignment(Qt::AlignLeft);
					tableCell.set(row,1,nprQuery->value(2).toString().toUpper() +" "+nprQuery->value(3).toString() +" "+nprQuery->value(4).toString(), textCharFormat);

					firstWorkerRow = false;
				}
			
				//Ознака доходу
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,2,oznakaDohodu, textCharFormat);
			
				//"Сума Нарах."
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
			
				//"Пенс.фонд"
				val = query->value(1).toDouble();
				V_PensFond_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,4,val, textCharFormat);
				else
					tableCell.set(row,4,"-", textCharFormat);
			
				//"Соцстрах"
				val = query->value(2).toDouble();
				V_SocStrah_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,5,val, textCharFormat);
				else
					tableCell.set(row,5,"-", textCharFormat);
			
				//"Фонд.\nзайн."
				val = query->value(3).toDouble();
				V_Fond_zainat_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,6,val, textCharFormat);
				else
					tableCell.set(row,6,"-", textCharFormat);
			
				//"Нещ.вип."
				val = query->value(4).toDouble();
				V_NeshchVyp_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,7,val, textCharFormat);
				else
					tableCell.set(row,7,"-", textCharFormat);
			
				//"Інші."
				val = query->value(5).toDouble();
				V_Inshi_Sum += val;
				curODVidrahSum += val;
				if (val != 0)
					tableCell.set(row,8,val, textCharFormat);
				else
					tableCell.set(row,8,"-", textCharFormat);
			
				//"Сума ВІДР.ОД"
				workerVidrahSum += curODVidrahSum;
				if (curODVidrahSum != 0)
					tableCell.set(row,9,curODVidrahSum, textCharFormat);
				else
					tableCell.set(row,9,"-", textCharFormat);
			
				if (curOdRow == operCount){
					//"Всього ВІДР Прац."
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
		//Заповнення стрічки "Всього"
		table->mergeCells(row,0,1,2);
	
		//Всього
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"Всього", textCharFormat);
	
		//Ознака доходу
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,2,"-", textCharFormat);
	
		//"Сума Нарах."
		tableCell.setAlignment(Qt::AlignRight);
		if (allNarahSum != 0)
			tableCell.set(row,3,allNarahSum, textCharFormat);
		else
			tableCell.set(row,3,"-", textCharFormat);
			
		//"Пенс.фонд"
		if (V_PensFond_Sum != 0)
			tableCell.set(row,4,V_PensFond_Sum, textCharFormat);
		else
			tableCell.set(row,4,"-", textCharFormat);
	
		//"Соцстрах"
		if (V_SocStrah_Sum != 0)
			tableCell.set(row,5,V_SocStrah_Sum, textCharFormat);
		else
			tableCell.set(row,5,"-", textCharFormat);
			
		//"Фонд.\nзайн."
		if (V_Fond_zainat_Sum != 0)
			tableCell.set(row,6,V_Fond_zainat_Sum, textCharFormat);
		else
			tableCell.set(row,6,"-", textCharFormat);
			
		//Нещасні випадки
		if (V_NeshchVyp_Sum != 0)
			tableCell.set(row,7,V_NeshchVyp_Sum, textCharFormat);
		else
			tableCell.set(row,7,"-", textCharFormat);
			
		//Інші
		if (V_Inshi_Sum != 0)
			tableCell.set(row,8,V_Inshi_Sum, textCharFormat);
			else
		tableCell.set(row,8,"-", textCharFormat);
			
		//Всього
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
	printform->printer()->setDocName("Відрахування за "+QString::number(month)+"."+QString::number(year)+"р.");
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
	
	printform = new UPrintForm(0, "Відомість оплати/заборгованості за " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//Шапка документа
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
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ ОПЛАТИ/ЗАБОРГОВАНОСТІ ПО ЗАРПЛАТІ ПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("за ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("р."), textCharFormat);
	//Створення таблиці
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
		cursor.insertText("	Підрозділ: "+pQuery.value(1).toString(),textCharFormat_bold);
	
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
		//Заповнення шапки таблиці
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
		tableCell.set(0,0,"№ п/п", textCharFormat);
		tableCell.set(0,1,"П І Б", textCharFormat);
		tableCell.set(0,2,"Вх.\nСальдо.", textCharFormat);
		tableCell.set(0,3,"Нар.\nОД=1.", textCharFormat);
		tableCell.set(0,4,"Нар.\nОД>1.", textCharFormat);
		tableCell.set(0,5,"Утр.\nОД=1", textCharFormat);
		tableCell.set(0,6,"Утр.\nОД>1", textCharFormat);
		tableCell.set(0,7,"Сума\nУТР", textCharFormat);
		tableCell.set(0,8,"На\nруки", textCharFormat);
		tableCell.set(0,9,"Інша опл.", textCharFormat);
		tableCell.set(0,10,"Штраф", textCharFormat);
		tableCell.set(0,11,"Алім.", textCharFormat);
	
		tableCell.set(0,12,"Попер.\nміс.", textCharFormat);
		tableCell.set(0,13,"Премія.", textCharFormat);
	
		tableCell.set(0,14,"Аванс", textCharFormat);
		tableCell.set(0,15,"Видано", textCharFormat);
		tableCell.set(0,16,"Вих.\nСальдо.", textCharFormat);
	
		//Заповнення таблиці
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
		
			//№ п/п
			tableCell.setAlignment(Qt::AlignCenter);
			tableCell.set(row,0,QString::number(row), textCharFormat);
			
			//П І Б
			tableCell.setAlignment(Qt::AlignLeft);
			tableCell.set(row,1,nprQuery.value(1).toString().toUpper() +" "+nprQuery.value(2).toString(), textCharFormat);
		
			query.exec("SELECT Vh_saldo, O_Inshi, O_Shtraf, O_Alimenty, O_ZaPoperMis, O_Premiya, O_Avans, O_Oplacheno \
					FROM operacii \
					WHERE Npr_id="+mysqlStr(nprId)+" and \
							OznakaDohodu=1 and \
							year(CurDate)="+mysqlStr(year)+" and \
							month(CurDate)="+mysqlStr(month)+" LIMIT 1");
			query.seek(0);
		
			//"Вх.\nСальдо."
			tableCell.setAlignment(Qt::AlignRight);
			val = query.value(0).toDouble();
			WorkerVhSaldoSum = val;
			allVhSaldo += val;
			tableCell.set(row,2,val, textCharFormat);
		
			//"Інші."
			val = query.value(1).toDouble();
			WorkerVyhSaldoSum -= val;
			allInshiSum += val;
			tableCell.setDash(row,9,val, textCharFormat);
			
			//"Штраф"
			val = query.value(2).toDouble();
			WorkerVyhSaldoSum -= val;
			allShtrafSum += val;
			tableCell.setDash(row,10,val, textCharFormat);
		
			//"Алім."
			val = query.value(3).toDouble();
			WorkerVyhSaldoSum -= val;
			allAlimentySum +=val;
			tableCell.setDash(row,11,val, textCharFormat);
		
			//"За попер міс."
			val = query.value(4).toDouble();
			WorkerVyhSaldoSum -= val;
			allPoperMonthSum +=val;
			tableCell.setDash(row,12,val, textCharFormat);
		
			//"Премія"
			val = query.value(5).toDouble();
			WorkerVyhSaldoSum -= val;
			allPremiyaSum +=val;
			tableCell.setDash(row,13,val, textCharFormat);
			
			//"Аванс"
			val = query.value(6).toDouble();
			WorkerVyhSaldoSum -= val;
			allAvansSum += val;
			tableCell.setDash(row,14,val, textCharFormat);
		
			//"Видано"
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
		
			//"Нар.\nОД=1."
			val = query.value(0).toDouble();
			WorkerNarahSum = val;
			WorkerNarahOD01Sum = val;
			allNarah01Sum += val;
			tableCell.setDash(row,3,val, textCharFormat);
		
			//Утр.\nОД=1
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
		
			//"Нар.\nОД>1."
			val = query.value(0).toDouble();
			//WorkerNarahSum += val;
			allNarah02Sum += val;
			tableCell.setDash(row,4,val, textCharFormat);
		
			//Утр.\nОД>1
			val = query.value(1).toDouble();
			WorkerUtrymSum += val;
			allUtrymSum += val;
			allUtrym02Sum += val;
			tableCell.setDash(row,6,val, textCharFormat);
		
			//"Сума\nУТР"
			val = WorkerUtrymSum;
			tableCell.setDash(row,7,val, textCharFormat);
		
			//На руки
			val = WorkerNarahOD01Sum + WorkerNaRukyOtherDohid - WorkerUtrymSum;
			allNaRukySum += val;
			tableCell.setDash(row,8,val, textCharFormat);
		
			//Вихідне сальдо
			val = WorkerVyhSaldoSum += WorkerVhSaldoSum + WorkerNarahOD01Sum + WorkerNaRukyOtherDohid - WorkerUtrymSum;
			allVyhSaldo += val;
			tableCell.set(row,16,val, textCharFormat);
		
			postupForm->inc_pos();
		}
		//Заповнення стрічки "Всього"
		table->mergeCells(row,0,1,2);
	
		//Всього
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.set(row,0,"Всього", textCharFormat);
	
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
	printform->printer()->setDocName("Оплата/заборгованість за "+QString::number(month)+"."+QString::number(year)+"р.");
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
	
	printform = new UPrintForm(0, "Нарахування і відрахування в розрізі БухРахунків за " + monthlist[month-1]);
	printform->printer()->setOrientation( QPrinter::Landscape );
	//Шапка документа
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
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ НАРАХУВАННЯ В РОЗРІЗІ БУХРАХУНКІВ\nПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("за ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("р."), textCharFormat);
	//Створення таблиці
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
	
	//Заповнення шапки таблиці
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
	
	tableCell.set(0,0,"№ п/п", textCharFormat);
	tableCell.set(0,1,"Бух. Рах.", textCharFormat);
	tableCell.set(0,2,"К-сть", textCharFormat);
	tableCell.set(0,3,"Осн.\nнарах.", textCharFormat);
	tableCell.set(0,4,"Вислуга", textCharFormat);
	tableCell.set(0,5,"Лікарн.", textCharFormat);
	tableCell.set(0,6,"Відпуск", textCharFormat);
	tableCell.set(0,7,"Святк.", textCharFormat);
	tableCell.set(0,8,"Нічні", textCharFormat);
	tableCell.set(0,9,"Надурочн.", textCharFormat);
	tableCell.set(0,10,"Класн.\nвод.", textCharFormat);
	tableCell.set(0,11,"Премія", textCharFormat);
	tableCell.set(0,12,"Матер.\nдоп.", textCharFormat);
	tableCell.set(0,13,"Доплата", textCharFormat);
	tableCell.set(0,14,"Індекс.", textCharFormat);
	tableCell.set(0,15,"Випл.\nпрофсп.", textCharFormat);
	tableCell.set(0,16,"Ін.\nнарах.", textCharFormat);
	tableCell.set(0,17,"Всього\nнарах.", textCharFormat);
	
	//Заповнення таблиці
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
		//"№ п/п"
		tableCell.set(row,0,row, textCharFormat);
		//"Бух. Рах."
		tableCell.setDash(row,1,query->value(0).toInt(), textCharFormat);
		//"К-сть"
		iVal = query->value(1).toInt();
		pracCount += iVal;
		tableCell.set(row,2,iVal, textCharFormat);
		
		//"Осн.\nнарах."
		dVal = query->value(2).toDouble();
		curNarahSum = dVal;
		N_Osn_narah_Sum += dVal;
		tableCell.setDash(row,3,dVal, textCharFormat);
		//"Вислуга"
		dVal = query->value(3).toDouble();
		curNarahSum += dVal;
		N_VuslRokiv_Sum += dVal;
		tableCell.setDash(row,4,dVal, textCharFormat);
		//"Лікарн."
		dVal = query->value(4).toDouble();
		curNarahSum += dVal;
		N_Likarn_Sum += dVal;
		tableCell.setDash(row,5,dVal, textCharFormat);
		//"Відпуск"
		dVal = query->value(5).toDouble();
		curNarahSum += dVal;
		N_Vidpuskni_Sum += dVal;
		tableCell.setDash(row,6,dVal, textCharFormat);
		//"Святк."
		dVal = query->value(6).toDouble();
		curNarahSum += dVal;
		N_Svatkovi_dni_Sum += dVal;
		tableCell.setDash(row,7,dVal, textCharFormat);
		//"Нічні"
		dVal = query->value(7).toDouble();
		curNarahSum += dVal;
		N_Nichni_Sum += dVal;
		tableCell.setDash(row,8,dVal, textCharFormat);
		//"Надурочн."
		dVal = query->value(8).toDouble();
		curNarahSum += dVal;
		N_Nadurochni_Sum += dVal;
		tableCell.setDash(row,9,dVal, textCharFormat);
		//"Класн.\nвод."
		dVal = query->value(9).toDouble();
		curNarahSum += dVal;
		N_Klasn_vodiya_Sum += dVal;
		tableCell.setDash(row,10,dVal, textCharFormat);
		//"Премія"
		dVal = query->value(10).toDouble();
		curNarahSum += dVal;
		N_Premia_Sum += dVal;
		tableCell.setDash(row,11,dVal, textCharFormat);
		//"Матер.\nдоп."
		dVal = query->value(11).toDouble();
		curNarahSum += dVal;
		N_MaterDopomoga_Sum += dVal;
		tableCell.setDash(row,12,dVal, textCharFormat);
		//"Доплата"
		dVal = query->value(12).toDouble();
		curNarahSum += dVal;
		N_Doplata_Sum += dVal;
		tableCell.setDash(row,13,dVal, textCharFormat);
		//"Індексація"
		dVal = query->value(13).toDouble();
		curNarahSum += dVal;
		N_Index_Sum += dVal;
		tableCell.setDash(row,14,dVal, textCharFormat);
		//"Випл.\nпрофсп."
		dVal = query->value(14).toDouble();
		curNarahSum += dVal;
		N_VyplProfspilky_Sum += dVal;
		tableCell.setDash(row,15,dVal, textCharFormat);
		//"Ін.\nнарах."
		dVal = query->value(15).toDouble();
		curNarahSum += dVal;
		N_InshiNarahuvannya_Sum += dVal;
		tableCell.setDash(row,16,dVal, textCharFormat);
		//"Всього.\nнарах."
		allNarahSum += curNarahSum;
		tableCell.setDash(row,17,curNarahSum, textCharFormat);
		
		postupForm->inc_pos();
	}
	
	//Заповнення стрічки "Всього"
	table->mergeCells(row,0,1,2);
	
	//Всього
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(row,0,"Всього", textCharFormat);
	
	//"К-сть"
	tableCell.set(row,2,pracCount, textCharFormat);
		
	//"Осн.\nнарах."
	tableCell.setDash(row,3,N_Osn_narah_Sum, textCharFormat);
	//"Вислуга"
	tableCell.setDash(row,4,N_VuslRokiv_Sum, textCharFormat);
	//"Лікарн."
	tableCell.setDash(row,5,N_Likarn_Sum, textCharFormat);
	//"Відпуск"
	tableCell.setDash(row,6,N_Vidpuskni_Sum, textCharFormat);
	//"Святк."
	tableCell.setDash(row,7,N_Svatkovi_dni_Sum, textCharFormat);
	//"Нічні"
	tableCell.setDash(row,8,N_Nichni_Sum, textCharFormat);
	//"Надурочн."
	tableCell.setDash(row,9,N_Nadurochni_Sum, textCharFormat);
	//"Класн.\nвод."
	tableCell.setDash(row,10,N_Klasn_vodiya_Sum, textCharFormat);
	//"Премія"
	tableCell.setDash(row,11,N_Premia_Sum, textCharFormat);
	//"Матер.\nдоп."
	tableCell.setDash(row,12,N_MaterDopomoga_Sum, textCharFormat);
	//"Доплата"
	tableCell.setDash(row,13,N_Doplata_Sum, textCharFormat);
	//"Індексація"
	tableCell.setDash(row,14,N_Index_Sum, textCharFormat);
	//"Випл.\nпрофсп."
	tableCell.setDash(row,15,N_VyplProfspilky_Sum, textCharFormat);
	//"Ін.\nнарах."
	tableCell.setDash(row,16,N_InshiNarahuvannya_Sum, textCharFormat);
	//"Всього.\nнарах."
	tableCell.setDash(row,17,allNarahSum, textCharFormat);
		
	
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ ВІДРАХУВАННЯ В РОЗРІЗІ БУХРАХУНКІВ\nПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 9 );
	cursor.insertText(("за ") + monthlist.at(month-1) +
						(" ") + QVariant(year).toString() + ("р."), textCharFormat);
	//Створення таблиці
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
	
	//Заповнення шапки таблиці
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
	
	tableCell.set(0,0,"№ п/п", textCharFormat);
	tableCell.set(0,1,"Бух. Рах.", textCharFormat);
	tableCell.set(0,2,"К-сть", textCharFormat);
	tableCell.set(0,3,"Пенс.фонд", textCharFormat);
	tableCell.set(0,4,"Соцстрах", textCharFormat);
	tableCell.set(0,5,"Фонд.\nзайн.", textCharFormat);
	tableCell.set(0,6,"Нещ.вип.", textCharFormat);
	tableCell.set(0,7,"Інші.", textCharFormat);
	tableCell.set(0,8,"Всього\nвідр.", textCharFormat);
	
	//Заповнення таблиці
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
		//"№ п/п"
		tableCell.set(row,0,row, textCharFormat);
		//"Бух. Рах."
		tableCell.setDash(row,1,query->value(0).toInt(), textCharFormat);
		//"К-сть"
		iVal = query->value(1).toInt();
		pracCount += iVal;
		tableCell.set(row,2,iVal, textCharFormat);
		
		//"Пенс.фонд"
		dVal = query->value(16).toDouble();
		curVidrahSum = dVal;
		V_PensFond += dVal;
		tableCell.setDash(row,3,dVal, textCharFormat);
		//"Соцстрах"
		dVal = query->value(17).toDouble();
		curVidrahSum += dVal;
		V_SocStrah += dVal;
		tableCell.setDash(row,4,dVal, textCharFormat);
		//"Фонд.\nзайн."
		dVal = query->value(18).toDouble();
		curVidrahSum += dVal;
		V_Fond_zainat += dVal;
		tableCell.setDash(row,5,dVal, textCharFormat);
		//"Нещ.вип."
		dVal = query->value(19).toDouble();
		curVidrahSum += dVal;
		V_NeshchVyp += dVal;
		tableCell.setDash(row,6,dVal, textCharFormat);
		//"Інші."
		dVal = query->value(20).toDouble();
		curVidrahSum += dVal;
		V_Inshi += dVal;
		tableCell.setDash(row,7,dVal, textCharFormat);
		//"Всього відрахування"
		allVidrahSum += curVidrahSum;
		tableCell.setDash(row,8,curVidrahSum, textCharFormat);
			
		postupForm->inc_pos();
	}
	
	//Заповнення стрічки "Всього"
	table->mergeCells(row,0,1,2);
	
	//Всього
	tableCell.setAlignment(Qt::AlignRight);
	tableCell.set(row,0,"Всього", textCharFormat);
	
	//"К-сть"
	tableCell.set(row,2,pracCount, textCharFormat);
	
	//"Пенс.фонд"
	tableCell.setDash(row,3,V_PensFond, textCharFormat);
	//"Соцстрах"
	tableCell.setDash(row,4,V_SocStrah, textCharFormat);
	//"Фонд.\nзайн."
	tableCell.setDash(row,5,V_Fond_zainat, textCharFormat);
	//"Нещ.вип."
	tableCell.setDash(row,6,V_NeshchVyp, textCharFormat);
	//"Інші."
	tableCell.setDash(row,7,V_Inshi, textCharFormat);
	//"Всього відрахування"
	tableCell.setDash(row,8,allVidrahSum, textCharFormat);
	
	
	// Друк відсотків УТРИМАНЬ єдиного внеску
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ УТРИМАНЬ ЄДИНОГО ВНЕСКУ В РОЗРІЗІ ВІДСОТКІВ\nПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	//Створення таблиці
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
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 9 );
	textCharFormat_bold.setFontPointSize( 9 );
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"Відсоток, %", textCharFormat);
	tableCell.set(0,1,"К-сть прац.", textCharFormat);
	tableCell.set(0,2,"Сума, грн.", textCharFormat);
	
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
	//Підсумок
	tableCell.set(row,0,"Разом", textCharFormat_bold);
	tableCell.set(row,1,workersCount, textCharFormat_bold);
	tableCell.set(row,2,uMToStr2(salSum), textCharFormat_bold);
	
	
	// Друк відсотків НАРАХУВАНЬ єдиного внеску
	cursor.movePosition(QTextCursor::End);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat.setFontPointSize( 10 );
	cursor.insertText("ВІДОМІСТЬ НАРАХУВАНЬ ЄДИНОГО ВНЕСКУ В РОЗРІЗІ ВІДСОТКІВ\nПО " + OrganizShortName(), textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	//Створення таблиці
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
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 9 );
	textCharFormat_bold.setFontPointSize( 9 );
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	tableCell.setAlignment(Qt::AlignCenter);
	tableCell.set(0,0,"Відсоток, %", textCharFormat);
	tableCell.set(0,1,"К-сть прац.", textCharFormat);
	tableCell.set(0,2,"Сума, грн.", textCharFormat);
	
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
	//Підсумок
	tableCell.set(row,0,"Разом", textCharFormat_bold);
	tableCell.set(row,1,workersCount, textCharFormat_bold);
	tableCell.set(row,2,uMToStr2(salSum), textCharFormat_bold);
	
	
	delete postupForm;
	delete query;
	printform->printer()->setDocName("Нарах. і відрах. в розрізі контрагентів за "+QString::number(month)+"."+QString::number(year)+"р.");
	printform->printer()->setPageMargins ( 5, 5, 5, 10, QPrinter::Millimeter );
	printform->show();
}
//--------------------utech--------------------utech--------------------utech--------------------
QTextDocumentFragment UPrintDocs::createWorkerCardFragment(int worker_id, int year, int month)
{
	QTextDocument * doc = new QTextDocument();
	QSqlQuery * query = new QSqlQuery();
	QSqlQuery * tQuery = new QSqlQuery();
	
	//Шапка документа
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
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 10 );
	cursor.insertText("Підприємство: " + OrganizShortName(), textCharFormat);
	blockFormat.setAlignment( Qt::AlignHCenter );
	cursor.insertBlock(blockFormat);
	textCharFormat_bold.setFontPointSize( 10 );
	cursor.insertText("КАРТКА ПО ЗАРПЛАТІ ЗА " + monthlist.at(month-1).toUpper() + " " + QString::number(year)+"р.", textCharFormat_bold);
	
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
	cursor.insertText("Працівник: " + query->value(42).toString().toUpper() + " " + str+"." + str2 + ". [" + str3 + "]", textCharFormat);
	
	//Створення таблиці ВИХІДНА ІНФОРМАЦІЯ
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
	
	//Заповнення таблиці ВИХІДНА ІНФОРМАЦІЯ
	//---
	cell = table->cellAt(0, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Посада",textCharFormat);
	
	//---
	cell = table->cellAt(1, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Тип роботи",textCharFormat);
	
	//---
	cell = table->cellAt(2, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Оплата",textCharFormat);
	
	//---
	cell = table->cellAt(3, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Оклад",textCharFormat);
	
	//---
	cell = table->cellAt(4, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Розряд",textCharFormat);
	
	//Посада
	cell = table->cellAt(0, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(query->value(46).toString(),textCharFormat);
	
	//Тип роботи
	cell = table->cellAt(1, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(29).toInt()==0)
		str = "Штат";
	else
		str = "Договір";
	cellCursor.insertText(str,textCharFormat);
	
	//Оплата
	cell = table->cellAt(2, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(30).toInt()==0)
		str = "Помісячна";
	else
		str = "Погодинна";
	cellCursor.insertText(str,textCharFormat);
	
	//Оклад
	cell = table->cellAt(3, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(query->value(31).toString()+"грн.",textCharFormat);
	
	//Розряд
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
	cellCursor.insertText("Ознака пільги",textCharFormat);
	
	//---
	cell = table->cellAt(1, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Класність водія",textCharFormat);
	
	//---
	cell = table->cellAt(2, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Доплата",textCharFormat);
	
	//---
	cell = table->cellAt(3, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Аліменти",textCharFormat);
	
	//---
	cell = table->cellAt(4, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Профспілка",textCharFormat);
	
	//Ознака пільги
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
	
	//Класність водія
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
	
	//Доплата
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
	
	//Аліменти
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
	
	//Профспілка
	cell = table->cellAt(4, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(37).toBool())
		str = "Так";
	else
		str = "-";
	cellCursor.insertText(str,textCharFormat);
	
	//Заголовок Табель
	table->mergeCells(5,0,1,4);
	cell = table->cellAt(5, 0);
	cell.setFormat(textCharFormat_bold);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Табель",textCharFormat_bold);
	
	//---
	cell = table->cellAt(6, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(30).toInt() == 0)
		str = "Загальні дні";
	else
		str = "Загальні год.";
	cellCursor.insertText(str,textCharFormat);
	
	//---
	cell = table->cellAt(7, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Нічні год.",textCharFormat);
	
	//---
	cell = table->cellAt(8, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Святкові год.",textCharFormat);
	
	//Загальні години/дні
	cell = table->cellAt(6, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//Загальний табель
		if (query->value(30).toInt() == 0)//Помісячна оплата
			tQuery->exec("SELECT count(*) \
						FROM tabel \
						WHERE Pracivn_status_id=1 \
							and Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
		else//погодинна оплата
			tQuery->exec("SELECT sum(God_zag) \
						FROM tabel \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
	}
	else{//Підсумковий табель
		if (query->value(30).toInt() == 0)//Помісячна оплата
			tQuery->exec("SELECT Work_days_sum \
							FROM tabel_sum \
							WHERE Npr_id="+QString::number(worker_id)+" \
								and year(Cur_date)="+QString::number(year)+" \
								and month(Cur_date)="+QString::number(month));
		else//погодинна оплата
			tQuery->exec("SELECT God_zag_sum \
						FROM tabel_sum \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
	}
	tQuery->seek(0);
	cellCursor.insertText(tQuery->value(0).toString(),textCharFormat);
	
	//Нічні
	cell = table->cellAt(7, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//Загальний табель
		if (query->value(30).toInt() == 0)//Помісячна оплата
			str = "-";
		else{//погодинна оплата
			tQuery->exec("SELECT sum(God_nichni) \
						FROM tabel \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
			tQuery->seek(0);
			str = tQuery->value(0).toString();
		}
	}
	else{//Підсумковий табель
		if (query->value(30).toInt() == 0)//Помісячна оплата
			str = "-";
		else{//погодинна оплата
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
	
	//Святкові
	cell = table->cellAt(8, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//Загальний табель
		if (query->value(30).toInt() == 0)//Помісячна оплата
			str = "-";
		else{//погодинна оплата
			tQuery->exec("SELECT sum(God_sv) \
						FROM tabel \
						WHERE Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
			tQuery->seek(0);
			str = tQuery->value(0).toString();
		}
	}
	else{//Підсумковий табель
		if (query->value(30).toInt() == 0)//Помісячна оплата
			str = "-";
		else{//погодинна оплата
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
	cellCursor.insertText("Лікарняні дні",textCharFormat);
	
	//---
	cell = table->cellAt(7, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Відпускні дні",textCharFormat);
	
	//Лікарняні
	cell = table->cellAt(6, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//Загальний табель
		tQuery->exec("SELECT count(*) \
						FROM tabel \
						WHERE Pracivn_status_id=2 \
							and Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
		tQuery->seek(0);
		str = tQuery->value(0).toString();
	}
	else{//Підсумковий табель
		tQuery->exec("SELECT Likarn_days_sum \
					FROM tabel_sum \
					WHERE Npr_id="+QString::number(worker_id)+" \
						and year(Cur_date)="+QString::number(year)+" \
						and month(Cur_date)="+QString::number(month));
		tQuery->seek(0);
		str = tQuery->value(0).toString();
	}
	cellCursor.insertText(str,textCharFormat);
	
	
	//Відпускні
	cell = table->cellAt(7, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	if (query->value(47).toInt() == 0){//Загальний табель
		tQuery->exec("SELECT count(*) \
						FROM tabel \
						WHERE Pracivn_status_id=3 \
							and Npr_id="+QString::number(worker_id)+" \
							and year(Cur_date)="+QString::number(year)+" \
							and month(Cur_date)="+QString::number(month));
		tQuery->seek(0);
		str = tQuery->value(0).toString();
	}
	else{//Підсумковий табель
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
	//cursor.insertText("ОПЕРАЦІЇ",textCharFormat_bold);
	
	//Створення таблиці операцій
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
	
	//Заповнення таблиці операцій
	cell = table->cellAt(0, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("ОПЕРАЦІЇ",textCharFormat_bold);
	
	//---
	cell = table->cellAt(0, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Вхідне сальдо",textCharFormat);
	
	//Вхідне сальдо
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
	cellCursor.insertText("Нарахування:",textCharFormat_bold);
	
	//---
	cell = table->cellAt(1, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Утримання:",textCharFormat_bold);
	
	//Заповнення показників нарахування
	//---
	cell = table->cellAt(2, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Основне нарахування",textCharFormat);
	
	//---
	cell = table->cellAt(3, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Вислуга років",textCharFormat);
	
	//---
	cell = table->cellAt(4, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Лікарняні",textCharFormat);
	
	//---
	cell = table->cellAt(5, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Відпускні",textCharFormat);
	
	//---
	cell = table->cellAt(6, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Святкові",textCharFormat);
	
	//---
	cell = table->cellAt(7, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Нічні",textCharFormat);
	
	//---
	cell = table->cellAt(8, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Надурочні",textCharFormat);
	
	//---
	cell = table->cellAt(9, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Класність водія",textCharFormat);
	
	//---
	cell = table->cellAt(10, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Премія",textCharFormat);
	
	//---
	cell = table->cellAt(11, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Матеріальна допомога",textCharFormat);
	
	//---
	cell = table->cellAt(12, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Доплата",textCharFormat);
	
	//---
	cell = table->cellAt(13, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Виплати профспілки",textCharFormat);
	
	//---
	cell = table->cellAt(14, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Інші нарахування",textCharFormat);
	
	//Нарахування Індексація
	cell = table->cellAt(14, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Індексація зарплати",textCharFormat);
	
	//---
	cell = table->cellAt(15, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Сума нараховання з ОД=01",textCharFormat_bold);
	
	//Заповнення показників нарахування
	double narahSum=0;
	double val;
	//Основне нарахування
	cell = table->cellAt(2, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(1).toDouble();
	narahSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//Вислуга років
	cell = table->cellAt(3, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(2).toDouble();
	narahSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	
	//Лікарняні
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
	
	//Відпускні
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
	
	//Святкові
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
	
	//Нічні
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
	
	//Надурочні
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
	
	//Класність водія
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
	
	//Премія
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
	
	//Матеріальна допомога
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
	
	//Доплата
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
	
	//Виплати профспілки
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
	
	//Інші нарахування
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
	
	//індексація зарплати
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
	
	//Всього нараховано
	cell = table->cellAt(15, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(narahSum),textCharFormat_bold);
	
	//Заповнення показників утримання
	//---
	cell = table->cellAt(2, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Пенсійний фонд",textCharFormat);
	
	//---
	cell = table->cellAt(3, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Фонд зайнятості",textCharFormat);
	
	//---
	cell = table->cellAt(4, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Фонд соц.страхування",textCharFormat);
	
	//---
	cell = table->cellAt(5, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Прибутковий податок",textCharFormat);
	
	//---
	cell = table->cellAt(6, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("в т.ч. Соціальна пільга",textCharFormat);
	
	//---
	cell = table->cellAt(7, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Відрах.на профспілку",textCharFormat);
	
	//---
	cell = table->cellAt(8, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Інші відрахування",textCharFormat);
	
	//---
	cell = table->cellAt(15, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Сума утримань з ОД = 01",textCharFormat);
	
	double utrymSum = 0;
	//Пенсійний фонд
	cell = table->cellAt(2, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(15).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//Фонд зайнятості
	cell = table->cellAt(3, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(16).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//Фонд соц.страхування
	cell = table->cellAt(4, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(17).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//Прибутковий податок
	cell = table->cellAt(5, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(19).toDouble();
	utrymSum += val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat);
	
	//в т.ч. Соціальна пільга
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
	
	//Відрах.на профспілку
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
	
	//Інші відрахування
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
	
	//Всього утримано
	cell = table->cellAt(15, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(utrymSum),textCharFormat_bold);
	
	//Заповнення стрічок додаткового нарахування і утримання
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
	cellCursor.insertText("Сума нарахування з ОД > 01");
	
	//Нарахування з ОД > 01
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
	cellCursor.insertText("Утримання з ОД > 01");
	
	//table->mergeCells(17,0,1,3);
	cell = table->cellAt(17, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("На руки з ОД = 01");
	
	cell = table->cellAt(17, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("На руки з ОД > 01");
	
	cell = table->cellAt(18, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Всього на руки за місяць.", textCharFormat_bold);
	
	cell = table->cellAt(18, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignLeft );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Всього утрим.", textCharFormat_bold);
	
	//Утримання з ОД > 01
	cell = table->cellAt(16, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (otherUtrynSum != 0)
		cellCursor.insertText(m_to_str_2(otherUtrynSum),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//На руки з ОД = 01
	cell = table->cellAt(17, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (otherNarahNaRuky != 0)
		cellCursor.insertText(m_to_str_2(narahSum - utrymSum - otherUtrynSum),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//На руки з ОД > 01
	cell = table->cellAt(17, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	if (otherNarahNaRuky != 0)
		cellCursor.insertText(m_to_str_2(otherNarahNaRuky),textCharFormat);
	else
		cellCursor.insertText("-",textCharFormat);
	
	//Всього на руки у поточному місяці
	cell = table->cellAt(18, 1);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(narahSum - utrymSum - otherUtrynSum + otherNarahNaRuky),textCharFormat_bold);
	
	//Всього утрим.
	cell = table->cellAt(18, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText(m_to_str_2(utrymSum + otherUtrynSum),textCharFormat_bold);
	
	
	
	//Виплата
	table->mergeCells(19,0,1,4);
	
	//---
	cell = table->cellAt(19, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignCenter );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Виплата",textCharFormat_bold);
	
	
	//Авванс, заборгованість, штраф, аліменти
	//---
	cell = table->cellAt(20, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Аванс",textCharFormat);
	
	//---
	cell = table->cellAt(21, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Виплачено за попер. міс.",textCharFormat);
	
	//---
	cell = table->cellAt(22, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Попер. опл. в поточному місяці",textCharFormat);
	
	//---
	cell = table->cellAt(23, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	//cellCursor.insertText("Заборгованість",textCharFormat);
	cellCursor.insertText("Премія",textCharFormat);
	
	//---
	cell = table->cellAt(20, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Штраф",textCharFormat);
	
	//---
	cell = table->cellAt(21, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Аліменти",textCharFormat);
	
	//---
	cell = table->cellAt(22, 2);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Інші виплати",textCharFormat);
	
	//Заповнення показників аванс, заборгованість, штраф, аліменти
	//Аванс
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
	
	//Виплачено за попер. міс.
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
	
	//Попер. опл. в поточному місяці
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
	
	//Премія
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
	
	//Штраф
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
	
	//Аліменти
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
	
	//Інші
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
	cellCursor.insertText("Виплачено в поточному місяці",textCharFormat_bold);
	
	//---
	cell = table->cellAt(25, 0);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	cellCursor.insertText("Вихідне сальдо",textCharFormat);
	
	table->mergeCells(24,0,1,3);
	table->mergeCells(25,0,1,3);
	
	//Виплачено в поточному місяці
	cell = table->cellAt(24, 3);
	cell.setFormat(textCharFormat);
	cellCursor = cell.firstCursorPosition();
	blockFormat.setAlignment( Qt::AlignRight );
	cellCursor.setBlockFormat( blockFormat );
	val = query->value(22).toDouble() + query->value(23).toDouble() + query->value(24).toDouble() + query->value(25).toDouble() + 
			query->value(26).toDouble() + query->value(27).toDouble() + query->value(28).toDouble();
	double oplataSum = val;
	cellCursor.insertText(m_to_str_2(val),textCharFormat_bold);
	
	//Вихідне сальдо
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
