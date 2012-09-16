//--------------------utech--------------------utech--------------------utech--------------------
//	Пробний скрипт програми нарахування зарплати
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

print("   ---   Завантажено скрипт prob.js (C)-UTECH Company   ---");

//Вивід значень змінних-членів класу (UWorkerInfo wInfo)
print("      ----   wInfo.Npr_ID="+wInfo.Npr_ID);
print("      ----   wInfo.Data_nar="+wInfo.Data_nar);
print("      ----   wInfo.Data_nar_day="+wInfo.Data_nar_day);
print("      ----   wInfo.Data_nar_month="+wInfo.Data_nar_month);
print("      ----   wInfo.Data_nar_year="+wInfo.Data_nar_year);
print("      ----   wInfo.Posada_id="+wInfo.Posada_id);
print("      ----   wInfo.TypRoboty="+wInfo.TypRoboty);
print("      ----   wInfo.TypOplaty="+wInfo.TypOplaty);
print("      ----   wInfo.Rozryad="+wInfo.Rozryad);
print("      ----   wInfo.Stavka="+wInfo.Stavka);
print("      ----   wInfo.Num_dit="+wInfo.Num_dit);
print("      ----   wInfo.Ozn_pilgu="+wInfo.Ozn_pilgu);
print("      ----   wInfo.Data_pruin_na_rob="+wInfo.Data_pruin_na_rob);
print("      ----   wInfo.Data_pruin_na_rob_day="+wInfo.Data_pruin_na_rob_day);
print("      ----   wInfo.Data_pruin_na_rob_month="+wInfo.Data_pruin_na_rob_month);
print("      ----   wInfo.Data_pruin_na_rob_year="+wInfo.Data_pruin_na_rob_year);
print("      ----   wInfo.Klasnist_vodiya="+wInfo.Klasnist_vodiya);
print("      ----   wInfo.VidsDoplaty="+wInfo.VidsDoplaty);
print("      ----   wInfo.Alimenty="+wInfo.Alimenty);
print("      ----   wInfo.Pensioner="+wInfo.Pensioner);
print("      ----   wInfo.Vids_vuslugy_rokiv="+wInfo.Vids_vuslugy_rokiv);
print("      ----   wInfo.Staj_roboty="+wInfo.Staj_roboty);
print("      ----   wInfo.Buh_rahunok="+wInfo.Buh_rahunok);
print("      ----   wInfo.Grupa_id="+wInfo.Grupa_id);
print("      ----   wInfo.ChlenProfspilky="+wInfo.ChlenProfspilky);
print("      ----   wInfo.Invalid="+wInfo.Invalid);
print("      ----   wInfo.Pracuye="+wInfo.Pracuye);

//Виведення табеля
for (iter=0;iter<32;iter++){
	print("      -----   wTabel["+iter+"].Pracivn_status_ID="+wTabel[iter].Pracivn_status_ID);
	print("      -----   wTabel["+iter+"].God_zag="+wTabel[iter].God_zag);
	print("      -----   wTabel["+iter+"].God_nichni="+wTabel[iter].God_nichni);
	print("      -----   wTabel["+iter+"].God_sv="+wTabel[iter].God_sv);
}

//Виведення підсумкового табеля
print("       --   sumTabel.Work_days_sum="+sumTabel.Work_days_sum);
print("       --   sumTabel.Likarn_days_sum="+sumTabel.Likarn_days_sum);
print("       --   sumTabel.Vidpusk_days_sum="+sumTabel.Vidpusk_days_sum);
print("       --   sumTabel.God_zag_sum="+sumTabel.God_zag_sum);
print("       --   sumTabel.God_nichni_sum="+sumTabel.God_nichni_sum);
print("       --   sumTabel.God_sv_sum="+sumTabel.God_sv_sum);

