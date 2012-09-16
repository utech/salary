#TARGET =
DEPENDPATH += $$PWD/. $$PWD/ui $$PWD/base $$PWD/gui $$PWD/additional \
				$$PWD/engine $$PWD/moneyToStrConverter $$PWD/export $$PWD/dbUpdater
INCLUDEPATH += $$PWD/. $$PWD/ui $$PWD/base $$PWD/gui $$PWD/additional \
				$$PWD/engine $$PWD/moneyToStrConverter $$PWD/export  $$PWD/dbUpdater
QT += script sql xml network

RESOURCES += uPictRc.qrc

HEADERS += UPeopleSalaryMainWindow.h \
 uregistermanager.h \
 UTaryfDelegate.h \
 UUserDelegate.h \
 UMonthlyBudjetniNormyForm.h \
 UPrintDocs.h \
 UComboBoxSQLRelation.h \
 UDialogYMFilter.h \
 USalaryScript.h \
 UDialogOptions.h \
 UDifFunc.h \
 UDialogFirmInfo.h \
 UWidgetPrintPlatijky.h \
 UZvit1df.h \
 USalaryOptions.h \
 UDialogIndexaciyaType.h \
 USalary.h \
 UDialogLogin_salary.h \
 USalaryDbUpdaterFunctions.h \
 UDialogSalaryPidrozdily.h \
 UDialogSalarySvyatkoviDni.h \
 UDialogSalaryEditTabelCell.h \
 UFormSalaryEditTabelWidget.h \
 UPFUMonthPersonDbf.h \
 UDialogChoosePidrozdil_salary.h \
 UFormSalaryTabelSumWidget.h \
 UPFUEdynyiVnesokDbf.h
FORMS += UPeopleSalaryMainWindow.ui \
	registerForm.ui \
	UMonthlyBudjetniNormyForm.ui \
	UDialogYMFilter.ui \
	UDialogOptions.ui \
	UDialogFirmInfo.ui \
	UWidgetPrintPlatijky.ui \
	UDialogIndexaciyaType.ui \
	UDialogLogin_salary.ui \
	UDialogSalaryPidrozdily.ui \
	UDialogSalarySvyatkoviDni.ui \
	UDialogSalaryEditTabelCell.ui \
	UFormSalaryEditTabelWidget.ui \
	UDialogChoosePidrozdil_salary.ui \
	UFormSalaryTabelSumWidget.ui
SOURCES += main_salary.cpp \
 UPeopleSalaryMainWindow.cpp \
 UPeopleSalaryMainWindow_actions.cpp \
 UPeopleSalaryMainWindow_constructor.cpp \
 UTaryfDelegate.cpp \
 uregistermanager.cpp \
 UUserDelegate.cpp \
 UMonthlyBudjetniNormyForm.cpp \
 UPrintDocs.cpp \
 UPrintDocs_2.cpp \
 UPrintDocsTemplates.cpp \
 UComboBoxSQLRelation.cpp \
 USalaryScript.cpp \
 USalaryScript_c.cpp \
 UDialogYMFilter.cpp \
 UDialogOptions.cpp \
 UDifFunc.cpp \
 UDialogFirmInfo.cpp \
 UWidgetPrintPlatijky.cpp \
 UZvit1df.cpp \
 USalaryOptions.cpp \
 UDialogIndexaciyaType.cpp \
 USalary.cpp \
 UDialogLogin_salary.cpp \
 USalaryDbUpdaterFunctions.cpp \
 UDialogSalaryPidrozdily.cpp \
 UDialogSalarySvyatkoviDni.cpp \
 UDialogSalaryEditTabelCell.cpp \
 UFormSalaryEditTabelWidget.cpp \
 UPFUMonthPersonDbf.cpp \
 UDialogChoosePidrozdil_salary.cpp \
 UFormSalaryTabelSumWidget.cpp \
 UPFUEdynyiVnesokDbf.cpp

#include("UXBase64/UXBase64.pro")
 
RC_FILE +=   salaryProgIcon.rc 
