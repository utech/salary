//--------------------utech--------------------utech--------------------utech--------------------
//	���� ������� ��� ��������� ����������� ����������� � ������� ����������� ��������
//	����������� �������-������� �:
//		class UWorkerInfo wInfo - ���������� ��� ����������, �������� � ������� npr;
//		class UDayWorkerTabel wTabel[32] - ������ ����������, �������� � ������� tabel;
//		class UWorkerOperacii wOperacii[13] - ���������� ��� ����������� ����������, �������� � ������� operacii;
//		class UIdVal budjetniNormy[] - ���������� ��� ���������� �������� ������� �����, �������� � ������� budjetni_normy;
//		class UNumWorkDays numWorkDays[13] - ���������� ��� ������� ������� ��� � ����� (�� ����� ��+�������� �����), �������� � ������� workday;
//		class USumTabel sumTabel - ��������� ���������� ��� ��������� ������ ���������� �� ����� (��������������� ��� ������� �� ������� ������, � ���� �������);
//		class UWorkerDodatkDohody wDodatkDohody - ��������� ���������� ��� �������� ������ ����������
//	�����, ��� �������� ������� �������:
//		double budjNorma(int id) - ��������� �������� �������� ����� �� �������������� � ������� budjetni_normy; 
//		double workDays(int year, int month) ������� ������� ��� � ����� month, ���� year;
//		double d2m(double d) - ������������ �� �����;
//	����������� � ����� �������� ����
//		vydTabelya - ��������, ��� ����������� ��� ������, ���� ��������������� � ����������
//		budjetniNormyCount - ������� ������� ��������� ���� ���� �����
//	� ������  wOperacii, numWorkDays ������� ������ ������ � �������� ��������� �����, ������ ������������ �� ��������� � �.�.
//	
//	��������� ������������ ������ "����" 11.12.2007�.
//--------------------utech--------------------utech--------------------utech--------------------

//----------------������ ����������� ---����������� ��� �������

var vsogoNarahNoLik = wOperacii[0].N_Osn_narah + wOperacii[0].N_VuslRokiv + wOperacii[0].N_Vidpuskni +
							wOperacii[0].N_Svatkovi_dni + wOperacii[0].N_Nichni + wOperacii[0].N_Nadurochni + wOperacii[0].N_Klasn_vodiya +
							wOperacii[0].N_Premia + wOperacii[0].N_MaterDopomoga + wOperacii[0].N_Doplata + wOperacii[0].N_Indexaciya + 
							wOperacii[0].N_VyplProfspilky + wOperacii[0].N_InshiNarahuvannya;
var vsogoNarah = vsogoNarahNoLik + wOperacii[0].N_Likarn;
//----------------�������� ����
if (wInfo.TypRoboty == 0 || wInfo.TypRoboty == 2) { // ��� ������� ���������� ��� ��������
	wOperacii[0].U_PensFond = vsogoNarahNoLik * budjNorma(5) / 100.0;
	wOperacii[0].Vids_U_PensFond = budjNorma(5);
}
else { // ��� ����������
	wOperacii[0].U_PensFond = vsogoNarahNoLik * budjNorma(6) / 100.0;
	wOperacii[0].Vids_U_PensFond = budjNorma(6);
}

// ��� �������� ���������
if (wOperacii[0].N_Likarn > 0) {
	wOperacii[0].U_EV_Likarn = wOperacii[0].N_Likarn * budjNorma(7) / 100.0;
	wOperacii[0].Vids_U_EV_Likarn = budjNorma(7);
}
else {
	wOperacii[0].U_EV_Likarn = 0;
	wOperacii[0].Vids_U_EV_Likarn = 0;
}

//----------------���� ���������
wOperacii[0].U_Fond_zainat = vsogoNarahNoLik * budjNorma(8) / 100.0;

