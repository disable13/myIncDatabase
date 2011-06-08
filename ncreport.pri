# THIS YOU MUST SET SOURCE DIR OF YOUR NCReport
NCREPORT_SRCDIR="C:\\Qt\\ncreport-qt4-0.7.30"
#
win32: LIBS += $$NCREPORT_SRCDIR/src/bin/ncreport.dll
unix: LIBS += -lncreport -L$$NCREPORT_SRCDIR/lib -L/usr/local/bin
#
INCLUDEPATH += $$NCREPORT_SRCDIR/src/libncreport