//Виведення значення з таблиці operacii
for (iter=0;iter<13;iter++){
	print("      ---   wOperacii["+iter+"].id="+wOperacii[iter].id);
	print("      ---   wOperacii["+iter+"].CurDate="+wOperacii[iter].CurDate);
	print("      ---   wOperacii["+iter+"].CurDate_year="+wOperacii[iter].CurDate_year);
	print("      ---   wOperacii["+iter+"].CurDate_month="+wOperacii[iter].CurDate_month);
	print("      ---   wOperacii["+iter+"].CurDate_day="+wOperacii[iter].CurDate_day);
	print("      ---   wOperacii["+iter+"].Vh_saldo="+wOperacii[iter].Vh_saldo);
	print("      ---   wOperacii["+iter+"].N_Osn_narah="+wOperacii[iter].N_Osn_narah);
	print("      ---   wOperacii["+iter+"].N_VuslRokiv="+wOperacii[iter].N_VuslRokiv);
	print("      ---   wOperacii["+iter+"].N_Likarn="+wOperacii[iter].N_Likarn);
	print("      ---   wOperacii["+iter+"].N_Vidpuskni="+wOperacii[iter].N_Vidpuskni);
	print("      ---   wOperacii["+iter+"].N_Svatkovi_dni="+wOperacii[iter].N_Svatkovi_dni);
	print("      ---   wOperacii["+iter+"].N_Nichni="+wOperacii[iter].N_Nichni);
	print("      ---   wOperacii["+iter+"].N_Nadurochni="+wOperacii[iter].N_Nadurochni);
	print("      ---   wOperacii["+iter+"].N_Klasn_vodiya="+wOperacii[iter].N_Klasn_vodiya);
	print("      ---   wOperacii["+iter+"].N_Premia="+wOperacii[iter].N_Premia);
	print("      ---   wOperacii["+iter+"].N_MaterDopomoga="+wOperacii[iter].N_MaterDopomoga);
	print("      ---   wOperacii["+iter+"].N_Doplata="+wOperacii[iter].N_Doplata);
	print("      ---   wOperacii["+iter+"].N_VyplProfspilky="+wOperacii[iter].N_VyplProfspilky);
	print("      ---   wOperacii["+iter+"].N_InshiNarahuvannya="+wOperacii[iter].N_InshiNarahuvannya);
	print("      ---   wOperacii["+iter+"].U_PensFond="+wOperacii[iter].U_PensFond);
	print("      ---   wOperacii["+iter+"].U_Fond_zainat="+wOperacii[iter].U_Fond_zainat);
	print("      ---   wOperacii["+iter+"].U_SocStrah="+wOperacii[iter].U_SocStrah);
	print("      ---   wOperacii["+iter+"].U_SocPilga="+wOperacii[iter].U_SocPilga);
	print("      ---   wOperacii["+iter+"].U_Prybutkov_podatok="+wOperacii[iter].U_Prybutkov_podatok);
	print("      ---   wOperacii["+iter+"].U_VidrahProfspilka="+wOperacii[iter].U_VidrahProfspilka);
	print("      ---   wOperacii["+iter+"].U_InshiVidrahuvannya="+wOperacii[iter].U_InshiVidrahuvannya);
	print("      ---   wOperacii["+iter+"].O_Zaborgovanist="+wOperacii[iter].O_Zaborgovanist);
	print("      ---   wOperacii["+iter+"].O_Shtraf="+wOperacii[iter].O_Shtraf);
	print("      ---   wOperacii["+iter+"].O_Alimenty="+wOperacii[iter].O_Alimenty);
	print("      ---   wOperacii["+iter+"].O_VsogoNaRuky="+wOperacii[iter].O_VsogoNaRuky);
	print("      ---   wOperacii["+iter+"].O_Avans="+wOperacii[iter].O_Avans);
	print("      ---   wOperacii["+iter+"].O_Oplacheno="+wOperacii[iter].O_Oplacheno);
	print("      ---   wOperacii["+iter+"].V_PensFond="+wOperacii[iter].V_PensFond);
	print("      ---   wOperacii["+iter+"].V_SocStrah="+wOperacii[iter].V_SocStrah);
	print("      ---   wOperacii["+iter+"].V_Fond_zainat="+wOperacii[iter].V_Fond_zainat);
	print("      ---   wOperacii["+iter+"].V_NeshchVyp="+wOperacii[iter].V_NeshchVyp);
	print("      ---   wOperacii["+iter+"].V_Inshi="+wOperacii[iter].V_Inshi);
	
	print("      ---   wOperacii["+iter+"].Vids_U_PensFond="+wOperacii[iter].Vids_U_PensFond);
	print("      ---   wOperacii["+iter+"].Vids_U_Fond_zainat="+wOperacii[iter].Vids_U_Fond_zainat);
	print("      ---   wOperacii["+iter+"].Vids_U_Socstrah="+wOperacii[iter].Vids_U_Socstrah);
	print("      ---   wOperacii["+iter+"].Vids_U_SocPilga="+wOperacii[iter].Vids_U_SocPilga);
	print("      ---   wOperacii["+iter+"].Vids_U_Prybutkov_podatok="+wOperacii[iter].Vids_U_Prybutkov_podatok);
	print("      ---   wOperacii["+iter+"].Vids_U_VidrahProfspilka="+wOperacii[iter].Vids_U_VidrahProfspilka);
	print("      ---   wOperacii["+iter+"].Vids_U_InshiVidrahuvannya="+wOperacii[iter].Vids_U_InshiVidrahuvannya);
	print("      ---   wOperacii["+iter+"].Vids_V_PensFond="+wOperacii[iter].Vids_V_PensFond);
	print("      ---   wOperacii["+iter+"].Vids_V_SocStrah="+wOperacii[iter].Vids_V_SocStrah);
	print("      ---   wOperacii["+iter+"].Vids_V_Fond_zainat="+wOperacii[iter].Vids_V_Fond_zainat);
	print("      ---   wOperacii["+iter+"].Vids_V_NeshchVyp="+wOperacii[iter].Vids_V_NeshchVyp);
	print("      ---   wOperacii["+iter+"].Vids_V_Inshi="+wOperacii[iter].Vids_V_Inshi);
	
	print("      ---   wOperacii["+iter+"].Ozn_pilgy="+wOperacii[iter].Ozn_pilgu);
	print("      ---   wOperacii["+iter+"].OznakaDohodu="+wOperacii[iter].OznakaDohodu);
}

