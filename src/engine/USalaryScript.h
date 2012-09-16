//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації класу нарахування зарплати 
// з допомогою скриптів для програми нарахування зарплати
// Створений програмістами Компанії "Утех" 9.12.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------
#ifndef USALARYSCRIPT_H
#define USALARYSCRIPT_H

#include <QObject>
#include <QDate>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QTextStream>
#include <QtScript>
#include <QVariant>
#include <QMessageBox>
#include <QtDebug>

#include <UMath>
#include "../additional/UDifFunc.h"
#include "../gui/UDialogOptions.h"
#include <UPostupForm>

#include <QSqlQuery>
#include <QObject>
class USqlQuery: public QObject, public QSqlQuery
{
  Q_OBJECT
  public:
	USqlQuery(QObject *parent=0):QObject(parent){};
	USqlQuery(const USqlQuery & q){};
	Q_INVOKABLE bool exec(QString q) {
		QSqlQuery::prepare(q);
		if (QSqlQuery::isSelect())
			return QSqlQuery::exec();
		return false;
	};
	Q_INVOKABLE bool first(){return QSqlQuery::first();};
	Q_INVOKABLE bool next(){return QSqlQuery::next();};
	Q_INVOKABLE bool last(){return QSqlQuery::last();};
	Q_INVOKABLE bool seek(int pos){return QSqlQuery::seek(pos);};
	Q_INVOKABLE QVariant value(int pos){return QSqlQuery::value(pos);};
	Q_INVOKABLE QString	lastQuery(){return QSqlQuery::lastQuery();};
	
};
Q_DECLARE_METATYPE(USqlQuery);

