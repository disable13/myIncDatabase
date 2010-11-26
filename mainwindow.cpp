#include "mainwindow.h"

#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget * w = new QWidget();
    l = new QGridLayout( w );

    setCentralWidget( w );

    menuBar = new QMenuBar( w );
    setMenuWidget( menuBar );
    QMenu * m = menuBar->addMenu( tr("File") );
    m->addAction( tr("Create Project...") );
    m->addSeparator();
    m->addAction( tr("Open Project...") );
    m->addAction( tr("Save") );
    m->addAction( tr("Save as...") );
    m->addSeparator();
    m->addAction( tr("Close") );
    m->addAction( tr("Exit") );

}

MainWindow::~MainWindow()
{

}