//----------------���� ����������� ����������� � ������� ��� ���������
if (vsogoNarahNoLik <= mbn[0].MinProjytkovyi){
	wOperacii[0].U_SocStrah = vsogoNarahNoLik * budjNorma(9) / 100.0;
	wOperacii[0].Vids_U_Socstrah = budjNorma(9);
}
else{
	wOperacii[0].U_SocStrah = vsogoNarahNoLik * budjNorma(10) / 100.0;
	wOperacii[0].Vids_U_Socstrah = budjNorma(10);
}

//----------------³���������� �� ���������
if (wInfo.ChlenProfspilky)
	wOperacii[0].U_VidrahProfspilka = vsogoNarahNoLik * budjNorma(16) / 100.0;
else
	wOperacii[0].U_VidrahProfspilka = 0;

//----------------��������� �����
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

//----------------����������� �������
__10ProjytkMin = mbn[0].MinProjytkovyi * 10;
__sumaForPrybutk = vsogoNarah - (wOperacii[0].U_PensFond + wOperacii[0].U_SocStrah 
				+ wOperacii[0].U_Fond_zainat + wOperacii[0].U_SocPilga
				+ wOperacii[0].U_EV_Likarn);

if (__sumaForPrybutk <= __10ProjytkMin)
	wOperacii[0].U_Prybutkov_podatok = __sumaForPrybutk * budjNorma(13) / 100.0;
else {
	wOperacii[0].U_Prybutkov_podatok = __10ProjytkMin * budjNorma(13) / 100.0
					+ ( __sumaForPrybutk - __10ProjytkMin ) * budjNorma(14) / 100.0;
}
//----------------���� �����������

//--������ ���������
var vsogoUtrym = wOperacii[0].U_PensFond - wOperacii[0].U_Fond_zainat - wOperacii[0].U_SocStrah - wOperacii[0].U_Prybutkov_podatok - wOperacii[0].U_VidrahProfspilka - wOperacii[0].U_InshiVidrahuvannya;
//----------------�����

//----------------������������� ���������� ����� �����������

//----------------�����

//----------------³����������-----�������� ����
if (wInfo.Invalid) { // ��� �������
	wOperacii[0].V_PensFond = vsogoNarahNoLik * budjNorma(21) / 100.0;
	wOperacii[0].Vids_V_PensFond = budjNorma(21);
}
else {
	if (wInfo.TypRoboty == 0  || wInfo.TypRoboty == 2) { // ��� ������� ���������� ��� ��������
		wOperacii[0].V_PensFond = vsogoNarahNoLik * budjNorma(20) / 100.0;
		wOperacii[0].Vids_V_PensFond = budjNorma(20);
	}
	else { // ��� ����������
		wOperacii[0].V_PensFond = vsogoNarahNoLik * budjNorma(17) / 100.0;
		wOperacii[0].Vids_V_PensFond = budjNorma(17);
	}
}

// ��� �������� ���������
if (wOperacii[0].N_Likarn > 0) {
	wOperacii[0].V_EV_Likarn = wOperacii[0].N_Likarn * budjNorma(18) / 100.0;
	wOperacii[0].Vids_V_EV_Likarn = budjNorma(18);
}
else {
	wOperacii[0].V_EV_Likarn = 0;
	wOperacii[0].Vids_V_EV_Likarn = 0;
}
//----------------³����������-----���� ����������� ����������� � ��������� ������ �������������
wOperacii[0].V_SocStrah = vsogoNarahNoLik * budjNorma(22) / 100.0;

//----------------³����������-----���� ���������
//�������
wOperacii[0].V_Fond_zainat = vsogoNarahNoLik * budjNorma(23) / 100.0;

//----------------³����������-----����� ����������� ����������� �� �������� ������� �� ���������� �� ���������������
wOperacii[0].V_NeshchVyp = vsogoNarahNoLik * budjNorma(24) / 100.0;

//----------------³����������-----���� �����������
wOperacii[0].V_Inshi = vsogoNarah * budjNorma(26) / 100.0;