//--------------utech--------------------utech--------------------utech--------------------
class UIdVal : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(int id READ id WRITE setid)
	Q_PROPERTY(double val READ val WRITE setval)
	
  public:
	friend class USalaryScript;
	friend class UWorkerInfo;
	UIdVal(){__id=0; __val=0;};
	
	void setid(int v)	{__id = v;};
	int id()	{return __id;};
	void setval(double v)	{__val = v;};
	double val()	{return __val;};
	
  private:
	int __id;
	double __val;
};
//--------------utech--------------------utech--------------------utech--------------------
class UProgramOptions : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(short TypIndexaciyi READ TypIndexaciyi)
	Q_PROPERTY(short TypSocPilgy READ TypSocPilgy)
	
  public:
	friend class USalaryScript;
	short TypIndexaciyi();
	short TypSocPilgy();
  private:
	
};
//--------------utech--------------------utech--------------------utech--------------------
class UBudjetniNormyMonthly : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(QDate CurDate READ CurDate)
	Q_PROPERTY(short WorkDaysInMonth READ WorkDaysInMonth)
	Q_PROPERTY(short WorkHoursInMonth READ WorkHoursInMonth)
	Q_PROPERTY(double MinZarplata READ MinZarplata)
	Q_PROPERTY(double MinProjytkovyi READ MinProjytkovyi)
	Q_PROPERTY(double IndexSpojyvCin READ IndexSpojyvCin)
	
  public:
	friend class USalaryScript;
	UBudjetniNormyMonthly(){
		clear();
	};
	void clear(){
		__CurDate = QDate();
		__WorkDaysInMonth =0;
		__WorkHoursInMonth = 0;
		__MinZarplata = 0;
		__MinProjytkovyi = 0;
		__IndexSpojyvCin = 0;
	};
	bool isValid(){
		return __CurDate.isValid();
	}
	
	QDate CurDate()	{return __CurDate;};
	short WorkDaysInMonth() {return __WorkDaysInMonth;};
	short WorkHoursInMonth() {return __WorkHoursInMonth;};
	double MinZarplata()	{return __MinZarplata;};
	double MinProjytkovyi()	{return __MinProjytkovyi;};
	double IndexSpojyvCin()	{return __IndexSpojyvCin;};
	
  private:
	QDate 	__CurDate;
	short 	__WorkDaysInMonth;
	short 	__WorkHoursInMonth;
	double 	__MinZarplata;
	double 	__MinProjytkovyi;
	double 	__IndexSpojyvCin;
};
//--------------utech--------------------utech--------------------utech--------------------
class UDayWorkerTabel : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(short Pracivn_status_ID READ Pracivn_status_ID WRITE setPracivn_status_ID)
	Q_PROPERTY(QTime God_zag READ God_zag WRITE setGod_zag)
	Q_PROPERTY(QTime God_nichni READ God_nichni WRITE setGod_nichni)
	Q_PROPERTY(QTime God_sv READ God_sv WRITE setGod_sv)
  public:
	friend class USalaryScript;
	friend class UWorkerInfo;
	UDayWorkerTabel()
	{
		__Pracivn_status_ID=0;
		__God_zag=QTime();
		__God_nichni=QTime();
		__God_sv=QTime();
	};
	
	void setPracivn_status_ID(short val)	{__Pracivn_status_ID = val;};
	short Pracivn_status_ID()	{return __Pracivn_status_ID;};
	void setGod_zag(QTime val)	{__God_zag = val;};
	QTime God_zag()	{return __God_zag;};
	void setGod_nichni(QTime val)	{__God_nichni = val;};
	QTime God_nichni()	{return __God_nichni;};
	void setGod_sv(QTime val)	{__God_sv = val;};
	QTime God_sv()	{return __God_sv;};
	
  private:
	short __Pracivn_status_ID;
	QTime __God_zag;
	QTime __God_nichni;
	QTime __God_sv;
};
//--------------utech--------------------utech--------------------utech--------------------
class UWorkerInfo : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(int Npr_ID READ Npr_ID WRITE setNpr_ID)
	Q_PROPERTY(short Gender READ Gender WRITE setGender)
	Q_PROPERTY(QDate Data_nar READ Data_nar WRITE setData_nar)
	Q_PROPERTY(int Data_nar_day READ Data_nar_day WRITE setData_nar_day)
	Q_PROPERTY(int Data_nar_month READ Data_nar_month WRITE setData_nar_month)
	Q_PROPERTY(int Data_nar_year READ Data_nar_year WRITE setData_nar_year)
	Q_PROPERTY(short Posada_id READ Posada_id WRITE setPosada_id)
	Q_PROPERTY(short TypRoboty READ TypRoboty WRITE setTypRoboty)
	Q_PROPERTY(short TypOplaty READ TypOplaty WRITE setTypOplaty)
	Q_PROPERTY(short Rozryad READ Rozryad WRITE setRozryad)
	Q_PROPERTY(double Stavka READ Stavka WRITE setStavka)
	Q_PROPERTY(short Num_dit READ Num_dit WRITE setNum_dit)
	Q_PROPERTY(short Ozn_pilgy READ Ozn_pilgy WRITE setOzn_pilgy)
	Q_PROPERTY(QDate Data_pruin_na_rob READ Data_pruin_na_rob WRITE setData_pruin_na_rob)
	Q_PROPERTY(int Data_pruin_na_rob_day READ Data_pruin_na_rob_day WRITE setData_pruin_na_rob_day)
	Q_PROPERTY(int Data_pruin_na_rob_month READ Data_pruin_na_rob_month WRITE setData_pruin_na_rob_month)
	Q_PROPERTY(int Data_pruin_na_rob_year READ Data_pruin_na_rob_year WRITE setData_pruin_na_rob_year)
	Q_PROPERTY(double Klasnist_vodiya READ Klasnist_vodiya WRITE setKlasnist_vodiya)
	Q_PROPERTY(double VidsDoplaty READ VidsDoplaty WRITE setVidsDoplaty)
	Q_PROPERTY(double Alimenty READ Alimenty WRITE setAlimenty)
	Q_PROPERTY(bool Pensioner READ Pensioner WRITE setPensioner)
	Q_PROPERTY(double Vids_vuslugy_rokiv READ Vids_vuslugy_rokiv WRITE setVids_vuslugy_rokiv)
	Q_PROPERTY(double Staj_roboty READ Staj_roboty WRITE setStaj_roboty)
	Q_PROPERTY(int Buh_rahunok READ Buh_rahunok WRITE setBuh_rahunok)
	Q_PROPERTY(bool ChlenProfspilky READ ChlenProfspilky WRITE setChlenProfspilky)
	Q_PROPERTY(bool Invalid READ Invalid WRITE setInvalid)
	Q_PROPERTY(bool Pracuye READ Pracuye WRITE setPracuye)
	Q_PROPERTY(short IndexaciyaType READ IndexaciyaType WRITE setIndexaciyaType)
	Q_PROPERTY(short Pidrozdil_id READ Pidrozdil_id WRITE setPidrozdil_id)
	Q_PROPERTY(short VydTabelya READ VydTabelya WRITE setVydTabelya)
  public:
	friend class USalaryScript;
		//methods
	void setGender(short val)	{__Gender = val;};
	short Gender()	{return __Npr_ID;};
	void setNpr_ID(int val)	{__Npr_ID = val;};
	int Npr_ID()	{return __Npr_ID;};
	
	void setData_nar(QDate val)	{__Data_nar = val;};
	QDate Data_nar()	{return __Data_nar;};
	void setData_nar_day(int val){	
		if(val<1)
			val=1;
		if(__Data_nar.daysInMonth()<val)
			val = __Data_nar.daysInMonth();
		__Data_nar.setDate(__Data_nar.year(),__Data_nar.month(),val);
	};
	int Data_nar_day()	{return __Data_nar.day();};
	void setData_nar_month(int val){	
		if(val<1)
			val=1;
		if(val>12)
			val = 12;
		__Data_nar.setDate(__Data_nar.year(),val,__Data_nar.day());
	};
	int Data_nar_month()	{return __Data_nar.month();};
	void setData_nar_year(int val){	
		if(val<1900)
			val=1900;
		if(val>2200)
			val = 2200;
		__Data_nar.setDate(val,__Data_nar.month(),__Data_nar.day());
	};
	int Data_nar_year()	{return __Data_nar.year();};
	void setPosada_id(short val)	{__Posada_id = val;};
	short Posada_id()	{return __Posada_id;};
	void setTypRoboty(short val)	{__TypRoboty = val;};
	short TypRoboty()	{return __TypRoboty;};
	void setTypOplaty(short val)	{__TypOplaty = val;};
	short TypOplaty()	{return __TypOplaty;};
	void setRozryad(short val)	{__Rozryad = val;};
	short Rozryad()	{return __Rozryad;};
	void setStavka(double val)	{__Stavka = val;};
	double Stavka()	{return __Stavka;};
	void setNum_dit(short val)	{__Num_dit = val;};
	short Num_dit()	{return __Num_dit;};
	void setOzn_pilgy(short val)	{__Ozn_pilgy = val;};
	short Ozn_pilgy()	{return __Ozn_pilgy;};
	void setData_pruin_na_rob(QDate val)	{__Data_pruin_na_rob = val;};
	QDate Data_pruin_na_rob()	{return __Data_pruin_na_rob;};
	void setData_pruin_na_rob_day(int val){	
		if(val<1)
			val=1;
		if(__Data_pruin_na_rob.daysInMonth()<val)
			val = __Data_pruin_na_rob.daysInMonth();
		__Data_pruin_na_rob.setDate(__Data_pruin_na_rob.year(),__Data_pruin_na_rob.month(),val);
	};
	int Data_pruin_na_rob_day()	{return __Data_pruin_na_rob.day();};
	void setData_pruin_na_rob_month(int val){	
		if(val<1)
			val=1;
		if(val>12)
			val = 12;
		__Data_pruin_na_rob.setDate(__Data_pruin_na_rob.year(),val,__Data_pruin_na_rob.day());
	};
	int Data_pruin_na_rob_month()	{return __Data_pruin_na_rob.month();};
	void setData_pruin_na_rob_year(int val){	
		if(val<1900)
			val=1900;
		if(val>2200)
			val = 2200;
		__Data_pruin_na_rob.setDate(val,__Data_pruin_na_rob.month(),__Data_pruin_na_rob.day());
	};
	int Data_pruin_na_rob_year()	{return __Data_pruin_na_rob.year();};
	void setKlasnist_vodiya(double val)	{__Klasnist_vodiya = val;};
	double Klasnist_vodiya()	{return __Klasnist_vodiya;};
	void setVidsDoplaty(double val)	{__VidsDoplaty = val;};
	double VidsDoplaty()	{return __VidsDoplaty;};
	void setAlimenty(double val)	{__Alimenty = val;};
	double Alimenty()	{return __Alimenty;};
	void setPensioner(bool val)	{__Pensioner = val;};
	bool Pensioner()	{return __Pensioner;};
	void setVids_vuslugy_rokiv(double val)	{__Vids_vuslugy_rokiv = val;};
	double Vids_vuslugy_rokiv()	{return __Vids_vuslugy_rokiv;};
	void setStaj_roboty(double val)	{__Staj_roboty = val;};
	double Staj_roboty()	{return __Staj_roboty;};
	int Buh_rahunok()	{return __Buh_rahunok;};
	void setBuh_rahunok(int val)	{__Buh_rahunok = val;};
	void setChlenProfspilky(bool val)	{__ChlenProfspilky = val;};
	bool ChlenProfspilky()	{return __ChlenProfspilky;};
	void setInvalid(bool val)	{__Invalid = val;};
	bool Invalid()	{return __Invalid;};
	void setPracuye(bool val)	{__Pracuye = val;};
	bool Pracuye()	{return __Pracuye;};
	void setIndexaciyaType(short val)	{__IndexaciyaType = val;};
	short IndexaciyaType()	{return __IndexaciyaType;};
	void setPidrozdil_id(short val)	{__Pidrozdil_id = val;};
	short Pidrozdil_id()	{return __Pidrozdil_id;};
	void setVydTabelya(short val)	{__VydTabelya = val;};
	short VydTabelya()	{return __VydTabelya;};
	
  private:
	//npr table
	int 			__Npr_ID;
	short			__Gender;
	QDate 			__Data_nar;
	short 			__Posada_id;
	short 			__TypRoboty;
	short 			__TypOplaty;
	short 			__Rozryad;
	double 			__Stavka;
	short 			__Num_dit;
	short 			__Ozn_pilgy;
	QDate 			__Data_pruin_na_rob;
	double 			__Klasnist_vodiya;
	double			__VidsDoplaty;
	double 			__Alimenty;
	bool 			__Pensioner;
	double 			__Vids_vuslugy_rokiv;
	double			__Staj_roboty;
	int				__Buh_rahunok;
	bool			__ChlenProfspilky;
	bool			__Invalid;
	bool			__Pracuye;
	short			__IndexaciyaType;
	short			__Pidrozdil_id;
	short			__VydTabelya;
};
//--------------utech--------------------utech--------------------utech--------------------
class UWorkerOperacii : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(int id READ id WRITE setId)
	Q_PROPERTY(QDate CurDate READ CurDate WRITE setCurDate)
	Q_PROPERTY(int CurDate_day READ CurDate_day WRITE setCurDate_day)
	Q_PROPERTY(int CurDate_month READ CurDate_month WRITE setCurDate_month)
	Q_PROPERTY(int CurDate_year READ CurDate_year WRITE setCurDate_year)
	Q_PROPERTY(double Vh_saldo READ Vh_saldo WRITE setVh_saldo)
	Q_PROPERTY(double N_Osn_narah READ N_Osn_narah WRITE setN_Osn_narah)
	Q_PROPERTY(double N_VuslRokiv READ N_VuslRokiv WRITE setN_VuslRokiv)
	Q_PROPERTY(double N_Likarn READ N_Likarn WRITE setN_Likarn)
	Q_PROPERTY(double N_LikarnExt READ N_LikarnExt)
	Q_PROPERTY(int N_LikarnExtCount READ N_LikarnExtCount)
	Q_PROPERTY(double N_Vidpuskni READ N_Vidpuskni WRITE setN_Vidpuskni)
	Q_PROPERTY(double N_VidpuskniExt READ N_VidpuskniExt)
	Q_PROPERTY(int N_VidpuskniExtCount READ N_VidpuskniExtCount)
	Q_PROPERTY(double N_Svatkovi_dni READ N_Svatkovi_dni WRITE setN_Svatkovi_dni)
	Q_PROPERTY(double N_Nichni READ N_Nichni WRITE setN_Nichni)
	Q_PROPERTY(double N_Nadurochni READ N_Nadurochni WRITE setN_Nadurochni)
	Q_PROPERTY(double N_Klasn_vodiya READ N_Klasn_vodiya WRITE setN_Klasn_vodiya)
	Q_PROPERTY(double N_Premia READ N_Premia WRITE setN_Premia)
	Q_PROPERTY(double N_MaterDopomoga READ N_MaterDopomoga WRITE setN_MaterDopomoga)
	Q_PROPERTY(double N_Doplata READ N_Doplata WRITE setN_Doplata)
	Q_PROPERTY(double N_Indexaciya READ N_Indexaciya WRITE setN_Indexaciya)
	Q_PROPERTY(double N_VyplProfspilky READ N_VyplProfspilky WRITE setN_VyplProfspilky)
	Q_PROPERTY(double N_InshiNarahuvannya READ N_InshiNarahuvannya WRITE setN_InshiNarahuvannya)
	Q_PROPERTY(double U_PensFond READ U_PensFond WRITE setU_PensFond)
	Q_PROPERTY(double U_EV_Likarn READ U_EV_Likarn WRITE setU_EV_Likarn)
	Q_PROPERTY(double U_EV_Vidpuskni READ U_EV_Vidpuskni WRITE setU_EV_Vidpuskni)
	Q_PROPERTY(double U_Fond_zainat READ U_Fond_zainat WRITE setU_Fond_zainat)
	Q_PROPERTY(double U_SocStrah READ U_SocStrah WRITE setU_SocStrah)
	Q_PROPERTY(double U_SocPilga READ U_SocPilga WRITE setU_SocPilga)
	Q_PROPERTY(double U_Prybutkov_podatok READ U_Prybutkov_podatok WRITE setU_Prybutkov_podatok)
	Q_PROPERTY(double U_VidrahProfspilka READ U_VidrahProfspilka WRITE setU_VidrahProfspilka)
	Q_PROPERTY(double U_InshiVidrahuvannya READ U_InshiVidrahuvannya WRITE setU_InshiVidrahuvannya)
	Q_PROPERTY(double O_Inshi READ O_Inshi WRITE setO_Inshi)
	Q_PROPERTY(double O_Shtraf READ O_Shtraf WRITE setO_Shtraf)
	Q_PROPERTY(double O_Alimenty READ O_Alimenty WRITE setO_Alimenty)
	Q_PROPERTY(double O_ZaPoperMis READ O_ZaPoperMis WRITE setO_ZaPoperMis)
	Q_PROPERTY(double O_Premiya READ O_Premiya WRITE setO_Premiya)
	Q_PROPERTY(double O_VsogoNaRuky READ O_VsogoNaRuky WRITE setO_VsogoNaRuky)
	Q_PROPERTY(double O_VsogoNaRukyOtherDohid READ O_VsogoNaRukyOtherDohid WRITE setO_VsogoNaRukyOtherDohid)
	Q_PROPERTY(double O_Avans READ O_Avans WRITE setO_Avans)
	Q_PROPERTY(double O_Oplacheno READ O_Oplacheno WRITE setO_Oplacheno)
	Q_PROPERTY(double V_PensFond READ V_PensFond WRITE setV_PensFond)
	Q_PROPERTY(double V_EV_Likarn READ V_EV_Likarn WRITE setV_EV_Likarn)
	Q_PROPERTY(double V_EV_Vidpuskni READ V_EV_Vidpuskni WRITE setV_EV_Vidpuskni)
	Q_PROPERTY(double V_SocStrah READ V_SocStrah WRITE setV_SocStrah)
	Q_PROPERTY(double V_Fond_zainat READ V_Fond_zainat WRITE setV_Fond_zainat)
	Q_PROPERTY(double V_NeshchVyp READ V_NeshchVyp WRITE setV_NeshchVyp)
	Q_PROPERTY(double V_Inshi READ V_Inshi WRITE setV_Inshi)
	Q_PROPERTY(double Vids_U_PensFond READ Vids_U_PensFond WRITE setVids_U_PensFond)
	Q_PROPERTY(double Vids_U_EV_Likarn READ Vids_U_EV_Likarn WRITE setVids_U_EV_Likarn)
	Q_PROPERTY(double Vids_U_EV_Vidpuskni READ Vids_U_EV_Vidpuskni WRITE setVids_U_EV_Vidpuskni)
	Q_PROPERTY(double Vids_U_Fond_zainat READ Vids_U_Fond_zainat WRITE setVids_U_Fond_zainat)
	Q_PROPERTY(double Vids_U_Socstrah READ Vids_U_Socstrah WRITE setVids_U_Socstrah)
	Q_PROPERTY(double Vids_U_SocPilga READ Vids_U_SocPilga WRITE setVids_U_SocPilga)
	Q_PROPERTY(double Vids_U_Prybutkov_podatok READ Vids_U_Prybutkov_podatok WRITE setVids_U_Prybutkov_podatok)
	Q_PROPERTY(double Vids_U_VidrahProfspilka READ Vids_U_VidrahProfspilka WRITE setVids_U_VidrahProfspilka)
	Q_PROPERTY(double Vids_U_InshiVidrahuvannya READ Vids_U_InshiVidrahuvannya WRITE setVids_U_InshiVidrahuvannya)
	Q_PROPERTY(double Vids_V_PensFond READ Vids_V_PensFond WRITE setVids_V_PensFond)
	Q_PROPERTY(double Vids_V_EV_Likarn READ Vids_V_EV_Likarn WRITE setVids_V_EV_Likarn)
	Q_PROPERTY(double Vids_V_EV_Vidpuskni READ Vids_V_EV_Vidpuskni WRITE setVids_V_EV_Vidpuskni)
	Q_PROPERTY(double Vids_V_SocStrah READ Vids_V_SocStrah WRITE setVids_V_SocStrah)
	Q_PROPERTY(double Vids_V_Fond_zainat READ Vids_V_Fond_zainat WRITE setVids_V_Fond_zainat)
	Q_PROPERTY(double Vids_V_NeshchVyp READ Vids_V_NeshchVyp WRITE setVids_V_NeshchVyp)
	Q_PROPERTY(double Vids_V_Inshi READ Vids_V_Inshi WRITE setVids_V_Inshi)
	Q_PROPERTY(int Ozn_pilgy READ Ozn_pilgy WRITE setOzn_pilgy)
	Q_PROPERTY(short OznakaDohodu READ OznakaDohodu WRITE setOznakaDohodu)
	Q_PROPERTY(double Oklad READ Oklad)
	Q_PROPERTY(double Stavka READ Stavka)
	
	Q_PROPERTY(double 	I_PromIndex READ I_PromIndex WRITE setI_PromIndex)
	Q_PROPERTY(double 	I_Index READ I_Index WRITE setI_Index)
	Q_PROPERTY(double 	I_ProindexovanaSuma READ I_ProindexovanaSuma WRITE setI_ProindexovanaSuma)
	Q_PROPERTY(double 	I_SumaDlyaIndexacii READ I_SumaDlyaIndexacii WRITE setI_SumaDlyaIndexacii)
	Q_PROPERTY(bool 	I_BaseMonth READ I_BaseMonth WRITE setI_BaseMonth)
	
  public:
	friend class USalaryScript;
		//methods
	void setId(int val)	{__id = val;};
	int id()	{return __id;};
	void setCurDate(QDate val)	{__CurDate = val;};
	QDate CurDate()	{return __CurDate;};
	
	void setCurDate_day(int val){	
		if(val<1)
			val=1;
		if(__CurDate.daysInMonth()<val)
			val = __CurDate.daysInMonth();
		__CurDate.setDate(__CurDate.year(),__CurDate.month(),val);
	};
	int CurDate_day()	{return __CurDate.day();};
	void setCurDate_month(int val){	
		if(val<1)
			val=1;
		if(val>12)
			val = 12;
		__CurDate.setDate(__CurDate.year(),val,__CurDate.day());
	};
	int CurDate_month()	{return __CurDate.month();};
	void setCurDate_year(int val){	
		if(val<1900)
			val=1900;
		if(val>2200)
			val = 2200;
		__CurDate.setDate(val,__CurDate.month(),__CurDate.day());
	};
	int CurDate_year()	{return __CurDate.year();};
	
	void setVh_saldo(double val)	{__Vh_saldo = d_to_m(val);};
	double Vh_saldo()	{return __Vh_saldo;};
	void setN_Osn_narah(double val)	{__N_Osn_narah = d_to_m(val);};
	double N_Osn_narah()	{return __N_Osn_narah;};
	void setN_VuslRokiv(double val)	{__N_VuslRokiv = d_to_m(val);};
	double N_VuslRokiv()	{return __N_VuslRokiv;};
	void setN_Likarn(double val)	{__N_Likarn = d_to_m(val);};
	double N_Likarn()	{return __N_Likarn;};
	double N_LikarnExt()	{return __N_LikarnExt;};
	int N_LikarnExtCount()	{return __N_LikarnExtCount;};
	void setN_Vidpuskni(double val)	{__N_Vidpuskni = d_to_m(val);};
	double N_Vidpuskni()	{return __N_Vidpuskni;};
	double N_VidpuskniExt()	{return __N_VidpuskniExt;};
	int N_VidpuskniExtCount()	{return __N_VidpuskniExtCount;};
	void setN_Svatkovi_dni(double val)	{__N_Svatkovi_dni = d_to_m(val);};
	double N_Svatkovi_dni()	{return __N_Svatkovi_dni;};
	void setN_Nichni(double val)	{__N_Nichni = d_to_m(val);};
	double N_Nichni()	{return __N_Nichni;};
	void setN_Nadurochni(double val)	{__N_Nadurochni = d_to_m(val);};
	double N_Nadurochni()	{return __N_Nadurochni;};
	void setN_Klasn_vodiya(double val)	{__N_Klasn_vodiya = d_to_m(val);};
	double N_Klasn_vodiya()	{return __N_Klasn_vodiya;};
	void setN_Premia(double val)	{__N_Premia = d_to_m(val);};
	double N_Premia()	{return __N_Premia;};
	void setN_MaterDopomoga(double val)	{__N_MaterDopomoga = d_to_m(val);};
	double N_MaterDopomoga()	{return __N_MaterDopomoga;};
	void setN_Doplata(double val)	{__N_Doplata = d_to_m(val);};
	double N_Doplata()	{return __N_Doplata;};
	void setN_Indexaciya(double val)	{__N_Indexaciya = d_to_m(val);};
	double N_Indexaciya()	{return __N_Indexaciya;};
	void setN_VyplProfspilky(double val)	{__N_VyplProfspilky = d_to_m(val);};
	double N_VyplProfspilky()	{return __N_VyplProfspilky;};
	void setN_InshiNarahuvannya(double val)	{__N_InshiNarahuvannya = d_to_m(val);};
	double N_InshiNarahuvannya()	{return __N_InshiNarahuvannya;};
	void setU_PensFond(double val)	{if (val>0) __U_PensFond = d_to_m(val); else __U_PensFond = 0;};
	double U_PensFond()	{return __U_PensFond;};
	void setU_EV_Likarn(double val)	{if (val>0) __U_EV_Likarn = d_to_m(val); else __U_EV_Likarn = 0;};
	double U_EV_Likarn()	{return __U_EV_Likarn;};
	void setU_EV_Vidpuskni(double val)	{if (val>0) __U_EV_Vidpuskni = d_to_m(val); else __U_EV_Vidpuskni = 0;};
	double U_EV_Vidpuskni()	{return __U_EV_Vidpuskni;};
	void setU_Fond_zainat(double val)	{if (val>0) __U_Fond_zainat = d_to_m(val); else __U_Fond_zainat = 0;};
	double U_Fond_zainat()	{return __U_Fond_zainat;};
	void setU_SocStrah(double val)	{if (val>0) __U_SocStrah = d_to_m(val); else __U_SocStrah = 0;};
	double U_SocStrah()	{return __U_SocStrah;};
	void setU_SocPilga(double val)	{if (val>0) __U_SocPilga = d_to_m(val); else __U_SocPilga = 0;};
	double U_SocPilga()	{return __U_SocPilga;};
	void setU_Prybutkov_podatok(double val)	{if (val>0) __U_Prybutkov_podatok = d_to_m(val); else __U_Prybutkov_podatok = 0;};
	double U_Prybutkov_podatok()	{return __U_Prybutkov_podatok;};
	void setU_VidrahProfspilka(double val)	{__U_VidrahProfspilka = d_to_m(val);};
	double U_VidrahProfspilka()	{return __U_VidrahProfspilka;};
	void setU_InshiVidrahuvannya(double val)	{__U_InshiVidrahuvannya = d_to_m(val);};
	double U_InshiVidrahuvannya()	{return __U_InshiVidrahuvannya;};
	void setO_Inshi(double val)	{__O_Inshi = d_to_m(val);};
	double O_Inshi()	{return __O_Inshi;};
	void setO_Shtraf(double val)	{__O_Shtraf = d_to_m(val);};
	double O_Shtraf()	{return __O_Shtraf;};
	void setO_Alimenty(double val)	{__O_Alimenty = d_to_m(val);};
	double O_Alimenty()	{return __O_Alimenty;};
	void setO_ZaPoperMis(double val)	{__O_ZaPoperMis = d_to_m(val);};
	double O_ZaPoperMis()	{return __O_ZaPoperMis;};
	void setO_Premiya(double val)	{__O_Premiya = d_to_m(val);};
	double O_Premiya()	{return __O_Premiya;};
	void setO_VsogoNaRuky(double val)	{__O_VsogoNaRuky = d_to_m(val);};
	double O_VsogoNaRuky()	{return __O_VsogoNaRuky;};
	void setO_VsogoNaRukyOtherDohid(double val)	{__O_VsogoNaRukyOtherDohid = d_to_m(val);};
	double O_VsogoNaRukyOtherDohid()	{return __O_VsogoNaRukyOtherDohid;};
	void setO_Avans(double val)	{__O_Avans = d_to_m(val);};
	double O_Avans()	{return __O_Avans;};
	void setO_Oplacheno(double val)	{__O_Oplacheno = d_to_m(val);};
	double O_Oplacheno()	{return __O_Oplacheno;};
	
	void setV_PensFond(double val)	{__V_PensFond = d_to_m(val);};
	double V_PensFond()	{return __V_PensFond;};
	void setV_EV_Likarn(double val)	{__V_EV_Likarn = d_to_m(val);};
	double V_EV_Likarn()	{return __V_EV_Likarn;};
	void setV_EV_Vidpuskni(double val)	{__V_EV_Vidpuskni = d_to_m(val);};
	double V_EV_Vidpuskni()	{return __V_EV_Vidpuskni;};
	void setV_SocStrah(double val)	{__V_SocStrah = d_to_m(val);};
	double V_SocStrah()	{return __V_SocStrah;};
	void setV_Fond_zainat(double val)	{__V_Fond_zainat = d_to_m(val);};
	double V_Fond_zainat()	{return __V_Fond_zainat;};
	void setV_NeshchVyp(double val)	{__V_NeshchVyp = d_to_m(val);};
	double V_NeshchVyp()	{return __V_NeshchVyp;};
	void setV_Inshi(double val)	{__V_Inshi = d_to_m(val);};
	double V_Inshi()	{return __V_Inshi;};
	
	void setVids_U_PensFond(double val)	{__Vids_U_PensFond = d_to_m(val);};
	double Vids_U_PensFond()	{return __Vids_U_PensFond;};
	void setVids_U_EV_Likarn(double val)	{__Vids_U_EV_Likarn = d_to_m(val);};
	double Vids_U_EV_Likarn()	{return __Vids_U_EV_Likarn;};
	void setVids_U_EV_Vidpuskni(double val)	{__Vids_U_EV_Vidpuskni = d_to_m(val);};
	double Vids_U_EV_Vidpuskni()	{return __Vids_U_EV_Vidpuskni;};
	void setVids_U_Fond_zainat(double val)	{__Vids_U_Fond_zainat = d_to_m(val);};
	double Vids_U_Fond_zainat()	{return __Vids_U_Fond_zainat;};
	void setVids_U_Socstrah(double val)	{__Vids_U_Socstrah = d_to_m(val);};
	double Vids_U_Socstrah()	{return __Vids_U_Socstrah;};
	void setVids_U_SocPilga(double val)	{__Vids_U_SocPilga = d_to_m(val);};
	double Vids_U_SocPilga()	{return __Vids_U_SocPilga;};
	void setVids_U_Prybutkov_podatok(double val)	{__Vids_U_Prybutkov_podatok = d_to_m(val);};
	double Vids_U_Prybutkov_podatok()	{return __Vids_U_Prybutkov_podatok;};
	void setVids_U_VidrahProfspilka(double val)	{__Vids_U_VidrahProfspilka = d_to_m(val);};
	double Vids_U_VidrahProfspilka()	{return __Vids_U_VidrahProfspilka;};
	void setVids_U_InshiVidrahuvannya(double val)	{__Vids_U_InshiVidrahuvannya = d_to_m(val);};
	double Vids_U_InshiVidrahuvannya()	{return __Vids_U_InshiVidrahuvannya;};
	void setVids_V_PensFond(double val)	{__Vids_V_PensFond = d_to_m(val);};
	double Vids_V_PensFond()	{return __Vids_V_PensFond;};
	void setVids_V_EV_Likarn(double val)	{__Vids_V_EV_Likarn = d_to_m(val);};
	double Vids_V_EV_Likarn()	{return __Vids_V_EV_Likarn;};
	void setVids_V_EV_Vidpuskni(double val)	{__Vids_V_EV_Vidpuskni = d_to_m(val);};
	double Vids_V_EV_Vidpuskni()	{return __Vids_V_EV_Vidpuskni;};
	void setVids_V_SocStrah(double val)	{__Vids_V_SocStrah = d_to_m(val);};
	double Vids_V_SocStrah()	{return __Vids_V_SocStrah;};
	void setVids_V_Fond_zainat(double val)	{__Vids_V_Fond_zainat = d_to_m(val);};
	double Vids_V_Fond_zainat()	{return __Vids_V_Fond_zainat;};
	void setVids_V_NeshchVyp(double val)	{__Vids_V_NeshchVyp = d_to_m(val);};
	double Vids_V_NeshchVyp()	{return __Vids_V_NeshchVyp;};
	void setVids_V_Inshi(double val)	{__Vids_V_Inshi = d_to_m(val);};
	double Vids_V_Inshi()	{return __Vids_V_Inshi;};
	void setOzn_pilgy(int val)	{__Ozn_pilgy = val;};
	int Ozn_pilgy()	{return __Ozn_pilgy;};
	void setOznakaDohodu(short val)	{__OznakaDohodu = val;};
	short OznakaDohodu()	{return __OznakaDohodu;};
	double Oklad()	{return __Oklad;};
	double Stavka()	{return __Oklad;};
	
	void setI_PromIndex(double val)	{__I_PromIndex = qRound(val*10)/10.0;};
	double I_PromIndex()	{return __I_PromIndex;};
	void setI_Index(double val)	{__I_Index = qRound(val*10)/10.0;};
	double I_Index()	{return __I_Index;};
	void setI_ProindexovanaSuma(double val)	{__I_ProindexovanaSuma = d_to_m(val);};
	double I_ProindexovanaSuma()	{return __I_ProindexovanaSuma;};
	void setI_SumaDlyaIndexacii(double val)	{__I_SumaDlyaIndexacii = d_to_m(val);};
	double I_SumaDlyaIndexacii()	{return __I_SumaDlyaIndexacii;};
	void setI_BaseMonth(bool val)	{__I_BaseMonth = val;};
	bool I_BaseMonth()	{return __I_BaseMonth;};
	
	void clear(){
		__id=0;
		__Vh_saldo=0;
		__N_Osn_narah=0;
		__N_VuslRokiv=0;
		__N_Likarn=0;
		__N_LikarnExt=0;
		__N_LikarnExtCount=0;
		__N_Vidpuskni=0;
		__N_VidpuskniExt=0;
		__N_VidpuskniExtCount=0;
		__N_Svatkovi_dni=0;
		__N_Nichni=0;
		__N_Nadurochni=0;
		__N_Klasn_vodiya=0;
		__N_Premia=0;
		__N_MaterDopomoga=0;
		__N_Doplata=0;
		__N_Indexaciya=0;
		__N_VyplProfspilky=0;
		__N_InshiNarahuvannya=0;
		__U_PensFond=0;
		__U_EV_Likarn=0;
		__U_EV_Vidpuskni=0;
		__U_Fond_zainat=0;
		__U_SocStrah=0;
		__U_SocPilga=0;
		__U_Prybutkov_podatok=0;
		__U_VidrahProfspilka=0;
		__U_InshiVidrahuvannya=0;
		__O_Inshi=0;
		__O_Shtraf=0;
		__O_Alimenty=0;
		__O_ZaPoperMis=0;
		__O_Premiya=0;
		__O_VsogoNaRuky=0;
		__O_VsogoNaRukyOtherDohid=0;
		__O_Avans=0;
		__O_Oplacheno=0;
		__V_PensFond=0;
		__V_EV_Likarn=0;
		__V_EV_Vidpuskni=0;
		__V_SocStrah=0;
		__V_Fond_zainat=0;
		__V_NeshchVyp=0;
		__V_Inshi=0;
		__Vids_U_PensFond=0;
		__Vids_U_EV_Likarn=0;
		__Vids_U_EV_Vidpuskni=0;
		__Vids_U_Fond_zainat=0;
		__Vids_U_Socstrah=0;
		__Vids_U_SocPilga=0;
		__Vids_U_Prybutkov_podatok=0;
		__Vids_U_VidrahProfspilka=0;
		__Vids_U_InshiVidrahuvannya=0;
		__Vids_V_PensFond=0;
		__Vids_V_EV_Likarn=0;
		__Vids_V_EV_Vidpuskni=0;
		__Vids_V_SocStrah=0;
		__Vids_V_Fond_zainat=0;
		__Vids_V_NeshchVyp=0;
		__Vids_V_Inshi=0;
		__Ozn_pilgy=0;
		__Oklad=0;
		__I_PromIndex=0;
		__I_Index=0;
		__I_ProindexovanaSuma=0;
		__I_SumaDlyaIndexacii=0;
		__I_BaseMonth=false;
	};
  private:
	//operacii table
	int				__id;
	QDate			__CurDate;
	double			__Vh_saldo;
	double			__N_Osn_narah;
	double			__N_VuslRokiv;
	double			__N_Likarn;
	double			__N_LikarnExt;
	int			__N_LikarnExtCount;
	double			__N_Vidpuskni;
	double			__N_VidpuskniExt;
	int			__N_VidpuskniExtCount;
	double			__N_Svatkovi_dni;
	double			__N_Nichni;
	double			__N_Nadurochni;
	double			__N_Klasn_vodiya;
	double			__N_Premia;
	double			__N_MaterDopomoga;
	double			__N_Doplata;
	double			__N_Indexaciya;
	double			__N_VyplProfspilky;
	double			__N_InshiNarahuvannya;
	double			__U_PensFond;
	double			__U_EV_Likarn;
	double			__U_EV_Vidpuskni;
	double			__U_Fond_zainat;
	double			__U_SocStrah;
	double			__U_SocPilga;
	double			__U_Prybutkov_podatok;
	double			__U_VidrahProfspilka;
	double			__U_InshiVidrahuvannya;
	double			__O_Inshi;
	double			__O_Shtraf;
	double			__O_Alimenty;
	double			__O_VsogoNaRuky;
	double			__O_VsogoNaRukyOtherDohid;
	double			__O_ZaPoperMis;
	double			__O_Premiya;
	double			__O_Avans;
	double			__O_Oplacheno;
	double			__V_PensFond;
	double			__V_EV_Likarn;
	double			__V_EV_Vidpuskni;
	double			__V_SocStrah;
	double			__V_Fond_zainat;
	double			__V_NeshchVyp;
	double			__V_Inshi;
	double			__Vids_U_PensFond;
	double			__Vids_U_EV_Likarn;
	double			__Vids_U_EV_Vidpuskni;
	double			__Vids_U_Fond_zainat;
	double			__Vids_U_Socstrah;
	double			__Vids_U_SocPilga;
	double			__Vids_U_Prybutkov_podatok;
	double			__Vids_U_VidrahProfspilka;
	double			__Vids_U_InshiVidrahuvannya;
	double			__Vids_V_PensFond;
	double			__Vids_V_EV_Likarn;
	double			__Vids_V_EV_Vidpuskni;
	double			__Vids_V_SocStrah;
	double			__Vids_V_Fond_zainat;
	double			__Vids_V_NeshchVyp;
	double			__Vids_V_Inshi;
	int				__Ozn_pilgy;
	short			__OznakaDohodu;
	double			__Oklad;
	double			__I_PromIndex;
	double			__I_Index;
	double			__I_ProindexovanaSuma;
	double			__I_SumaDlyaIndexacii;
	bool			__I_BaseMonth;
};
//--------------utech--------------------utech--------------------utech--------------------
class UWorkerOplata : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(double Vh_saldo READ Vh_saldo WRITE setVh_saldo)
	Q_PROPERTY(double NarahSum READ NarahSum WRITE setNarahSum)
	Q_PROPERTY(double UtrymSum READ UtrymSum WRITE setUtrymSum)
	Q_PROPERTY(double UtrymInshiSum READ UtrymInshiSum WRITE setUtrymInshiSum)
	Q_PROPERTY(double VidrahSum READ VidrahSum WRITE setVidrahSum)
	Q_PROPERTY(double O_Inshi READ O_Inshi WRITE setO_Inshi)
	Q_PROPERTY(double O_Shtraf READ O_Shtraf WRITE setO_Shtraf)
	Q_PROPERTY(double O_Alimenty READ O_Alimenty WRITE setO_Alimenty)
	Q_PROPERTY(double O_ZaPoperMis READ O_ZaPoperMis WRITE setO_ZaPoperMis)
	Q_PROPERTY(double O_Premiya READ O_Premiya WRITE setO_Premiya)
	Q_PROPERTY(double O_VsogoNaRuky READ O_VsogoNaRuky)
	Q_PROPERTY(double O_VsogoNaRukyOtherDohid READ O_VsogoNaRukyOtherDohid)
	Q_PROPERTY(double O_Avans READ O_Avans WRITE setO_Avans)
	Q_PROPERTY(double O_Oplacheno READ O_Oplacheno WRITE setO_Oplacheno)
	Q_PROPERTY(double Vyh_saldo READ Vyh_saldo WRITE setVyh_saldo)
	
	public:
	friend class USalaryScript;
	
	//опис властивостей класу (Q_PROPERTY)
	void setVh_saldo(double val)	{__Vh_saldo = val;};
	double Vh_saldo()	{return __Vh_saldo;};
	void setNarahSum(double val)	{__NarahSum = val;};
	double NarahSum()	{return __NarahSum;};
	void setUtrymSum(double val)	{__UtrymSum = val;};
	double UtrymSum()	{return __UtrymSum;};
	void setUtrymInshiSum(double val)	{__UtrymInshiSum = val;};
	double UtrymInshiSum()	{return __UtrymInshiSum;};
	void setVidrahSum(double val)	{__VidrahSum = val;};
	double VidrahSum()	{return __VidrahSum;};
	void setO_Inshi(double val)	{__O_Inshi = val;};
	double O_Inshi()	{return __O_Inshi;};
	void setO_Shtraf(double val)	{__O_Shtraf = val;};
	double O_Shtraf()	{return __O_Shtraf;};
	void setO_Alimenty(double val)	{__O_Alimenty = val;};
	double O_Alimenty()	{return __O_Alimenty;};
	void setO_ZaPoperMis(double val)	{__O_ZaPoperMis = d_to_m(val);};
	double O_ZaPoperMis()	{return __O_ZaPoperMis;};
	void setO_Premiya(double val)	{__O_Premiya = d_to_m(val);};
	double O_Premiya()	{return __O_Premiya;};
	double O_VsogoNaRuky()	{return __O_VsogoNaRuky;};
	double O_VsogoNaRukyOtherDohid()	{return __O_VsogoNaRukyOtherDohid;};
	void setO_Avans(double val)	{__O_Avans = val;};
	double O_Avans()	{return __O_Avans;};
	void setO_Oplacheno(double val)	{__O_Oplacheno = val;};
	double O_Oplacheno()	{return __O_Oplacheno;};
	void setVyh_saldo(double val)	{__Vyh_saldo = val;};
	double Vyh_saldo()	{return __Vyh_saldo;};
	
	void clear(){
		__Vh_saldo = 0;
		__NarahSum = 0;
		__UtrymSum = 0;
		__UtrymInshiSum = 0;
		__VidrahSum = 0;
		__O_Inshi = 0;
		__O_Shtraf = 0;
		__O_Alimenty = 0;
		__O_ZaPoperMis = 0;
		__O_Premiya = 0;
		__O_VsogoNaRuky = 0;
		__O_VsogoNaRukyOtherDohid = 0;
		__O_Avans = 0;
		__O_Oplacheno = 0;
		__Vyh_saldo = 0;
	}
	
  private:
	double	__Vh_saldo;
	double	__NarahSum;
	double	__UtrymSum;
	double	__UtrymInshiSum;
	double	__VidrahSum;
	double	__O_Inshi;
	double	__O_Shtraf;
	double	__O_Alimenty;
	double __O_ZaPoperMis;
	double __O_Premiya;
	double	__O_VsogoNaRuky;
	double	__O_VsogoNaRukyOtherDohid;
	double	__O_Avans;
	double	__O_Oplacheno;
	double	__Vyh_saldo;
};
//--------------utech--------------------utech--------------------utech--------------------
class UNeadCalcNarahValues : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(bool For_U_PensFond READ For_U_PensFond WRITE setFor_U_PensFond)
	Q_PROPERTY(bool For_U_FondZainat READ For_U_FondZainat WRITE setFor_U_FondZainat)
	Q_PROPERTY(bool For_U_SocStrah READ For_U_SocStrah WRITE setFor_U_SocStrah)
	Q_PROPERTY(bool For_U_PrybutkovyiPodatok READ For_U_PrybutkovyiPodatok WRITE setFor_U_PrybutkovyiPodatok)
	Q_PROPERTY(bool For_U_VidrahProfspilka READ For_U_VidrahProfspilka WRITE setFor_U_VidrahProfspilka)
	Q_PROPERTY(bool For_U_InshiVidrahuvannya READ For_U_InshiVidrahuvannya WRITE setFor_U_InshiVidrahuvannya)
	Q_PROPERTY(bool For_O_NaRuky READ For_O_NaRuky WRITE setFor_O_NaRuky)
	Q_PROPERTY(bool For_V_PensFond READ For_V_PensFond WRITE setFor_V_PensFond)
	Q_PROPERTY(bool For_V_SocStrah READ For_V_SocStrah WRITE setFor_V_SocStrah)
	Q_PROPERTY(bool For_V_FondZainat READ For_V_FondZainat WRITE setFor_V_FondZainat)
	Q_PROPERTY(bool For_V_NeshchVyp READ For_V_NeshchVyp WRITE setFor_V_NeshchVyp)
	Q_PROPERTY(bool For_V_Inshi READ For_V_Inshi WRITE setFor_V_Inshi)
	
	Q_PROPERTY(short CurOznakaDohodu READ CurOznakaDohodu WRITE setCurOznakaDohodu)
	
  public:
	friend class USalaryScript;
	UNeadCalcNarahValues(){__oznakaDohodu=0;};
	bool populateCalcNarahValuesForOD(short oznDoh);
	
	//опис властивостей класу (Q_PROPERTY)
	void setFor_U_PensFond(bool val)	{__For_U_PensFond = val;};
	bool For_U_PensFond()	{return __For_U_PensFond;};
	void setFor_U_FondZainat(bool val)	{__For_U_FondZainat = val;};
	bool For_U_FondZainat()	{return __For_U_FondZainat;};
	void setFor_U_SocStrah(bool val)	{__For_U_SocStrah = val;};
	bool For_U_SocStrah()	{return __For_U_SocStrah;};
	void setFor_U_PrybutkovyiPodatok(bool val)	{__For_U_PrybutkovyiPodatok = val;};
	bool For_U_PrybutkovyiPodatok()	{return __For_U_PrybutkovyiPodatok;};
	void setFor_U_VidrahProfspilka(bool val)	{__For_U_VidrahProfspilka = val;};
	bool For_U_VidrahProfspilka()	{return __For_U_VidrahProfspilka;};
	void setFor_U_InshiVidrahuvannya(bool val)	{__For_U_InshiVidrahuvannya = val;};
	bool For_U_InshiVidrahuvannya()	{return __For_U_InshiVidrahuvannya;};
	void setFor_O_NaRuky(bool val)	{__For_O_NaRuky = val;};
	bool For_O_NaRuky()	{return __For_O_NaRuky;};
	void setFor_V_PensFond(bool val)	{__For_V_PensFond = val;};
	bool For_V_PensFond()	{return __For_V_PensFond;};
	void setFor_V_SocStrah(bool val)	{__For_V_SocStrah = val;};
	bool For_V_SocStrah()	{return __For_V_SocStrah;};
	void setFor_V_FondZainat(bool val)	{__For_V_FondZainat = val;};
	bool For_V_FondZainat()	{return __For_V_FondZainat;};
	void setFor_V_NeshchVyp(bool val)	{__For_V_NeshchVyp = val;};
	bool For_V_NeshchVyp()	{return __For_V_NeshchVyp;};
	void setFor_V_Inshi(bool val)	{__For_V_Inshi = val;};
	bool For_V_Inshi()	{return __For_V_Inshi;};
	
	void setCurOznakaDohodu(short val)	{__oznakaDohodu = val;};
	short CurOznakaDohodu()	{return __oznakaDohodu;};
	
  private:
	//Змінні для Властивостей (Q_PROPERTY)
	bool __For_U_PensFond;
	bool __For_U_FondZainat;
	bool __For_U_SocStrah;
	bool __For_U_PrybutkovyiPodatok;
	bool __For_U_VidrahProfspilka;
	bool __For_U_InshiVidrahuvannya;
	bool __For_O_NaRuky;
	bool __For_V_PensFond;
	bool __For_V_SocStrah;
	bool __For_V_FondZainat;
	bool __For_V_NeshchVyp;
	bool __For_V_Inshi;
	//Загальні змінні
	int __oznakaDohodu;
};
//--------------utech--------------------utech--------------------utech--------------------
class USumTabel : public QObject, public QScriptable
{
	Q_OBJECT
	Q_PROPERTY(int Work_days_sum READ Work_days_sum)
	Q_PROPERTY(int Likarn_days_sum READ Likarn_days_sum)
	Q_PROPERTY(int Vidpusk_days_sum READ Vidpusk_days_sum)
	Q_PROPERTY(double God_zag_sum READ God_zag_sum)
	Q_PROPERTY(double God_nichni_sum READ God_nichni_sum)
	Q_PROPERTY(double God_sv_sum READ God_sv_sum)
  public:
	friend class USalaryScript;
	
