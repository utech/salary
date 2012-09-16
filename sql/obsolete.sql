DROP TABLE IF EXISTS `workDaysInMonth`;
CREATE TABLE `workDaysInMonth` (
  `id` INT(10) 			NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `numDays` 	DATE DEFAULT NULL,
  `numGod` 	DOUBLE(4, 1) DEFAULT 0.0
)
ENGINE = MyISAM;

INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-1-21',168);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-2-20',160);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-3-21',168);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-4-20',160);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-5-19',152);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-6-20',160);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-7-22',176);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-8-22',176);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-9-20',160);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-10-23',184);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-11-22',176);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2007-12-21',168);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-1-21',168);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-2-21',168);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-3-20',160);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-4-21',168);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-5-19',152);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-6-19',152);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-7-23',184);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-8-20',160);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-9-22',176);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-10-23',184);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-11-20',160);
INSERT INTO workDaysInMonth (numDays, numGod) VALUES ('2008-12-23',184);