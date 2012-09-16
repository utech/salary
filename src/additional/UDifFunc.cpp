//--------------------utech--------------------utech--------------------utech--------------------
//
// ���� �������� ���������� ��������� ������� �������� ����������� ��������
//
// ��������� ������������ ������ "����" 17.07.2007�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UDifFunc.h"

#include <QtDebug>
#include <math.h>

//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizShortName()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT ScorochNazvaPidpr FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizName()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT PovnaNazvaPidpr FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizEDRPOU()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT Edrpoy FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizAdresa()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT Adresa FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString KerivnykPib()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT PipDurect FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString KerivnykTel()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT TelefonDurekt FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString BuhgalterPib()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT PipBuh FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString BuhgalterTel()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT TelefonBuh FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizBank()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT Bank FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizBankMFO()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT MFO FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizBankRahunok()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT BankRahunok FROM orgInfo WHERE id=1");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizEMail()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT Email FROM orgInfo");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizFax()
{
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT Fax FROM orgInfo");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizIPN()
{
    QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT IPN FROM orgInfo");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizSvidPDVNum()
{
    QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT SvidPDVNum FROM orgInfo");
	query->seek(0);
	QString rezString = query->value(0).toString();
	delete query;
	return rezString;
}
//--------------------utech--------------------utech--------------------utech--------------------
/*QString encrypt(QString str)
{
	return str.toLocal8Bit().toBase64();
}
//--------------------utech--------------------utech--------------------utech--------------------
QString decrypt(QString str)
{
	return QByteArray::fromBase64(str.toLocal8Bit());
}*/
//--------------------utech--------------------utech--------------------utech--------------------
QString tabelResString(int workerId, int curYear, int curMonth, int vydTabelya, int typOplaty)
{
	QSqlQuery query;
	
	int numWDaysInMonth=0, numWGodInMonth=0;
	query.exec("SELECT WorkDaysInMonth, WorkHoursInMonth FROM budjetni_normy_monthly \
				WHERE year(CurDate)="+QString::number(curYear)+" and month(CurDate)="+QString::number(curMonth));
	if (query.seek(0)){
		numWDaysInMonth = query.value(0).toInt();
		numWGodInMonth = query.value(1).toInt();
	}
	
	if (vydTabelya == 0){ // ������ ������
		if(typOplaty== 1){ //��������� ������
			query.exec("SELECT sum(TIME_TO_SEC(God_zag)), sum(TIME_TO_SEC(God_nichni)), \
							sum(TIME_TO_SEC(God_sv)) FROM tabel \
						WHERE Npr_ID="+QString::number(workerId)+" \
							and year(Cur_date) = "+QString::number(curYear)+" \
							and month(Cur_date)= "+QString::number(curMonth));
			if (query.seek(0))
				return "���: ���-"+QString::number(query.value(0).toInt()/3600)+":"+QString::number(fmod(query.value(0).toInt(),3600)/60)
						+"; ��-"+QString::number(query.value(1).toInt()/3600)+":"+QString::number(fmod(query.value(1).toInt(),3600)/60)
						+"; ��-"+QString::number(query.value(2).toInt()/3600)+":"+QString::number(fmod(query.value(2).toInt(),3600)/60)
						+"; ����-"+QString::number(numWGodInMonth);
			else
				return "�� ������.";
		}
		else{ //�������� ������
			query.exec("SELECT Work_days_sum FROM tabel_sum \
						WHERE Npr_ID="+QString::number(workerId)+" \
							and year(Cur_date) = "+QString::number(curYear)+" \
							and month(Cur_date)= "+QString::number(curMonth));
			if (query.seek(0))
				return "��: ���-"+query.value(0).toString()+"; ����-"+QString::number(numWDaysInMonth);
			else
				return "�� ������.";
		}
	}
	else if (vydTabelya == 1){ //�������� ������
		if(typOplaty== 1){ // ��������� ������
			query.exec("SELECT God_zag_sum, God_nichni_sum, \
							God_sv_sum FROM tabel_sum \
						WHERE Npr_ID="+QString::number(workerId)+" \
							and year(Cur_date) = "+QString::number(curYear)+" \
							and month(Cur_date)= "+QString::number(curMonth));
			if (query.seek(0))
				return "���: ���-"+query.value(0).toString()+"; ��-"+query.value(1).toString()+"; ��-"+query.value(2).toString()+"; ����-"+QString::number(numWGodInMonth);
			else
				return "�� ������.";
		}
		else{ //�������� ������
			query.exec("SELECT Work_days_sum \
						FROM tabel_sum \
						WHERE Npr_ID = "+QString::number(workerId)+" \
							and year(Cur_date) = "+QString::number(curYear)+" \
							and month(Cur_date)= "+QString::number(curMonth));
			if (query.seek(0))
				return "��: ���-"+query.value(0).toString()+"; ����-"+QString::number(numWDaysInMonth);
			else
				return "�� ������.";
		}
	}
	else{
		return "�� ������.";
	}
}
//--------------------utech--------------------utech--------------------utech--------------------
