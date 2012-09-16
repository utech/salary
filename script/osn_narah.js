var kilkistRob=0;
var kilkistRobHours=0;
var kilkistRobMinutes=0;
var i=0;

//----------------Основне нарахування
if (wInfo.VydTabelya == 0){ //Нарахування за повним табелем
	if (wInfo.TypOplaty==0){  //помісячна оплата
		for (i=0; i<32; i++)
			if (wTabel[i].Pracivn_status_ID==1)
				kilkistRob++;
		if (kilkistRob>mbn[0].WorkDaysInMonth)
			kilkistRob = mbn[0].WorkDaysInMonth;
		wOperacii[0].N_Osn_narah = (kilkistRob / mbn[0].WorkDaysInMonth) * wInfo.Stavka;
	}
	else if (wInfo.TypOplaty==1){ //погодинна оплата
		kilkistRobHours=0;
		kilkistRobMinutes=0;
		for (i=0; i<32; i++){
			kilkistRobHours += hour(wTabel[i].God_zag);
			kilkistRobMinutes += minute(wTabel[i].God_zag);
		}
		wOperacii[0].N_Osn_narah = (kilkistRobHours+kilkistRobMinutes/60)*wInfo.Stavka;
	}
	else
		wOperacii[0].N_Osn_narah = 0;
}
else if (wInfo.VydTabelya == 1){ //Нарахування за підсумковим табелем
	if (wInfo.TypOplaty==0){  //помісячна оплата
		kilkistRob = sumTabel.Work_days_sum;
		if (kilkistRob>mbn[0].WorkDaysInMonth)
			kilkistRob=mbn[0].WorkDaysInMonth;
		wOperacii[0].N_Osn_narah=(kilkistRob / mbn[0].WorkDaysInMonth) * wInfo.Stavka;
	}
	else if (wInfo.TypOplaty==1){ //погодинна оплата
		wOperacii[0].N_Osn_narah=sumTabel.God_zag_sum * wInfo.Stavka;
	}
	else
		wOperacii[0].N_Osn_narah = 0;
}

//----------------Класність водія
wOperacii[0].N_Klasn_vodiya = wInfo.Klasnist_vodiya * wOperacii[0].N_Osn_narah / 100.0;

//----------------Вислуга років
wOperacii[0].N_VuslRokiv = wInfo.Vids_vuslugy_rokiv * (wOperacii[0].N_Osn_narah + wOperacii[0].N_Klasn_vodiya) / 100.0;

//----------------Лікарняні
if (wOperacii[0].N_LikarnExtCount>0)
	wOperacii[0].N_Likarn = wOperacii[0].N_LikarnExt;
	
//----------------Відпускні
if (wOperacii[0].N_VidpuskniExtCount>0)
	wOperacii[0].N_Vidpuskni = wOperacii[0].N_VidpuskniExt;

//----------------Святкові
var svyatkoviGod = 0;
var svyatkoviGodHours = 0;
var svyatkoviGodMinutes = 0;
if (wInfo.TypOplaty == 1){ //Погодинна оплата
	if (wInfo.VydTabelya == 0){ //Нарахування за повним табелем
		for (i=0; i<32; i++){
			svyatkoviGodHours += hour(wTabel[i].God_sv);
			svyatkoviGodMinutes += minute(wTabel[i].God_sv);
		}
		wOperacii[0].N_Svatkovi_dni = budjNorma(12) * (svyatkoviGodHours + svyatkoviGodMinutes/60) * wInfo.Stavka / 100.0;
	}
	else if (wInfo.VydTabelya == 1){ //Нарахування за підсумковим табелем
		svyatkoviGod = sumTabel.God_sv_sum;
		wOperacii[0].N_Svatkovi_dni = budjNorma(12) * svyatkoviGod * wInfo.Stavka / 100.0;
	}
}
else if (wInfo.TypOplaty == 0) //Помісячна оплата
	if (wInfo.VydTabelya == 0){ //Нарахування за повним табелем
		for (i=0; i<32; i++){
			svyatkoviGodHours += hour(wTabel[i].God_sv);
			svyatkoviGodMinutes += minute(wTabel[i].God_sv);
		}
		wOperacii[0].N_Svatkovi_dni = budjNorma(12) * (svyatkoviGodHours + svyatkoviGodMinutes/60) 
										* wInfo.Stavka / mbn[0].WorkHoursInMonth / 100.0;
	}
	else if (wInfo.VydTabelya == 1){ //Нарахування за підсумковим табелем
		svyatkoviGod = sumTabel.God_sv_sum;
		wOperacii[0].N_Svatkovi_dni = budjNorma(12) * svyatkoviGod * wInfo.Stavka / mbn[0].WorkHoursInMonth / 100.0;
	}