	int Work_days_sum()	{return __Work_days_sum;};
	int Likarn_days_sum()	{return __Likarn_days_sum;};
	int Vidpusk_days_sum()	{return __Vidpusk_days_sum;};
	double God_zag_sum()	{return __God_zag_sum;};
	double God_nichni_sum()	{return __God_nichni_sum;};
	double God_sv_sum()	{return __God_sv_sum;};
  private:
	int __Work_days_sum;
	int __Likarn_days_sum;
	int __Vidpusk_days_sum;
	double __God_zag_sum;
	double __God_nichni_sum;
	double __God_sv_sum;
};
//--------------utech--------------------utech--------------------utech--------------------
class USalaryScript: public QObject
{
		Q_OBJECT
	public:
		USalaryScript();
		void populateWorkerInfo(QDate date, int workerID, short oznakaDohodu=1);
		void populateWorkerOperaciiInfo(QDate date, int workerID, short oznakaDohodu=1);
		void populateWorkerOperaciiHistoryInfo(QDate date, int workerID);
		bool populateWorkerOplataInfo(QDate date, int workerID);
		bool populateMonthlyBudjetniNormy(QDate date);
		bool zdiysnOsnNarahAllWorkers(QDate date);
		bool zdiysnOsnNarahOneWorker(QDate date, int workerID, bool __project=false);
		bool zdiysnDodatkNarahAllWorkers(QDate date);
		bool zdiysnDodatkNarahOneWorker(QDate date, int workerID, bool __project=false);
		bool zdiysnDodatkNarahOneWorkerOneOD(QDate date, int workerID, short oznakaDohodu, int operId);
		bool zdiysnObrahunkuVhidnogoSaldo(QDate date);
		bool zdiysnObrahunkuVhidnogoSaldoOneWorker(QDate date, int workerID);
		bool zdiysnObrahunkuPokaznOplaty(QDate date);
		bool zdiysnObrahunkuPokaznOplatyOneWorker(QDate date, int workerID);
		bool readNarahOsnNarahScript();
		bool readUtrVidrahOsnNarahScript();
		bool readUtrVidrahDodNarahScript();
		bool readVhSaldoScript();
		bool readCalcOplataValScript();
		void prob(QDate date, int workerID);
		~USalaryScript();
	public slots:
		
	private:
		QScriptEngine salaryScriptEngine;
		QScriptValue workerInfoSV;
		UWorkerInfo *wInfo;
		QScriptValue workerTabelSV;
		UDayWorkerTabel wTabel[32];
		QScriptValue scriptValue;
		UIdVal *budjetniNormy;
		UBudjetniNormyMonthly * mbn;
		UWorkerOperacii *workerOperacii;
		UWorkerOperacii *wAdOperacii;
		UWorkerOplata *wOplata;
		USumTabel *sumTabel;
		UNeadCalcNarahValues * neadCalcNarahValues;
		UProgramOptions * opt;
		int budjetniNormyCount;
		bool populateMonthlyBudjetniNormy_done;
		
		QString osnNarahScriptString;
		QString osnUrtVidrahScriptString;
		QString dodUrtVidrahScriptString;
		QString saldoScriptString;
		QString oplataValScriptString;
};
//--------------------utech--------------------utech--------------------utech--------------------

#endif
