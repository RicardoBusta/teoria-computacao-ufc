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
    tmcommand.cpp \
    turingmachine.cpp \
    tmhistory.cpp \
    tmgui.cpp \
    tmguistate.cpp \
    tmguiedge.cpp

HEADERS  += mainwindow.h \
    tmsyntax.h \
    exnamespace.h \
    tmcommand.h \
    turingmachine.h \
    tmhistory.h \
    tmgui.h \
    tmguistate.h \
    tmguiedge.h

FORMS    += mainwindow.ui
