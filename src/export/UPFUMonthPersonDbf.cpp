//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� ���������� ����� ��������� ���� 1��
//
// ��������� ������������ �������� "����" 12.10.2008�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UPFUMonthPersonDbf.h"
#include <xbase64.h>

#include <USqlAccessible>
#include <UPostupForm>

#include <QtDebug>

//--------------------utech--------------------utech--------------------utech--------------------
UPFUMonthPersonDbf::UPFUMonthPersonDbf()
{
	funcCodec = QTextCodec::codecForName("Windows-1251");
}
//--------------------utech--------------------utech--------------------utech--------------------
UPFUMonthPersonDbf::~UPFUMonthPersonDbf()
{
	
}
//--------------------utech--------------------utech--------------------utech--------------------
inline char * UPFUMonthPersonDbf::strToxBaseChar(QString str)
{
	return funcCodec->fromUnicode(str).data();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UPFUMonthPersonDbf::showCantDeleteDbfFileMessage(QString file)
{
	QMessageBox::critical(0, "������� ��������� ������� dbf-����� ����",
				"� ������� ���� ����� ����������� ���� "+file+" � �������� 5 ������� 4 ���� ��� ���������� ������� ������� ������� �� �����. "
				"��� ������� �� ������� �������� ����. �������� �� ��������, �� ������ ��������������� ���������� ����, "
				"��� ������ ���� ����� ��� �������� ������ ��������������");
}
//--------------------utech--------------------utech--------------------utech--------------------
bool UPFUMonthPersonDbf::createElZvit(QString path, short year, short month)
{
	path = path.replace("/","\\")+"\\";
	QString dbfFileNameT5 = "P04t05b.dbf", 
			dbfFileNameT6 = "P04t06b.dbf", 
			dbfFileNameT7 = "P04t07b.dbf";
	QString dbfFilePathT5 = path + dbfFileNameT5,
			dbfFilePathT6 = path + dbfFileNameT6,
			dbfFilePathT7 = path + dbfFileNameT7;
	
	//��������� �������� �����, �� ������� �������
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
	if (QFile::exists(dbfFilePathT7)) {
		if (!QFile::remove(dbfFilePathT7)) {
			showCantDeleteDbfFileMessage(dbfFileNameT7);
			return false;
		}
	}
	
	QSqlQuery query;
	int rowNum;
	bool t5DoneOk=true, t6DoneOk=true, t7DoneOk=true;
	UPostupForm *postup = new UPostupForm(0, 0);
	postup->showLable(true);
	postup->setPostupLabel("����� ������� 5 ������� 4 (������ �������� ������������� ���)");
	postup->show();
	
	xbXBase x;
	xbDbf dbfFile( &x );
	
	//------ ������� ������ ������� �5
	xbSchema table5Shema[] =
	{
		{ "ROWNUM",		XB_NUMERIC_FLD, 5, 0 },
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
		QMessageBox::critical(0, "�������", "������� ��������� dbf ����� "+dbfFileNameT5);
		delete postup;
		return false;
	}
	
	query.exec("SELECT count(*) FROM tabel_sum \
				LEFT JOIN npr ON npr.id=tabel_sum.Npr_id \
				WHERE year(tabel_sum.Cur_Date)="+sqlStr(year)+" \
						and month(tabel_sum.Cur_date)="+sqlStr(month)+" \
						and (Pryinyato>0 or Zvilneno>0)");
	query.next();
	postup->setNumPos(query.value(0).toInt());
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
		dbfFile.PutLongField( "ROWNUM", rowNum );
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
	//------ ʳ���� ������ ������� �5
	
	//------ ������� ������ ������� �6
	postup->setPostupLabel("����� ������� 6 ������� 4 (³������ ��� ����������� �������� ������������� ������)");
	
	xbSchema table6Shema[] =
	{
		{ "ROWNUM",		XB_NUMERIC_FLD, 5, 0 },
		{ "NUMIDENT",	XB_CHAR_FLD,   10, 0 },
		{ "LN",			XB_CHAR_FLD,   251, 0 },
		{ "NM",			XB_CHAR_FLD,   251, 0 },
		{ "FTN",		XB_CHAR_FLD,   251, 0 },
		{ "ZO",			XB_NUMERIC_FLD, 2, 0 },
		{ "ZIC",		XB_NUMERIC_FLD, 2, 0 },
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
		QMessageBox::critical(0, "�������", "������� ��������� dbf ����� "+dbfFileNameT6);
		delete postup;
		return false;
	}
	
	query.exec("SELECT count(*) \
				FROM (SELECT Npr_id, sum(N_Osn_narah+N_VuslRokiv+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
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
						o.narahSum, o.utrPensFond, npr.TypRoboty \
				FROM (SELECT Npr_id, sum(N_Osn_narah+N_VuslRokiv+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_Indexaciya+N_VyplProfspilky+N_InshiNarahuvannya) AS narahSum, \
							sum(U_PensFond) AS utrPensFond \
					FROM operacii \
					WHERE year(CurDate)="+sqlStr(year)+" \
						and month(CurDate)="+sqlStr(month)+" \
						and OznakaDohodu=1 \
					GROUP BY Npr_id) AS o \
				LEFT JOIN npr ON npr.id=o.Npr_id \
				WHERE o.narahSum>0 and o.utrPensFond>0 \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk, npr.Iden_kod");
	while ( query.next() ) {
		rowNum++;
		
		dbfFile.BlankRecord();
		dbfFile.PutLongField( "ROWNUM", rowNum );
		dbfFile.PutField( "NUMIDENT", strToxBaseChar(query.value(0).toString()));
		dbfFile.PutField( "LN", strToxBaseChar(query.value(1).toString()));
		dbfFile.PutField( "NM", strToxBaseChar(query.value(2).toString()));
		dbfFile.PutField( "FTN", strToxBaseChar(query.value(3).toString()));
		if (query.value(4).toBool()) { //������
			dbfFile.PutLongField( "ZO", 2 ); // 2 - ������
			dbfFile.PutLongField( "ZIC", 2 );
		}
		else{
			dbfFile.PutLongField( "ZO", 1 ); // 1 - ��������� ���������
			dbfFile.PutLongField( "ZIC", 1 );
		}
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
	//------ ʳ���� ������ ������� �6
	
	//------ ������� ������ ������� �7
	postup->setPostupLabel("����� ������� 7 ������� 4 (³������ ��� ����������� �������� ������������� ������)");
	
	xbSchema table7Shema[] =
	{
		{ "ROWNUM",		XB_NUMERIC_FLD, 5, 0 },
		{ "NUMIDENT",	XB_CHAR_FLD,   10, 0 },
		{ "LN",			XB_CHAR_FLD,   251, 0 },
		{ "NM",			XB_CHAR_FLD,   251, 0 },
		{ "FTN",		XB_CHAR_FLD,   251, 0 },
		{ "ZO",			XB_NUMERIC_FLD, 2, 0 },
		{ "ZIC",		XB_NUMERIC_FLD, 2, 0 },
		{ "PAY_MNTH",	XB_NUMERIC_FLD, 2, 0 },
		{ "PAY_YEAR",	XB_NUMERIC_FLD, 4, 0 },
		{ "PAY_TP",		XB_NUMERIC_FLD, 1, 0 },
		{ "SUM_TOTAL",	XB_NUMERIC_FLD,16, 2 },
		{ "SUM_MAX",	XB_NUMERIC_FLD,16, 2 },
		{ "SUM_INS",	XB_NUMERIC_FLD,16, 2 },
		{ "OTK",		XB_NUMERIC_FLD, 1, 0 },
		{ "EXP",		XB_NUMERIC_FLD, 1, 0 },
		{ "",0,0,0 }
	};
	
	dbfFile.SetVersion( 3 );
	
	if ( dbfFile.CreateDatabase((dbfFilePathT7).toAscii().data(),table7Shema,XB_OVERLAY) 
				!= XB_NO_ERROR) {
		QMessageBox::critical(0, "�������", "������� ��������� dbf ����� "+dbfFileNameT7);
		delete postup;
		return false;
	}
	
	int rowCount;
	query.exec("SELECT count(*) \
				FROM (SELECT Npr_id, Narah_type_id, year(Narah_date) AS CYear, month(Narah_date) AS CMonth, \
						sum(Suma) AS narahSum \
					FROM likarnyani \
					WHERE year(Cur_date)="+sqlStr(year)+" \
						and month(Cur_date)="+sqlStr(month)+" \
					GROUP BY Npr_id, Narah_type_id, year(Narah_date), month(Narah_date)) AS l \
				LEFT JOIN npr ON npr.id=l.Npr_id \
				WHERE l.narahSum>0");
	query.next();
	rowCount = query.value(0).toInt();
	query.exec("SELECT count(*) \
				FROM (SELECT Npr_id, year(Narah_date) AS CYear, month(Narah_date) AS CMonth, sum(Suma) AS narahSum \
					FROM vidpuskni \
					WHERE year(Cur_date)="+sqlStr(year)+" \
						and month(Cur_date)="+sqlStr(month)+" \
					GROUP BY Npr_id, year(Narah_date), month(Narah_date)) AS v \
				LEFT JOIN npr ON npr.id=v.Npr_id \
				WHERE v.narahSum>0");
	query.next();
	rowCount += query.value(0).toInt();
	postup->setNumPos(rowCount);
	postup->setCurrentPos(0);
	qDebug() << "rowCount" << rowCount;
	
	rowNum = 0;
	//����� ���������
	query.exec("SELECT npr.Iden_kod, npr.Prizv, npr.Imia, npr.Pobatk, npr.Invalid, \
					l.CMonth, l.CYear, l.narahSum, npr.TypRoboty, l.Narah_type_id \
				FROM (SELECT Npr_id, Narah_type_id, year(Narah_date) AS CYear, month(Narah_date) AS CMonth, \
						sum(Suma) AS narahSum \
					FROM likarnyani \
					WHERE year(Cur_date)="+sqlStr(year)+" \
						and month(Cur_date)="+sqlStr(month)+" \
					GROUP BY Npr_id, Narah_type_id, year(Narah_date), month(Narah_date)) AS l \
				LEFT JOIN npr ON npr.id=l.Npr_id \
				WHERE l.narahSum>0 \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk, npr.Iden_kod");
	while ( query.next() ) {
		rowNum++;
		
		dbfFile.BlankRecord();
		dbfFile.PutLongField( "ROWNUM", rowNum );
		dbfFile.PutField( "NUMIDENT", strToxBaseChar(query.value(0).toString()));
		dbfFile.PutField( "LN", strToxBaseChar(query.value(1).toString()));
		dbfFile.PutField( "NM", strToxBaseChar(query.value(2).toString()));
		dbfFile.PutField( "FTN", strToxBaseChar(query.value(3).toString()));
		if (query.value(4).toBool()) { //������
			dbfFile.PutLongField( "ZO", 2 );
			dbfFile.PutLongField( "ZIC", 2 );
		}
		else{
			dbfFile.PutLongField( "ZO", 1 );
			dbfFile.PutLongField( "ZIC", 1 );
		}
		
		dbfFile.PutLongField( "PAY_MNTH", query.value(5).toInt() );
		dbfFile.PutLongField( "PAY_YEAR", query.value(6).toInt() );
		dbfFile.PutLongField( "PAY_TP", query.value(9).toInt() );
		
		dbfFile.PutDoubleField( "SUM_TOTAL", query.value(7).toDouble());
		dbfFile.PutDoubleField( "SUM_MAX", query.value(7).toDouble());
		//dbfFile.PutDoubleField( "SUM_INS", query.value(8).toDouble());
		if (query.value(8).toInt() > 0)
			dbfFile.PutLongField( "OTK", 0);
		else
			dbfFile.PutLongField( "OTK", 1);
		dbfFile.PutLongField( "EXP", 0 );
		
		if (dbfFile.AppendRecord() != XB_NO_ERROR)
			t7DoneOk = false;
	}
	
	//����� ³��������
	query.exec("SELECT npr.Iden_kod, npr.Prizv, npr.Imia, npr.Pobatk, npr.Invalid, \
					v.CMonth, v.CYear, v.narahSum, npr.TypRoboty \
				FROM (SELECT Npr_id, year(Narah_date) AS CYear, month(Narah_date) AS CMonth, sum(Suma) AS narahSum \
					FROM vidpuskni \
					WHERE year(Cur_date)="+sqlStr(year)+" \
						and month(Cur_date)="+sqlStr(month)+" \
					GROUP BY Npr_id, year(Narah_date), month(Narah_date)) AS v \
				LEFT JOIN npr ON npr.id=v.Npr_id \
				WHERE v.narahSum>0 \
				ORDER BY npr.Prizv, npr.Imia, npr.Pobatk, npr.Iden_kod");
	while ( query.next() ) {
		rowNum++;
		
		dbfFile.BlankRecord();
		dbfFile.PutLongField( "ROWNUM", rowNum );
		dbfFile.PutField( "NUMIDENT", strToxBaseChar(query.value(0).toString()));
		dbfFile.PutField( "LN", strToxBaseChar(query.value(1).toString()));
		dbfFile.PutField( "NM", strToxBaseChar(query.value(2).toString()));
		dbfFile.PutField( "FTN", strToxBaseChar(query.value(3).toString()));
		if (query.value(4).toBool()) { //������
			dbfFile.PutLongField( "ZO", 2 );
			dbfFile.PutLongField( "ZIC", 2 );
		}
		else{
			dbfFile.PutLongField( "ZO", 1 );
			dbfFile.PutLongField( "ZIC", 1 );
		}
		
		dbfFile.PutLongField( "PAY_MNTH", query.value(5).toInt() );
		dbfFile.PutLongField( "PAY_YEAR", query.value(6).toInt() );
		dbfFile.PutLongField( "PAY_TP", 3 ); // 3 - ��� ��� ���������
		
		dbfFile.PutDoubleField( "SUM_TOTAL", query.value(7).toDouble());
		dbfFile.PutDoubleField( "SUM_MAX", query.value(7).toDouble());
		//dbfFile.PutDoubleField( "SUM_INS", query.value(8).toDouble());
		if (query.value(8).toInt() > 0)
			dbfFile.PutLongField( "OTK", 0);
		else
			dbfFile.PutLongField( "OTK", 1);
		dbfFile.PutLongField( "EXP", 0 );
		
		if (dbfFile.AppendRecord() != XB_NO_ERROR)
			t7DoneOk = false;
	}
	
	dbfFile.CloseDatabase();
	//------ ʳ���� ������ ������� �7
	
	qDebug() << "t5DoneOk: " << t5DoneOk;
	qDebug() << "t6DoneOk: " << t6DoneOk;
	qDebug() << "t7DoneOk: " << t7DoneOk;
	
	delete postup;
	return t5DoneOk && t6DoneOk && t7DoneOk;
}
//--------------------utech--------------------utech--------------------utech--------------------
