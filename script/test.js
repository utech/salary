//--------------------utech--------------------utech--------------------utech--------------------
//	���� ������� ��� ��������� ��������� ����������� � ������� ����������� ��������
//	����������� �������-������� �:
//		class UWorkerInfo wInfo - ���������� ��� ����������, �������� � ������� npr;
//		class UDayWorkerTabel wTabel[32] - ������ ����������, �������� � ������� tabel;
//		class UWorkerOperacii wOperacii[13] - ���������� ��� ����������� ����������, �������� � ������� operacii;
//		class UIdVal budjetniNormy[] - ���������� ��� ���������� �������� ������� �����, �������� � ������� budjetni_normy;
//		class USumTabel sumTabel - ��������� ���������� ��� ��������� ������ ���������� �� ����� (��������������� ��� ������� �� ������� ������, � ���� �������);
//		class UWorkerDodatkDohody wDodatkDohody - ��������� ���������� ��� �������� ������ ����������
//	�����, ��� �������� ������� �������:
//		double budjNorma(int id) - ��������� �������� �������� ����� �� �������������� � ������� budjetni_normy; 
//		double workDays(int year, int month) ������� ������� ��� � ����� month, ���� year;
//		double d2m(double d) - ������������ �� �����;
//	����������� � ����� �������� ����
//		vydTabelya - ��������, ��� ����������� ��� ������, ���� ��������������� � ����������
//		budjetniNormyCount - ������� ������� ��������� ���� ���� �����
//	� ������  wOperacii, mbn ������� ������ ������ � �������� ��������� �����, ������ ������������ �� ��������� � �.�.
//	
//	��������� ������������ ������ "����" 11.12.2007�.
//--------------------utech--------------------utech--------------------utech--------------------

//-----���������� ��������
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

//----------------³������
/*var serZarplata=0;
for (i=1; i<13; i++){//���� ����������� �� ��
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
if (vydTabelya == 0){ //����������� �� ������ �������
for (i=0; i<32; i++)//��������� ������� ��� ��������
	if (wTabel[i].Pracivn_status_ID==3)
		dniVidpustky++;
}
else if (vydTabelya == 1){ //����������� �� ���������� �������
	dniVidpustky = sumTabel.Vidpusk_days_sum;
}
wOperacii[0].N_Vidpuskni = serZarplata * dniVidpustky;
print("wOperacii[0].N_Vidpuskni: ", wOperacii[0].N_Vidpuskni);
print("serZarplata: ", serZarplata);
print("dniVidpustky: ", dniVidpustky);
*/

