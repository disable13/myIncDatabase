TEMPLATE = lib
#
QT = core gui
TARGET = wizard
#
unix {
    INSTALLS = target
    target.path = /usr/local/lib/myinc
}
#
include(../libs.pri)
#
HEADERS += \
    dwizard.h
#
SOURCES += \
    dwizard.cpp
#
OTHER_FILES += \
    wizard.pri
