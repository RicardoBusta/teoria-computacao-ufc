#-------------------------------------------------
#
# Project created by QtCreator 2013-04-13T16:58:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuringMachine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tmsyntax.cpp \
    tmstate.cpp \
    tmcharacter.cpp \
    tmcommand.cpp

HEADERS  += mainwindow.h \
    tmsyntax.h \
    exnamespace.h \
    tmstate.h \
    tmcharacter.h \
    tmcommand.h

FORMS    += mainwindow.ui
