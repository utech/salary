CONFIG		+= release
CONFIG		-= debug debug_and_release debug_and_release_target

TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .
QT += sql
#CONFIG += console
HEADERS += UConfigWizard.h delegates.h
SOURCES += UConfigWizard.cpp delegates.cpp main.cpp
DESTDIR += ../../bin

OBJECTS_DIR = ../../tmp/configSalary
MOC_DIR = ../../tmp/configSalary
UI_HEADERS_DIR = ../../tmp/configSalary
UI_SOURCES_DIR = ../../tmp/configSalary
RCC_DIR = ../../tmp/configSalary

RC_FILE = configSalaryIcon.rc 