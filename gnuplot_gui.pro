#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T10:11:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = gnuplot_gui_control
TEMPLATE = app


SOURCES  += main.cpp \
            mainwindow.cpp \
            gnuplot.cpp

HEADERS  += mainwindow.h \
            gnuplot.h

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`
