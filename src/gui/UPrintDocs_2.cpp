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
void UPrintDocs::print_tabel_for_pidrozdil(short year, short month, int pidrozdilId)
{
	printform = new UPrintForm(0, "Табель за "+QString::number(month)+"."+QString::number(year));
	
	QSqlQuery query;
	QString pidrozdilFilter = pidrozdilId?QString("Pidrozdil_id=")+sqlStr(pidrozdilId):
							QString(" Pidrozdil_id not in (SELECT id FROM pidrozdily_salary) ");
	QDate begDate(year, month, 1), endDate(year, month, begDate.daysInMonth());
	int iter;
	const short daysInMonth = begDate.daysTo(endDate) + 1;
	const short columnCount= 16 + daysInMonth;
	query.exec("SELECT count(*) FROM npr WHERE "+pidrozdilFilter);
	query.next();
	const int wCount = query.value(0).toInt();
	
	//Перевірка необхідності друку надурочних
	query.exec("SELECT Znach FROM budjetni_normu WHERE id=19");
	bool needNadurochni = false;
	if ( query.next() && (query.value(0).toDouble() > 0.009) )
		needNadurochni = true;
	
	QList<QDate> svList;
	query.exec("SELECT CDate FROM svyatkovidni \
				WHERE CDate BETWEEN date("+sqlStr(begDate)+") \
					and date("+sqlStr(endDate)+")");
	while (query.next())
		svList << query.value(0).toDate();
	
	UPostupForm *postup = new UPostupForm(0, wCount);
	postup->show();
	
	QTextCursor cursor(printform->document());
	QTextBlockFormat blockFormat;
	QTextTableCell cell;
	QTextCharFormat textCharFormat, textCharFormat_bold, textCharFormat_small;
	QFont fnt = textCharFormat_bold.font();
	fnt.setBold(true);
	textCharFormat_bold.setFont(fnt);
	
	blockFormat.setAlignment( Qt::AlignRight );
	cursor.setBlockFormat( blockFormat );
	textCharFormat.setFontPointSize( 7 );
	cursor.insertText("Cтворено програмою \"Ель-Зар\" "+QDate::currentDate().toString("dd.MM.yyyy")+"р. "+QTime::currentTime().toString("hh:mm"), textCharFormat);
	blockFormat.setAlignment( Qt::AlignCenter );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	textCharFormat_bold.setFontPointSize( 12 );
	textCharFormat_small.setFontPointSize( 5 );
	cursor.insertText("ТАБЕЛЬ ОБЛІКУ ВИКОРИСТАННЯ РОБОЧОГО ЧАСУ", textCharFormat_bold);
	textCharFormat_bold.setFontPointSize( 11 );
	textCharFormat.setFontPointSize( 11 );
	cursor.insertBlock(blockFormat);
	cursor.insertText("за "+monthlist.at(month-1)+" "+QString::number(year)+" року", textCharFormat_bold);
	blockFormat.setAlignment( Qt::AlignLeft );
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertText("Підприємство, організація:  ", textCharFormat);
	cursor.insertText(OrganizName(), textCharFormat_bold);
	cursor.insertBlock(blockFormat);
	cursor.insertText("Цех, ділянка, підрозділ  ", textCharFormat);
	query.exec("SELECT Name FROM pidrozdily_salary WHERE id="+sqlStr(pidrozdilId)+" LIMIT 1");
	if (query.next())
		cursor.insertText(query.value(0).toString(), textCharFormat_bold);
	else
		cursor.insertText("________________________________", textCharFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	//Створення таблиці
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	constraints << QTextLength(QTextLength::FixedLength, 20);
	constraints << QTextLength(QTextLength::FixedLength, 90);
	constraints << QTextLength(QTextLength::FixedLength, 25);
	for (iter=0; iter<columnCount-6; iter++)
		constraints << QTextLength(QTextLength::FixedLength, 20);
	constraints << QTextLength(QTextLength::FixedLength, 30); //всього робочого часу
	constraints << QTextLength(QTextLength::FixedLength, 20); //нічні
	constraints << QTextLength(QTextLength::FixedLength, 20); //понадурочні
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(0.5);
	tableFormat.setBorder(0.5);
	tableFormat.setHeaderRowCount(2);
	QTextTable *table = cursor.insertTable(2 + wCount, columnCount, tableFormat);
	UPopulateCell tableCell(table);
	
	blockFormat.setBottomMargin(0);
	blockFormat.setTopMargin(0);
	
	//Заповнення шапки таблиці
	textCharFormat.setFontPointSize( 8 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	textCharFormat_bold.setFontPointSize( 8 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignMiddle);
	
	QTextCharFormat textCharFormatBottom = textCharFormat;
	textCharFormatBottom.setVerticalAlignment(QTextCharFormat::AlignBottom);
	
	int col=0;
	tableCell.setAlignment(Qt::AlignCenter);
	table->mergeCells(0,0,2,1);
	table->mergeCells(0,1,2,1);
	table->mergeCells(0,2,2,1);
	table->mergeCells(0,3,1,daysInMonth);
	table->mergeCells(0,3+daysInMonth,1,3);
	table->mergeCells(0,3+3+daysInMonth,1,6);
	table->mergeCells(0,3+3+6+daysInMonth,2,1);
	table->mergeCells(0,3+3+6+1+daysInMonth,1,3);
	
	tableCell.set(0,col,"№ з п", textCharFormat);
	tableCell.set(0,++col,"Прівзвище, ім'я, по батькові", textCharFormat);
	tableCell.set(0,++col,"Таб. но-мер", textCharFormat);
	tableCell.set(0,++col,"Числа місяця", textCharFormat);
	for (iter=0; iter<daysInMonth; iter++){
		QDate dt = begDate.addDays(iter);
		if (svList.contains(dt))
			tableCell.set(1,col++,QString::number(dt.day())+" св", textCharFormat_bold);
		else
			tableCell.set(1,col++,QString::number(dt.day())+" "+dt.toString("ddd"), textCharFormat);
	}
	tableCell.set(0,col,"Дні явок", textCharFormat);
	tableCell.set(1,col,verticalTextToImage("факт. роботи", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("цілозм. простоїв", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("рем.автобуса,год.", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(0,++col,"Дні неявок /чол. дні/", textCharFormat);
	tableCell.set(1,col,verticalTextToImage("чергові відп.", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("відп.у зв.з род.", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("лікарняні", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("неявки дозв.зак.", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("з дозв.адмін.", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("прогули", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(0,++col,verticalTextToImage("вихідні і святкові дні", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(0,++col,"Відпра-цьовано годин", textCharFormat);
	tableCell.set(1,col,verticalTextToImage("всього", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("нічні", textCharFormatBottom), textCharFormatBottom);
	tableCell.set(1,++col,verticalTextToImage("понадурочні", textCharFormatBottom), textCharFormatBottom);
	
	textCharFormat.setFontPointSize( 7 );
	textCharFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
	textCharFormat_bold.setFontPointSize( 7 );
	textCharFormat_bold.setVerticalAlignment(QTextCharFormat::AlignTop);
	QTextCharFormat textCharFormat_header = textCharFormat;
	textCharFormat_header.setFontPointSize( 8 );
	
	QSqlQuery wQuery("SELECT id, Prizv, Imia, Pobatk FROM npr WHERE "+pidrozdilFilter+" \
						ORDER BY Prizv, Imia, Pobatk");
	int row=1;
	iter=0;
	for (; iter<wCount and wQuery.next(); ++iter){
		row++;
		QString nprStr;
		nprStr = wQuery.value(1).toString();
		if (wQuery.value(2).toString().size() > 0){
			nprStr += QString(" %1.").arg(wQuery.value(2).toString().at(0));
			if (wQuery.value(3).toString().size() > 0)
				nprStr += QString(" %1.").arg(wQuery.value(3).toString().at(0));
		}
		
		col = 0;
		//"№ з п"
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,col,iter+1, textCharFormat_header);
		//"Прівзвище, ім'я, по батькові"
		tableCell.setAlignment(Qt::AlignLeft);
		tableCell.set(row,++col,nprStr, textCharFormat_header);
		//"Таб. номер"
		tableCell.setAlignment(Qt::AlignCenter);
		tableCell.set(row,++col,wQuery.value(0).toString(), textCharFormat_header);
		
		int curWorkTime=0, curNichTime=0, curSvTime=0, curVidpusk=0, curRodVidpusk=0, curLikarn=0, curWorkDays=0, 
			curNadurochni=0, curCilozmProstDays=0, curProgulyDays=0, curNeyavkaZakon=0, curNeyavkaAdmDays=0,
			curRemontAvtobusaTime=0;
		
		query.exec("SELECT t.Cur_Date, t.Pracivn_status_id, t.God_zag, t.God_nichni, t.God_sv, \
						s.Status_char \
					FROM tabel t \
					LEFT JOIN pracivn_status s ON s.id=t.Pracivn_status_id \
					WHERE t.npr_id="+sqlStr(wQuery.value(0).toInt())+" \
						and t.Cur_Date BETWEEN date("+sqlStr(begDate)+") \
							and date("+sqlStr(endDate)+")");
		while (query.next()){
			col = 3 + begDate.daysTo(query.value(0).toDate());
			
			if ((query.value(1).toInt() == 1) || (query.value(1).toInt() == 5)){
				if (timeToMinute(query.value(2).toTime())>0){
					//Робочий час
					tableCell.setAlignment(Qt::AlignRight);
					tableCell.set(row,col,query.value(2).toTime().toString("h:mm"), textCharFormat);
					
					curWorkDays++;
					int iVal = query.value(2).toTime().hour() * 60 + query.value(2).toTime().minute();	
					curWorkTime += iVal;
					if ( needNadurochni && (iVal > 480) ) //час більший за вісім годин -> надурочні
						curNadurochni += iVal - 480;
					curNichTime += query.value(3).toTime().hour() * 60 + query.value(3).toTime().minute();
					curSvTime += query.value(4).toTime().hour() * 60 + query.value(4).toTime().minute();
				}
			}
			else if (query.value(1).toInt() == 13){ //ремонт автобуса
				if (timeToMinute(query.value(2).toTime())>0){
					tableCell.setAlignment(Qt::AlignCenter);
					tableCell.set(row,col,query.value(5).toString(), textCharFormat_bold);
					
					curWorkDays++;
					
					int iVal = query.value(2).toTime().hour() * 60 + query.value(2).toTime().minute();	
					curRemontAvtobusaTime += iVal;
					/*if (iVal > 480) //час більший за вісім годин -> надурочні
						curNadurochni += iVal - 480;
					curNichTime += query.value(3).toTime().hour() * 60 + query.value(3).toTime().minute();
					curSvTime += query.value(4).toTime().hour() * 60 + query.value(4).toTime().minute();*/
				}
			}
			else{
				//Буква типу присутності
				tableCell.setAlignment(Qt::AlignCenter);
				tableCell.set(row,col,query.value(5).toString(), textCharFormat_bold);
				
				switch (query.value(1).toInt()){
					case 3  : curVidpusk++; break;
					case 8  : curRodVidpusk++; break;
					case 2  : curLikarn++; break;
					case 12 : curCilozmProstDays++; break;
					case 14 : curProgulyDays++; break;
					case 6  : curNeyavkaZakon++; break;
					case 17 : curNeyavkaAdmDays++; break;
				}
			}
		}
		
		//Заповнення підсумкової стрічки поточного працівника
		col = 3+daysInMonth;
		tableCell.setAlignment(Qt::AlignRight);
		tableCell.setDash(row,col,curWorkDays, textCharFormat);
		col += 1;
		tableCell.setDash(row,col,curCilozmProstDays, textCharFormat);
		col += 1;
		tableCell.set(row,col,secToTime(curRemontAvtobusaTime), textCharFormat);
		col += 1;
		tableCell.setDash(row,col,curVidpusk, textCharFormat);
		col += 1;
		tableCell.setDash(row,col,curRodVidpusk, textCharFormat);
		col += 1;
		tableCell.setDash(row,col,curLikarn, textCharFormat);
		col += 1;
		tableCell.setDash(row,col,curNeyavkaZakon, textCharFormat);
		col += 1;
		tableCell.setDash(row,col,curNeyavkaAdmDays, textCharFormat);
		col += 1;
		tableCell.setDash(row,col,curProgulyDays, textCharFormat);
		col += 1;
		tableCell.set(row,col,secToTime(curSvTime), textCharFormat);
		
		col += 1;
		tableCell.set(row,col,secToTime(curWorkTime), textCharFormat);
		col += 1;
		tableCell.set(row,col,secToTime(curNichTime), textCharFormat);
		col += 1;
		tableCell.set(row,col,secToTime(curNadurochni), textCharFormat);
		
		postup->incPos();
	}
	
	//Довідник позначень
	cursor.movePosition(QTextCursor::End);
	
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	cursor.insertBlock(blockFormat);
	
	constraints.clear();
	constraints << QTextLength(QTextLength::FixedLength, 310);
	constraints << QTextLength(QTextLength::FixedLength, 310);
	constraints << QTextLength(QTextLength::FixedLength, 310);
		
	tableFormat.setColumnWidthConstraints(constraints);
	tableFormat.setCellSpacing(0);
	tableFormat.setCellPadding(0.5);
	tableFormat.setBorder(0);
	tableFormat.setHeaderRowCount(0);
	table = cursor.insertTable(2, 3, tableFormat);
	tableCell.setTextTable(table);
	table->mergeCells(0,0,1,3);
	
	tableCell.setAlignment(Qt::AlignLeft);
	textCharFormat.setFontPointSize(9);
	tableCell.set(0,0,"      Позначення у табелі:\n", textCharFormat);
	
	tableCell.set(1,0,"Р  - Робочі дні", textCharFormat);
	tableCell.insertText("\nТН - Лікарняне, тимчасова непрацездатність", textCharFormat);
	tableCell.insertText("\nРА - Ремонт автобуса", textCharFormat);
	tableCell.insertText("\nВ  - Відпустрка", textCharFormat);
	tableCell.insertText("\nНА - За свій рахунок", textCharFormat);
	tableCell.insertText("\nРВ - Вихідні і святкові дні", textCharFormat);
	tableCell.set(1,1,"ІН - Невиходи дозволені Законом (державні обов'язки)", textCharFormat);
	tableCell.insertText("\nВД - Службові відрядження", textCharFormat);
	tableCell.insertText("\nВП - Відпустка у зв'язку з родами", textCharFormat);
	tableCell.insertText("\nРС - Пільгові години матерям", textCharFormat);
	tableCell.insertText("\nНУ - Понаднормовані години роботи", textCharFormat);
	tableCell.insertText("\nП  - Цілозмінні простої", textCharFormat);
	tableCell.set(1,2,"ПР - Прогули", textCharFormat);
	tableCell.insertText("\nПР - Прогули", textCharFormat);
	tableCell.insertText("\nІВ - Спізнення", textCharFormat);
	tableCell.insertText("\nВЧ - Вечірні години роботи", textCharFormat);
	tableCell.insertText("\nА  - Невихід з дозволу адміністрації", textCharFormat);
	tableCell.insertText("\nД  - Пільгові год роб. з шкідливими умовами праці", textCharFormat);
	
	
	
	//Підписи
	textCharFormat.setFontPointSize( 9 );
	cursor.movePosition(QTextCursor::End);
	cursor.insertBlock(blockFormat);
	cursor.insertText("\n\n\n	_________________						_________________", textCharFormat);
	cursor.insertText("\n\n\n	_________________						_________________", textCharFormat);
	
	printform->printer()->setDocName("Табель за "+QString::number(month)+"."+QString::number(year));
	printform->printer()->setOrientation(QPrinter::Landscape);
	printform->printer()->setPageMargins( 3, 5, 4, 10, QPrinter::Millimeter );
	printform->show();
	delete postup;
}
//--------------------utech--------------------utech--------------------utech--------------------
