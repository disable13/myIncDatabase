#---------------------------------------------------------
# Project created by Timoshenko Sergey 2010-11-26T10:50:18
#---------------------------------------------------------

QT = core gui
#QT += sql

TARGET = myinc
TEMPLATE = app
DEFINES += NO_SQL


SOURCES += main.cpp\
        mainwindow.cpp \
    dproject.cpp

HEADERS  += mainwindow.h \
    dproject.h
