#---------------------------------------------------------
# Project created by Timoshenko Sergey 2010-11-26T10:50:18
#---------------------------------------------------------

QT = core gui xml
QT += sql

TARGET = myinc
TEMPLATE = app
DESTDIR += bin
OBJECTS_DIR += build
MOC_DIR += build
UI_DIR += build
CONFIG += uitools

SOURCES += main.cpp\
        mainwindow.cpp \
    dproject.cpp \
    dhomescreen.cpp \
    dtablewidget.cpp \
    dworkwidget.cpp \
    dnamespace.cpp

HEADERS  += mainwindow.h \
    dproject.h \
    dhomescreen.h \
    dtablewidget.h \
    dworkwidget.h \
    dnamespace.h \
    errors.h

OTHER_FILES += \
    templates/sql.xml \
    templates/ui.xml