else
	wOperacii[0].N_Svatkovi_dni = 0;
//----------------Нічні
var nichniGod = 0;
var nichniGodHours = 0;
var nichniGodMinutes = 0;
if (wInfo.TypOplaty == 1){
	if (wInfo.VydTabelya == 0){ //Нарахування за повним табелем
		for (i=0; i<32; i++){
			nichniGodHours += hour(wTabel[i].God_nichni);
			nichniGodMinutes += minute(wTabel[i].God_nichni);
		}
		wOperacii[0].N_Nichni = budjNorma(11) * (nichniGodHours + nichniGodMinutes/60) * wInfo.Stavka / 100.0;
	}
	else if (wInfo.VydTabelya == 1){ //Нарахування за підсумковим табелем
		nichniGod = sumTabel.God_nichni_sum;
		wOperacii[0].N_Nichni = budjNorma(11) * nichniGod * wInfo.Stavka / 100.0;
	}
}
else if (wInfo.TypOplaty == 0) { //Помісячна оплата
	if (wInfo.VydTabelya == 0){ //Нарахування за повним табелем
		for (i=0; i<32; i++){
			nichniGodHours += hour(wTabel[i].God_nichni);
			nichniGodMinutes += minute(wTabel[i].God_nichni);
		}
		wOperacii[0].N_Nichni = budjNorma(11) * (nichniGodHours + nichniGodMinutes/60) 
								* wInfo.Stavka / mbn[0].WorkHoursInMonth / 100.0;
	}
	else if (wInfo.VydTabelya == 1){ //Нарахування за підсумковим табелем
		nichniGod = sumTabel.God_nichni_sum;
		wOperacii[0].N_Nichni = budjNorma(11) * nichniGod 
								* wInfo.Stavka / mbn[0].WorkHoursInMonth / 100.0;
	}
}
else
	wOperacii[0].N_Nichni = 0;

//----------------Надурочні
wOperacii[0].N_Nadurochni = 0;
if (wInfo.TypOplaty == 1)
	if (wInfo.VydTabelya == 0){ //Нарахування за повним табелем
		zagGod = 0;
		for (i=0; i<32; i++)
			zagGod += wTabel[i].God_zag;
		if (zagGod > mbn[0].WorkHoursInMonth)
		wOperacii[0].N_Nadurochni = budjNorma(19) * (zagGod - mbn[0].WorkHoursInMonth) * wInfo.Stavka / 100.0;
	}
	else if ((wInfo.VydTabelya==1) && (sumTabel.God_zag_sum>mbn[0].WorkHoursInMonth) ){ //Нарахування за підсумковим табелем
		wOperacii[0].N_Nadurochni = budjNorma(19) * (sumTabel.God_zag_sum-mbn[0].WorkHoursInMonth) * wInfo.Stavka / 100.0;
	}
	
//----------------Премія

//----------------Матеріальна допомога

//----------------Доплата
wOperacii[0].N_Doplata = wInfo.VidsDoplaty * wOperacii[0].N_Osn_narah / 100.0;

//----------------Виплата профспілки

//----------------Інші нарахування

