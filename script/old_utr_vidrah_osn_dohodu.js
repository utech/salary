//--------------------utech--------------------utech--------------------utech--------------------
//	Файл скрипта для здійснення додаткового нарахування в програмі нарахування зарплати
//	Глобальними змінними-класами є:
//		class UWorkerInfo wInfo - інформація про працівника, береться з таблиці npr;
//		class UDayWorkerTabel wTabel[32] - табель працівника, береться з таблиці tabel;
//		class UWorkerOperacii wOperacii[13] - інформація про нарахування працівника, береться з таблиці operacii;
//		class UIdVal budjetniNormy[] - інформація про встановлені державою бюджетні норми, береться з таблиці budjetni_normy;
//		class UNumWorkDays numWorkDays[13] - інформація про кількість робочих днів у місяці (за цілий рік+поточний місяць), береться з таблиці workday;
//		class USumTabel sumTabel - підсумкова інформація про показники роботи працівника за місяць (використовується при внесенні не повного табеля, а лише підсумків);
//		class UWorkerDodatkDohody wDodatkDohody - підсумкова інформація про додаткові доходи працівника
//	Також, для зручності внесено функції:
//		double budjNorma(int id) - отримання чначення бюджетної норми по ідентифікатору з таблиці budjetni_normy; 
//		double workDays(int year, int month) кількість робочих днів в місяці month, року year;
//		double d2m(double d) - заокруглення до сотих;
//	Передбачено і прості глобальні змінні
//		vydTabelya - значення, яке характеризує вид табеля, який використовується в нарахуванні
//		budjetniNormyCount - кількість значень бюджетних норм вбазі даних
//	У класах  wOperacii, numWorkDays нульвим членом масиву є значення поточного місяця, першим попереднього до поточного і т.д.
//	
//	Створений програмістами компанії "УТех" 11.12.2007р.
//--------------------utech--------------------utech--------------------utech--------------------

//----------------Всього нарахування ---перерахунок для точності

var vsogoNarahNoLik = wOperacii[0].N_Osn_narah + wOperacii[0].N_VuslRokiv + wOperacii[0].N_Vidpuskni +
							wOperacii[0].N_Svatkovi_dni + wOperacii[0].N_Nichni + wOperacii[0].N_Nadurochni + wOperacii[0].N_Klasn_vodiya +
							wOperacii[0].N_Premia + wOperacii[0].N_MaterDopomoga + wOperacii[0].N_Doplata + wOperacii[0].N_Indexaciya + 
							wOperacii[0].N_VyplProfspilky + wOperacii[0].N_InshiNarahuvannya;
var vsogoNarah = vsogoNarahNoLik + wOperacii[0].N_Likarn;
//----------------Пенсійний Фонд
wOperacii[0].U_PensFond = vsogoNarah * budjNorma(7) / 100;
//----------------Фонд зайнятості
//if (!wInfo.Pensioner){
	wOperacii[0].U_Fond_zainat = vsogoNarahNoLik * budjNorma(8) / 100;
	//wOperacii[0].U_Fond_zainat = vsogoNarahNoLik * 6 * 0.5 / (100 * 20);
	//wOperacii[0].U_Fond_zainat += vsogoNarahNoLik * 14 * 0.6 / (100 * 20);
//}
//else
//	wOperacii[0].U_Fond_zainat = 0;
//----------------Фонд Соціального страхування і відсоток для соцстраху
if (vsogoNarahNoLik <= budjNorma(5)){
	wOperacii[0].U_SocStrah = vsogoNarahNoLik * budjNorma(9) / 100;
	wOperacii[0].Vids_U_Socstrah = budjNorma(9);
}
else{
	wOperacii[0].U_SocStrah = vsogoNarahNoLik * budjNorma(10) / 100;
	wOperacii[0].Vids_U_Socstrah = budjNorma(10);
}

//----------------Відрахування на профспілку
if (wInfo.ChlenProfspilky)
	wOperacii[0].U_VidrahProfspilka = vsogoNarahNoLik * budjNorma(16) / 100;
else
	wOperacii[0].U_VidrahProfspilka = 0;

//----------------Соціальна пільга
wOperacii[0].Ozn_pilgy = 0;
wOperacii[0].U_SocPilga = 0;
if (opt.TypSocPilgy == 0){
	wOperacii[0].Ozn_pilgy = wInfo.Ozn_pilgy;
	wOperacii[0].U_SocPilga = sumaSocPilgy(wInfo.Ozn_pilgy);
}
else if (vsogoNarah<budjNorma(15) && wInfo.Ozn_pilgy>0 && wInfo.Ozn_pilgy<5 && wInfo.TypRoboty==0){
	wOperacii[0].Ozn_pilgy = wInfo.Ozn_pilgy;
	wOperacii[0].U_SocPilga = sumaSocPilgy(wInfo.Ozn_pilgy);
}

//----------------Прибутковий податок
wOperacii[0].U_Prybutkov_podatok = (vsogoNarah - (wOperacii[0].U_PensFond + wOperacii[0].U_SocStrah + wOperacii[0].U_Fond_zainat + wOperacii[0].U_SocPilga)) * budjNorma(13) / 100;
//----------------Інші відрахування

//--Всього утримання
var vsogoUtrym = wOperacii[0].U_PensFond - wOperacii[0].U_Fond_zainat - wOperacii[0].U_SocStrah - wOperacii[0].U_Prybutkov_podatok - wOperacii[0].U_VidrahProfspilka - wOperacii[0].U_InshiVidrahuvannya;
//----------------Аванс

//----------------Заборгованість працівника перед організацією

//----------------Штраф

//----------------Відрахування-----Пенсійний фонд
if (!wInfo.Invalid){
	wOperacii[0].V_PensFond = vsogoNarah * budjNorma(20) / 100;
	wOperacii[0].Vids_V_PensFond = budjNorma(20);
}
else{
	wOperacii[0].V_PensFond = vsogoNarah * budjNorma(21) / 100;
	wOperacii[0].Vids_V_PensFond = budjNorma(21);
}
//----------------Відрахування-----Фонд соціального страхування з тимчасової втрати працездатності
wOperacii[0].V_SocStrah = vsogoNarahNoLik * budjNorma(22) / 100;

//----------------Відрахування-----Фонд Зайнятості
//Основна
wOperacii[0].V_Fond_zainat = vsogoNarahNoLik * budjNorma(23) / 100;

//----------------Відрахування-----Фонду соціального страхування від нещасних випадків на виробництві та профзахворювань
wOperacii[0].V_NeshchVyp = vsogoNarahNoLik * budjNorma(24) / 100;

//----------------Відрахування-----Інші відрахування
wOperacii[0].V_Inshi = vsogoNarah * budjNorma(26) / 100;
