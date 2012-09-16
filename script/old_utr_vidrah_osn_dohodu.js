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
wOperacii[0].U_PensFond = vsogoNarah * budjNorma(7) / 100;
//----------------���� ���������
//if (!wInfo.Pensioner){
	wOperacii[0].U_Fond_zainat = vsogoNarahNoLik * budjNorma(8) / 100;
	//wOperacii[0].U_Fond_zainat = vsogoNarahNoLik * 6 * 0.5 / (100 * 20);
	//wOperacii[0].U_Fond_zainat += vsogoNarahNoLik * 14 * 0.6 / (100 * 20);
//}
//else
//	wOperacii[0].U_Fond_zainat = 0;
//----------------���� ����������� ����������� � ������� ��� ���������
if (vsogoNarahNoLik <= budjNorma(5)){
	wOperacii[0].U_SocStrah = vsogoNarahNoLik * budjNorma(9) / 100;
	wOperacii[0].Vids_U_Socstrah = budjNorma(9);
}
else{
	wOperacii[0].U_SocStrah = vsogoNarahNoLik * budjNorma(10) / 100;
	wOperacii[0].Vids_U_Socstrah = budjNorma(10);
}

//----------------³���������� �� ���������
if (wInfo.ChlenProfspilky)
	wOperacii[0].U_VidrahProfspilka = vsogoNarahNoLik * budjNorma(16) / 100;
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
wOperacii[0].U_Prybutkov_podatok = (vsogoNarah - (wOperacii[0].U_PensFond + wOperacii[0].U_SocStrah + wOperacii[0].U_Fond_zainat + wOperacii[0].U_SocPilga)) * budjNorma(13) / 100;
//----------------���� �����������

//--������ ���������
var vsogoUtrym = wOperacii[0].U_PensFond - wOperacii[0].U_Fond_zainat - wOperacii[0].U_SocStrah - wOperacii[0].U_Prybutkov_podatok - wOperacii[0].U_VidrahProfspilka - wOperacii[0].U_InshiVidrahuvannya;
//----------------�����

//----------------������������� ���������� ����� �����������

//----------------�����

//----------------³����������-----�������� ����
if (!wInfo.Invalid){
	wOperacii[0].V_PensFond = vsogoNarah * budjNorma(20) / 100;
	wOperacii[0].Vids_V_PensFond = budjNorma(20);
}
else{
	wOperacii[0].V_PensFond = vsogoNarah * budjNorma(21) / 100;
	wOperacii[0].Vids_V_PensFond = budjNorma(21);
}
//----------------³����������-----���� ����������� ����������� � ��������� ������ �������������
wOperacii[0].V_SocStrah = vsogoNarahNoLik * budjNorma(22) / 100;

//----------------³����������-----���� ���������
//�������
wOperacii[0].V_Fond_zainat = vsogoNarahNoLik * budjNorma(23) / 100;

//----------------³����������-----����� ����������� ����������� �� �������� ������� �� ���������� �� ���������������
wOperacii[0].V_NeshchVyp = vsogoNarahNoLik * budjNorma(24) / 100;

//----------------³����������-----���� �����������
wOperacii[0].V_Inshi = vsogoNarah * budjNorma(26) / 100;
