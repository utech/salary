//--------------------utech--------------------utech--------------------utech--------------------
//	Файл скрипта для здійснення основного нарахування в програмі нарахування зарплати
//	Глобальними змінними-класами є:
//		class UWorkerInfo wInfo - інформація про працівника, береться з таблиці npr;
//		class UDayWorkerTabel wTabel[32] - табель працівника, береться з таблиці tabel;
//		class UWorkerOperacii wOperacii[13] - інформація про нарахування працівника, береться з таблиці operacii;
//		class UIdVal budjetniNormy[] - інформація про встановлені державою бюджетні норми, береться з таблиці budjetni_normy;
//		class USumTabel sumTabel - підсумкова інформація про показники роботи працівника за місяць (використовується при внесенні не повного табеля, а лише підсумків);
//		class UWorkerDodatkDohody wDodatkDohody - підсумкова інформація про додаткові доходи працівника
//	Також, для зручності внесено функції:
//		double budjNorma(int id) - отримання чначення бюджетної норми по ідентифікатору з таблиці budjetni_normy; 
//		double workDays(int year, int month) кількість робочих днів в місяці month, року year;
//		double d2m(double d) - заокруглення до сотих;
//	Передбачено і прості глобальні змінні
//		vydTabelya - значення, яке характеризує вид табеля, який використовується в нарахуванні
//		budjetniNormyCount - кількість значень бюджетних норм вбазі даних
//	У класах  wOperacii, mbn нульвим членом масиву є значення поточного місяця, першим попереднього до поточного і т.д.
//	
//	Створений програмістами компанії "УТех" 11.12.2007р.
//--------------------utech--------------------utech--------------------utech--------------------

//-----Індексація зарплати
/*wOperacii[0].I_PromIndex = 10.1;
wOperacii[0].I_Index = 20.2;
wOperacii[0].I_ProindexovanaSuma = 30.3;
wOperacii[0].I_SumaDlyaIndexacii = 40.4;
wOperacii[1].I_PromIndex = 11.1;
wOperacii[1].I_Index = 21.2;
wOperacii[1].I_ProindexovanaSuma = 31.3;
wOperacii[1].I_SumaDlyaIndexacii = 41.4;
wOperacii[2].I_PromIndex = 12.1;
wOperacii[2].I_Index = 22.2;
wOperacii[2].I_ProindexovanaSuma = 32.3;
wOperacii[2].I_SumaDlyaIndexacii = 42.4;
*/
print("mbn[0].WorkDaysInMonth: ", mbn[0].WorkDaysInMonth);
print("mbn[0].WorkHoursInMonth: ", mbn[0].WorkHoursInMonth);
print("mbn[0].MinZarplata: ", mbn[0].MinZarplata);
print("mbn[0].MinProjytkovyi: ", mbn[0].MinProjytkovyi);
print("mbn[0].IndexSpojyvCin: ", mbn[0].IndexSpojyvCin);

//----------------Відпускні
/*var serZarplata=0;
for (i=1; i<13; i++){//Сума нарахування за рік
	serZarplata += wOperacii[i].N_Osn_narah;
	serZarplata += wOperacii[i].N_VuslRokiv;
	serZarplata += wOperacii[i].N_Likarn;
	serZarplata += wOperacii[i].N_Vidpuskni;
	serZarplata += wOperacii[i].N_Svatkovi_dni;
	serZarplata += wOperacii[i].N_Nichni;
}
serZarplata = serZarplata / (budjNorma(18) - budjNorma(17));
print("budjNorma(18)", budjNorma(18));
print("budjNorma(17)", budjNorma(17));

var dniVidpustky = 0;
if (vydTabelya == 0){ //Нарахування за повним табелем
for (i=0; i<32; i++)//Обрахунок кількості днів відпустки
	if (wTabel[i].Pracivn_status_ID==3)
		dniVidpustky++;
}
else if (vydTabelya == 1){ //Нарахування за підсумковим табелем
	dniVidpustky = sumTabel.Vidpusk_days_sum;
}
wOperacii[0].N_Vidpuskni = serZarplata * dniVidpustky;
print("wOperacii[0].N_Vidpuskni: ", wOperacii[0].N_Vidpuskni);
print("serZarplata: ", serZarplata);
print("dniVidpustky: ", dniVidpustky);
*/

