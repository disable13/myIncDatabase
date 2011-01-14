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
INCLUDEPATH += src \
    src/ui

SOURCES += src/main.cpp\
        src/ui/mainwindow.cpp \
    src/dproject.cpp \
    src/ui/dhomescreen.cpp \
    src/ui/dtablewidget.cpp \
    src/ui/dworkwidget.cpp \
    src/dnamespace.cpp \
    src/ui/dfooter.cpp \
    src/QProgressIndicator.cpp

HEADERS  += src/ui/mainwindow.h \
    src/dproject.h \
    src/ui/dhomescreen.h \
    src/ui/dtablewidget.h \
    src/ui/dworkwidget.h \
    src/dnamespace.h \
    src/errors.h \
    src/ui/dfooter.h \
    src/QProgressIndicator.h

OTHER_FILES += \
    res/templates/sql.xml \
    res/templates/ui.xml
