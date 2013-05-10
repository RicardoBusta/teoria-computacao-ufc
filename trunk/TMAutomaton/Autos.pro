#-------------------------------------------------
#
# Project created by QtCreator 2013-05-08T17:02:26
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Autos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dragwidget.cpp \
    dragelement.cpp \
    dragedge.cpp

HEADERS  += mainwindow.h \
    dragwidget.h \
    dragelement.h \
    dragedge.h

FORMS    += mainwindow.ui
