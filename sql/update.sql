ALTER TABLE `operacii`
 ADD COLUMN `N_Indexaciya` DOUBLE(8,2) DEFAULT '0.00' AFTER `N_Doplata`,
 CHANGE COLUMN `O_Zaborgovanist` `O_Inshi` DOUBLE(8,2) DEFAULT '0.00',
 ADD COLUMN `O_ZaPoperMis` DOUBLE(8,2) DEFAULT '0.00' AFTER `O_VsogoNaRuky`,
 ADD COLUMN `O_Premiya` DOUBLE(8,2) DEFAULT '0.00' AFTER `O_ZaPoperMis`;

DROP TABLE IF EXISTS workdaysinmonth

