//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл визначення класу створення звіту 1ДФ
//
// Створений програмістами Компанії "Утех" 15.02.2011р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UPFUEdynyiVnesokDbf.h"
#include <xbase64.h>

#include <USqlAccessible>
#include <UPostupForm>

#include <QtDebug>

//--------------------utech--------------------utech--------------------utech--------------------
UPFUEdynyiVnesokDbf::UPFUEdynyiVnesokDbf()
{
	funcCodec = QTextCodec::codecForName("Windows-1251");
}
//--------------------utech--------------------utech--------------------utech--------------------
UPFUEdynyiVnesokDbf::~UPFUEdynyiVnesokDbf()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
inline char * UPFUEdynyiVnesokDbf::strToxBaseChar(QString str)
{
	return funcCodec->fromUnicode(str).data();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPFUEdynyiVnesokDbf::showCantDeleteDbfFileMessage(QString file)
{
	QMessageBox::critical(0, "Помилка видалення старого dbf-файлу звіту",
				"В обраній Вами папці знаходиться файл "+file+" з Таблицею 5 Додатку 4 який при формуванні звітності потрібно замінити на новий. "
				"Але програмі не вдалося видалити його. Закрийте усі програми, що можуть використовувати зазначений файл, "
				"або оберіть іншу папку для експорту місячної персоніфікації");
}
//--------------------utech--------------------utech--------------------utech--------------------
bool UPFUEdynyiVnesokDbf::createElZvit(QString path, short year, short month)
{
	path = path.replace("/","\\")+"\\";
	QString dbfFileNameT5 = "E04T05C.dbf", 
			dbfFileNameT6 = "E04T06C.dbf";
	QString dbfFilePathT5 = path + dbfFileNameT5,
			dbfFilePathT6 = path + dbfFileNameT6;
	
	//Видалення існуючих файлів, які потрібно замінити
	if (QFile::exists(dbfFilePathT5)) {
		if (!QFile::remove(dbfFilePathT5)) {
			showCantDeleteDbfFileMessage(dbfFileNameT5);
			return false;
		}
	}
	if (QFile::exists(dbfFilePathT6)) {
		if (!QFile::remove(dbfFilePathT6)) {
			showCantDeleteDbfFileMessage(dbfFileNameT6);
			return false;
		}
	}
	
	QSqlQuery query;
	int rowCount;
	int rowNum;
	bool t5DoneOk=true, t6DoneOk=true;
	UPostupForm *postup = new UPostupForm(0, 0);
	postup->showLable(true);
	postup->setPostupLabel("Запис Таблиці 5 Додатку 4 (Трудові відносини застрахованих осіб)");
	postup->show();
	
	xbXBase x;
	xbDbf dbfFile( &x );
	
	//------ Початок запису Таблиці №5
	query.exec("SELECT count(*) FROM tabel_sum \
				LEFT JOIN npr ON npr.id=tabel_sum.Npr_id \
				WHERE year(tabel_sum.Cur_Date)="+sqlStr(year)+" \
						and month(tabel_sum.Cur_date)="+sqlStr(month)+" \
						and (Pryinyato>0 or Zvilneno>0)");
	query.next();
	rowCount = query.value(0).toInt();
	if (rowCount > 0) {
		xbSchema table5Shema[] =
		{
			{ "PERIOD_M",		XB_NUMERIC_FLD, 2, 0 },
			{ "PERIOD_Y",		XB_NUMERIC_FLD, 4, 0 },
			
			{ "ROWNUM",		XB_NUMERIC_FLD, 6, 0 },
			
			{ "UKR_GROMAD",		XB_NUMERIC_FLD, 1, 0 },
			
			{ "NUMIDENT",	XB_CHAR_FLD,   10, 0 },
			{ "LN",			XB_CHAR_FLD,   100, 0 },
			{ "NM",			XB_CHAR_FLD,   100, 0 },
			{ "FTN",		XB_CHAR_FLD,   100, 0 },
			{ "START_DT",	XB_NUMERIC_FLD,2, 0 },
			{ "END_DT",		XB_NUMERIC_FLD,2, 0 },
			{ "",0,0,0 }
		};
		
		dbfFile.SetVersion( 3 );
		
		if ( dbfFile.CreateDatabase((dbfFilePathT5).toAscii().data(),table5Shema,XB_OVERLAY) 
					!= XB_NO_ERROR) {
			QMessageBox::critical(0, "Помилка", "Помилка створення dbf файлу "+dbfFileNameT5);
			delete postup;
			return false;
		}
		
		postup->setNumPos(rowCount);
		postup->setCurrentPos(0);
		
		rowNum = 0;
		query.exec("SELECT npr.Iden_kod, npr.Prizv, npr.Imia, npr.Pobatk, \
							tabel_sum.Pryinyato, tabel_sum.Zvilneno FROM tabel_sum \
					LEFT JOIN npr ON npr.id=tabel_sum.Npr_id \
					WHERE year(tabel_sum.Cur_Date)="+sqlStr(year)+" \
							and month(tabel_sum.Cur_date)="+sqlStr(month)+" \
							and (Pryinyato>0 or Zvilneno>0) \
					ORDER BY npr.Prizv, npr.Imia, npr.Pobatk, npr.Iden_kod");
		while ( query.next() ) {
			rowNum++;
			dbfFile.BlankRecord();
			dbfFile.PutLongField( "PERIOD_M", month );
			dbfFile.PutLongField( "PERIOD_Y", year );
			dbfFile.PutLongField( "ROWNUM", rowNum );
			dbfFile.PutLongField( "UKR_GROMAD", 1 );
			dbfFile.PutField( "NUMIDENT", strToxBaseChar(query.value(0).toString()));
			dbfFile.PutField( "LN", strToxBaseChar(query.value(1).toString()));
			dbfFile.PutField( "NM", strToxBaseChar(query.value(2).toString()));
			dbfFile.PutField( "FTN", strToxBaseChar(query.value(3).toString()));
			if ( query.value(4).toInt() )
				dbfFile.PutLongField( "START_DT", query.value(4).toInt());
			if ( query.value(5).toInt() )
			dbfFile.PutLongField( "END_DT", query.value(5).toInt());
			
			if (dbfFile.AppendRecord() != XB_NO_ERROR)
				t5DoneOk = false;
		}
		
		dbfFile.CloseDatabase();
	}
	//------ Кінець запису Таблиці №5
	
	//------ Початок запису Таблиці №6
	postup->setPostupLabel("Запис Таблиці 6 Додатку 4 (Відомості про нарахування заробітку застрахованим особам)");
	
	xbSchema table6Shema[] =
	{
		{ "PERIOD_M",	XB_NUMERIC_FLD, 2, 0 },
		{ "PERIOD_Y",	XB_NUMERIC_FLD, 4, 0 },
		
		{ "ROWNUM",		XB_NUMERIC_FLD, 6, 0 },
		
		{ "UKR_GROMAD",	XB_NUMERIC_FLD, 1, 0 },
		{ "ST",			XB_NUMERIC_FLD, 1, 0 },
		
		{ "NUMIDENT",	XB_CHAR_FLD,   10, 0 },
		{ "LN",			XB_CHAR_FLD,   100, 0 },
		{ "NM",			XB_CHAR_FLD,   100, 0 },
		{ "FTN",		XB_CHAR_FLD,   100, 0 },
		{ "ZO",			XB_NUMERIC_FLD, 2, 0 },
		
		{ "PAY_TP",		XB_NUMERIC_FLD, 2, 0 },
		{ "PAY_MNTH",	XB_NUMERIC_FLD, 2, 0 },
		{ "PAY_YEAR",	XB_NUMERIC_FLD, 4, 0 },
		
		{ "SUM_TOTAL",	XB_NUMERIC_FLD,16, 2 },
		{ "SUM_MAX",	XB_NUMERIC_FLD,16, 2 },
		{ "SUM_INS",	XB_NUMERIC_FLD,16, 2 },
		{ "OTK",		XB_NUMERIC_FLD, 1, 0 },
		{ "EXP",		XB_NUMERIC_FLD, 1, 0 },
		{ "",0,0,0 }
	};
	
	dbfFile.SetVersion( 3 );
	
	if ( dbfFile.CreateDatabase((dbfFilePathT6).toAscii().data(),table6Shema,XB_OVERLAY) 
				!= XB_NO_ERROR) {
		QMessageBox::critical(0, "Помилка", "Помилка створення dbf файлу "+dbfFileNameT6);
		delete postup;
		return false;
	}
	
	query.exec("SELECT count(*) \
				FROM (SELECT Npr_id, sum(N_Osn_narah+N_VuslRokiv+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Vidpuskni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_Indexaciya+N_VyplProfspilky+N_InshiNarahuvannya) AS narahSum, \
							sum(U_PensFond) AS utrPensFond \
					FROM operacii \
					WHERE year(CurDate)="+sqlStr(year)+" \
						and month(CurDate)="+sqlStr(month)+" \
						and OznakaDohodu=1 \
					GROUP BY Npr_id) AS o \
				LEFT JOIN npr ON npr.id=o.Npr_id \
				WHERE o.narahSum>0 and o.utrPensFond>0");
	query.next();
	postup->setNumPos(query.value(0).toInt());
	postup->setCurrentPos(0);
	
	rowNum = 0;
	query.exec("SELECT npr.Iden_kod, npr.Prizv, npr.Imia, npr.Pobatk, npr.Invalid, \
						o.narahSum, o.utrPensFond, npr.TypRoboty, npr.Gender \
				FROM (SELECT Npr_id, sum(N_Osn_narah+N_VuslRokiv+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Vidpuskni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_Indexaciya+N_VyplProfspilky+N_InshiNarahuvannya) AS narahSum, \
							sum(U_PensFond) AS utrPensFond \
					FROM operacii \
					WHERE year(CurDate)="+sqlStr(year)+" \
						and month(CurDate)="+sqlStr(month)+" \
						and OznakaDohodu=1 \
					GROUP BY Npr_id) AS o \
				LEFT JOIN npr ON npr.id=o.Npr_id \
				WHERE o.narahSum>0 or o.utrPensFond>0 \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk, npr.Iden_kod");
	while ( query.next() ) {
		rowNum++;
		
		dbfFile.BlankRecord();
		
		dbfFile.PutLongField( "PERIOD_M", month );
		dbfFile.PutLongField( "PERIOD_Y", year );
		
		dbfFile.PutLongField( "ROWNUM", rowNum );
		
		dbfFile.PutLongField( "UKR_GROMAD", 1 );
		dbfFile.PutLongField( "ST", query.value(8).toInt() );
		
		dbfFile.PutField( "NUMIDENT", strToxBaseChar(query.value(0).toString()));
		dbfFile.PutField( "LN", strToxBaseChar(query.value(1).toString()));
		dbfFile.PutField( "NM", strToxBaseChar(query.value(2).toString()));
		dbfFile.PutField( "FTN", strToxBaseChar(query.value(3).toString()));
		if (query.value(4).toBool()) { //Інвалід
			dbfFile.PutLongField( "ZO", 2 ); // 2 - інвалід
		}
		else{
			dbfFile.PutLongField( "ZO", 1 ); // 1 - звичайний працівник
		}
		
		dbfFile.PutLongField( "PAY_TP", 0 );
		dbfFile.PutLongField( "PAY_MNTH", month );
		dbfFile.PutLongField( "PAY_YEAR", year );
		
		dbfFile.PutDoubleField( "SUM_TOTAL", query.value(5).toDouble());
		dbfFile.PutDoubleField( "SUM_MAX", query.value(5).toDouble());
		dbfFile.PutDoubleField( "SUM_INS", query.value(6).toDouble());
		if (query.value(7).toInt() > 0)
			dbfFile.PutLongField( "OTK", 0);
		else
			dbfFile.PutLongField( "OTK", 1);
		dbfFile.PutLongField( "EXP", 0 );
		
		if (dbfFile.AppendRecord() != XB_NO_ERROR)
			t6DoneOk = false;
	}
	
		// Запис лікарняних
	query.exec("SELECT npr.Iden_kod, npr.Prizv, npr.Imia, npr.Pobatk, npr.Invalid, \
						o.narahSum, o.utrPensFond, npr.TypRoboty, npr.Gender \
				FROM (SELECT Npr_id, sum(N_Likarn) AS narahSum, \
							sum(U_EV_Likarn) AS utrPensFond \
					FROM operacii \
					WHERE year(CurDate)="+sqlStr(year)+" \
						and month(CurDate)="+sqlStr(month)+" \
						and OznakaDohodu=1 \
					GROUP BY Npr_id) AS o \
				LEFT JOIN npr ON npr.id=o.Npr_id \
				WHERE o.narahSum>0 or o.utrPensFond>0 \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk, npr.Iden_kod");
	while ( query.next() ) {
		rowNum++;
		
		dbfFile.BlankRecord();
		
		dbfFile.PutLongField( "PERIOD_M", month );
		dbfFile.PutLongField( "PERIOD_Y", year );
		
		dbfFile.PutLongField( "ROWNUM", rowNum );
		
		dbfFile.PutLongField( "UKR_GROMAD", 1 );
		dbfFile.PutLongField( "ST", query.value(8).toInt() );
		
		dbfFile.PutField( "NUMIDENT", strToxBaseChar(query.value(0).toString()));
		dbfFile.PutField( "LN", strToxBaseChar(query.value(1).toString()));
		dbfFile.PutField( "NM", strToxBaseChar(query.value(2).toString()));
		dbfFile.PutField( "FTN", strToxBaseChar(query.value(3).toString()));
		dbfFile.PutLongField( "ZO", 29 ); // лікарняне
		
		dbfFile.PutLongField( "PAY_TP", 0 );
		dbfFile.PutLongField( "PAY_MNTH", month );
		dbfFile.PutLongField( "PAY_YEAR", year );
		
		dbfFile.PutDoubleField( "SUM_TOTAL", query.value(5).toDouble());
		dbfFile.PutDoubleField( "SUM_MAX", query.value(5).toDouble());
		dbfFile.PutDoubleField( "SUM_INS", query.value(6).toDouble());
		if (query.value(7).toInt() > 0)
			dbfFile.PutLongField( "OTK", 0);
		else
			dbfFile.PutLongField( "OTK", 1);
		dbfFile.PutLongField( "EXP", 0 );
		
		if (dbfFile.AppendRecord() != XB_NO_ERROR)
			t6DoneOk = false;
	}
	
	dbfFile.CloseDatabase();
	//------ Кінець запису Таблиці №6
	
	qDebug() << "t5DoneOk: " << t5DoneOk;
	qDebug() << "t6DoneOk: " << t6DoneOk;
	
	delete postup;
	return t5DoneOk && t6DoneOk;
}
//--------------------utech--------------------utech--------------------utech--------------------
