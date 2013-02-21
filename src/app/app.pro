#---------------------------------------------------------
# Project created by Timoshenko Sergey 2010-11-26T10:50:18
#---------------------------------------------------------
QT = core xml sql
#
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets uitools
    DEFINES += HAVE_QT5
} else {
    QT += gui
    CONFIG += uitools
}
#
TARGET = myinc
TEMPLATE = app
DESTDIR = ../../build/bin
#
TRANSLATIONS = ../share/lang/ru_RU.UTF-8.ts
#
OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build
#
unix {
    INSTALLS = target
    target.path = /usr/local/bin
}
#
CONFIG +=  console static
release : CONFIG -= console
#
SOURCES = main.cpp\
    core/dproject.cpp \
    core/dnamespace.cpp \
    QProgressIndicator.cpp \
    ui/mainwindow.cpp \
    ui/dfooter.cpp \
    ui/ddbconfig.cpp \
    ui/dhomescreen.cpp \
    ui/dworkwidget.cpp \
    core/myincapplication.cpp \
    ui/dsqlquertyviewer.cpp \
    core/dsystemfuncs.cpp \
    core/dauth.cpp \
    core/durihelper.cpp \
    ui/qaboutwidget.cpp \
    core/ddebug.cpp \
    ui/ddebugwidget.cpp
#
HEADERS =  core/dproject.h \
    core/dnamespace.h \
    QProgressIndicator.h \
    ui/mainwindow.h \
    ui/dfooter.h \
    ui/ddbconfig.h \
    ui/dhomescreen.h \
    ui/dworkwidget.h \
    errors.h \
    core/myincapplication.h \
    ui/dsqlquertyviewer.h \
    core/dsystemfuncs.h \
    core/dauth.h \
    core/durihelper.h \
    ui/qaboutwidget.h \
    core/ddebug.h \
    ui/ddebugwidget.h
#
OTHER_FILES += \
    samples/simple-project.xml \
    samples/sample-db.sqlite \
    samples/sample-main.ui
#
RESOURCES += \
    ../share/resource.qrc
