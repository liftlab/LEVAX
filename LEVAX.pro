#-------------------------------------------------
#
# Project created by QtCreator 2014-12-04T18:43:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LEVAX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    rsc.qrc
	
win32:RC_FILE = application.rc
