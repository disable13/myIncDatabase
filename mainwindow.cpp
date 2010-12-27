#include "mainwindow.h"

#include "dproject.h"
#include "dhomescreen.h"

#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    central = new QWidget();
    l = new QGridLayout( central );

    setCentralWidget( central );

    menuBar = new QMenuBar( central );
    setMenuWidget( menuBar );
    menFile = menuBar->addMenu( tr("File") );
    actCreateProject = menFile->addAction( tr("Create Project...") );
    menFile->addSeparator();
    actOpenProject = menFile->addAction( tr("Open Project...") );
    actSave = menFile->addAction( tr("Save") );
    actSaveAs = menFile->addAction( tr("Save as...") );
    menFile->addSeparator();
    actClose = menFile->addAction( tr("Close") );
    actExit = menFile->addAction( tr("Exit"), qApp, SLOT(quit()) );

    home = new DHomeScreen( central );
    l->addWidget( home, 0, 0);

    lockUI( true );
}

MainWindow::~MainWindow()
{
    /// FIXME: ~DProject
    //if (current)
    //   delete current;
    delete home;
    delete actExit;
    delete actClose;
    delete actSaveAs;
    delete actSave;
    delete actOpenProject;
    delete actCreateProject;
    delete menFile;
    delete menuBar;
    delete l;
    delete central;
}

DProject * MainWindow::getProject()
{
    return current;
}

bool MainWindow::loadProject( QString & filename)
{
    current = new DProject( filename );
    if (!current->load())
        return false;
    return current->connectDatabase();
}

void MainWindow::lockUI(bool lo)
{
    lo = !lo;

    actSave->setEnabled( lo );
    actSaveAs->setEnabled( lo );
    actClose->setEnabled( lo );
}
