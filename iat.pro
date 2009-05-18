DESTDIR        = ./
OBJECTS_DIR    = tmp/
MOC_DIR        = tmp/moc
UI_HEADERS_DIR = src/uic/
UI_SOURCES_DIR = src/uic/

QMAKE_CXXFLAGS_RELEASE += -g0 -O3
QMAKE_CXXFLAGS_DEBUG   += -ggdb -O0

CONFIG += debug
CONFIG += warn_on


FORMS += src/dialogs/ItemDialog.ui
FORMS += src/dialogs/InstructionsDialog.ui

SOURCES += src/Item.cpp
SOURCES += src/Instructions.cpp
SOURCES += src/Controller.cpp
SOURCES += src/main.cpp

HEADERS += src/Item.h
HEADERS += src/Instructions.h
HEADERS += src/Controller.h