//----------------Індексація зарплати
if ( (opt.TypIndexaciyi > 0) && (wOperacii[0].CurDate >= wInfo.Data_pruin_na_rob) ) { //Провірка необхідності проведення індексації
	wOperacii[0].I_SumaDlyaIndexacii = wOperacii[0].N_Osn_narah + wOperacii[0].N_VuslRokiv +
							wOperacii[0].N_Likarn + wOperacii[0].N_Klasn_vodiya +
							wOperacii[0].N_Premia + wOperacii[0].N_Doplata + 
							+ wOperacii[0].N_InshiNarahuvannya;
	var pryristZarplaty = 0;
	if (wInfo.TypOplaty != 1) //Визначення приросту зарплати лише для працівників без погодинної оплати праці
		pryristZarplaty = (wOperacii[0].N_Osn_narah + wOperacii[0].N_VuslRokiv +
							wOperacii[0].N_Likarn + wOperacii[0].N_Klasn_vodiya +
							wOperacii[0].N_Premia + wOperacii[0].N_Doplata + 
							+ wOperacii[0].N_InshiNarahuvannya) - 
							(wOperacii[1].N_Osn_narah + wOperacii[1].N_VuslRokiv +
							wOperacii[1].N_Likarn + wOperacii[1].N_Klasn_vodiya +
							wOperacii[1].N_Premia + wOperacii[1].N_Doplata + 
							+ wOperacii[1].N_InshiNarahuvannya);
	if (wOperacii[0].I_SumaDlyaIndexacii > mbn[0].MinProjytkovyi)
		wOperacii[0].I_SumaDlyaIndexacii = mbn[0].MinProjytkovyi;
	
	var prevPromIndexZnachennya;
	var prevIndexZnachennya;
	if (wOperacii[3].I_BaseMonth)
		prevPromIndexZnachennya = 100.0;
	else if (wOperacii[3].I_PromIndex)
		prevPromIndexZnachennya = wOperacii[3].I_PromIndex;
	else
		prevPromIndexZnachennya = 100.0;
	
	if (!wOperacii[2].I_BaseMonth && (wOperacii[3].I_PromIndex < 101))
		wOperacii[2].I_PromIndex = prevPromIndexZnachennya * mbn[2].IndexSpojyvCin / 100.0;
	else if (!wOperacii[2].I_BaseMonth && (wOperacii[3].I_PromIndex >= 101))
		wOperacii[2].I_PromIndex = mbn[2].IndexSpojyvCin;
	else if (wOperacii[2].I_BaseMonth)
		wOperacii[2].I_PromIndex = -1;
	else
		wOperacii[2].I_PromIndex = -1;
	
	if (!wOperacii[2].I_BaseMonth && (wOperacii[2].I_PromIndex >= 101) && (wOperacii[3].I_PromIndex < 101))
		wOperacii[2].I_Index = ((wOperacii[3].I_Index + 100) * wOperacii[2].I_PromIndex / 100.0) - 100;
	else if (!wOperacii[2].I_BaseMonth && (wOperacii[2].I_PromIndex >= 101) && (wOperacii[3].I_PromIndex >= 101))
		wOperacii[2].I_Index = mbn[2].IndexSpojyvCin / 100.0 * (wOperacii[3].I_Index+100) - 100;
	else if (wOperacii[2].I_BaseMonth)
		wOperacii[2].I_Index = 0;
	else
		wOperacii[2].I_Index = wOperacii[3].I_Index;
	
	//Якщо два місяці тому індекс інфляції нульовий, то індксація у поточному місяці не проводиться (сума для індексації нуль)
	if (wOperacii[2].I_Index <= 0)
		wOperacii[0].I_SumaDlyaIndexacii = 0;
	
	var probIndexSuma = (wOperacii[0].I_SumaDlyaIndexacii * wOperacii[2].I_Index / 100.0) - wOperacii[0].I_ProindexovanaSuma;
	
	if ( (wInfo.IndexaciyaType == 0) && (wInfo.TypOplaty != 1) ) { //визначення скриптом базовості поточного місяця
		wOperacii[0].I_BaseMonth = false;
		if (wOperacii[0].CurDate_year==wInfo.Data_pruin_na_rob_year && wOperacii[0].CurDate_month==wInfo.Data_pruin_na_rob_month){
			wOperacii[0].I_BaseMonth = true;
		}
		else if ((wInfo.TypOplaty==0) && (probIndexSuma > 0) && (pryristZarplaty >= probIndexSuma)){
			wOperacii[0].I_BaseMonth = true;
		}
	}
	else if (wInfo.IndexaciyaType==1) { //попередньо задано, що поточний місяць базовий
		wOperacii[0].I_BaseMonth = true;
	}
	else if (wInfo.IndexaciyaType==2) { //попередньо задано, що поточний місяць не базовий
		wOperacii[0].I_BaseMonth = false;
	}

	if (wOperacii[0].I_BaseMonth) { //базовий місяць
		wOperacii[0].I_PromIndex = 0;
		wOperacii[0].I_Index = 0;
	
		wOperacii[0].N_Indexaciya = 0;
	}
	else { //небазовий місяць
		wOperacii[0].I_PromIndex = 0;
		wOperacii[0].I_Index = 0;
		if (opt.TypIndexaciyi == 2)
			wOperacii[0].N_Indexaciya = probIndexSuma;
	}
	
	if (wOperacii[2].CurDate < wInfo.Data_pruin_na_rob) { //перевірка, чи працівник працював два місяці перед поточним
		wOperacii[2].I_PromIndex = 0;
		wOperacii[2].I_Index = 0;
		wOperacii[2].N_Indexaciya = 0;
		wOperacii[2].I_SumaDlyaIndexacii = 0;
		wOperacii[2].N_Indexaciya = 0;
	}
}
else {
	wOperacii[0].I_BaseMonth = false;
	wOperacii[0].I_PromIndex = 0;
	wOperacii[0].I_Index = 0;
	wOperacii[0].I_SumaDlyaIndexacii = 0;
	wOperacii[0].N_Indexaciya = 0;
	wOperacii[2].I_BaseMonth = false;
	wOperacii[2].I_PromIndex = 0;
	wOperacii[2].I_Index = 0;
	wOperacii[2].I_SumaDlyaIndexacii = 0;
	wOperacii[2].N_Indexaciya = 0;
	
}