//Виведення значення з таблиці budjetni_normu
print("      -------   budjetniNormyCount="+budjetniNormyCount);
for (iter=0;iter<budjetniNormyCount;iter++){
	print("      -------   budjetniNormy["+iter+"].id="+budjetniNormy[iter].id);
	print("      -------   budjetniNormy["+iter+"].val="+budjetniNormy[iter].val);
	print("      -------   budjNorma("+budjetniNormy[iter].id+")="+budjNorma(budjetniNormy[iter].id));
}

//Виведення значення з таблиці workday
for (iter=0;iter<13;iter++){
	print("      -----   numWorkDays["+iter+"].year="+numWorkDays[iter].year);
	print("      -----   numWorkDays["+iter+"].month="+numWorkDays[iter].month);
	print("      -----   numWorkDays["+iter+"].numDays="+numWorkDays[iter].numDays);
	print("      -----   numWorkDays["+iter+"].numGod="+numWorkDays[iter].numGod);
	print("      -----   workDays("+numWorkDays[iter].year+","+numWorkDays[iter].month+")="+workDays(numWorkDays[iter].year, numWorkDays[iter].month));
}

//Виведення значень глобальних змінних
print("     ---   budjetniNormyCount="+budjetniNormyCount);
print("     ---   vydTabelya="+vydTabelya);

print("   ---   Завершено  виконання скрипта prob.js   ---");
