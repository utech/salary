//--------------------utech--------------------utech--------------------utech--------------------
//	���� ������� ��� ��������� ������ � ������� ����������� ��������
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

wOplata.Vyh_saldo = wOplata.Vh_saldo + wOplata.NarahSum + wOplata.O_VsogoNaRukyOtherDohid - wOplata.UtrymSum - wOplata.UtrymInshiSum -
					wOplata.O_Inshi - wOplata.O_Shtraf - wOplata.O_Alimenty - 
					wOplata.O_ZaPoperMis - wOplata.O_Premiya -
					wOplata.O_Avans - wOplata.O_Oplacheno;
