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
var vsogoNarah = wOperacii[0].N_Osn_narah;

//----------------Утримання-----Пенсійний Фонд
if (uNeadCalcNarahValues.For_U_PensFond){
	wOperacii[0].U_PensFond = vsogoNarah * budjNorma(7) / 100;
}
else{
	wOperacii[0].U_PensFond = 0;
}

//----------------Утримання-----Фонд зайнятості
if (uNeadCalcNarahValues.For_U_FondZainat){
	wOperacii[0].U_Fond_zainat = vsogoNarah * budjNorma(8) / 100;
}
else{
	wOperacii[0].U_Fond_zainat = 0;
}

//----------------Утримання-----Фонд Соціального страхування і відсоток для соцстраху
if (uNeadCalcNarahValues.For_U_SocStrah){
	wOperacii[0].U_SocStrah = vsogoNarah * budjNorma(9) / 100;
}
else{
	wOperacii[0].U_SocStrah = 0;
}

//----------------Утримання-----Соціальна пільга
wOperacii[0].U_SocPilga = 0;
wOperacii[0].Ozn_pilgy = 0;

//----------------Утримання-----Прибутковий податок
if (uNeadCalcNarahValues.For_U_PrybutkovyiPodatok){
	wOperacii[0].U_Prybutkov_podatok = vsogoNarah * budjNorma(13) / 100;
}
else{
	wOperacii[0].U_Prybutkov_podatok = 0;
}

//----------------Утримання-----Відрахування на профспілку
if (uNeadCalcNarahValues.For_U_VidrahProfspilka){
	wOperacii[0].U_VidrahProfspilka = vsogoNarah * budjNorma(16) / 100;
}
else{
	wOperacii[0].U_VidrahProfspilka = 0;
}

//----------------Утримання-----Інші утримання
if (uNeadCalcNarahValues.For_U_InshiVidrahuvannya){
	wOperacii[0].U_InshiVidrahuvannya = vsogoNarah * budjNorma(25) / 100;
}
else{
	wOperacii[0].U_InshiVidrahuvannya = 0;
}

//--Всього утримано
var vsogoUtrym = wOperacii[0].U_PensFond + wOperacii[0].U_Fond_zainat + wOperacii[0].U_SocStrah + wOperacii[0].U_Prybutkov_podatok + wOperacii[0].U_VidrahProfspilka + wOperacii[0].U_InshiVidrahuvannya;

//----------------Відрахування-----Пенсійний фонд
if (uNeadCalcNarahValues.For_V_PensFond){
	if (!wInfo.Invalid){
		wOperacii[0].V_PensFond = vsogoNarah * budjNorma(20) / 100;
		wOperacii[0].Vids_V_PensFond = budjNorma(20);
	}
	else{
		wOperacii[0].V_PensFond = vsogoNarah * budjNorma(21) / 100;
		wOperacii[0].Vids_V_PensFond = budjNorma(21);
	}
}
else{
	wOperacii[0].V_PensFond = 0;
}

//----------------Відрахування-----Фонд соціального страхування з тимчасової втрати працездатності
if (uNeadCalcNarahValues.For_V_SocStrah){
	wOperacii[0].V_SocStrah = vsogoNarah * budjNorma(22) / 100;
}
else{
	wOperacii[0].V_SocStrah = 0;
}

//----------------Відрахування-----Фонд Зайнятості
if (uNeadCalcNarahValues.For_V_FondZainat){
	wOperacii[0].V_Fond_zainat = vsogoNarah * budjNorma(23) / 100;
}
else{
	wOperacii[0].V_Fond_zainat = 0;
}

//----------------Відрахування-----Фонду соціального страхування від нещасних випадків на виробництві та профзахворювань
if (uNeadCalcNarahValues.For_V_NeshchVyp){
	wOperacii[0].V_NeshchVyp = vsogoNarah * budjNorma(24) / 100;
}
else{
	wOperacii[0].V_NeshchVyp = 0;
}

//----------------Відрахування-----Інші відрахування
if (uNeadCalcNarahValues.For_V_Inshi){
	wOperacii[0].V_Inshi = vsogoNarah * budjNorma(26) / 100;
}
else{
	wOperacii[0].V_Inshi = 0;
}

//----------------Оплата-----На руки
if (uNeadCalcNarahValues.For_O_NaRuky){
	wOperacii[0].O_VsogoNaRuky = vsogoNarah;
}
else{
	wOperacii[0].O_VsogoNaRuky = 0;
}
