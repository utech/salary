-- -------------------------------------------------------------------------
-- Скрипт створення бази даних для програми "Ель-Зар"
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

INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (1, 257.5, 'Сума соцпільги для ОП 01, грн.', 'ОП',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (2, 386.25, 'Сума соцпільги для ОП 02, грн.', 'ОП',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (3, 515, 'Сума соцпільги для ОП 03, грн.', 'ОП',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (4, 643.75, 'Сума соцпільги для ОП 04, грн.', 'ОП',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (5, 633, 'Прожитковий мінімум, грн.', 'З',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (6, 515, 'Мінімальна зарплата, грн.', 'З',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (7, 2, 'Пенсійний фонд, %', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (8, 0.5, 'Фонд зайнятості, %', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (9, 0.5, 'Фонд соцстрахування до прож.мін, %', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (10, 1, 'Фонд соцстрахування більше прож.мін, %', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (11, 40, 'Відсоток для нічних годин, %', 'Нар.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (12, 100, 'Відсоток для святкових годин, %', 'Нар.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (13, 15, 'Прибутковий податок, %', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (14, 257.5, 'Податкова соціальна пільга, грн', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (15, 890, 'Сума зарплати для податк.соц.пільги, грн', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (16, 1, 'Утримання для профспілки, %', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (17, 10, 'Кількість святкових днів у році, дні', 'Календ.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (18, 365, 'Кількість календарних днів у році, дні', 'Календ.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (19, 100, 'Відсоток для надурочних годин, %', 'Нар.',true);

INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (20, 33.2, 'Відс. Пенсійний фонд, %', 'Відрах.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (21, 4, 'Відс. Пенсійний фонд, для інвалідів, %', 'Відрах.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (22, 1.5, 'Відс. Фонд соціального страхування з тимчасової втрати працездатності, %', 'Відрах.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (23, 1.3, 'Відс. Фонд Зайнятості, %', 'Відрах.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (24, 1.77, 'Відс. Фонду соціального страхування від нещасних випадків на виробництві та профзахворювань, %', 'Відрах.',true);

INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (25, 0, 'Відс. інших утримань, %', 'Утр.',true);
INSERT INTO `budjetni_normu`(id, Znach, Opus, GroupName, ReadOnly) VALUES (26, 0, 'Відс. інших відрахувань, %', 'Відрах.',true);

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
 (1 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дох.у вигл.зар.плати,iншi вип-ти та винаг.,нарах.(виплаченi) ПП, вiд-но до умов'),
 (2 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дох.вiд продажу об''єктiв прав iнтелект.(промисл.) власностi; доходи у виглядi су'),
 (3 ,0,0,0,1,0,0,0,0,0,0,0,0,1,'Сума пенс.внескiв у межах недерж.пенс.заб. вiд-но до закону, страх.внескiв(премi'),
 (4 ,0,0,0,1,0,0,0,0,0,0,0,0,1,'Сума пенс.внескiв у межах недерж.пенс.заб. вiд-но до закону, страх.внескiв(премi'),
 (6 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'Доходи вiд операцiй з продажу нерухомого майна, згiдно з положеннями ст.11 Закон'),
 (7 ,0,0,0,1,0,0,0,0,0,0,0,0,1,'Операції з рухомим майном згідно зі ст12 Закону крім пп\"ж\" п1.3. ст1 та п9.6 ст9'),
 (8 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд вiд надання майна в оренду або суборенду (строкове володiння та/або корист'),
 (9 ,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд, отриманий платником податку вiд його працедавця (податкового агента) як д'),
 (10,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд у виглядi неустойки, штрафiв або пенi, фактично одержаних платником податк'),
 (11,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд у виглядi процентiв (дисконтних доходiв) (пп. 4.2.12 п. 4.2 ст. 4 Закону,'),
 (12,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд у виглядi дивiдендiв та роялтi (пп.4.2.12 п.4.2 ст.4, п.9.3 ст.9 Закону кр'),
 (13,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд у виглядi виграшiв, призiв, крiм сум доходiв, отриманих платником податку'),
 (14,0,0,0,1,0,0,0,0,0,0,0,0,1,'Iншi доходи (пп.4.2.12 п.4.2, пп.4.3.35 п.4.3 ст.4 Закону),'),
 (15,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд вiд операцiй з iнвестицiйними активами (пп.4.2.13 ст.4, п.9.6. ст.9 Закону'),
 (16,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума коштiв, отриманих платником податку на вiдрядження або пiд звiт, з урахуван'),
 (17,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума нецiльової благодiйної допомоги вiд юр. або фiз.особи (податк.агента), що п'),
 (18,0,0,0,1,0,0,1,0,0,0,0,0,1,'Доходи з джерелом походження з України, виплаченi нерезиденту iншим нерезидентом'),
 (20,0,0,0,1,0,0,1,0,0,0,0,0,1,'Кошти, виплаченi фiз.особам згiдно з договорами пенсiйних вкладiв, укладеними вi'),
 (21,0,0,0,1,0,0,1,0,0,0,0,0,1,'Доходи виплаченi фiзичнiй особi з фонду фiнансування будiвництва (п.4.2.17 п.4.2'),
 (22,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума держ.мат.та соц.допомоги у виглядi адрес.виплат коштiв на надання соц.послу'),
 (23,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума доходiв, отриманих платником податку вiд розмiщення ним коштiв у цiннi папе'),
 (24,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума вiдшкодування платнику податку розмiру шкоди, завданої йому внаслiдок Чорно'),
 (25,0,0,0,1,0,0,1,0,0,0,0,0,1,'Суми виплат чи вiдш-нь (крiм зар.плати чи iн.виплат та вiдш-нь за цивiльно-прав.'),
 (26,0,0,0,1,0,0,0,0,0,0,0,0,1,'Сума внескiв на обов''язкове страхування платника податку вiдповiдно до закону, i'),
 (27,0,0,0,1,0,0,0,0,0,0,0,0,1,'Сума збору на державне пенсiйне страхування та внескiв на загальнообов''язкове де'),
 (28,0,0,0,1,0,0,0,0,0,0,0,0,1,'Кошти або вартiсть майна (немайн.активiв), якi надходять ПП за рiшенням суду вна'),
 (29,0,0,0,1,0,0,1,0,0,0,0,0,1,'Алiменти, що виплачуються платнику податку (пп.4.3.9 п.4.3. ст.4 Закону),'),
 (30,0,0,0,1,0,0,0,0,0,0,0,0,1,'Кошти або майно (майновi чи немайновi права, вартiсть робiт, послуг), якi отриму'),
 (31,0,0,0,1,0,0,0,0,0,0,0,0,1,'Вартiсть товарiв, якi надходять ПП як їх гарант-на замiна у порядку, вст-ому зак'),
 (32,0,0,0,1,0,0,0,0,0,0,0,0,1,'Кошти, отриманi ПП в рах.компенсацiї(вiдшкодування) вартостi майна(немайн.активi'),
 (33,0,0,0,1,0,0,0,0,0,0,0,0,1,'Вартiсть вугiлля та вугiльних брикетiв, безоплатно наданих ПП (пп.4.3.15 п.4.3.'),
 (34,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дивiд, якi нарах-ся на користь ПП у виглядi акцiй(часток, паїв) емiтованих юр.ос'),
 (35,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума доходу, отрим-го ПП внаслiдок вiдч-ня акцiй (iнш.корпоративних прав), одерж'),
 (36,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума доходу, отрим. ПП внаслiдок вiдчуження земельних дiлянок с/г призначення, з'),
 (37,0,0,0,1,0,0,0,0,0,0,0,0,1,'Сума виплачена працедавцем на користь закладiв освiти у рахунок компенсацiї варт'),
 (38,0,0,0,1,0,0,0,0,0,0,0,0,1,'Кошти або вартiсть майна(послуг), що надаються як допомога на поховання ПП благо'),
 (39,0,0,0,1,0,0,0,0,0,0,0,0,1,'Кошти або вартiсть майна(послуг), що надаються як допомога на лiкування та мед.о'),
 (40,0,0,0,1,0,0,1,0,0,0,0,0,1,'Доходи від операції з рухомим майном згідно зі ст.12 Закону та пп.\"ж\" п.1.3. ст.'),
 (41,0,0,0,1,0,0,0,0,0,0,0,0,1,'Вартiсть путiвок на вiдпоч.,оздоровл.,лiкування на територiї України ПП або дiте'),
 (42,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохiд, виплачений самозайнятiй особi (ст.4 Закону),'),
 (43,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума стипендiї, яка виплач.з бюджету учню, студенту, ординатору, аспiранту або а'),
 (44,0,0,0,1,0,0,0,0,0,0,0,0,1,'Вартiсть одягу, взуття, а також суми грош.допомоги, що надаються дiтям-сиротам i'),
 (45,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума грош. або майнового утримання чи забезпечення вiйськовослужбовцiв строкової'),
 (46,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума, одержувана ПП за здавання ним кровi, грудного жiночого молока, iнших видiв'),
 (47,0,0,0,1,0,0,1,0,0,0,0,0,1,'Вартiсть житла, яке передається з державної або комунальної власностi у власнiст'),
 (48,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума, одержувана ПП за здану(продану) ним вторинну сировину та побутовi вiдходи'),
 (49,0,0,0,1,0,0,0,0,0,0,0,0,1,'Сума страхової виплати, страхового вiдшкодування або викупна сума, отримувана ПП'),
 (50,0,0,0,1,0,0,0,0,0,0,0,0,1,'Сума страхової виплати, страхового вiдшкодування, викупна сума або їх частина, о'),
 (51,0,0,0,1,0,0,1,0,0,0,0,0,1,'Сума пенсiйної виплати з системи недержавного пенсiйного забезпечення (пп.4.2.5'),
 (52,0,0,0,1,0,0,1,0,0,0,0,0,1,'сума виплат за договором пенсiйного вкладу (пп.4.2.5 п.4.2,пп.4.3.33 п.4.3 ст.4,'),
 (53,0,0,0,1,0,0,1,0,0,0,0,0,1,'Доходи вiд продажу сiльськогосподарської продукцiї (пп.4.3.36 п.4.3 ст.4 Закону)'),
 (54,0,0,0,1,0,0,1,0,0,0,0,0,1,'Доходи, у виглядi заробiтної плати, iншi виплати та винагороди, що були нарах. П'),
 (55,0,0,0,1,0,0,0,0,0,0,0,0,1,'Кошти, якi вносяться особою, що не є ПП, або її працедавцем(третьою особою) на к'),
 (56,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохід, отриманий у спадщину згідно зі ст.13 Закону'),
 (57,0,0,0,1,0,0,1,0,0,0,0,0,1,'Дохід, отриманий як подарунок згідно зі ст.14 Закону');

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
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (1, 'Утр. Пенс. фонд', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (2, 'Утр. Фонд зайнятості', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (3, 'Утр. Соцстрах', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (4, 'Утр. Прибутк.податок', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (5, 'Відр. Пенс. фонд', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (6, 'Відр. Соцстрах.', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (7, 'Відр. Фонд зайнятості', true);
INSERT INTO platijkyInfo (id, NazvaOrg, ReadOnly) VALUES (8, 'Відр. Нещасні випадки', true);

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

INSERT INTO `npr`(Prizv, Password, Grupa_id) VALUES ("Директор", '111', 1);
INSERT INTO `npr`(Prizv, Password, Grupa_id) VALUES ("Бухгалтер", '111', 2);
INSERT INTO `npr`(Prizv, Password, Grupa_id) VALUES ("Представник профспілки", '111', 4);

CREATE TABLE IF NOT EXISTS `operacii` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Npr_id` INT(10) NOT NULL,
  `CurDate` DATE NULL,
  --		Ознака доходу 01, 02, 03, ...
  `OznakaDohodu` SMALLINT(3) DEFAULT 0,
  `Comentar` TEXT DEFAULT NULL,
  `ComentarForOplata` TEXT DEFAULT NULL,
  `Vh_saldo` DOUBLE(8, 2) DEFAULT 0.00,
--		Нарахування
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
--		Утримання
  `U_PensFond` DOUBLE(8, 2) DEFAULT 0.00,
  `U_Fond_zainat` DOUBLE(8, 2) DEFAULT 0.00,
  `U_SocStrah` DOUBLE(8, 2) DEFAULT 0.00,
  `U_SocPilga` DOUBLE(8, 2) DEFAULT 0.00,
  `U_Prybutkov_podatok` DOUBLE(8, 2) DEFAULT 0.00,
  `U_VidrahProfspilka` DOUBLE(8, 2) DEFAULT 0.00,
  `U_InshiVidrahuvannya` DOUBLE(8, 2) DEFAULT 0.00,
--		Оплата
  `O_Inshi` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Shtraf` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Alimenty` DOUBLE(8, 2) DEFAULT 0.00,
  `O_VsogoNaRuky` DOUBLE(8, 2) DEFAULT 0.00,
  `O_ZaPoperMis` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Premiya` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Avans` DOUBLE(8, 2) DEFAULT 0.00,
  `O_Oplacheno` DOUBLE(8, 2) DEFAULT 0.00,
--		Відрахування на зарплату
  `V_PensFond` DOUBLE(8, 2) DEFAULT 0.00,
  `V_SocStrah` DOUBLE(8, 2) DEFAULT 0.00,
  `V_Fond_zainat` DOUBLE(8, 2) DEFAULT 0.00,
  `V_NeshchVyp` DOUBLE(8, 2) DEFAULT 0.00,
  `V_Inshi` DOUBLE(8, 2) DEFAULT 0.00,
--		Інформація про використані у відрахуванні і утриманні відсотки
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
--		Запис інформації про працівника в поточному місяці
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
--		Колонки індексації
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

INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (1, 'Прац.');
INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (2, 'Лікарняне');
INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (3, 'Відпустка');
INSERT INTO `pracivn_status`(id, `Status_name`) VALUES (4, 'За свій рах.');

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

INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (1,'Керівництво',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (2,'Адмін',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (3,'Бухгалтерія',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (4,'Представник профспілки',true);
INSERT INTO grupa (id, GrupaName, ReadOnly) VALUES (5,'Працівники',false);

CREATE TABLE `taryfni_sitky` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Posada` VARCHAR(30) NULL,
  `Taryf` DOUBLE(8, 2) DEFAULT 0.0
)
ENGINE = MyISAM;

INSERT INTO taryfni_sitky (Posada, Taryf) VALUES ('Директор', 2500);
INSERT INTO taryfni_sitky (Posada, Taryf) VALUES ('Бухгалтер', 1500);
INSERT INTO taryfni_sitky (Posada, Taryf) VALUES ('Працівники', 1000);

CREATE TABLE `progOptions` (
  `id` INT(10) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `Opys` VARCHAR(50) DEFAULT NULL,
  `Znach` VARCHAR(300) DEFAULT NULL
)
ENGINE = MyISAM;

INSERT INTO `progOptions` VALUES (1,   'Вид табеля', '1');
INSERT INTO `progOptions` VALUES (2,   'Шлях до скрипта вхідного сальдо', '../script/saldo.js');
INSERT INTO `progOptions` VALUES (3,   'Шлях до скрипта основного нарахування', '../script/osn_narah.js');
INSERT INTO `progOptions` VALUES (4,   'Шлях до скрипта утримання і відрахування ОД=01', '../script/utr_vidrah_osn_dohodu.js');
INSERT INTO `progOptions` VALUES (5,   'Шлях до скрипта утримання і відрахування ОД=02', '../script/utr_vidrah_inshi_dohody.js');
INSERT INTO `progOptions` VALUES (6,   'Шлях до скрипта обрахунку оплати', '../script/oplata.js');
INSERT INTO `progOptions` VALUES (7,   'Нарах. індекс. 0-ні,1-лише індекси,2-повна індекс', '0');
INSERT INTO `progOptions` VALUES (8,   'Розрахунок соцпільги: 0-особ рах.,1-сума нарах.', '1');
INSERT INTO `progOptions` VALUES (100, 'Шлях до папки програми \"Податкова звітність\" (OPZ)', 'c:/OPZ/');
INSERT INTO `progOptions` VALUES (101, 'Номер організації в системі \"Податкова звітність\"', '1');

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
