#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T18:43:06
#
#-------------------------------------------------

QT       += core gui\
            printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LEVAX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tinyXML/tinystr.cpp \
    tinyXML/tinyxml.cpp \
    tinyXML/tinyxmlerror.cpp \
    tinyXML/tinyxmlparser.cpp \
    BuildingHandler.cpp \
    Building.cpp \
    LiftHandler.cpp \
    Lift.cpp \
    SimulatedHuman.cpp \
    SimulatedHumanHandler.cpp \
    Algorithm.cpp

HEADERS  += mainwindow.h \
    tinyXML/tinystr.h \
    tinyXML/tinyxml.h \
    BuildingHandler.h \
    Building.h \
    LiftHandler.h \
    Lift.h \
    SimulatedHuman.h \
    SimulatedHumanHandler.h \
    Algorithm.h

FORMS    += mainwindow.ui

RESOURCES += \
    rsc.qrc
	
win32:RC_FILE = application.rc

ICON = Images/icon.icns

DEFINES *= QT_USE_QSTRINGBUILDER

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
