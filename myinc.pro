#---------------------------------------------------------
# Project created by Timoshenko Sergey 2010-11-26T10:50:18
#---------------------------------------------------------
QT = core gui xml sql
include(ncreport.pri)
#
TARGET = myinc
TEMPLATE = app
DESTDIR = bin
#
INCLUDEPATH += $$PWD/src
#
TRANSLATIONS = lang/ru.ts lang/en.ts
#
OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build
#
CONFIG += uitools
#
SOURCES = src/main.cpp\
    src/core/dproject.cpp \
    src/core/dnamespace.cpp \
    src/QProgressIndicator.cpp \
    src/ui/mainwindow.cpp \
    src/ui/dfooter.cpp \
    src/ui/ddbconfig.cpp \
    src/ui/dhomescreen.cpp \
    src/ui/dworkwidget.cpp \
    src/core/myincapplication.cpp \
    src/ui/dsqlquertyviewer.cpp \
    src/core/dsystemfuncs.cpp \
    src/core/dauth.cpp \
    src/core/durihelper.cpp
#
HEADERS =  src/core/dproject.h \
    src/core/dnamespace.h \
    src/QProgressIndicator.h \
    src/ui/mainwindow.h \
    src/ui/dfooter.h \
    src/ui/ddbconfig.h \
    src/ui/dhomescreen.h \
    src/ui/dworkwidget.h \
    src/errors.h \
    src/core/myincapplication.h \
    src/ui/dsqlquertyviewer.h \
    src/core/dsystemfuncs.h \
    src/core/dauth.h \
    src/core/durihelper.h
#
OTHER_FILES += \
    TODO \
    README \
    COPYING \
    INSTALL
#
INSTALLS = target
target.path = /usr/local/bin
