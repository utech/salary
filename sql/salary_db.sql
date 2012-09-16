-- -------------------------------------------------------------------------
-- ������ ��������� ���� ����� ��� �������� "���-���"
-- -------------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS `version`(
  `id` int(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `ver` int(10) default NULL
)
ENGINE=MyISAM;
INSERT INTO `version` (id, ver) VALUES (1, 1);

DROP TABLE IF EXISTS `budjetni_normu`;
CREATE TABLE `budjetni_normu` (
  `id` INT(4) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `GroupName` VARCHAR(20) NULL,
  `Opus` VARCHAR(100) NULL,
  `Znach` DOUBLE(8, 3) UNSIGNED NULL DEFAULT 0.00,
  `ReadOnly` BOOLEAN DEFAULT false
)
ENGINE = MyISAM;

INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (1, 257.5, '���� �������� ��� �� 01, ���.', '��',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (2, 386.25, '���� �������� ��� �� 02, ���.', '��',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (3, 515, '���� �������� ��� �� 03, ���.', '��',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (4, 643.75, '���� �������� ��� �� 04, ���.', '��',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (5, 633, '����������� �����, ���.', '�',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (6, 515, '̳������� ��������, ���.', '�',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (7, 2, '�������� ����, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (8, 0.5, '���� ���������, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (9, 0.5, '���� �������������� �� ����.��, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (10, 1, '���� �������������� ����� ����.��, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (11, 40, '³������ ��� ����� �����, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (12, 100, '³������ ��� ��������� �����, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (13, 15, '����������� �������, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (14, 257.5, '��������� ��������� �����, ���', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (15, 890, '���� �������� ��� ������.���.�����, ���', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (16, 1, '��������� ��� ���������, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (17, 10, 'ʳ������ ��������� ��� � ����, ��', '������.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (18, 365, 'ʳ������ ����������� ��� � ����, ��', '������.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (19, 100, '³������ ��� ���������� �����, %', '���.',true);

INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (20, 33.2, '³��. �������� ����, %', '³����.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (21, 4, '³��. �������� ����, ��� �������, %', '³����.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (22, 1.5, '³��. ���� ����������� ����������� � ��������� ������ �������������, %', '³����.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (23, 1.3, '³��. ���� ���������, %', '³����.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (24, 1.77, '³��. ����� ����������� ����������� �� �������� ������� �� ���������� �� ���������������, %', '³����.',true);

INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (25, 0, '³��. ����� ��������, %', '���.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (26, 0, '³��. ����� ����������, %', '³����.',true);

DROP TABLE IF EXISTS `budjetni_normy_monthly`;
CREATE TABLE `budjetni_normy_monthly` (
  `id` INT(4) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `CurDate` DATE NULL,
  `WorkDaysInMonth` SMALLINT(2) DEFAULT 0,
  `WorkHoursInMonth` SMALLINT(3) DEFAULT 0,
  `MinZarplata` DOUBLE(8, 2) DEFAULT 0.00,
  `MinProjytkovyi` DOUBLE(8, 2) DEFAULT 0.00,
  `IndexSpojyvCin` DOUBLE(8, 2) DEFAULT 0.00
)
ENGINE = MyISAM;


DROP TABLE IF EXISTS `oznakyDohodu`;
CREATE TABLE `oznakyDohodu` (
  `id` INT(4) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Opus` VARCHAR(150) DEFAULT NULL,
  `For_U_PensFond` BOOLEAN DEFAULT false,
  `For_U_FondZainat` BOOLEAN DEFAULT false,
  `For_U_SocStrah` BOOLEAN DEFAULT false,
  `For_U_PrybutkovyiPodatok` BOOLEAN DEFAULT false,
  `For_U_VidrahProfspilka` BOOLEAN DEFAULT false,
  `For_U_InshiVidrahuvannya` BOOLEAN DEFAULT false,
  `For_O_NaRuky` BOOLEAN DEFAULT false,
  `For_V_PensFond` BOOLEAN DEFAULT false,
  `For_V_SocStrah` BOOLEAN DEFAULT false,
  `For_V_FondZainat` BOOLEAN DEFAULT false,
  `For_V_NeshchVyp` BOOLEAN DEFAULT false,
  `For_V_Inshi` BOOLEAN DEFAULT false,
  `ReadOnly` BOOLEAN DEFAULT true
)
ENGINE = MyISAM;

INSERT INTO oznakydohodu (id,For_U_PensFond,For_U_FondZainat,For_U_SocStrah,For_U_PrybutkovyiPodatok,For_U_VidrahProfspilka,For_U_InshiVidrahuvannya,For_O_NaRuky,For_V_PensFond,For_V_SocStrah,For_V_FondZainat,For_V_NeshchVyp,For_V_Inshi,ReadOnly,Opus) VALUES 
 (1 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'���.� ����.���.�����,i��i ���-�� �� �����.,�����.(��������i) ��, �i�-�� �� ����'),
 (2 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'���.�i� ������� ��''���i� ���� i�������.(�������.) ��������i; ������ � ������i ��'),
 (3 ,0,0,0,1,0,0,0,0,0,0,0,0,1,'���� ����.�����i� � ����� ������.����.���. �i�-�� �� ������, �����.�����i�(����i'),
 (4 ,0,0,0,1,0,0,0,0,0,0,0,0,1,'���� ����.�����i� � ����� ������.����.���. �i�-�� �� ������, �����.�����i�(����i'),
 (6 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'������ �i� ������i� � ������� ���������� �����, ��i��� � ����������� ��.11 �����'),
 (7 ,0,0,0,1,0,0,0,0,0,0,0,0,1,'�������� � ������� ������ ����� � ��12 ������ ��� ��\"�\" �1.3. ��1 �� �9.6 ��9'),
 (8 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i� �i� ������� ����� � ������ ��� ��������� (�������� �����i��� ��/��� ������'),
 (9 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i�, ��������� ��������� ������� �i� ���� ���������� (����������� ������) �� �'),
 (10,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i� � ������i ���������, �����i� ��� ���i, �������� ��������� ��������� ������'),
 (11,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i� � ������i �������i� (���������� �����i�) (��. 4.2.12 �. 4.2 ��. 4 ������,'),
 (12,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i� � ������i ���i����i� �� �����i (��.4.2.12 �.4.2 ��.4, �.9.3 ��.9 ������ ��'),
 (13,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i� � ������i ������i�, ����i�, ��i� ��� �����i�, ��������� ��������� �������'),
 (14,0,0,0,1,0,0,0,0,0,0,0,0,1,'I��i ������ (��.4.2.12 �.4.2, ��.4.3.35 �.4.3 ��.4 ������),'),
 (15,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i� �i� ������i� � i�������i����� �������� (��.4.2.13 ��.4, �.9.6. ��.9 ������'),
 (16,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ����i�, ��������� ��������� ������� �� �i��������� ��� �i� ��i�, � ��������'),
 (17,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ���i����� ������i��� �������� �i� ��. ��� �i�.����� (������.������), �� �'),
 (18,0,0,0,1,0,0,1,0,0,0,0,0,1,'������ � �������� ���������� � ������, ��������i ����������� i���� ������������'),
 (20,0,0,0,1,0,0,1,0,0,0,0,0,1,'�����, ��������i �i�.������ ��i��� � ���������� ����i���� �����i�, ���������� �i'),
 (21,0,0,0,1,0,0,1,0,0,0,0,0,1,'������ ��������i �i����i� ����i � ����� �i���������� ���i������� (�.4.2.17 �.4.2'),
 (22,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ����.���.�� ���.�������� � ������i �����.������ ����i� �� ������� ���.�����'),
 (23,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� �����i�, ��������� ��������� ������� �i� ����i����� ��� ����i� � �i��i ����'),
 (24,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� �i����������� �������� ������� ����i�� �����, ������� ���� �����i��� �����'),
 (25,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ������ �� �i��-�� (��i� ���.����� �� i�.������ �� �i��-�� �� ���i����-����.'),
 (26,0,0,0,1,0,0,0,0,0,0,0,0,1,'���� �����i� �� ����''������ ����������� �������� ������� �i����i��� �� ������, i'),
 (27,0,0,0,1,0,0,0,0,0,0,0,0,1,'���� ����� �� �������� ����i��� ����������� �� �����i� �� ������������''������ ��'),
 (28,0,0,0,1,0,0,0,0,0,0,0,0,1,'����� ��� ����i��� ����� (������.�����i�), ��i ��������� �� �� �i������ ���� ���'),
 (29,0,0,0,1,0,0,1,0,0,0,0,0,1,'��i�����, �� ������������ �������� ������� (��.4.3.9 �.4.3. ��.4 ������),'),
 (30,0,0,0,1,0,0,0,0,0,0,0,0,1,'����� ��� ����� (������i �� ��������i �����, ����i��� ���i�, ������), ��i ������'),
 (31,0,0,0,1,0,0,0,0,0,0,0,0,1,'����i��� �����i�, ��i ��������� �� �� �� ������-�� ���i�� � �������, ���-��� ���'),
 (32,0,0,0,1,0,0,0,0,0,0,0,0,1,'�����, �������i �� � ���.���������i�(�i�����������) �������i �����(������.�����i'),
 (33,0,0,0,1,0,0,0,0,0,0,0,0,1,'����i��� ���i��� �� ���i����� ������i�, ���������� ������� �� (��.4.3.15 �.4.3.'),
 (34,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i�, ��i �����-�� �� ������� �� � ������i ���i�(������, ���) ��i������� ��.��'),
 (35,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ������, �����-�� �� �����i��� �i��-�� ���i� (i��.������������� ����), �����'),
 (36,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ������, �����. �� �����i��� �i�������� ��������� �i����� �/� �����������, �'),
 (37,0,0,0,1,0,0,0,0,0,0,0,0,1,'���� ��������� ����������� �� ������� ������i� ���i�� � ������� ���������i� ����'),
 (38,0,0,0,1,0,0,0,0,0,0,0,0,1,'����� ��� ����i��� �����(������), �� ��������� �� �������� �� ��������� �� �����'),
 (39,0,0,0,1,0,0,0,0,0,0,0,0,1,'����� ��� ����i��� �����(������), �� ��������� �� �������� �� �i������� �� ���.�'),
 (40,0,0,0,1,0,0,1,0,0,0,0,0,1,'������ �� �������� � ������� ������ ����� � ��.12 ������ �� ��.\"�\" �.1.3. ��.'),
 (41,0,0,0,1,0,0,0,0,0,0,0,0,1,'����i��� ���i��� �� �i����.,��������.,�i������� �� �������i� ������ �� ��� �i��'),
 (42,0,0,0,1,0,0,1,0,0,0,0,0,1,'���i�, ���������� ����������i� ����i (��.4 ������),'),
 (43,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� �������i�, ��� ������.� ������� ����, ��������, ����������, ���i����� ��� �'),
 (44,0,0,0,1,0,0,0,0,0,0,0,0,1,'����i��� �����, ������, � ����� ���� ����.��������, �� ��������� �i���-������� i'),
 (45,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ����. ��� ��������� ��������� �� ������������ �i���������������i� ��������'),
 (46,0,0,0,1,0,0,1,0,0,0,0,0,1,'����, ���������� �� �� �������� ��� ����i, �������� �i������ ������, i���� ���i�'),
 (47,0,0,0,1,0,0,1,0,0,0,0,0,1,'����i��� �����, ��� ���������� � �������� ��� ���������� ��������i � �����i��'),
 (48,0,0,0,1,0,0,1,0,0,0,0,0,1,'����, ���������� �� �� �����(�������) ��� �������� �������� �� �������i �i�����'),
 (49,0,0,0,1,0,0,0,0,0,0,0,0,1,'���� �������� �������, ���������� �i����������� ��� ������� ����, ���������� ��'),
 (50,0,0,0,1,0,0,0,0,0,0,0,0,1,'���� �������� �������, ���������� �i�����������, ������� ���� ��� �� �������, �'),
 (51,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ����i��� ������� � ������� ������������ ����i����� ������������ (��.4.2.5'),
 (52,0,0,0,1,0,0,1,0,0,0,0,0,1,'���� ������ �� ��������� ����i����� ������ (��.4.2.5 �.4.2,��.4.3.33 �.4.3 ��.4,'),
 (53,0,0,0,1,0,0,1,0,0,0,0,0,1,'������ �i� ������� �i������������������ �������i� (��.4.3.36 �.4.3 ��.4 ������)'),
 (54,0,0,0,1,0,0,1,0,0,0,0,0,1,'������, � ������i �����i��� �����, i��i ������� �� ����������, �� ���� �����. �'),
 (55,0,0,0,1,0,0,0,0,0,0,0,0,1,'�����, ��i ��������� ������, �� �� � ��, ��� �� �����������(������� ������) �� �'),
 (56,0,0,0,1,0,0,1,0,0,0,0,0,1,'�����, ��������� � �������� ����� � ��.13 ������'),
 (57,0,0,0,1,0,0,1,0,0,0,0,0,1,'�����, ��������� �� ��������� ����� � ��.14 ������');

CREATE TABLE IF NOT EXISTS `orgInfo` (
   `id` INT(4) NOT NULL PRIMARY KEY AUTO_INCREMENT,
	PidprId int,
	PovnaNazvaPidpr VARCHAR(100) NULL,
	ScorochNazvaPidpr VARCHAR(80) NULL,
	Edrpoy VARCHAR(8) NULL,
	Adresa VARCHAR(90) NULL,
	Fax VARCHAR(20) NULL,
	Email VARCHAR(50) NULL,
	PipDurect VARCHAR(50) NULL,
	PipBuh VARCHAR(50) NULL,
	TelefonDurekt VARCHAR(20) NULL,
	TelefonBuh VARCHAR(20) NULL,
	Bank VARCHAR(150) NULL,
	MFO VARCHAR(10) NULL,
	BankRahunok VARCHAR(20) NULL
)
ENGINE = MyISAM;
INSERT INTO `orgInfo` (PidprId) VALUES (1);

CREATE TABLE IF NOT EXISTS `platijkyInfo` (
   `id` INT(4) NOT NULL PRIMARY KEY AUTO_INCREMENT,
	NazvaOrg VARCHAR(100) NULL,
	Pryznach VARCHAR(100) NULL,
	Edrpoy VARCHAR(8) NULL,
	Bank VARCHAR(100) NULL,
	MFO VARCHAR(10) NULL,
	BankRahunok VARCHAR(20) NULL,
	ReadOnly BOOLEAN default false
)
ENGINE = MyISAM;
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (1, '���. ����. ����', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (2, '���. ���� ���������', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (3, '���. ��������', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (4, '���. �������.�������', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (5, '³��. ����. ����', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (6, '³��. ��������.', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (7, '³��. ���� ���������', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (8, '³��. ������ �������', true);

CREATE TABLE IF NOT EXISTS `npr` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Prizv` VARCHAR(25) NULL,
  `Imia` VARCHAR(15) NULL,
  `Pobatk` VARCHAR(20) NULL,
  `Data_nar` DATE NULL,
  `Posada_id` INT(4) NULL,
  `TypRoboty` TINYINT(1) DEFAULT 0,
  `TypOplaty` TINYINT(1) DEFAULT 0,
  `Iden_kod` VARCHAR(12) NULL,
  `Tel` VARCHAR(20) NULL,
  `Rozryad` TINYINT(1) DEFAULT 1,
  `Stavka` DOUBLE(8, 2) DEFAULT 0.00,
  `Num_dit` TINYINT(1) NULL,
  `Ozn_pilgy` SMALLINT(2) NULL,
  `Data_pruin_na_rob` DATE NULL,
  `Klasnist_vodiya` DOUBLE(4,2) DEFAULT 0.00,
  `VidsDoplaty` DOUBLE(4,2) DEFAULT 0.00,
  `Alimenty` DOUBLE(4,2) DEFAULT 0.00,
  `Pensioner` BOOLEAN DEFAULT false,
  Password varchar(20) NULL,
  Buh_rahunok INT UNSIGNED DEFAULT 0,
  Grupa_id INT UNSIGNED DEFAULT 11,
  `ChlenProfspilky` BOOLEAN DEFAULT false,
  `Invalid` BOOLEAN DEFAULT false,
  `Pracuye` BOOLEAN DEFAULT true
)
ENGINE = MyISAM;

INSERT INTO `npr`(Prizv, Password, Grupa_id) VALUES ("��������", '111', 1);
INSERT INTO `npr`(Prizv, Password, Grupa_id) VALUES ("���������", '111', 2);
INSERT INTO `npr`(Prizv, Password, Grupa_id) VALUES ("����������� ���������", '111', 4);

CREATE TABLE IF NOT EXISTS `operacii` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Npr_id` INT(10) NOT NULL,
  `CurDate` DATE NULL,
  --		������ ������ 01, 02, 03, ...
  `OznakaDohodu` SMALLINT(3) DEFAULT 0,
  `Comentar` TEXT DEFAULT NULL,
  `ComentarForOplata` TEXT DEFAULT NULL,
  `Vh_saldo` DOUBLE(8, 2) DEFAULT 0.00,
--		�����������
  `N_Osn_narah` DOUBLE(8, 2) DEFAULT 0.00,
  `N_VuslRokiv` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Likarn` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Vidpuskni` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Svatkovi_dni` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Nichni` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Nadurochni` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Klasn_vodiya` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Premia` DOUBLE(8, 2) DEFAULT 0.00,
  `N_MaterDopomoga` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Doplata` DOUBLE(8, 2) DEFAULT 0.00,
  `N_Indexaciya` DOUBLE(8, 2) DEFAULT 0.00,
  `N_VyplProfspilky` DOUBLE(8, 2) DEFAULT 0.00,
  `N_InshiNarahuvannya` DOUBLE(8, 2) DEFAULT 0.00,
--		���������
  `U_PensFond` DOUBLE(8, 2) DEFAULT 0.00,
  `U_Fond_zainat` DOUBLE(8, 2) DEFAULT 0.00,
  `U_SocStrah` DOUBLE(8, 2) DEFAULT 0.00,
  `U_SocPilga` DOUBLE(8, 2) DEFAULT 0.00,
  `U_Prybutkov_podatok` DOUBLE(8, 2) DEFAULT 0.00,
  `U_VidrahProfspilka` DOUBLE(8, 2) DEFAULT 0.00,
  `U_InshiVidrahuvannya` DOUBLE(8, 2) DEFAULT 0.00,
--		������
  `O_Inshi` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Shtraf` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Alimenty` DOUBLE(8, 2) DEFAULT 0.00,
  `O_VsogoNaRuky` DOUBLE(8, 2) DEFAULT 0.00,
  `O_ZaPoperMis` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Premiya` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Avans` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Oplacheno` DOUBLE(8, 2) DEFAULT 0.00,
--		³���������� �� ��������
  `V_PensFond` DOUBLE(8, 2) DEFAULT 0.00,
  `V_SocStrah` DOUBLE(8, 2) DEFAULT 0.00,
  `V_Fond_zainat` DOUBLE(8, 2) DEFAULT 0.00,
  `V_NeshchVyp` DOUBLE(8, 2) DEFAULT 0.00,
  `V_Inshi` DOUBLE(8, 2) DEFAULT 0.00,
--		���������� ��� ���������� � ���������� � �������� �������
  Vids_U_PensFond DOUBLE(4,2) DEFAULT 0.00,
  Vids_U_Fond_zainat DOUBLE(4,2) DEFAULT 0.00,
  Vids_U_Socstrah DOUBLE(4,2) DEFAULT 0.00,
  Vids_U_SocPilga DOUBLE(4,2) DEFAULT 0.00,
  Vids_U_Prybutkov_podatok DOUBLE(4,2) DEFAULT 0.00,
  Vids_U_VidrahProfspilka DOUBLE(4,2) DEFAULT 0.00,
  Vids_U_InshiVidrahuvannya DOUBLE(4,2) DEFAULT 0.00,
  Vids_V_PensFond DOUBLE(4,2) DEFAULT 0.00,
  Vids_V_SocStrah DOUBLE(4,2) DEFAULT 0.00,
  Vids_V_Fond_zainat DOUBLE(4,2) DEFAULT 0.00,
  Vids_V_NeshchVyp DOUBLE(4,2) DEFAULT 0.00,
  Vids_V_Inshi DOUBLE(4,2) DEFAULT 0.00,
--		����� ���������� ��� ���������� � ��������� �����
  `TypRoboty` TINYINT(1) DEFAULT 1,
  `TypOplaty` TINYINT(1) DEFAULT 1,
  `Stavka` DOUBLE(8, 2) DEFAULT 0.00,
  `Ozn_pilgy` SMALLINT(2) NULL,
  `Klasnist_vodiya` DOUBLE(7,2) DEFAULT 0.00,
  `VidsDoplaty` DOUBLE(4,2) DEFAULT 0.00,
  `VidsAlimentiv` DOUBLE(8,2) DEFAULT 0.00,
  `Pensioner` BOOLEAN DEFAULT false,
  Buh_rahunok INT UNSIGNED DEFAULT 0,
  `ChlenProfspilky` BOOLEAN DEFAULT false,
  `Invalid` BOOLEAN DEFAULT false,
  VydTabelyaInMonth INT(2) DEFAULT 0,
  `Posada_id` INT(4) NULL,
  `Rozryad` TINYINT(1) DEFAULT 1,
  `Pracuye` BOOLEAN DEFAULT false,
--		������� ����������
  `I_PromIndex` DOUBLE(8, 2) DEFAULT 0.00,
  `I_Index` DOUBLE(8, 2) DEFAULT 0.00,
  `I_ProindexovanaSuma` DOUBLE(8, 2) DEFAULT 0.00,
  `I_SumaDlyaIndexacii` DOUBLE(8, 2) DEFAULT 0.00,
  `I_BaseMonth` BOOLEAN DEFAULT false
)
ENGINE = MyISAM;

CREATE TABLE `statusIndexaciiPracivnykiv` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Npr_id` INT(10) NOT NULL,
  `CurDate` DATE NULL,
  `Status` SMALLINT(1) DEFAULT NULL
)
ENGINE = MyISAM;

DROP TABLE IF EXISTS `pracivn_status`;
CREATE TABLE `pracivn_status` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Status_name` VARCHAR(50) NULL
)
ENGINE = MyISAM;

INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (1, '����.');
INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (2, '˳�������');
INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (3, '³�������');
INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (4, '�� ��� ���.');

CREATE TABLE IF NOT EXISTS `tabel` (
  `id` 					INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Cur_date` 			DATE NULL,
  `Npr_ID` 				INT(10) NULL,
  `Pracivn_status_ID` 	INT(10) NULL,
  `God_zag` 			DOUBLE(4, 1) DEFAULT 0.0,
  `God_nichni` 			DOUBLE(4, 1) DEFAULT 0.0,
  `God_sv` 				DOUBLE(4, 1) DEFAULT 0.0,
  `Pryinyato` 			INT(3) DEFAULT NULL,
  `Zvilneno` 			INT(3) DEFAULT NULL
)
ENGINE = MyISAM;

CREATE TABLE IF NOT EXISTS `tabel_sum` (
  `id` 					INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Cur_date` 			DATE NULL,
  `Npr_ID` 				INT(10) NULL,
  `Work_days_sum` 		INT(3) NULL,
  `Likarn_days_sum` 	INT(3) NULL,
  `Vidpusk_days_sum` 	INT(3) NULL,
  `God_zag_sum` 		DOUBLE(4, 1) DEFAULT 0.0,
  `God_nichni_sum` 		DOUBLE(4, 1) DEFAULT 0.0,
  `God_sv_sum` 			DOUBLE(4, 1) DEFAULT 0.0,
  `Pryinyato` 			INT(3) DEFAULT NULL,
  `Zvilneno` 			INT(3) DEFAULT NULL
)
ENGINE = MyISAM;

DROP TABLE IF EXISTS `vusluga_rokiv`;
CREATE TABLE `vusluga_rokiv` (
  `id` INT(10) 	NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Befour_year` TINYINT(3) UNSIGNED NULL,
  `Vidsotky` 	DOUBLE(4, 1) DEFAULT 0.0
)
ENGINE = MyISAM;

INSERT INTO `vusluga_rokiv`(`Befour_year`, `Vidsotky`) VALUES (3, 6);
INSERT INTO `vusluga_rokiv`(`Befour_year`, `Vidsotky`) VALUES (5, 9);
INSERT INTO `vusluga_rokiv`(`Befour_year`, `Vidsotky`) VALUES (10, 2);
INSERT INTO `vusluga_rokiv`(`Befour_year`, `Vidsotky`) VALUES (13, 5);
INSERT INTO `vusluga_rokiv`(`Befour_year`, `Vidsotky`) VALUES (100, 20);


CREATE TABLE IF NOT EXISTS grupa(
  id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
  GrupaName varchar(30),
  `ReadOnly` BOOLEAN DEFAULT false
)
ENGINE = MyISAM;

INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (1,'����������',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (2,'����',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (3,'����������',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (4,'����������� ���������',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (5,'����������',false);

CREATE TABLE `taryfni_sitky` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Posada` VARCHAR(30) NULL,
  `Taryf` DOUBLE(8, 2) DEFAULT 0.0
)
ENGINE = MyISAM;

INSERT INTO taryfni_sitky (Posada, Taryf) VALUES ('��������', 2500);
INSERT INTO taryfni_sitky (Posada, Taryf) VALUES ('���������', 1500);
INSERT INTO taryfni_sitky (Posada, Taryf) VALUES ('����������', 1000);

CREATE TABLE `progOptions` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Opys` VARCHAR(50) DEFAULT NULL,
  `Znach` VARCHAR(300) DEFAULT NULL
)
ENGINE = MyISAM;

INSERT INTO `progOptions` VALUES (1,   '��� ������', '1');
INSERT INTO `progOptions` VALUES (2,   '���� �� ������� �������� ������', '../script/saldo.js');
INSERT INTO `progOptions` VALUES (3,   '���� �� ������� ��������� �����������', '../script/osn_narah.js');
INSERT INTO `progOptions` VALUES (4,   '���� �� ������� ��������� � ����������� ��=01', '../script/utr_vidrah_osn_dohodu.js');
INSERT INTO `progOptions` VALUES (5,   '���� �� ������� ��������� � ����������� ��=02', '../script/utr_vidrah_inshi_dohody.js');
INSERT INTO `progOptions` VALUES (6,   '���� �� ������� ��������� ������', '../script/oplata.js');
INSERT INTO `progOptions` VALUES (7,   '�����. ������. 0-�,1-���� �������,2-����� ������', '0');
INSERT INTO `progOptions` VALUES (8,   '���������� ��������: 0-���� ���.,1-���� �����.', '1');
INSERT INTO `progOptions` VALUES (100, '���� �� ����� �������� \"��������� �������\" (OPZ)', 'c:/OPZ/');
INSERT INTO `progOptions` VALUES (101, '����� ���������� � ������ \"��������� �������\"', '1');

CREATE TABLE IF NOT EXISTS `vidpuskni`(
  `id` int(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Cur_date` date default NULL,
  `Npr_ID` int(10) default NULL,
  `Narah_date` date default NULL,
  `Suma` double(8,2) default 0.00,
  `NumDays` int(2) default 0,
  `Comentar` TEXT
)
ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS `likarnyani`(
  `id` int(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Cur_date` date default NULL,
  `Npr_ID` int(10) default NULL,
  `Narah_date` date default NULL,
  `Suma` double(8,2) default 0.00,
  `NumDays` int(2) default 0,
  `Comentar` TEXT
)
ENGINE=MyISAM;

-- ----------------------------------------------------------------------
-- EOF