//----------------Вхідне сальдо
wOperacii[0].Vh_saldo = wOperacii[1].Vh_saldo + wOperacii[1].N_Osn_narah + wOperacii[1].N_VuslRokiv +
						wOperacii[1].N_Likarn + wOperacii[1].N_Vidpuskni + wOperacii[1].N_Svatkovi_dni +
						wOperacii[1].N_Nichni + wOperacii[1].N_Nadurochni + wOperacii[1].N_Klasn_vodiya +
						wOperacii[1].N_Premia + wOperacii[1].N_MaterDopomoga + wOperacii[1].N_Doplata + 
						wOperacii[1].N_Indexaciya +	wOperacii[1].N_VyplProfspilky + wOperacii[1].N_InshiNarahuvannya + 
						wAdOperacii[1].O_VsogoNaRukyOtherDohid -
						wAdOperacii[1].U_PensFond - wAdOperacii[1].U_Fond_zainat - wAdOperacii[1].U_SocStrah - 
						wAdOperacii[1].U_Prybutkov_podatok - wAdOperacii[1].U_VidrahProfspilka - 
						wOperacii[1].U_InshiVidrahuvannya - 
						wOperacii[1].U_PensFond - wOperacii[1].U_EV_Likarn - wOperacii[1].U_EV_Vidpuskni
						- wOperacii[1].U_Fond_zainat - wOperacii[1].U_SocStrah - 
						wOperacii[1].U_Prybutkov_podatok - wOperacii[1].U_VidrahProfspilka - wOperacii[1].U_InshiVidrahuvannya - 
						wOperacii[1].O_Inshi -
						wOperacii[1].O_Shtraf - wOperacii[1].O_Alimenty - wOperacii[1].O_ZaPoperMis - 
						wOperacii[1].O_Premiya - wOperacii[1].O_Avans - wOperacii[1].O_Oplacheno;
