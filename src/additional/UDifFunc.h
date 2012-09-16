//--------------------utech--------------------utech--------------------utech--------------------
//
// Файл декларації глобальних службових фнккцій програми нарахування зарплати
//
// Створений програмістами Компанії "Утех" 17.07.2007р.
// У класі використовуються компоненти бібліотеки QT4.x
//
//--------------------utech--------------------utech--------------------utech--------------------

#ifndef UDIFFUNC_H
#define UDIFFUNC_H

#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QSettings>
#include <QDate>
#include <QColor>

#include <UMath>
//--------------------utech--------------------utech--------------------utech--------------------
static QColor 	operaciiSaldoColor(250,247,116);
static QColor 	operaciiNarahColor(136,255,111);
static QColor 	operaciiUtrymColor(255,183,240);
static QColor 	operaciiOplataColor(149,206,223);
static QColor 	operaciiVidrahColor(211,163,176);
static QColor 	greenColor(142,255,130);
static QColor 	redColor(255,160,160);
//--------------------utech--------------------utech--------------------utech--------------------
QString OrganizShortName();
QString OrganizName();
QString OrganizEDRPOU();
QString OrganizAdresa();
QString KerivnykPib();
QString KerivnykTel();
QString BuhgalterPib();
QString BuhgalterTel();
QString OrganizBank();
QString OrganizBankMFO();
QString OrganizBankRahunok();
QString OrganizEMail();
QString OrganizFax();
QString OrganizIPN();
QString OrganizSvidPDVNum();
QString tabelResString(int workerId, int curYear, int curMonth, int vydTabelya, int typOplaty);
//--------------------utech--------------------utech--------------------utech--------------------
#endif
