#include <QtGui/QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (argc != 0)
        if (QFile::exists(argv[0])) { // Open DB-Connection

    }

    MainWindow w;
    w.show();

    return app.exec();
}
