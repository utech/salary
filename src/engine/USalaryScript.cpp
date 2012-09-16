//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл виконання класу нарахування зарплати 
// з допомогою скриптів для програми нарахування зарплати
// Створений програмістами Компанії "Утех" 9.12.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#include <QSqlError>

#include "USalaryScript.h"
#include <USqlAccessible>

QHash<int, double> budjNormyHash;

//--------------------utech--------------------utech--------------------utech--------------------
QScriptValue progOption(QScriptContext *sC=0, QScriptEngine *sE=0)
{
	if (sC->argumentCount()>0){
		QSqlQuery query("SELECT Znach FROM progoptions WHERE id="+mysqlStr(sC->argument(0).toInt32()));
		if (query.seek(0))
			return QScriptValue(sE, query.value(0).toString());
		else
			return QScriptValue(sE, -1);
	}
	else
		return QScriptValue(sE, -1);
}
//--------------------utech--------------------utech--------------------utech--------------------
QScriptValue budjNorma(QScriptContext *sC=0, QScriptEngine *sE=0)
{
	if (sC->argumentCount()>0){
		return QScriptValue(sE, budjNormyHash.value(sC->argument(0).toInt32(),0));
	}
	else
		return QScriptValue(sE, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------
QScriptValue sumaSocPilgy(QScriptContext *sC=0, QScriptEngine *sE=0)
{
	if (sC->argumentCount()>0){
		int bN = sC->argument(0).toInt32();
		if (bN>0 && bN<5)
			return QScriptValue(sE, budjNormyHash.value(bN,0));
		else
			return QScriptValue(sE, 0);
	}
	else
		return QScriptValue(sE, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------
QScriptValue workDays(QScriptContext *sC=0, QScriptEngine *sE=0)
//Перший аргумент рік, другий місяць
{
	if (sC->argumentCount()>1){
		QSqlQuery *query = new QSqlQuery();
		query->exec("SELECT day(numDays) FROM workdaysinmonth \
						WHERE year(numDays)="+sC->argument(0).toString()+" \
							and month(numDays)="+sC->argument(1).toString());
		query->seek(0);
		int _numDays = query->value(0).toInt();
		return QScriptValue(sE, _numDays);
	}
	else
		return QScriptValue(sE, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------
QScriptValue d2m(QScriptContext *sC=0, QScriptEngine *sE=0)
{
	if (sC->argumentCount()>0)
		return QScriptValue(sE, d_to_m(sC->argument(0).toNumber()));
	else
		return QScriptValue(sE, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------
QScriptValue print(QScriptContext *sC=0, QScriptEngine *sE=0)
{
	if (sC->argumentCount()>0){
		int argCount = sC->argumentCount();
		QString str;
		for (int arg=0; arg<argCount; arg++)
			str += sC->argument(arg).toString();
		qDebug() << str;
		return QScriptValue(sE, str);
	}
	else
		return QScriptValue(sE, "");
}
//--------------------utech--------------------utech--------------------utech--------------------


QScriptValue hour(QScriptContext *sC=0, QScriptEngine *sE=0)
{
	if (sC->argumentCount()>0){
		QTime tm = qscriptvalue_cast<QTime>(sC->argument(0));
		if (tm.isValid())
			return QScriptValue(sE, tm.hour());
	}
	return QScriptValue(sE, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------
QScriptValue minute(QScriptContext *sC=0, QScriptEngine *sE=0)
{
	if (sC->argumentCount()>0){
		QTime tm = qscriptvalue_cast<QTime>(sC->argument(0));
		if (tm.isValid())
			return QScriptValue(sE, tm.minute());
	}
	return QScriptValue(sE, 0);
}
//--------------------utech--------------------utech--------------------utech--------------------


bool UNeadCalcNarahValues::populateCalcNarahValuesForOD(short oznDoh)
{
	QSqlQuery query;
	if (!query.exec("SELECT For_U_PensFond, For_U_FondZainat, For_U_SocStrah, For_U_PrybutkovyiPodatok, \
						For_U_VidrahProfspilka, For_U_InshiVidrahuvannya, \
						For_O_NaRuky, For_V_PensFond, For_V_SocStrah, For_V_FondZainat, \
						For_V_NeshchVyp, For_V_Inshi \
				FROM oznakydohodu \
				WHERE id="+mysqlStr(oznDoh)+" LIMIT 1"))
		return false;
	if (!query.seek(0))
		return false;
	__For_U_PensFond = query.value(0).toBool();
	__For_U_FondZainat = query.value(1).toBool();
	__For_U_SocStrah = query.value(2).toBool();
	__For_U_PrybutkovyiPodatok = query.value(3).toBool();
	__For_U_VidrahProfspilka = query.value(4).toBool();
	__For_U_InshiVidrahuvannya = query.value(5).toBool();
	__For_O_NaRuky = query.value(6).toBool();
	__For_V_PensFond = query.value(7).toBool();
	__For_V_SocStrah = query.value(8).toBool();
	__For_V_FondZainat = query.value(9).toBool();
	__For_V_NeshchVyp = query.value(10).toBool();
	__For_V_Inshi = query.value(11).toBool();
	
	__oznakaDohodu = oznDoh;
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
#include <QLineEdit>
Q_SCRIPT_DECLARE_QMETAOBJECT(USqlQuery, QWidget*)

USalaryScript::USalaryScript()
{
	int i;
	populateMonthlyBudjetniNormy_done = false;
	
	wInfo = new UWorkerInfo();
	workerInfoSV = salaryScriptEngine.newQObject(wInfo);
	salaryScriptEngine.globalObject().setProperty("wInfo", workerInfoSV);
	
	workerTabelSV = salaryScriptEngine.newArray(32);
	salaryScriptEngine.globalObject().setProperty("wTabel", workerTabelSV);
	for (i=0; i<32; i++){
		scriptValue = salaryScriptEngine.newQObject(&wTabel[i]);
		workerTabelSV.setProperty(i, scriptValue);
	}
	
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) FROM budjetni_normu");
	query->seek(0);
	budjetniNormyCount = query->value(0).toInt();
	budjetniNormy = new UIdVal[budjetniNormyCount];
	
	scriptValue = QScriptValue(&salaryScriptEngine, budjetniNormyCount);
	salaryScriptEngine.globalObject().setProperty("budjetniNormyCount", scriptValue);
	
	query->exec("SELECT id, znach FROM budjetni_normu");
	budjNormyHash.clear();
	for (i=0; i<budjetniNormyCount; i++){
		query->next();
		budjetniNormy[i].__id = query->value(0).toInt();
		budjetniNormy[i].__val = query->value(1).toDouble();
		budjNormyHash.insert(budjetniNormy[i].__id, budjetniNormy[i].__val);
	}
	delete query;
	
	workerTabelSV = salaryScriptEngine.newArray(budjetniNormyCount);
	salaryScriptEngine.globalObject().setProperty("budjetniNormy", workerTabelSV);
	for (i=0; i<budjetniNormyCount; i++){
		scriptValue = salaryScriptEngine.newQObject(&budjetniNormy[i]);
		workerTabelSV.setProperty(i, scriptValue);
	}
	
	mbn = new UBudjetniNormyMonthly[13];
	workerTabelSV = salaryScriptEngine.newArray(13);
	salaryScriptEngine.globalObject().setProperty("mbn", workerTabelSV);
	for (i=0; i<13; i++){
		scriptValue = salaryScriptEngine.newQObject(&mbn[i]);
		workerTabelSV.setProperty(i, scriptValue);
	}
	
	workerOperacii = new UWorkerOperacii[13];
	workerTabelSV = salaryScriptEngine.newArray(13);
	salaryScriptEngine.globalObject().setProperty("wOperacii", workerTabelSV);
	for (i=0; i<13; i++){
		scriptValue = salaryScriptEngine.newQObject(&workerOperacii[i]);
		workerTabelSV.setProperty(i, scriptValue);
	}
	
	wAdOperacii = new UWorkerOperacii[13];
	workerTabelSV = salaryScriptEngine.newArray(13);
	salaryScriptEngine.globalObject().setProperty("wAdOperacii", workerTabelSV);
	for (i=0; i<13; i++){
		scriptValue = salaryScriptEngine.newQObject(&wAdOperacii[i]);
		workerTabelSV.setProperty(i, scriptValue);
	}
	
	wOplata = new UWorkerOplata();
	workerInfoSV = salaryScriptEngine.newQObject(wOplata);
	salaryScriptEngine.globalObject().setProperty("wOplata", workerInfoSV);
	
	neadCalcNarahValues = new UNeadCalcNarahValues();
	workerInfoSV = salaryScriptEngine.newQObject(neadCalcNarahValues);
	salaryScriptEngine.globalObject().setProperty("uNeadCalcNarahValues", workerInfoSV);
	
	sumTabel = new USumTabel();
	workerInfoSV = salaryScriptEngine.newQObject(sumTabel);
	salaryScriptEngine.globalObject().setProperty("sumTabel", workerInfoSV);
	
	opt = new UProgramOptions();
	workerInfoSV = salaryScriptEngine.newQObject(opt);
	salaryScriptEngine.globalObject().setProperty("opt", workerInfoSV);
	
	//Оголошення функцій
	workerTabelSV = salaryScriptEngine.newFunction(progOption);
	salaryScriptEngine.globalObject().setProperty("progOption", workerTabelSV);
	
	workerTabelSV = salaryScriptEngine.newFunction(budjNorma);
	salaryScriptEngine.globalObject().setProperty("budjNorma", workerTabelSV);
	
	workerTabelSV = salaryScriptEngine.newFunction(sumaSocPilgy);
	salaryScriptEngine.globalObject().setProperty("sumaSocPilgy", workerTabelSV);
	
	workerTabelSV = salaryScriptEngine.newFunction(workDays);
	salaryScriptEngine.globalObject().setProperty("workDays", workerTabelSV);
	
	workerTabelSV = salaryScriptEngine.newFunction(d2m);
	salaryScriptEngine.globalObject().setProperty("d2m", workerTabelSV);
	
	workerTabelSV = salaryScriptEngine.newFunction(print);
	salaryScriptEngine.globalObject().setProperty("print", workerTabelSV);
	
	workerTabelSV = salaryScriptEngine.newFunction(minute);
	salaryScriptEngine.globalObject().setProperty("minute", workerTabelSV);
	
	workerTabelSV = salaryScriptEngine.newFunction(hour);
	salaryScriptEngine.globalObject().setProperty("hour", workerTabelSV);
	
	//Створення метаоб'єктних класів для скрипта
	QScriptValue scriptValue = salaryScriptEngine.scriptValueFromQMetaObject<USqlQuery>();
	salaryScriptEngine.globalObject().setProperty("USqlQuery", scriptValue);
}
//--------------------utech--------------------utech--------------------utech--------------------
void USalaryScript::populateWorkerInfo(QDate date, int workerID, short oznakaDohodu)
{
	QSqlQuery query;
	//Ініціалізація частини з таблиці npr
	query.exec("SELECT Data_nar, Posada_id, TypRoboty, TypOplaty, Rozryad, Stavka, Num_dit, \
					Ozn_pilgy, Data_pruin_na_rob, Klasnist_vodiya, VidsDoplaty, Alimenty, \
					Pensioner, Buh_rahunok, ChlenProfspilky, Invalid, Pracuye, Pidrozdil_id, \
					Gender \
				FROM npr \
				WHERE id="+QVariant(workerID).toString());
	query.seek(0);
	wInfo->__Npr_ID = workerID;
	wInfo->__Data_nar = query.value(0).toDate();
	wInfo->__Posada_id = query.value(1).toInt();
	wInfo->__TypRoboty = query.value(2).toInt();
	wInfo->__TypOplaty = query.value(3).toInt();
	wInfo->__Rozryad = query.value(4).toInt();
	wInfo->__Stavka = query.value(5).toDouble();
	wInfo->__Num_dit = query.value(6).toInt();
	wInfo->__Ozn_pilgy = query.value(7).toInt();
	wInfo->__Data_pruin_na_rob = query.value(8).toDate();
	wInfo->__Klasnist_vodiya = query.value(9).toDouble();
	wInfo->__VidsDoplaty = query.value(10).toDouble();
	wInfo->__Alimenty = query.value(11).toDouble();
	wInfo->__Pensioner = query.value(12).toBool();
	wInfo->__Buh_rahunok = query.value(13).toInt();
	wInfo->__ChlenProfspilky = query.value(14).toBool();
	wInfo->__Invalid = query.value(15).toBool();
	wInfo->__Pracuye = query.value(16).toBool();
	wInfo->__Pidrozdil_id = query.value(17).toInt();
	wInfo->__Gender = query.value(18).toInt();
	
	//Ініціалізація частини з таблиці tabel
	query.exec("SELECT day(Cur_date), Pracivn_status_ID, God_zag, \
					God_nichni, God_sv  \
				FROM tabel \
				WHERE Npr_ID="+QVariant(workerID).toString()+" \
					and year(Cur_date)="+QVariant(date.year()).toString()+" \
					and month(Cur_date)="+QVariant(date.month()).toString());
	for (int i=0; i<32; i++){
		wTabel[i].__Pracivn_status_ID=0;
		wTabel[i].__God_zag=QTime();
		wTabel[i].__God_nichni=QTime();
		wTabel[i].__God_sv=QTime();
	}
	while(query.next()){
		int curDay = query.value(0).toInt();
		wTabel[curDay].__Pracivn_status_ID=query.value(1).toInt();
		wTabel[curDay].__God_zag=query.value(2).toTime();
		wTabel[curDay].__God_nichni=query.value(3).toTime();
		wTabel[curDay].__God_sv=query.value(4).toTime();
	}
	//Ініціалізація частини з таблиці vusluga_rokiv
	int staj = date.year() - wInfo->__Data_pruin_na_rob.year() - 1;
	if ((date.month()-wInfo->__Data_pruin_na_rob.month())>0){
		staj++;
	}
	else if (((date.month() - wInfo->__Data_pruin_na_rob.month())==0) and ((date.day()-wInfo->__Data_pruin_na_rob.day())>=0)){
		staj++;
	}
	if (staj<0)
		staj=0;
	wInfo->__Staj_roboty = staj;
	query.exec("SELECT Vidsotky FROM vusluga_rokiv WHERE Befour_year<="+QVariant(staj).toString()+" ORDER BY Befour_year DESC");
	query.seek(0);
	wInfo->__Vids_vuslugy_rokiv = query.value(0).toDouble();
	
	//Ініціалізація частини підсумкового табеля - sumTabel
	query.exec("SELECT Work_days_sum, Likarn_days_sum, Vidpusk_days_sum, God_zag_sum, God_nichni_sum, God_sv_sum \
				FROM tabel_sum \
				WHERE Npr_ID="+QString::number(workerID)+" \
					and year(Cur_date)="+QString::number(date.year())+" \
					and month(Cur_date)="+QString::number(date.month()));
	if (query.seek(0)){
		sumTabel->__Work_days_sum = query.value(0).toInt();
		sumTabel->__Likarn_days_sum = query.value(1).toInt();
		sumTabel->__Vidpusk_days_sum = query.value(2).toInt();
		sumTabel->__God_zag_sum = query.value(3).toDouble();
		sumTabel->__God_nichni_sum = query.value(4).toDouble();
		sumTabel->__God_sv_sum = query.value(5).toDouble();
	}
	else{
		sumTabel->__Work_days_sum = 0;
		sumTabel->__Likarn_days_sum = 0;
		sumTabel->__Vidpusk_days_sum = 0;
		sumTabel->__God_zag_sum = 0;
		sumTabel->__God_nichni_sum = 0;
		sumTabel->__God_sv_sum = 0;
	}
	
	//Ініціалізація типу індексації у місяці (базовий місяць, звичайний місяць, ін)
	query.exec("SELECT Status FROM statusIndexaciiPracivnykiv  WHERE Npr_id="+mysqlStr(workerID)+" \
					and year(CurDate)="+mysqlStr(date.year())+" \
					and month(CurDate)="+mysqlStr(date.month()));
	if (query.seek(0))
		wInfo->__IndexaciyaType = query.value(0).toInt();
	else
		wInfo->__IndexaciyaType = 0;
	
	//Ініціалізація типу табеля для працівника
	query.exec("SELECT TabelType FROM pidrozdily_salary WHERE id="+QString::number(wInfo->__Pidrozdil_id));
	if (query.next()){
		int tabelType = query.value(0).toInt();
		if (tabelType == 0) //визначається опціями програми
			wInfo->__VydTabelya = USalaryOptions::getOption(1).toInt();
		else
			wInfo->__VydTabelya = tabelType-1;
	}
	else //немає підрозділу, визначається опціями програми
		wInfo->__VydTabelya = USalaryOptions::getOption(1).toInt();
}
//--------------------utech--------------------utech--------------------utech--------------------
void USalaryScript::populateWorkerOperaciiInfo(QDate date, int workerID, short oznakaDohodu)
{
	QSqlQuery *query = new QSqlQuery();
	//Встановлення попередніх нарахувань в 0,00
	workerOperacii[0].clear();
	workerOperacii[0].__CurDate = date;

	//Ініціалізація частини з таблиці operacii
	query->exec("SELECT id, CurDate, Vh_saldo, N_Osn_narah, N_VuslRokiv, N_Likarn, N_Vidpuskni, N_Svatkovi_dni, N_Nichni,N_Nadurochni,\
					N_Klasn_vodiya, N_Premia, N_MaterDopomoga, N_Doplata, N_Indexaciya, N_VyplProfspilky, N_InshiNarahuvannya,\
					U_PensFond, U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, U_VidrahProfspilka, \
					U_InshiVidrahuvannya, O_Inshi, O_Shtraf, O_Alimenty, O_ZaPoperMis, O_Premiya, O_VsogoNaRuky, O_Avans, O_Oplacheno, \
					V_PensFond, V_SocStrah, V_Fond_zainat, V_NeshchVyp, V_Inshi,\
					Vids_U_PensFond, Vids_U_Fond_zainat, Vids_U_Socstrah, Vids_U_SocPilga, Vids_U_Prybutkov_podatok, \
					Vids_U_VidrahProfspilka, Vids_U_InshiVidrahuvannya, Vids_V_PensFond, Vids_V_SocStrah, \
					Vids_V_Fond_zainat, Vids_V_NeshchVyp, Vids_V_Inshi, Stavka, I_PromIndex, I_Index, I_ProindexovanaSuma, \
					I_SumaDlyaIndexacii, I_BaseMonth, \
					U_EV_Likarn, U_EV_Vidpuskni, V_EV_Likarn, V_EV_Vidpuskni, Vids_U_EV_Likarn, Vids_U_EV_Vidpuskni, \
					Vids_V_EV_Likarn, Vids_V_EV_Vidpuskni \
				FROM operacii \
				WHERE OznakaDohodu="+QString::number(oznakaDohodu)+" and Npr_ID="+QVariant(workerID).toString()+" and \
					year(CurDate)="+mysqlStr(date.year())+" and month(CurDate)="+mysqlStr(date.month()));
	if (query->seek(0)){
		workerOperacii[0].__OznakaDohodu = oznakaDohodu;
		workerOperacii[0].__id = query->value(0).toInt();
		//workerOperacii[0].__CurDate = query->value(1).toDate(); //Записується на початку функції
		
		workerOperacii[0].__Vh_saldo = query->value(2).toDouble();
		workerOperacii[0].__N_Osn_narah = query->value(3).toDouble();
		workerOperacii[0].__N_VuslRokiv = query->value(4).toDouble();
		workerOperacii[0].__N_Likarn = query->value(5).toDouble();
		workerOperacii[0].__N_Vidpuskni = query->value(6).toDouble();
		workerOperacii[0].__N_Svatkovi_dni = query->value(7).toDouble();
		workerOperacii[0].__N_Nichni = query->value(8).toDouble();
		workerOperacii[0].__N_Nadurochni = query->value(9).toDouble();
		workerOperacii[0].__N_Klasn_vodiya = query->value(10).toDouble();
		workerOperacii[0].__N_Premia = query->value(11).toDouble();
		workerOperacii[0].__N_MaterDopomoga = query->value(12).toDouble();
		workerOperacii[0].__N_Doplata = query->value(13).toDouble();
		workerOperacii[0].__N_Indexaciya = query->value(14).toDouble();
		workerOperacii[0].__N_VyplProfspilky = query->value(15).toDouble();
		workerOperacii[0].__N_InshiNarahuvannya = query->value(16).toDouble();
		workerOperacii[0].__U_PensFond = query->value(17).toDouble();
		workerOperacii[0].__U_Fond_zainat = query->value(18).toDouble();
		workerOperacii[0].__U_SocStrah = query->value(19).toDouble();
		workerOperacii[0].__U_SocPilga = query->value(20).toDouble();
		workerOperacii[0].__U_Prybutkov_podatok = query->value(21).toDouble();
		workerOperacii[0].__U_VidrahProfspilka = query->value(22).toDouble();
		workerOperacii[0].__U_InshiVidrahuvannya = query->value(23).toDouble();
		workerOperacii[0].__O_Inshi = query->value(24).toDouble();
		workerOperacii[0].__O_Shtraf = query->value(25).toDouble();
		workerOperacii[0].__O_Alimenty = query->value(26).toDouble();
		workerOperacii[0].__O_ZaPoperMis = query->value(27).toDouble();
		workerOperacii[0].__O_Premiya = query->value(28).toDouble();
		workerOperacii[0].__O_VsogoNaRuky = query->value(29).toDouble();
		workerOperacii[0].__O_Avans = query->value(30).toDouble();
		workerOperacii[0].__O_Oplacheno = query->value(31).toDouble();
		
		workerOperacii[0].__V_PensFond = query->value(32).toDouble();
		workerOperacii[0].__V_SocStrah = query->value(33).toDouble();
		workerOperacii[0].__V_Fond_zainat = query->value(34).toDouble();
		workerOperacii[0].__V_NeshchVyp = query->value(35).toDouble();
		workerOperacii[0].__V_Inshi = query->value(36).toDouble();
		
		workerOperacii[0].__Vids_U_PensFond = query->value(37).toDouble();
		workerOperacii[0].__Vids_U_Fond_zainat = query->value(38).toDouble();
		workerOperacii[0].__Vids_U_Socstrah = query->value(39).toDouble();
		workerOperacii[0].__Vids_U_SocPilga = query->value(40).toDouble();
		workerOperacii[0].__Vids_U_Prybutkov_podatok = query->value(41).toDouble();
		workerOperacii[0].__Vids_U_VidrahProfspilka = query->value(42).toDouble();
		workerOperacii[0].__Vids_U_InshiVidrahuvannya = query->value(43).toDouble();
		workerOperacii[0].__Vids_V_PensFond = query->value(44).toDouble();
		workerOperacii[0].__Vids_V_SocStrah = query->value(45).toDouble();
		workerOperacii[0].__Vids_V_Fond_zainat = query->value(46).toDouble();
		workerOperacii[0].__Vids_V_NeshchVyp = query->value(47).toDouble();
		workerOperacii[0].__Vids_V_Inshi = query->value(48).toDouble();
		
		workerOperacii[0].__Oklad = query->value(49).toDouble();
		workerOperacii[0].__I_PromIndex = query->value(50).toDouble();
		workerOperacii[0].__I_Index = query->value(51).toDouble();
		workerOperacii[0].__I_ProindexovanaSuma = query->value(52).toDouble();
		workerOperacii[0].__I_SumaDlyaIndexacii = query->value(53).toDouble();
		workerOperacii[0].__I_BaseMonth = query->value(54).toBool();
		
		workerOperacii[0].__U_EV_Likarn = query->value(55).toDouble();
		workerOperacii[0].__U_EV_Vidpuskni = query->value(56).toDouble();
		workerOperacii[0].__V_EV_Likarn = query->value(57).toDouble();
		workerOperacii[0].__V_EV_Vidpuskni = query->value(58).toDouble();
		workerOperacii[0].__Vids_U_EV_Likarn = query->value(59).toDouble();
		workerOperacii[0].__Vids_U_EV_Vidpuskni = query->value(60).toDouble();
		workerOperacii[0].__Vids_V_EV_Likarn = query->value(61).toDouble();
		workerOperacii[0].__Vids_V_EV_Vidpuskni = query->value(62).toDouble();
	}
	
	query->exec("SELECT sum(Suma), count(*) FROM likarnyani \
				WHERE Npr_ID="+QVariant(workerID).toString()+" \
					and year(Cur_date)="+mysqlStr(date.year())+" \
					and month(Cur_date)="+mysqlStr(date.month()));
	if (query->seek(0)){
		workerOperacii[0].__N_LikarnExt = query->value(0).toDouble();
		workerOperacii[0].__N_LikarnExtCount = query->value(1).toInt();
	}
	else{
		workerOperacii[0].__N_LikarnExt = 0;
		workerOperacii[0].__N_LikarnExtCount = 0;
	}
	
	query->exec("SELECT sum(Suma), count(*) FROM vidpuskni \
				WHERE Npr_ID="+QVariant(workerID).toString()+" \
					and year(Cur_date)="+mysqlStr(date.year())+" \
					and month(Cur_date)="+mysqlStr(date.month()));
	if (query->seek(0)){
		workerOperacii[0].__N_VidpuskniExt = query->value(0).toDouble();
		workerOperacii[0].__N_VidpuskniExtCount = query->value(1).toInt();
	}
	else{
		workerOperacii[0].__N_VidpuskniExt = 0;
		workerOperacii[0].__N_VidpuskniExtCount = 0;
	}
	
	delete query;
}
//--------------------utech--------------------utech--------------------utech--------------------
void USalaryScript::populateWorkerOperaciiHistoryInfo(QDate date, int workerID)
{
	QSqlQuery *query = new QSqlQuery();
	QDate xDate=date;
	int iter;
	//Встановлення попередніх нарахувань в 0,00
	for (iter=1; iter<13; iter++){
		workerOperacii[iter].clear();
		xDate = xDate.addMonths(-1);
	}
	
	//Ініціалізація частини з таблиці operacii де ОД=1
	xDate = date.addMonths(-12);
	QDate yDate = date.addMonths(-1);
	if (!query->exec("SELECT CurDate, Vh_saldo, N_Osn_narah, N_VuslRokiv, N_Likarn, N_Vidpuskni, N_Svatkovi_dni, N_Nichni,N_Nadurochni, \
					N_Klasn_vodiya, N_Premia, N_MaterDopomoga, N_Doplata, N_Indexaciya, N_VyplProfspilky, N_InshiNarahuvannya, \
					U_PensFond, U_Fond_zainat, U_SocStrah, U_SocPilga, U_Prybutkov_podatok, U_VidrahProfspilka, \
					U_InshiVidrahuvannya, O_Inshi, O_Shtraf, O_Alimenty, O_ZaPoperMis, O_Premiya, O_VsogoNaRuky, O_Avans, O_Oplacheno, \
					V_PensFond, V_SocStrah, V_Fond_zainat, V_NeshchVyp, V_Inshi, \
					Vids_U_PensFond, Vids_U_Fond_zainat, Vids_U_Socstrah, Vids_U_SocPilga, Vids_U_Prybutkov_podatok, \
					Vids_U_VidrahProfspilka, Vids_U_InshiVidrahuvannya, Vids_V_PensFond, Vids_V_SocStrah, \
					Vids_V_Fond_zainat, Vids_V_NeshchVyp, Vids_V_Inshi, Stavka, I_PromIndex, I_Index, I_ProindexovanaSuma, \
					I_SumaDlyaIndexacii, I_BaseMonth, \
					U_EV_Likarn, U_EV_Vidpuskni, V_EV_Likarn, V_EV_Vidpuskni, Vids_U_EV_Likarn, Vids_U_EV_Vidpuskni, \
					Vids_V_EV_Likarn, Vids_V_EV_Vidpuskni \
				FROM operacii \
				WHERE OznakaDohodu=1 and Npr_ID="+QVariant(workerID).toString()+" and \
					CurDate BETWEEN '"+xDate.toString("yyyy-MM")+"-1' and '"+yDate.toString("yyyy-MM")+"-"+QVariant(yDate.daysInMonth()).toString()+"' \
				GROUP BY CurDate"))
		qDebug() << "Помилка читання історії доходів з ОД=01 (Помилка запиту SQL).\nПричина: " <<query->lastError().text() << "\nЗапит: "+query->lastQuery();
	while (query->next()){
		xDate = query->value(0).toDate();
		int month1, month2;
		month1=xDate.month();
		month2=date.month();
		if ((month2==month1) && (xDate.year()==date.year()))
			iter=0;
		else if ((month2-month1)>0)
			iter = month2 - month1;
		else
			iter = 12 + month2 - month1;
			
		workerOperacii[iter].__CurDate = query->value(0).toDate();

		workerOperacii[iter].__Vh_saldo = query->value(1).toDouble();
		workerOperacii[iter].__N_Osn_narah = query->value(2).toDouble();
		workerOperacii[iter].__N_VuslRokiv = query->value(3).toDouble();
		workerOperacii[iter].__N_Likarn = query->value(4).toDouble();
		workerOperacii[iter].__N_Vidpuskni = query->value(5).toDouble();
		workerOperacii[iter].__N_Svatkovi_dni = query->value(6).toDouble();
		workerOperacii[iter].__N_Nichni = query->value(7).toDouble();
		workerOperacii[iter].__N_Nadurochni = query->value(8).toDouble();
		workerOperacii[iter].__N_Klasn_vodiya = query->value(9).toDouble();
		workerOperacii[iter].__N_Premia = query->value(10).toDouble();
		workerOperacii[iter].__N_MaterDopomoga = query->value(11).toDouble();
		workerOperacii[iter].__N_Doplata = query->value(12).toDouble();
		workerOperacii[iter].__N_Indexaciya = query->value(13).toDouble();
		workerOperacii[iter].__N_VyplProfspilky = query->value(14).toDouble();
		workerOperacii[iter].__N_InshiNarahuvannya = query->value(15).toDouble();
		workerOperacii[iter].__U_PensFond = query->value(16).toDouble();
		workerOperacii[iter].__U_Fond_zainat = query->value(17).toDouble();
		workerOperacii[iter].__U_SocStrah = query->value(18).toDouble();
		workerOperacii[iter].__U_SocPilga = query->value(19).toDouble();
		workerOperacii[iter].__U_Prybutkov_podatok = query->value(20).toDouble();
		workerOperacii[iter].__U_VidrahProfspilka = query->value(21).toDouble();
		workerOperacii[iter].__U_InshiVidrahuvannya = query->value(22).toDouble();
		workerOperacii[iter].__O_Inshi = query->value(23).toDouble();
		workerOperacii[iter].__O_Shtraf = query->value(24).toDouble();
		workerOperacii[iter].__O_Alimenty = query->value(25).toDouble();
		workerOperacii[iter].__O_ZaPoperMis = query->value(26).toDouble();
		workerOperacii[iter].__O_Premiya = query->value(27).toDouble();
		workerOperacii[iter].__O_VsogoNaRuky = query->value(28).toDouble();
		workerOperacii[iter].__O_Avans = query->value(29).toDouble();
		workerOperacii[iter].__O_Oplacheno = query->value(30).toDouble();
		
		workerOperacii[iter].__V_PensFond = query->value(31).toDouble();
		workerOperacii[iter].__V_SocStrah = query->value(32).toDouble();
		workerOperacii[iter].__V_Fond_zainat = query->value(33).toDouble();
		workerOperacii[iter].__V_NeshchVyp = query->value(34).toDouble();
		workerOperacii[iter].__V_Inshi = query->value(35).toDouble();
		
		workerOperacii[iter].__Vids_U_PensFond = query->value(36).toDouble();
		workerOperacii[iter].__Vids_U_Fond_zainat = query->value(37).toDouble();
		workerOperacii[iter].__Vids_U_Socstrah = query->value(38).toDouble();
		workerOperacii[iter].__Vids_U_SocPilga = query->value(39).toDouble();
		workerOperacii[iter].__Vids_U_Prybutkov_podatok = query->value(40).toDouble();
		workerOperacii[iter].__Vids_U_VidrahProfspilka = query->value(41).toDouble();
		workerOperacii[iter].__Vids_U_InshiVidrahuvannya = query->value(42).toDouble();
		workerOperacii[iter].__Vids_V_PensFond = query->value(43).toDouble();
		workerOperacii[iter].__Vids_V_SocStrah = query->value(44).toDouble();
		workerOperacii[iter].__Vids_V_Fond_zainat = query->value(45).toDouble();
		workerOperacii[iter].__Vids_V_NeshchVyp = query->value(46).toDouble();
		workerOperacii[iter].__Vids_V_Inshi = query->value(47).toDouble();
		
		workerOperacii[iter].__Oklad = query->value(48).toDouble();
		workerOperacii[iter].__I_PromIndex = query->value(49).toDouble();
		workerOperacii[iter].__I_Index = query->value(50).toDouble();
		workerOperacii[iter].__I_ProindexovanaSuma = query->value(51).toDouble();
		workerOperacii[iter].__I_SumaDlyaIndexacii = query->value(52).toDouble();
		workerOperacii[iter].__I_BaseMonth = query->value(53).toBool();
		
		workerOperacii[iter].__U_EV_Likarn = query->value(54).toDouble();
		workerOperacii[iter].__U_EV_Vidpuskni = query->value(55).toDouble();
		workerOperacii[iter].__V_EV_Likarn = query->value(56).toDouble();
		workerOperacii[iter].__V_EV_Vidpuskni = query->value(57).toDouble();
		workerOperacii[iter].__Vids_U_EV_Likarn = query->value(58).toDouble();
		workerOperacii[iter].__Vids_U_EV_Vidpuskni = query->value(59).toDouble();
		workerOperacii[iter].__Vids_V_EV_Likarn = query->value(60).toDouble();
		workerOperacii[iter].__Vids_V_EV_Vidpuskni = query->value(61).toDouble();
	}
	
		//Значення з попередніх місяців, ознака >01
	//Встановлення попередніх нарахувань в 0,00
	for (iter=1; iter<13; iter++){
		wAdOperacii[iter].clear();
		xDate = xDate.addMonths(-1);
	}
	//Ініціалізація частини з таблиці operacii
	xDate = date.addMonths(-12);
	if (!query->exec("SELECT CurDate, sum(N_Osn_narah), sum(U_PensFond), sum(U_Fond_zainat), sum(U_SocStrah), \
					sum(U_SocPilga), sum(U_Prybutkov_podatok), sum(U_VidrahProfspilka),sum(U_InshiVidrahuvannya), \
					sum(O_VsogoNaRuky),sum(V_PensFond), sum(V_SocStrah), sum(V_Fond_zainat), sum(V_NeshchVyp), sum(V_Inshi) \
				FROM operacii \
				WHERE OznakaDohodu>1 and Npr_ID="+QVariant(workerID).toString()+" and \
					CurDate BETWEEN '"+xDate.toString("yyyy-MM")+"-1' and '"+yDate.toString("yyyy-MM")+"-"+QVariant(yDate.daysInMonth()).toString()+"' \
				GROUP BY CurDate"))
		qDebug() << "Помилка читання історії доходів з ОД>01 (Помилка запиту SQL).\nПричина: " <<query->lastError().text() << "\nЗапит: "+query->lastQuery();
	while (query->next()){
		xDate = query->value(0).toDate();
		int month1, month2;
		month1=xDate.month();
		month2=date.month();
		if ((month2==month1) && (xDate.year()==date.year()))
			iter=0;
		else if ((month2-month1)>0)
			iter = month2 - month1;
		else
			iter = 12 + month2 - month1;
			
		wAdOperacii[iter].__CurDate = query->value(0).toDate();
		wAdOperacii[iter].__N_Osn_narah = query->value(1).toDouble();
		wAdOperacii[iter].__U_PensFond = query->value(2).toDouble();
		wAdOperacii[iter].__U_Fond_zainat = query->value(3).toDouble();
		wAdOperacii[iter].__U_SocStrah = query->value(4).toDouble();
		wAdOperacii[iter].__U_SocPilga = query->value(5).toDouble();
		wAdOperacii[iter].__U_Prybutkov_podatok = query->value(6).toDouble();
		wAdOperacii[iter].__U_VidrahProfspilka = query->value(7).toDouble();
		wAdOperacii[iter].__U_InshiVidrahuvannya = query->value(8).toDouble();
		wAdOperacii[iter].__O_VsogoNaRukyOtherDohid = query->value(9).toDouble();
		
		wAdOperacii[iter].__V_PensFond = query->value(10).toDouble();
		wAdOperacii[iter].__V_SocStrah = query->value(11).toDouble();
		wAdOperacii[iter].__V_Fond_zainat = query->value(12).toDouble();
		wAdOperacii[iter].__V_NeshchVyp = query->value(13).toDouble();
		wAdOperacii[iter].__V_Inshi = query->value(14).toDouble();
	}
	
	delete query;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::populateWorkerOplataInfo(QDate date, int workerID)
{
	wOplata->clear();
	
	QSqlQuery query;
	query.exec("SELECT sum(O_VsogoNaRuky) AS naRukySum \
					FROM operacii \
					WHERE OznakaDohodu>1 and Npr_id="+mysqlStr(workerID)+" and year(CurDate)="+mysqlStr(date.year())+" \
						and month(CurDate)="+mysqlStr(date.month()));
	if (!query.seek(0))
		return false;
	
	wOplata->__O_VsogoNaRukyOtherDohid = query.value(0).toDouble();
	
	query.exec("SELECT 	Vh_saldo, \
						N_Osn_narah+N_VuslRokiv+N_Likarn+N_Vidpuskni+N_Svatkovi_dni+ \
							N_Nichni+N_Nadurochni+N_Klasn_vodiya+N_Premia+N_MaterDopomoga+ \
							N_Doplata+N_Indexaciya+N_VyplProfspilky+N_InshiNarahuvannya AS narahSum, \
						U_PensFond+U_EV_Likarn+U_EV_Vidpuskni+U_Fond_zainat+U_SocStrah+U_Prybutkov_podatok+ \
							U_VidrahProfspilka+U_InshiVidrahuvannya AS utrymSum, \
						O_Inshi, O_Shtraf, O_Alimenty, O_ZaPoperMis, O_Premiya, O_Avans, O_Oplacheno \
				FROM operacii \
				WHERE OznakaDohodu=1 and Npr_id="+mysqlStr(workerID)+" and year(CurDate)="+mysqlStr(date.year())+" \
						and month(CurDate)="+mysqlStr(date.month()));
	query.seek(0);
	wOplata->__Vh_saldo = query.value(0).toDouble();
	wOplata->__NarahSum = query.value(1).toDouble();
	wOplata->__UtrymSum = query.value(2).toDouble();
	wOplata->__O_Inshi = query.value(3).toDouble();
	wOplata->__O_Shtraf = query.value(4).toDouble();
	wOplata->__O_Alimenty = query.value(5).toDouble();
	wOplata->__O_ZaPoperMis = query.value(6).toDouble();
	wOplata->__O_Premiya = query.value(7).toDouble();
	wOplata->__O_Avans = query.value(8).toDouble();
	wOplata->__O_Oplacheno = query.value(9).toDouble();
	
	query.exec("SELECT 	sum(U_PensFond+U_Fond_zainat+U_SocStrah+U_Prybutkov_podatok+ \
							U_VidrahProfspilka+U_InshiVidrahuvannya) AS utrymSum \
				FROM operacii \
				WHERE OznakaDohodu>1 and Npr_id="+mysqlStr(workerID)+" and year(CurDate)="+mysqlStr(date.year())+" \
						and month(CurDate)="+mysqlStr(date.month()));
	query.seek(0);
	wOplata->__UtrymInshiSum = query.value(0).toDouble();
	
	return true;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::populateMonthlyBudjetniNormy(QDate date)
{
	int i;
	bool done_ok=true;
	QDate beginDate = date.addMonths(-12), xDate;
	QSqlQuery *query = new QSqlQuery();
	
	for (int i=0; i<13; i++)
		mbn[i].clear();
	
	query->exec("SELECT CurDate, WorkDaysInMonth, WorkHoursInMonth, MinZarplata, MinProjytkovyi, IndexSpojyvCin \
				FROM budjetni_normy_monthly \
				WHERE CurDate BETWEEN '"+beginDate.toString("yyyy-MM")+"-1' and '"+date.toString("yyyy-MM")+"-"+QVariant(date.daysInMonth()).toString()+"'");
	while (query->next()){
		xDate = query->value(0).toDate();
		int month1, month2;
		month1=xDate.month();
		month2=date.month();
		if ((month2==month1) && (xDate.year()==date.year()))
			i=0;
		else if ((month2-month1)>0)
			i = month2 - month1;
		else
			i = 12 + month2 - month1;
		
		if (xDate.day()<1)
			xDate.setDate(xDate.year(),xDate.month(),1);
		else if (xDate.day()>xDate.daysInMonth())
			xDate.setDate(xDate.year(),xDate.month(),xDate.daysInMonth());
		
		mbn[i].__CurDate = xDate;
		mbn[i].__WorkDaysInMonth = query->value(1).toInt();
		mbn[i].__WorkHoursInMonth = query->value(2).toInt();
		mbn[i].__MinZarplata = query->value(3).toDouble();
		mbn[i].__MinProjytkovyi = query->value(4).toDouble();
		mbn[i].__IndexSpojyvCin = query->value(5).toDouble();
	}
	delete query;
	
	for (int i=0; i<3; i++){
		if (!mbn[i].isValid())
			done_ok = false;
	}
	
	populateMonthlyBudjetniNormy_done = done_ok;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnOsnNarahAllWorkers(QDate date)
{
	bool done_ok=true;
	
	if (!populateMonthlyBudjetniNormy(date)){
		qDebug() << "Осн.Нарах.- " << "помилка ініціалізації помісячних бюджетних норм";
		return false;
	}
	
	if (!readNarahOsnNarahScript())
		return false;
	
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) FROM npr");
	query->seek(0);
	UPostupForm *postupForm = new UPostupForm(0, query->value(0).toInt() );
	postupForm->show();
	query->exec("SELECT id FROM npr");
	while (query->next()){
		if (!zdiysnOsnNarahOneWorker(date, query->value(0).toInt(), true)){
			done_ok = false;
			qDebug() << "Помилка нарахування для працівника id=" << query->value(0).toInt();
		}
		postupForm->inc_pos();
	}
	delete query;
	delete postupForm;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnOsnNarahOneWorker(QDate date, int workerID, bool __project)
{
	bool done_ok=true;
	
	if (!__project && !populateMonthlyBudjetniNormy_done){
		if (!populateMonthlyBudjetniNormy(date)){
			qDebug() << "Осн.Нарах.- " << "помилка ініціалізації помісячних бюджетних норм";
			return false;
		}
	}
	
	if (!__project)
		if (!readNarahOsnNarahScript())
			return false;
	
	populateWorkerInfo(date, workerID);
	populateWorkerOperaciiInfo(date, workerID);
	populateWorkerOperaciiHistoryInfo(date, workerID);
	salaryScriptEngine.evaluate(osnNarahScriptString);
	if (salaryScriptEngine.hasUncaughtException()){
		qDebug() << "Осн.Нарах.- " << "помилка виконання скрипта. Причина-" << salaryScriptEngine.uncaughtException().toString() << ".";
		QMessageBox::critical( 0, "Збій виконання скрипта", "Виконання скрипта пройшло з помилками і викликало збій.\n"
								"Можлива причина \n" + salaryScriptEngine.uncaughtException().toString(), QMessageBox::Ok, QMessageBox::Ok );
		return false;
	}
	
	//Команди запису результатів до бази даних
	QSqlQuery q;
	q.exec("SELECT count(*) FROM operacii \
			WHERE OznakaDohodu=1 and Npr_ID="+QVariant(workerID).toString()+" \
					and year(CurDate)="+QVariant(date.year()).toString()+" \
					and month(CurDate)="+QVariant(date.month()).toString());
	if ( q.seek(0) && (q.value(0).toInt()==0) ){
		q.exec("INSERT INTO operacii (Npr_ID, CurDate, OznakaDohodu) \
				VALUES ("+mysqlStr(workerID)+", "+mysqlStr(date)+", "+mysqlStr(1)+")");
	}
	q.exec("SELECT id FROM operacii \
			WHERE OznakaDohodu=1 and Npr_ID="+QVariant(workerID).toString()+" \
					and year(CurDate)="+QVariant(date.year()).toString()+" \
					and month(CurDate)="+QVariant(date.month()).toString());
	q.seek(0);
	int operacii_id = q.value(0).toInt();
	bool ok = q.exec("UPDATE operacii SET \
				Vh_saldo = "+mysqlStr(workerOperacii[0].__Vh_saldo)+", \
				N_Osn_narah = "+mysqlStr(workerOperacii[0].__N_Osn_narah)+", \
				N_VuslRokiv = "+mysqlStr(workerOperacii[0].__N_VuslRokiv)+", \
				N_Likarn = "+mysqlStr(workerOperacii[0].__N_Likarn)+", \
				N_Vidpuskni = "+mysqlStr(workerOperacii[0].__N_Vidpuskni)+", \
				N_Svatkovi_dni = "+mysqlStr(workerOperacii[0].__N_Svatkovi_dni)+", \
				N_Nichni = "+mysqlStr(workerOperacii[0].__N_Nichni)+", \
				N_Nadurochni = "+mysqlStr(workerOperacii[0].__N_Nadurochni)+", \
				N_Klasn_vodiya = "+mysqlStr(workerOperacii[0].__N_Klasn_vodiya)+", \
				N_Premia = "+mysqlStr(workerOperacii[0].__N_Premia)+", \
				N_MaterDopomoga = "+mysqlStr(workerOperacii[0].__N_MaterDopomoga)+", \
				N_Doplata = "+mysqlStr(workerOperacii[0].__N_Doplata)+", \
				N_Indexaciya = "+mysqlStr(workerOperacii[0].__N_Indexaciya)+", \
				N_VyplProfspilky = "+mysqlStr(workerOperacii[0].__N_VyplProfspilky)+", \
				N_InshiNarahuvannya = "+mysqlStr(workerOperacii[0].__N_InshiNarahuvannya)+", \
				TypRoboty = "+mysqlStr(wInfo->__TypRoboty)+", \
				TypOplaty = "+mysqlStr(wInfo->__TypOplaty)+", \
				Stavka = "+mysqlStr(wInfo->__Stavka)+", \
				Ozn_pilgy = "+mysqlStr(wInfo->__Ozn_pilgy)+", \
				Klasnist_vodiya = "+mysqlStr(wInfo->__Klasnist_vodiya)+", \
				VidsDoplaty = "+mysqlStr(wInfo->__VidsDoplaty)+", \
				VidsAlimentiv = "+mysqlStr(wInfo->__Alimenty)+", \
				Pensioner = "+mysqlStr(wInfo->__Pensioner)+", \
				Buh_rahunok = "+mysqlStr(wInfo->__Buh_rahunok)+", \
				ChlenProfspilky = "+mysqlStr(wInfo->__ChlenProfspilky)+", \
				Invalid = "+mysqlStr(wInfo->__Invalid)+", \
				VydTabelyaInMonth = "+mysqlStr(wInfo->__VydTabelya)+", \
				Posada_id = "+mysqlStr(wInfo->__Posada_id)+", \
				Rozryad = "+mysqlStr(wInfo->__Rozryad)+", \
				Pracuye = "+mysqlStr(wInfo->__Pracuye)+", \
				Pidrozdil_id = "+mysqlStr(wInfo->__Pidrozdil_id)+", \
				I_PromIndex = "+mysqlStr(workerOperacii[0].__I_PromIndex)+", \
				I_Index = "+mysqlStr(workerOperacii[0].__I_Index)+", \
				I_ProindexovanaSuma = "+mysqlStr(workerOperacii[0].__I_ProindexovanaSuma)+", \
				I_SumaDlyaIndexacii = "+mysqlStr(workerOperacii[0].__I_SumaDlyaIndexacii)+", \
				I_BaseMonth = "+mysqlStr(workerOperacii[0].__I_BaseMonth)+" \
			WHERE id="+mysqlStr(operacii_id));
	
	if (!ok){
			qDebug() << "Осн.Нарах.- " << "помилка при зміні запису в табл.operacii під час нарахування. ІД працівника - " << workerID << ". Помилка-" << q.lastError().text();
			qDebug() << " -- Запит: " << q.lastQuery(); 
			done_ok = false;
	}
	
	QDate dt = date;
	for (int i=0; i<2; i++){
		dt = dt.addMonths(-1);
		ok = q.exec("UPDATE operacii SET \
						I_PromIndex = "+mysqlStr(workerOperacii[i+1].__I_PromIndex)+", \
						I_Index = "+mysqlStr(workerOperacii[i+1].__I_Index)+"\
					WHERE OznakaDohodu=1 and Npr_ID="+mysqlStr(workerID)+" \
						and year(CurDate)="+mysqlStr(dt.year())+" \
						and month(CurDate)="+mysqlStr(dt.month()));
		if (!ok)
			qDebug() << " -- Помилка зміни значення індексації за " << dt.toString("MM.yyyy") << ". Причина: " << q.lastQuery();
	}
	
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnDodatkNarahAllWorkers(QDate date)
{
	bool done_ok=true;
	
	if (!readUtrVidrahDodNarahScript())
		return false;
	if (!readUtrVidrahOsnNarahScript())
		return false;
	
	QSqlQuery *query = new QSqlQuery();
	query->exec("SELECT count(*) FROM npr");
	query->seek(0);
	UPostupForm *postupForm = new UPostupForm(0, query->value(0).toInt() );
	postupForm->show();
	query->exec("SELECT id FROM npr");
	while (query->next()){
		if (!zdiysnDodatkNarahOneWorker(date, query->value(0).toInt(), true)){
			done_ok = false;
			qDebug() << "Помилка нарахування для працівника id=" << query->value(0).toInt();
		}
		postupForm->inc_pos();
	}
	delete query;
	delete postupForm;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnDodatkNarahOneWorker(QDate date, int workerID, bool __project)
{
	bool done_ok=true;
	
	if (!__project){
		if (!readUtrVidrahDodNarahScript())
			return false;
		if (!readUtrVidrahOsnNarahScript())
			return false;
	}
	
	if (!populateMonthlyBudjetniNormy_done){
		if (!populateMonthlyBudjetniNormy(date)){
			qDebug() << "Програма не змогла ініціалізувати таблицю кількості робочих днів у місяці";
			return false;
		}
	}
	
	QSqlQuery q("SELECT id, OznakaDohodu FROM operacii \
				WHERE Npr_ID="+QVariant(workerID).toString()+" \
						and year(CurDate)="+QVariant(date.year()).toString()+" \
						and month(CurDate)="+QVariant(date.month()).toString());
	while (q.next()){
		if (q.value(1).toInt() == 0){
			QSqlQuery delQuery("DELETE FROM operacii WHERE id="+q.value(0).toString());
			qDebug() << "Видалено стрічку з нульовою ознакою доходу. Ід прац.=" << workerID << ", Ід стрічки табл.operacii=" << q.value(0).toString();
		}
		else if (!zdiysnDodatkNarahOneWorkerOneOD(date,workerID, q.value(1).toInt(), q.value(0).toInt())){
			qDebug() << "Програма не змогла здійснити нарахування для працівника id=" << workerID << ", з ОД - " << q.value(1).toInt();
			done_ok = false;
		}
	}
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnDodatkNarahOneWorkerOneOD(QDate date, int workerID, short oznakaDohodu, int operId)
{
	bool done_ok=true;
	populateWorkerInfo(date, workerID);
	populateWorkerOperaciiInfo(date, workerID, oznakaDohodu);
	if (!neadCalcNarahValues->populateCalcNarahValuesForOD(oznakaDohodu)){
		qDebug() << "Програма не змогла ініціалізувати значення необхідності обрахунку утримань і відрахувань для додаткових доходів";
		return false;
	}
	
	salaryScriptEngine.evaluate((oznakaDohodu==1) ? osnUrtVidrahScriptString : dodUrtVidrahScriptString);
	if (salaryScriptEngine.hasUncaughtException()){
		done_ok = false;
		qDebug() << "Скрипт дав hasUncaughtException()=true";
		QMessageBox::critical( 0, "Збій виконання скрипта", "Виконання скрипта пройшло з помилками і викликало збій.\n"
								"Можлива причина \n" + salaryScriptEngine.uncaughtException().toString(), QMessageBox::Ok, QMessageBox::Ok );
	}
	else{
		//Команди запису результатів до бази даних
		QSqlQuery query;
		
		//Занулення показника "НА РУКИ" для ознаки доходу 01
		if (oznakaDohodu==1)
			workerOperacii[0].__O_VsogoNaRuky = 0;
		
		if (!query.exec("UPDATE operacii SET \
							U_PensFond = "+mysqlStr(workerOperacii[0].__U_PensFond)+", \
							U_EV_Likarn = "+mysqlStr(workerOperacii[0].__U_EV_Likarn)+", \
							U_EV_Vidpuskni = "+mysqlStr(workerOperacii[0].__U_EV_Vidpuskni)+", \
							U_Fond_zainat = "+mysqlStr(workerOperacii[0].__U_Fond_zainat)+", \
							U_SocStrah = "+mysqlStr(workerOperacii[0].__U_SocStrah)+", \
							U_SocPilga = "+mysqlStr(workerOperacii[0].__U_SocPilga)+", \
							U_Prybutkov_podatok = "+mysqlStr(workerOperacii[0].__U_Prybutkov_podatok)+", \
							U_VidrahProfspilka = "+mysqlStr(workerOperacii[0].__U_VidrahProfspilka)+", \
							U_InshiVidrahuvannya = "+mysqlStr(workerOperacii[0].__U_InshiVidrahuvannya)+", \
							V_PensFond = "+mysqlStr(workerOperacii[0].__V_PensFond)+", \
							V_EV_Likarn = "+mysqlStr(workerOperacii[0].__V_EV_Likarn)+", \
							V_EV_Vidpuskni = "+mysqlStr(workerOperacii[0].__V_EV_Vidpuskni)+", \
							V_SocStrah = "+mysqlStr(workerOperacii[0].__V_SocStrah)+", \
							V_Fond_zainat = "+mysqlStr(workerOperacii[0].__V_Fond_zainat)+", \
							V_NeshchVyp = "+mysqlStr(workerOperacii[0].__V_NeshchVyp)+", \
							V_Inshi = "+mysqlStr(workerOperacii[0].__V_Inshi)+", \
							O_Inshi = "+mysqlStr(workerOperacii[0].__O_Inshi)+", \
							O_Shtraf = "+mysqlStr(workerOperacii[0].__O_Shtraf)+", \
							O_Alimenty = "+mysqlStr(workerOperacii[0].__O_Alimenty)+", \
							O_VsogoNaRuky = "+mysqlStr(workerOperacii[0].__O_VsogoNaRuky)+", \
							O_ZaPoperMis = "+mysqlStr(workerOperacii[0].__O_ZaPoperMis)+", \
							O_Premiya = "+mysqlStr(workerOperacii[0].__O_Premiya)+", \
							O_Avans = "+mysqlStr(workerOperacii[0].__O_Avans)+", \
							O_Oplacheno = "+mysqlStr(workerOperacii[0].__O_Oplacheno)+", \
							Vids_U_PensFond = "+mysqlStr(workerOperacii[0].__Vids_U_PensFond)+", \
							Vids_U_EV_Likarn = "+mysqlStr(workerOperacii[0].__Vids_U_EV_Likarn)+", \
							Vids_U_EV_Vidpuskni = "+mysqlStr(workerOperacii[0].__Vids_U_EV_Vidpuskni)+", \
							Vids_U_Fond_zainat = "+mysqlStr(workerOperacii[0].__Vids_U_Fond_zainat)+", \
							Vids_U_Socstrah = "+mysqlStr(workerOperacii[0].__Vids_U_Socstrah)+", \
							Vids_U_SocPilga = "+mysqlStr(workerOperacii[0].__Vids_U_SocPilga)+", \
							Vids_U_Prybutkov_podatok = "+mysqlStr(workerOperacii[0].__Vids_U_Prybutkov_podatok)+", \
							Vids_U_VidrahProfspilka = "+mysqlStr(workerOperacii[0].__Vids_U_VidrahProfspilka)+", \
							Vids_U_InshiVidrahuvannya = "+mysqlStr(workerOperacii[0].__Vids_U_InshiVidrahuvannya)+", \
							Vids_V_PensFond = "+mysqlStr(workerOperacii[0].__Vids_V_PensFond)+", \
							Vids_V_EV_Likarn = "+mysqlStr(workerOperacii[0].__Vids_V_EV_Likarn)+", \
							Vids_V_EV_Vidpuskni = "+mysqlStr(workerOperacii[0].__Vids_V_EV_Vidpuskni)+", \
							Vids_V_SocStrah = "+mysqlStr(workerOperacii[0].__Vids_V_SocStrah)+", \
							Vids_V_Fond_zainat = "+mysqlStr(workerOperacii[0].__Vids_V_Fond_zainat)+", \
							Vids_V_NeshchVyp = "+mysqlStr(workerOperacii[0].__Vids_V_NeshchVyp)+", \
							Vids_V_Inshi = "+mysqlStr(workerOperacii[0].__Vids_V_Inshi)+", \
							Ozn_pilgy = "+mysqlStr(wInfo->__Ozn_pilgy)+", \
							VidsAlimentiv = "+mysqlStr(wInfo->__Alimenty)+", \
							Pensioner = "+mysqlStr(wInfo->__Pensioner)+", \
							Buh_rahunok = "+mysqlStr(wInfo->__Buh_rahunok)+", \
							ChlenProfspilky = "+mysqlStr(wInfo->__ChlenProfspilky)+", \
							Pidrozdil_id = "+mysqlStr(wInfo->__Pidrozdil_id)+", \
							Invalid = "+mysqlStr(wInfo->__Invalid)+" \
						WHERE id="+mysqlStr(operId))){
			done_ok = false;
			qDebug() << "-- Помилка запису утрим., і відрахувань до бази наних. Причина - " << query.lastError().text()
					<< "\nзапит - " << query.lastQuery();
		}
	}
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnObrahunkuVhidnogoSaldo(QDate date)
{
	bool done_ok=true;
	QSqlQuery query;
	query.exec("SELECT count(*) FROM npr");
	query.seek(0);
	UPostupForm *postupForm = new UPostupForm(0, query.value(0).toInt() );
	postupForm->show();
	
	query.exec("SELECT id FROM npr");
	while (query.next()){
		done_ok = done_ok && zdiysnObrahunkuVhidnogoSaldoOneWorker(date, query.value(0).toInt());
		postupForm->inc_pos();
	}
	delete postupForm;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnObrahunkuVhidnogoSaldoOneWorker(QDate date, int workerID)
{
	bool done_ok=true;
	if (!populateWorkerOplataInfo(date.addMonths(-1), workerID))
		return false;
	readVhSaldoScript();
	
	salaryScriptEngine.evaluate(saldoScriptString);
	if (salaryScriptEngine.hasUncaughtException()){
		done_ok = false;
		qDebug() << "Скрипт дав hasUncaughtException()=true";
		QMessageBox::critical( 0, "Збій виконання скрипта", "Виконання скрипта пройшло з помилками і викликало збій.\n"
								"Можлива причина \n" + salaryScriptEngine.uncaughtException().toString(), QMessageBox::Ok, QMessageBox::Ok );
	}
	else{
		//Команди запису результатів до бази даних
		QSqlQuery query;
		if (!query.exec("UPDATE operacii SET \
							Vh_saldo = "+mysqlStr(wOplata[0].__Vyh_saldo)+" \
						WHERE OznakaDohodu=1 and Npr_id="+mysqlStr(workerID)+" and year(CurDate)="+mysqlStr(date.year())+" \
						and month(CurDate)="+mysqlStr(date.month()))){
			done_ok = false;
			qDebug() << "-- Помилка запису утрим., і відрахувань до бази наних. Причина - " << query.lastError().text()
					<< "\nзапит - " << query.lastQuery();
		}
	}
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnObrahunkuPokaznOplaty(QDate date)
{
	bool done_ok=true;
	QSqlQuery query;
	
	query.exec("SELECT count(*) FROM npr");
	UPostupForm *postupForm = new UPostupForm(0, query.value(0).toInt() );
	postupForm->show();
	
	query.exec("SELECT id FROM npr");
	while (query.next()){
		done_ok = done_ok && zdiysnObrahunkuPokaznOplatyOneWorker(date, query.value(0).toInt());
		postupForm->incPos();
	}
	delete postupForm;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::zdiysnObrahunkuPokaznOplatyOneWorker(QDate date, int workerID)
{
	bool done_ok=true;
	
	if (!populateWorkerOplataInfo(date, workerID))
		return false;
	populateWorkerInfo(date, workerID);
	
	readCalcOplataValScript();
	
	salaryScriptEngine.evaluate(oplataValScriptString);
	if (salaryScriptEngine.hasUncaughtException()){
		done_ok = false;
		qDebug() << "Скрипт дав hasUncaughtException()=true";
		QMessageBox::critical( 0, "Збій виконання скрипта", "Виконання скрипта пройшло з помилками і викликало збій.\n"
								"Можлива причина \n" + salaryScriptEngine.uncaughtException().toString(), QMessageBox::Ok, QMessageBox::Ok );
	}
	else{
		//Команди запису результатів до бази даних
		QSqlQuery query;
		if (!query.exec("UPDATE operacii SET \
							Vh_saldo = "+mysqlStr(wOplata[0].__Vh_saldo)+", \
							O_Inshi = "+mysqlStr(wOplata[0].__O_Inshi)+", \
							O_Shtraf = "+mysqlStr(wOplata[0].__O_Shtraf)+", \
							O_Alimenty = "+mysqlStr(wOplata[0].__O_Alimenty)+", \
							O_ZaPoperMis = "+mysqlStr(wOplata[0].__O_ZaPoperMis)+", \
							O_Premiya = "+mysqlStr(wOplata[0].__O_Premiya)+", \
							O_Avans = "+mysqlStr(wOplata[0].__O_Avans)+", \
							O_Oplacheno = "+mysqlStr(wOplata[0].__O_Oplacheno)+", \
							O_Alimenty = "+mysqlStr(wOplata[0].__O_Alimenty)+" \
						WHERE OznakaDohodu=1 and Npr_id="+mysqlStr(workerID)+" and year(CurDate)="+mysqlStr(date.year())+" \
						and month(CurDate)="+mysqlStr(date.month()))){
			done_ok = false;
			qDebug() << "-- Помилка запису утрим., і відрахувань до бази наних. Причина - " << query.lastError().text()
					<< "\nзапит - " << query.lastQuery();
		}
	}
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::readNarahOsnNarahScript()
{
	bool done_ok=true;
	QFile *scriptFile = new QFile(QCoreApplication::applicationDirPath()+"\\..\\script\\osn_narah.js");
	if (scriptFile->open(QFile::ReadOnly)) {
		QTextStream *stream = new QTextStream(scriptFile);
		osnNarahScriptString = stream->readAll();
		delete stream;
	}
	else{
		done_ok = false;
		qDebug() << "Помилка відкривання скрипта";
		QMessageBox::critical( 0, "Збій відкривання скрипта", "Не вдалося відкрити файл скрипта\nПричина - "
									+QString::number(scriptFile->error())+";\nІм'я файлу-скрипта - \""
									+scriptFile->fileName()+"\"", QMessageBox::Ok, QMessageBox::Ok );
	}
	delete scriptFile;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::readUtrVidrahOsnNarahScript()
{
	bool done_ok=true;
	QFile *scriptFile = new QFile(QCoreApplication::applicationDirPath()+"\\..\\script\\utr_vidrah_osn_dohodu.js");
	if (scriptFile->open(QFile::ReadOnly)) {
		QTextStream *stream = new QTextStream(scriptFile);
		osnUrtVidrahScriptString = stream->readAll();
		delete stream;
	}
	else{
		done_ok = false;
		qDebug() << "Помилка відкривання скрипта";
		QMessageBox::critical( 0, "Збій відкривання скрипта", "Не вдалося відкрити файл скрипта\nПричина - "
									+QString::number(scriptFile->error())+";\nІм'я файлу-скрипта - \""
									+scriptFile->fileName()+"\"", QMessageBox::Ok, QMessageBox::Ok );
	}
	delete scriptFile;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::readUtrVidrahDodNarahScript()
{
	bool done_ok=true;
	QFile *scriptFile = new QFile(QCoreApplication::applicationDirPath()+"\\..\\script\\utr_vidrah_inshi_dohody.js");
	if (scriptFile->open(QFile::ReadOnly)) {
		QTextStream *stream = new QTextStream(scriptFile);
		dodUrtVidrahScriptString = stream->readAll();
		delete stream;
	}
	else{
		done_ok = false;
		qDebug() << "Помилка відкривання скрипта";
		QMessageBox::critical( 0, "Збій відкривання скрипта", "Не вдалося відкрити файл скрипта\nПричина - "
									+QString::number(scriptFile->error())+";\nІм'я файлу-скрипта - \""
									+scriptFile->fileName()+"\"", QMessageBox::Ok, QMessageBox::Ok );
	}
	delete scriptFile;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::readVhSaldoScript()
{
	bool done_ok=true;
	QFile *scriptFile = new QFile(QCoreApplication::applicationDirPath()+"\\..\\script\\saldo.js");
	if (scriptFile->open(QFile::ReadOnly)) {
		QTextStream *stream = new QTextStream(scriptFile);
		saldoScriptString = stream->readAll();
		delete stream;
	}
	else{
		done_ok = false;
		qDebug() << "Помилка відкривання скрипта";
		QMessageBox::critical( 0, "Збій відкривання скрипта", "Не вдалося відкрити файл скрипта\nПричина - "
									+QString::number(scriptFile->error())+";\nІм'я файлу-скрипта - \""
									+scriptFile->fileName()+"\"", QMessageBox::Ok, QMessageBox::Ok );
	}
	delete scriptFile;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
bool USalaryScript::readCalcOplataValScript()
{
	bool done_ok=true;
	QFile *scriptFile = new QFile(QCoreApplication::applicationDirPath()+"\\..\\script\\oplata.js");
	if (scriptFile->open(QFile::ReadOnly)) {
		QTextStream *stream = new QTextStream(scriptFile);
		oplataValScriptString = stream->readAll();
		delete stream;
	}
	else{
		done_ok = false;
		qDebug() << "Помилка відкривання скрипта";
		QMessageBox::critical( 0, "Збій відкривання скрипта", "Не вдалося відкрити файл скрипта\nПричина - "
									+QString::number(scriptFile->error())+";\nІм'я файлу-скрипта - \""
									+scriptFile->fileName()+"\"", QMessageBox::Ok, QMessageBox::Ok );
	}
	delete scriptFile;
	return done_ok;
}
//--------------------utech--------------------utech--------------------utech--------------------
void USalaryScript::prob(QDate date, int workerID)
{
	if (!populateMonthlyBudjetniNormy_done)
		populateMonthlyBudjetniNormy(date);
	
	populateWorkerInfo(date, workerID);
	populateWorkerOperaciiInfo(date, workerID);
	populateWorkerOperaciiHistoryInfo(date, workerID);
	
	QFile *scriptFile = new QFile(QCoreApplication::applicationDirPath()+"\\..\\script\\prob.js");
	if (scriptFile->open(QFile::ReadOnly)) {
		QTextStream *stream = new QTextStream(scriptFile);
		salaryScriptEngine.evaluate(stream->readAll()).toNumber();
		if (salaryScriptEngine.hasUncaughtException()){
			QMessageBox::critical( 0, "Збій виконання скрипта", "Виконання скрипта пройшло з помилками і викликало збій.\n"
									"Можлива причина \n" + salaryScriptEngine.uncaughtException().toString(), QMessageBox::Ok, QMessageBox::Ok );
		}
		delete stream;
	}
	else{
		QMessageBox::critical( 0, "Збій відкривання скрипта", "Не вдалося відкрити файл скрипта\nПричина - "
									+QString::number(scriptFile->error())+";\nІм'я файлу-скрипта - \""
									+scriptFile->fileName()+"\"", QMessageBox::Ok, QMessageBox::Ok );
	}
	delete scriptFile;
}
//--------------------utech--------------------utech--------------------utech--------------------
USalaryScript::~USalaryScript()
{
	delete wInfo;
	delete [] budjetniNormy;
	delete [] workerOperacii;
	delete wOplata;
	delete sumTabel;
	delete neadCalcNarahValues;
	delete wAdOperacii;
	delete opt;
}

//--------------------utech--------------------utech--------------------utech--------------------
