CONFIG		+= release
CONFIG		-= debug debug_and_release debug_and_release_target

TEMPLATE = app
include(../../UtechLib/UtechLib.pri)

include(project_files.pri)
#CONFIG += console

DESTDIR = ../bin
UI_DIR = ./ui

OBJECTS_DIR = ../tmp
MOC_DIR = ../tmp
UI_HEADERS_DIR = ../tmp
UI_SOURCES_DIR = ../tmp
RCC_DIR = ../tmp