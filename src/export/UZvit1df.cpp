//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл визначення класу створення звіту 1ДФ
//
// Створений програмістами Компанії "Утех" 12.10.2008р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#include "UZvit1df.h"

#include <QtDebug>

//--------------------utech--------------------utech--------------------utech--------------------
UZvit1df::UZvit1df(QObject *parent)
    : QObject(parent)
{
  
}
//--------------------utech--------------------utech--------------------utech--------------------
bool UZvit1df::readHeader()
{
	QString opzPath = getOption(100);
	QString xmlInFileName = opzPath+"/"+OPZ_ORGS_SETTINGS_PATH;
	QFile file(xmlInFileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)){
		qDebug() << "Помилка: програмі не вдалося відкрити для читання файл " << xmlInFileName;
		return false;
	}
	QXmlStreamReader reader(&file);
	do{
		reader.readNext();
		if (reader.atEnd()){
			file.close();
			qDebug() << "Програма не змогла знайти у файлі " << xmlInFileName << " Елемента ROW з атрибутом num=" << getOption(101);
			return false;
		}
	}
	while (!(reader.isStartElement() && (reader.name()=="ROW") && (reader.attributes().value("num") == getOption(101))));
	do{
		reader.readNext();
		if (reader.atEnd()){
			file.close();
			qDebug() << "Неочікуваний для XML парсера кінець файлу "+xmlInFileName;
			return false;
		}
		if (reader.isStartElement() && reader.name()=="C_STI")
			_C_STI = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="TIN")
			_TIN = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="NAME")
			_NAME = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="DFIO")
			_DFIO = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="BFIO")
			_BFIO = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="DINN")
			_DINN = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="BINN")
			_BINN = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="SAVE_PATH")
			_SAVE_PATH = reader.readElementText();
		else if (reader.isStartElement() && reader.name()=="PERSON_TYPE")
			_PERSON_TYPE = reader.readElementText();
	} while (!(reader.isEndElement() && reader.name()=="ROW") && !reader.atEnd());
	_SOFTWARE = "OPZ "+opzVersion(opzPath);
	_HSTI = "1310 "+opzDpiName(opzPath, _C_STI);
	file.close(); 
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
QString UZvit1df::registerFileInOpz(int kvartal, int year, QDate impDate)
{
	//QString xmlLogFileName = "c:\\opz\\log\\30174644_1.log";
	QString xmlLogFileName = _SAVE_PATH + "..\\log\\"+_TIN+".log";
	QFile logFile(xmlLogFileName);
	if (!logFile.open(QFile::ReadOnly | QFile::Text)){
		qDebug() << "Помилка: програмі не вдалося відкрити для читання файл " << xmlLogFileName;
		return false;
	}
	QString inStr = logFile.readAll();
	logFile.close();
	
	if (!logFile.open(QFile::WriteOnly | QFile::Text)){
		qDebug() << "Помилка: програмі не вдалося відкрити для запису файл " << xmlLogFileName;
		return false;
	}
	QString opz1DfDocName;
	
	QString xStr;
	QXmlStreamWriter xmlWriter(&logFile);
	xmlWriter.setAutoFormatting(true);
	QXmlStreamReader xmlReader(inStr);
	short prevDocNumber=0;
	
	//Перша частина групування по датах або DocSet
	bool neadCreateDoc=true;
	bool neadCreateDocPeriod=true;
	xmlWriter.setCodec("windows-1251");
	xmlWriter.writeStartDocument();
	xmlReader.readNext();
	while (!xmlReader.atEnd()){
		xmlReader.readNext();
		
		if (xmlReader.isStartElement() && (xmlReader.name()=="C_DOC")){
			xmlWriter.writeCurrentToken(xmlReader);
			xmlReader.readNext();
			xStr=xmlReader.text().toString();
			xmlWriter.writeCurrentToken(xmlReader);
			if (xStr=="J05"){
				neadCreateDoc=false;
				while(!xmlReader.atEnd()){
					xmlReader.readNext();
					if (xmlReader.isStartElement() && (xmlReader.name()=="PERIOD")){
						xmlWriter.writeCurrentToken(xmlReader);
						bool kvOk=false, yOk=false;
						while (!xmlReader.atEnd()){
							xmlReader.readNext();
							if (xmlReader.isStartElement() && (xmlReader.name()=="PERIOD_MONTH")){
								xmlWriter.writeCurrentToken(xmlReader);
								xmlReader.readNext();
								xStr=xmlReader.text().toString();
								//xmlWriter.writeCurrentToken(xmlReader);
								if (xStr==QString::number(kvartal*3)){
									kvOk = true;
									if (yOk)
										neadCreateDocPeriod = false;
								}
							}
							else if (xmlReader.isStartElement() && (xmlReader.name()=="PERIOD_YEAR")){
								xmlWriter.writeCurrentToken(xmlReader);
								xmlReader.readNext();
								xStr=xmlReader.text().toString();
								//xmlWriter.writeCurrentToken(xmlReader);
								if (xStr==QString::number(year)){
									yOk = true;
									if (kvOk)
										neadCreateDocPeriod = false;
								}
							}
							else if (xmlReader.isEndElement() && (xmlReader.name()=="PERIOD")){
								break;
							}
							xmlWriter.writeCurrentToken(xmlReader);
						}
					}
					if (xmlReader.isEndElement() && (xmlReader.name()=="DOC")){
						if (neadCreateDocPeriod)
							writePeriodBlock(&xmlWriter, kvartal, year);
						xmlWriter.writeCurrentToken(xmlReader);
						break;
					}
					xmlWriter.writeCurrentToken(xmlReader);
				}
			}
		}
		else if (xmlReader.isEndElement() && (xmlReader.name()=="DocSet")){
			if (neadCreateDoc)
				writeDocDeclBlock(&xmlWriter, kvartal, year);
			xmlWriter.writeCurrentToken(xmlReader);
			break;
		}
		else
			xmlWriter.writeCurrentToken(xmlReader);
	}
	
	//Друга частина інформації про файли документів
	short xDocNumber;
	while (!xmlReader.atEnd()){
nextDocInfo:
		xmlReader.readNext();
		if (xmlReader.isStartElement() && (xmlReader.name()=="C_DOC")){
			xmlWriter.writeCurrentToken(xmlReader);
			xmlReader.readNext();
			xStr=xmlReader.text().toString();
			xmlWriter.writeCurrentToken(xmlReader);
			xmlReader.readNext();
			xmlWriter.writeCurrentToken(xmlReader);
			if (xStr=="J05"){
				while (!xmlReader.atEnd()){
					xmlReader.readNext();
					
					if (xmlReader.isStartElement() && (xmlReader.name()=="C_DOC_CNT")){
						xmlWriter.writeCurrentToken(xmlReader);
						xmlReader.readNext();
						xStr=xmlReader.text().toString();
						xDocNumber = xStr.toInt();
						xmlWriter.writeCurrentToken(xmlReader);
						while (!xmlReader.atEnd()){
							xmlReader.readNext();
							
							if (xmlReader.isStartElement() && (xmlReader.name()=="PERIOD_MONTH")){
								xmlWriter.writeCurrentToken(xmlReader);
								xmlReader.readNext();
								xStr=xmlReader.text().toString();
								xmlWriter.writeCurrentToken(xmlReader);
								xmlReader.readNext();
								xmlWriter.writeCurrentToken(xmlReader);
								if (xStr==QString::number(kvartal*3)){
									while (!xmlReader.atEnd()){
										xmlReader.readNext();
										if (xmlReader.isStartElement() && (xmlReader.name()=="PERIOD_YEAR")){
											xmlWriter.writeCurrentToken(xmlReader);
											xmlReader.readNext();
											xStr=xmlReader.text().toString();
											xmlWriter.writeCurrentToken(xmlReader);
											//xmlReader.readNext();
											//xmlWriter.writeCurrentToken(xmlReader);
											if (xStr==QString::number(year)){
												xmlReader.readNext();
												xmlWriter.writeCurrentToken(xmlReader);
												if (prevDocNumber < xDocNumber)
													prevDocNumber = xDocNumber;
											}
											goto nextDocInfo;
										}
										else
											xmlWriter.writeCurrentToken(xmlReader);
									}
								}
								else{
									goto nextDocInfo;
								}
							}
							else if (xmlReader.isEndElement() && (xmlReader.name()=="DocList")){
								xmlWriter.writeCurrentToken(xmlReader);
							}
							else{
								xmlWriter.writeCurrentToken(xmlReader);
							}
						}
					}
					else{
						xmlWriter.writeCurrentToken(xmlReader);
					}
				}
			}
			else{
				goto nextDocInfo;
			}
		}
		else if (xmlReader.isEndElement() && (xmlReader.name()=="DocList")){
			writeDocInfoBlock(&xmlWriter, kvartal, year, prevDocNumber+1);
			opz1DfDocName = opz1DfFileName(kvartal, year, prevDocNumber+1);
			xmlWriter.writeCurrentToken(xmlReader);
			break;
		}
		else
			xmlWriter.writeCurrentToken(xmlReader);
	}
	
	qDebug() << "prevDocNumber: " << prevDocNumber ;
	
	return opz1DfDocName;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool UZvit1df::import1dfToFile(int kvartal, int year, QDate impDate)
{
	if (!readHeader()){
		if (QMessageBox::warning(0,"Помилка","Помилка читання заголовку з програми \"Податкова звітність\"\n"
		"Можливо Ви невірно налаштували шлях до OPZ на сторінці Налаштування->Опції->OPZ\n"
		"Якщо Ви хочете проігнорувати це попередження і продовжити формування звіту натисніть OK",
		QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel)
			return false;
	}
	
	QString xmlOutFileName;
	
	xmlOutFileName = registerFileInOpz(kvartal,year,impDate);
	if (xmlOutFileName.size() == 0)
			return false;
	
	qDebug() << "xmlOutFileName: " << xmlOutFileName;
	
	int val;
	double narahSum=0, utrymSum=0;
	QFile file(xmlOutFileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)){
		qDebug() << "Помилка: програмі не вдалося відкрити для запису файл " << xmlOutFileName;
		return false;
	}
	xmlStream = new QXmlStreamWriter(&file);
	xmlStream->setAutoFormatting(true);
	xmlStream->setCodec("windows-1251");
	xmlStream->writeStartDocument();
	xmlStream->writeStartElement("DECLAR");
	xmlStream->writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
	xmlStream->writeAttribute("xsi:noNamespaceSchemaLocation","J0500102.xsd");
	xmlStream->writeStartElement("DECLARHEAD");
	
	xmlStream->writeTextElement("TIN", _TIN);
	xmlStream->writeTextElement("C_DOC", "J05");
	xmlStream->writeTextElement("C_DOC_SUB", "001");
	xmlStream->writeTextElement("C_DOC_VER", C_DOC_VER);
	xmlStream->writeTextElement("C_DOC_TYPE", "0");
	xmlStream->writeTextElement("C_DOC_CNT", "3");
	xmlStream->writeTextElement("C_REG", _C_STI.left(_C_STI.size()-2));
	xmlStream->writeTextElement("C_RAJ", QString::number(_C_STI.right(2).toInt()));
	xmlStream->writeTextElement("PERIOD_MONTH", QString::number(kvartal*3));
	xmlStream->writeTextElement("PERIOD_TYPE", "2");
	xmlStream->writeTextElement("PERIOD_YEAR", QString::number(year));
	xmlStream->writeTextElement("C_DOC_STAN", "1");
	xmlStream->writeTextElement("D_FILL", impDate.toString("ddMMyyyy"));
	xmlStream->writeTextElement("SOFTWARE", _SOFTWARE);
	
	xmlStream->writeEndElement();
	xmlStream->writeStartElement("DECLARBODY");
	
	QSqlQuery *query = new QSqlQuery();
	
	xmlStream->writeTextElement("HTIN", _TIN);
	xmlStream->writeEmptyElement("HPAGES"); xmlStream->writeAttribute("xsi:nil", "true");
	xmlStream->writeTextElement("HZ", "1");
	if (_PERSON_TYPE.toInt() == 2)
		xmlStream->writeTextElement("HF", "1");
	else
		xmlStream->writeTextElement("HJ", "1");
	xmlStream->writeTextElement("HNAME", _NAME);
	xmlStream->writeEmptyElement("HTINSTI"); xmlStream->writeAttribute("xsi:nil", "true");
	xmlStream->writeTextElement("HSTI", _HSTI);
	xmlStream->writeTextElement("HZP", QString::number(kvartal));
	xmlStream->writeTextElement("HZY", QString::number(year));
	int xPrac = 0;
	for (int iter=0; iter<3; iter++){
		query->exec("SELECT count(*) FROM (SELECT Npr_id FROM operacii op \
					WHERE month(op.CurDate) = "+QString::number(kvartal*3-iter)+" \
						and year(op.CurDate)="+QString::number(year)+" and (TypRoboty=0 or TypRoboty is null) \
						and Pracuye=true and \
						((op.N_Osn_narah+op.N_VuslRokiv+op.N_Likarn+op.N_Vidpuskni+op.N_Svatkovi_dni+ \
							op.N_Nichni+op.N_Nadurochni+op.N_Klasn_vodiya+op.N_Premia+op.N_MaterDopomoga+ \
							op.N_Doplata+op.N_Indexaciya+op.N_VyplProfspilky+op.N_InshiNarahuvannya) > 0.009) \
					GROUP BY Npr_id) AS oper");
		query->seek(0);
		if (xPrac < query->value(0).toInt())
			xPrac = query->value(0).toInt();
	}
	if (xPrac > 0)
		xmlStream->writeTextElement("R00G01I", QString::number(xPrac) );
	else
		xmlStream->writeTextElement("R00G01I", "" );
	xPrac = 0;
	for (int iter=0; iter<3; iter++){
		query->exec("SELECT count(*) FROM (SELECT Npr_id FROM operacii op \
				WHERE (month(op.CurDate)="+QString::number(kvartal*3-iter)+" \
					and year(op.CurDate)="+QString::number(year)+" and TypRoboty=1 and Pracuye=true and \
					((op.N_Osn_narah+op.N_VuslRokiv+op.N_Likarn+op.N_Vidpuskni+op.N_Svatkovi_dni+ \
						op.N_Nichni+op.N_Nadurochni+op.N_Klasn_vodiya+op.N_Premia+op.N_MaterDopomoga+ \
						op.N_Doplata+op.N_Indexaciya+op.N_VyplProfspilky+op.N_InshiNarahuvannya) > 0.009) \
				GROUP BY Npr_id) AS oper");
		query->seek(0);
		if (xPrac < query->value(0).toInt())
			xPrac = query->value(0).toInt();
	}
	if (xPrac > 0)
		xmlStream->writeTextElement("R00G02I", QString::number(xPrac) );
	else
		xmlStream->writeTextElement("R00G02I", "" );
	 //Запис таблиці
	query->exec("SELECT * FROM (SELECT op.OznakaDohodu, npr.Iden_kod, sum(op.N_Osn_narah+op.N_VuslRokiv+op.N_Likarn+op.N_Vidpuskni+op.N_Svatkovi_dni+ \
							op.N_Nichni+op.N_Nadurochni+op.N_Klasn_vodiya+op.N_Premia+op.N_MaterDopomoga+ \
							op.N_Doplata+op.N_Indexaciya+op.N_VyplProfspilky+op.N_InshiNarahuvannya) AS narahSum, \
						sum(op.U_Prybutkov_podatok) AS utrymSum, \
						op.Ozn_pilgy \
					FROM operacii op \
					LEFT JOIN npr ON npr.id=op.Npr_id \
					WHERE (month(op.CurDate) BETWEEN "+QString::number(kvartal*3-2)+" and "+QString::number(kvartal*3)+") \
						and (year(op.CurDate)="+QString::number(year)+") \
					GROUP BY op.OznakaDohodu, npr.Iden_kod, op.Ozn_pilgy) AS oper\
				WHERE narahSum>0.009 or utrymSum>0.009");
	int iter = 0;
	query->seek(-1);
	while (query->next()){
		iter++;
		if (query->value(1).toString().size() > 0)
			writeOpzTableElement("RXXXXG02", query->value(1).toString(),iter);
		else{
			xmlStream->writeEmptyElement("RXXXXG02"); 
			xmlStream->writeAttribute("xsi:nil", "true");
			xmlStream->writeAttribute("ROWNUM", QString::number(iter));
		}
	}
	int rowCount = iter;
	iter = 0;
	query->seek(-1);
	while (query->next()){
		iter++;
		writeOpzTableElement("RXXXXG03A", m_to_str_2_krapka(query->value(2).toDouble()),iter);
		narahSum += query->value(2).toDouble();
	}
	iter = 0;
	query->seek(-1);
	while (query->next()){
		iter++;
		writeOpzTableElement("RXXXXG03", m_to_str_2_krapka(query->value(2).toDouble()),iter);
	}
	iter = 0;
	query->seek(-1);
	while (query->next()){
		iter++;
		writeOpzTableElement("RXXXXG04A", m_to_str_2_krapka(query->value(3).toDouble()),iter);
		utrymSum += query->value(3).toDouble();
	}
	iter = 0;
	query->seek(-1);
	while (query->next()){
		iter++;
		writeOpzTableElement("RXXXXG04", m_to_str_2_krapka(query->value(3).toDouble()),iter);
	}
	iter = 0;
	query->seek(-1);
	while (query->next()){
		iter++;
		val = query->value(0).toInt();
		if (val>0){
			writeOpzTableElement("RXXXXG05", "0"+QString::number(val),iter);
		}
		else{
			xmlStream->writeEmptyElement("RXXXXG05"); 
			xmlStream->writeAttribute("xsi:nil", "true");
			xmlStream->writeAttribute("ROWNUM", QString::number(iter));
		}
	}
	for (iter=1;iter<rowCount; ++iter){
		xmlStream->writeEmptyElement("RXXXXG06D"); 
		xmlStream->writeAttribute("xsi:nil", "true");
		xmlStream->writeAttribute("ROWNUM", QString::number(iter));
	}
	for (iter=1;iter<rowCount; ++iter){
		xmlStream->writeEmptyElement("RXXXXG07D"); 
		xmlStream->writeAttribute("xsi:nil", "true");
		xmlStream->writeAttribute("ROWNUM", QString::number(iter));
	}
	iter = 0;
	query->seek(-1);
	while (query->next()){
		iter++;
		val = query->value(4).toInt();
		if (val>0)
			writeOpzTableElement("RXXXXG08", "0"+QString::number(val),iter);
		else{
			xmlStream->writeEmptyElement("RXXXXG08"); 
			xmlStream->writeAttribute("xsi:nil", "true");
			xmlStream->writeAttribute("ROWNUM", QString::number(iter));
		}
	}
	for (iter=1;iter<rowCount; ++iter)
		writeOpzTableElement("RXXXXG09", "0",iter);
	
	query->exec("SELECT count(*) FROM (SELECT Npr_id, sum(op.N_Osn_narah+op.N_VuslRokiv+op.N_Likarn+op.N_Vidpuskni+op.N_Svatkovi_dni+ \
							op.N_Nichni+op.N_Nadurochni+op.N_Klasn_vodiya+op.N_Premia+op.N_MaterDopomoga+ \
							op.N_Doplata+op.N_Indexaciya+op.N_VyplProfspilky+op.N_InshiNarahuvannya) AS narahSum, \
						sum(op.U_Prybutkov_podatok) AS utrymSum \
					FROM operacii op \
					LEFT JOIN npr ON npr.id=op.Npr_id \
					WHERE (month(op.CurDate) BETWEEN "+QString::number(kvartal*3-2)+" AND "+QString::number(kvartal*3)+") \
						and (year(op.CurDate)="+QString::number(year)+") \
					GROUP BY op.Npr_id) AS oper \
				WHERE narahSum>0.009 or utrymSum>0.009");
	query->seek(0);
	xPrac = query->value(0).toInt();
	
		//Кінець запису таблиці
	xmlStream->writeTextElement("R01G03A", m_to_str_2_krapka(narahSum));
	xmlStream->writeTextElement("R01G03", m_to_str_2_krapka(narahSum));
	xmlStream->writeTextElement("R01G04A", m_to_str_2_krapka(utrymSum));
	xmlStream->writeTextElement("R01G04", m_to_str_2_krapka(utrymSum));
	xmlStream->writeTextElement("R02G01I", QString::number(rowCount));
	if (xPrac > 0)//Кількість фізичних осіб
		xmlStream->writeTextElement("R02G02I", QString::number(xPrac));
	else{
		xmlStream->writeEmptyElement("R02G02I"); xmlStream->writeAttribute("xsi:nil", "true");
	}
	xmlStream->writeEmptyElement("R02G03I"); xmlStream->writeAttribute("xsi:nil", "true");
	xmlStream->writeTextElement("HKBOS", _DINN);
	xmlStream->writeTextElement("HBOS", _DFIO);
	xmlStream->writeTextElement("HTELBOS", KerivnykTel());
	xmlStream->writeTextElement("HKBUH", _BINN);
	xmlStream->writeTextElement("HBUH", _BFIO);
	xmlStream->writeTextElement("HTELBUH", KerivnykTel());
	if (_PERSON_TYPE.toInt() == 2){
		xmlStream->writeTextElement("HFO", _NAME);
		xmlStream->writeTextElement("HTELFO", KerivnykTel());
	}
	else{
		xmlStream->writeEmptyElement("HFO"); 
		xmlStream->writeAttribute("xsi:nil", "true");
		xmlStream->writeEmptyElement("HTELFO"); 
		xmlStream->writeAttribute("xsi:nil", "true");
	}
	xmlStream->writeTextElement("HFILL", impDate.toString("ddMMyyyy"));
	delete query;
	
	xmlStream->writeEndElement();
	xmlStream->writeEndElement();
	xmlStream->writeEndDocument();
	file.close();
	delete xmlStream;
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
void UZvit1df::writeOpzTableElement(QString qualifiedName, QString text, int ROWNUM)
{
	xmlStream->writeStartElement(qualifiedName);
	xmlStream->writeAttribute("ROWNUM", QString::number(ROWNUM));
	xmlStream->writeCharacters(text);
	xmlStream->writeEndElement();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UZvit1df::writeDocDeclBlock(QXmlStreamWriter *writer, int kvartal, int year)
{
/* Вигляд блоку
<DOC>
  <C_DOC>J05</C_DOC> 
  <C_DOC_SUB>001</C_DOC_SUB> 
  <NAME>Податковий розрахунок сум доходу, нарахованого (сплаченого) на користь платників податку, і сум утриманого з них податку (Форма № 1 ДФ)</NAME> 
  <PERIOD>
    <PERIOD_MONTH>3</PERIOD_MONTH> 
    <PERIOD_YEAR>2009</PERIOD_YEAR> 
    <PERIOD_TYPE>2</PERIOD_TYPE> 
  </PERIOD>
</DOC>
*/
	writer->writeStartElement("DOC");
	writer->writeTextElement("C_DOC", "J05");
	writer->writeTextElement("C_DOC_SUB", "001");
	writer->writeTextElement("NAME", "Податковий розрахунок сум доходу, нарахованого (сплаченого) на користь платників податку, і сум утриманого з них податку (Форма № 1 ДФ)");
	writePeriodBlock(writer, kvartal, year);
	writer->writeEndElement();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UZvit1df::writePeriodBlock(QXmlStreamWriter *writer, int kvartal, int year)
{
/* Вигляд блоку
<PERIOD>
  <PERIOD_MONTH>3</PERIOD_MONTH> 
  <PERIOD_YEAR>2009</PERIOD_YEAR> 
  <PERIOD_TYPE>2</PERIOD_TYPE> 
</PERIOD>
*/
	writer->writeStartElement("PERIOD");
	writer->writeTextElement("PERIOD_MONTH", QString::number(kvartal*3));
	writer->writeTextElement("PERIOD_YEAR", QString::number(year));
	writer->writeTextElement("PERIOD_TYPE", "2");
	writer->writeEndElement();
}
//--------------------utech--------------------utech--------------------utech--------------------
void UZvit1df::writeDocInfoBlock(QXmlStreamWriter *writer, int kvartal, int year, int docCounter )
{
/* Вигляд блока
<DOC>
  <C_DOC>J05</C_DOC> 
  <C_DOC_SUB>001</C_DOC_SUB> 
  <C_DOC_VER>2</C_DOC_VER> 
  <C_DOC_STAN>1</C_DOC_STAN> 
  <C_DOC_TYPE>0</C_DOC_TYPE> 
  <C_DOC_CNT>1</C_DOC_CNT> 
  <PERIOD_MONTH>3</PERIOD_MONTH> 
  <PERIOD_YEAR>2009</PERIOD_YEAR> 
  <PERIOD_TYPE>2</PERIOD_TYPE> 
  <D_FILL>16022009</D_FILL> 
  <FILENAME>C:\opz\xml\13100030174644J050010210000001032009.xml</FILENAME> 
</DOC>
  */
	writer->writeStartElement("DOC");
	writer->writeTextElement("C_DOC", "J05");
	writer->writeTextElement("C_DOC_SUB", "001");
	writer->writeTextElement("C_DOC_VER", C_DOC_VER);
	writer->writeTextElement("C_DOC_STAN", "1");
	writer->writeTextElement("C_DOC_TYPE", "0");
	writer->writeTextElement("C_DOC_CNT", QString::number(docCounter));
	writer->writeTextElement("PERIOD_MONTH", QString::number(kvartal*3));
	writer->writeTextElement("PERIOD_YEAR", QString::number(year));
	writer->writeTextElement("PERIOD_TYPE", "2");
	writer->writeTextElement("D_FILL", QDate::currentDate().toString("ddMMyyyy"));
	writer->writeTextElement("FILENAME", opz1DfFileName(kvartal, year, docCounter) );
	writer->writeEndElement();
}
//--------------------utech--------------------utech--------------------utech--------------------
QString UZvit1df::opz1DfFileName(int kvartal, int year, int docCounter)
{
	return _SAVE_PATH
			+ _C_STI.rightJustified(4,'0') + _TIN.rightJustified(10,'0') + "J05001" 
			+ QString(C_DOC_VER).rightJustified(2,'0') + "1" + "00" + QString::number(docCounter).rightJustified(5,'0') 
			+ QString::number(kvartal*3).rightJustified(2,'0') + QString::number(year).rightJustified(4,'0') + ".xml";
}
//--------------------utech--------------------utech--------------------utech--------------------
UZvit1df::~UZvit1df()
{

}
//--------------------utech--------------------utech--------------------utech--------------------
