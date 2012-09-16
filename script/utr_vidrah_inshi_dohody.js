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
var vsogoNarah = wOperacii[0].N_Osn_narah;

//----------------���������-----�������� ����
if (uNeadCalcNarahValues.For_U_PensFond){
	wOperacii[0].U_PensFond = vsogoNarah * budjNorma(7) / 100;
}
else{
	wOperacii[0].U_PensFond = 0;
}

//----------------���������-----���� ���������
if (uNeadCalcNarahValues.For_U_FondZainat){
	wOperacii[0].U_Fond_zainat = vsogoNarah * budjNorma(8) / 100;
}
else{
	wOperacii[0].U_Fond_zainat = 0;
}

//----------------���������-----���� ����������� ����������� � ������� ��� ���������
if (uNeadCalcNarahValues.For_U_SocStrah){
	wOperacii[0].U_SocStrah = vsogoNarah * budjNorma(9) / 100;
}
else{
	wOperacii[0].U_SocStrah = 0;
}

//----------------���������-----��������� �����
wOperacii[0].U_SocPilga = 0;
wOperacii[0].Ozn_pilgy = 0;

//----------------���������-----����������� �������
if (uNeadCalcNarahValues.For_U_PrybutkovyiPodatok){
	wOperacii[0].U_Prybutkov_podatok = vsogoNarah * budjNorma(13) / 100;
}
else{
	wOperacii[0].U_Prybutkov_podatok = 0;
}

//----------------���������-----³���������� �� ���������
if (uNeadCalcNarahValues.For_U_VidrahProfspilka){
	wOperacii[0].U_VidrahProfspilka = vsogoNarah * budjNorma(16) / 100;
}
else{
	wOperacii[0].U_VidrahProfspilka = 0;
}

//----------------���������-----���� ���������
if (uNeadCalcNarahValues.For_U_InshiVidrahuvannya){
	wOperacii[0].U_InshiVidrahuvannya = vsogoNarah * budjNorma(25) / 100;
}
else{
	wOperacii[0].U_InshiVidrahuvannya = 0;
}

//--������ ��������
var vsogoUtrym = wOperacii[0].U_PensFond + wOperacii[0].U_Fond_zainat + wOperacii[0].U_SocStrah + wOperacii[0].U_Prybutkov_podatok + wOperacii[0].U_VidrahProfspilka + wOperacii[0].U_InshiVidrahuvannya;

//----------------³����������-----�������� ����
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

//----------------³����������-----���� ����������� ����������� � ��������� ������ �������������
if (uNeadCalcNarahValues.For_V_SocStrah){
	wOperacii[0].V_SocStrah = vsogoNarah * budjNorma(22) / 100;
}
else{
	wOperacii[0].V_SocStrah = 0;
}

//----------------³����������-----���� ���������
if (uNeadCalcNarahValues.For_V_FondZainat){
	wOperacii[0].V_Fond_zainat = vsogoNarah * budjNorma(23) / 100;
}
else{
	wOperacii[0].V_Fond_zainat = 0;
}

//----------------³����������-----����� ����������� ����������� �� �������� ������� �� ���������� �� ���������������
if (uNeadCalcNarahValues.For_V_NeshchVyp){
	wOperacii[0].V_NeshchVyp = vsogoNarah * budjNorma(24) / 100;
}
else{
	wOperacii[0].V_NeshchVyp = 0;
}

//----------------³����������-----���� �����������
if (uNeadCalcNarahValues.For_V_Inshi){
	wOperacii[0].V_Inshi = vsogoNarah * budjNorma(26) / 100;
}
else{
	wOperacii[0].V_Inshi = 0;
}

//----------------������-----�� ����
if (uNeadCalcNarahValues.For_O_NaRuky){
	wOperacii[0].O_VsogoNaRuky = vsogoNarah;
}
else{
	wOperacii[0].O_VsogoNaRuky = 0;
}
