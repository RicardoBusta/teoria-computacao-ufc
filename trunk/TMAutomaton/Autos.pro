#-------------------------------------------------
#
# Project created by QtCreator 2013-05-08T17:02:26
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Autos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dragwidget.cpp \
    dragelement.cpp \
    dragedge.cpp \
    dragstate.cpp \
    dragtext.cpp

HEADERS  += mainwindow.h \
    dragwidget.h \
    dragelement.h \
    dragedge.h \
    dragstate.h \
    staticresource.h \
    dragtext.h

FORMS    += mainwindow.ui

RESOURCES += \
    cursor/cursor.qrc
