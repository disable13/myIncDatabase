#---------------------------------------------------------
# Project created by Timoshenko Sergey 2010-11-26T10:50:18
#---------------------------------------------------------
QT = core gui xml sql

include(myinc.pri)

TARGET = myinc
TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build
CONFIG += uitools

SOURCES = src/main.cpp\
    src/core/dproject.cpp \
    src/core/dnamespace.cpp \
    src/QProgressIndicator.cpp \
    src/core/dthreaduri.cpp \
    src/ui/mainwindow.cpp \
    src/ui/dfooter.cpp \
    src/ui/ddbconfig.cpp \
    src/ui/dhomescreen.cpp \
    src/ui/dtablewidget.cpp \
    src/ui/dworkwidget.cpp \
    src/core/dthreadpool.cpp \
    src/dwidget.cpp \
    src/core/myincapplication.cpp \
    src/ui/dsqlquertyviewer.cpp \
    src/core/dsystemfuncs.cpp

HEADERS =  src/core/dproject.h \
    src/core/dthreaduri.h \
    src/core/dnamespace.h \
    src/QProgressIndicator.h \
    src/ui/mainwindow.h \
    src/ui/dfooter.h \
    src/ui/ddbconfig.h \
    src/ui/dhomescreen.h \
    src/ui/dtablewidget.h \
    src/ui/dworkwidget.h \
    src/errors.h \
    src/core/dthreadpool.h \
    src/dwidget.h \
    src/core/myincapplication.h \
    src/ui/dsqlquertyviewer.h \
    src/core/dsystemfuncs.h

OTHER_FILES += \
    TODO \
    README

TRANSLATIONS += ru.ts en.ts
