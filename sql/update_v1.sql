CREATE TABLE version_salary (
  `id` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `version` INTEGER UNSIGNED NOT NULL,
  PRIMARY KEY (`id`)
)
ENGINE = MyISAM;

INSERT INTO version_salary (id, version) VALUES (1, 1);

ALTER TABLE `salaryslujbova`.`sl_table` ADD COLUMN `password` VARCHAR(40) NOT NULL DEFAULT '698d51a19d8a121ce581499d7b701668' AFTER `db`;

DROP TABLE IF EXISTS grupa;

ALTER TABLE npr 
 DROP COLUMN Password,
 DROP COLUMN Grupa_id;
