#include "mainwindow.h"

#include "dproject.h"
#include "dhomescreen.h"
#include "dfooter.h"

#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    isConnected = false;

    central = new QWidget();
    l = new QGridLayout( central );

    setCentralWidget( central );

    menuBar = new QMenuBar( central );
    setMenuWidget( menuBar );
    menFile = menuBar->addMenu( tr("File") );
    actCreateProject = menFile->addAction( tr("Create Project..."), this, SLOT(createProject()) );
    menFile->addSeparator();
    actOpenProject = menFile->addAction( tr("Open Project..."), this, SLOT(openProjectPush()));
    actSave = menFile->addAction( tr("Save") );
    actSaveAs = menFile->addAction( tr("Save as...") );
    menFile->addSeparator();
    actClose = menFile->addAction( tr("Close") );
    actExit = menFile->addAction( tr("Exit"), qApp, SLOT(quit()) );

    menProject = menuBar->addMenu( tr("Project") );
    actConnect = menProject->addAction( tr("Connect..."), this, SLOT(connectDatabase()) );
    menProject->addSeparator();
    actDbSettings = menProject->addAction( tr("Connection settings"), this, SLOT(openConnectionSettings()) );
    actQuerySettings = menProject->addAction( tr("Query settings"), this, SLOT(openQuerySettings()) );
    actUiSettings = menProject->addAction( tr("User interface"), this, SLOT(openUiSettings()) );

    home = new DHomeScreen( central );
    l->addWidget( home, 0, 0);

    footer = new DFooter( central );
    l->addWidget( footer, 1, 0);

    footer->setText( tr("Open MyInc Project File") );

    lockUI( true );
}

MainWindow::~MainWindow()
{
    /// FIXME: ~DProject
    qDebug("FIXME: MainWindow::~MainWindow()");
    if (current)
       delete current;
    delete home;
    delete footer;
    delete actExit;
    delete actClose;
    delete actSaveAs;
    delete actSave;
    delete actOpenProject;
    delete actCreateProject;
    delete actConnect;
    delete actDbSettings;
    delete actQuerySettings;
    delete actUiSettings;
    delete menProject;
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
    connect( current, SIGNAL(error(int)), this, SLOT(error(int)) );

    return current->load();
}

void MainWindow::lockUI(bool lo)
{
    lo = !lo;

    actSave->setEnabled( lo );
    actSaveAs->setEnabled( lo );
    actClose->setEnabled( lo );
    actConnect->setEnabled( lo );
    actDbSettings->setEnabled( lo );
    actQuerySettings->setEnabled( lo );
    actUiSettings->setEnabled( lo );
}

void MainWindow::createProject()
{
    /// FIXME
    qDebug("FIXME: MainWindow::createProject()");

    QFileDialog f(this, tr("Select MyInc Project"), "", "XML files (*.xml)" );
    if (f.exec()) {
        current = new DProject( f.selectedFiles()[0] );
        lockUI( false );
    }
    qDebug("TODO: Run Wizard");
}

void MainWindow::openProjectPush()
{
    QFileDialog f(this, tr("Select MyInc Project"), "", "XML files (*.xml)" );
    if (f.exec()) {
        footer->progressStart();
        if ( !loadProject(f.selectedFiles()[0]) ) {
            QMessageBox::warning( this, tr("Error"), tr("Can't read project file.") );
            return;
        }
        footer->progressStop();
        lockUI( false );
    }
}

void MainWindow::openConnectionSettings()
{
    /// TODO
    qDebug("TODO: MainWindow::openConnectionSettings()");
}

void MainWindow::openQuerySettings()
{
    /// TODO
    qDebug("TODO: MainWindow::openQuerySettings()");
}

void MainWindow::openUiSettings()
{
    /// TODO
    qDebug("TODO: MainWindow::openUiSettings()");
}

void MainWindow::connectDatabase()
{
    /// TODO
    qDebug("FIXME: MainWindow::connectDatabase()");
    footer->progressStart();
    if (isConnected) {
        current->disconnectDatabase();
    } else {
        if (current->connectDatabase())
            QMessageBox::warning( this, tr("Error"), tr("Can't connect to database.") );
        isConnected = true;
    }
    actConnect->setText( (isConnected) ? tr("Disconnect...") : tr("Connect...") );
}

void MainWindow::error(int e)
{
    qDebug() << "Error: " << e;
}
