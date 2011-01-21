#---------------------------------------------------------
# Project created by Timoshenko Sergey 2010-11-26T10:50:18
#---------------------------------------------------------

QT = core gui xml
QT += sql

include(myinc.pri)

TARGET = myinc
TEMPLATE = app
DESTDIR += bin
OBJECTS_DIR += build
MOC_DIR += build
UI_DIR += build
CONFIG += uitools
INCLUDEPATH += src \
    src/ui

SOURCES += src/main.cpp\
    src/dproject.cpp \
    src/dnamespace.cpp \
    src/QProgressIndicator.cpp \
    src/dthreaduri.cpp \
    src/ui/mainwindow.cpp \
    src/ui/dfooter.cpp \
    src/ui/ddbconfig.cpp \
    src/ui/dhomescreen.cpp \
    src/ui/dtablewidget.cpp \
    src/ui/dworkwidget.cpp

HEADERS  +=  src/dproject.h \
    src/dthreaduri.h \
    src/dnamespace.h \
    src/QProgressIndicator.h \
    src/ui/mainwindow.h \
    src/ui/dfooter.h \
    src/ui/ddbconfig.h \
    src/ui/dhomescreen.h \
    src/ui/dtablewidget.h \
    src/ui/dworkwidget.h \
    src/errors.h

OTHER_FILES += \
    res/templates/sql.xml \
    res/templates/ui.xml
