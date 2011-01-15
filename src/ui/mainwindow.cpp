#include "mainwindow.h"

#include "dproject.h"
#include "dhomescreen.h"
#include "dfooter.h"
#include "errors.h"
#include "ddbconfig.h"

#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#warning "FIXME: class MainWindow. include qDebug()"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    isConnected = false;
    isOpened = false;

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
    if (isOpened)
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
    isOpened = true;
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

#warning "FIXME: MainWindow::createProject()"

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
    DDbConfig c(this);
    c.setProject( current );
    c.exec();
}

#warning "TODO: MainWindow::openQuerySettings()"

void MainWindow::openQuerySettings()
{
    /// TODO
    qDebug("TODO: MainWindow::openQuerySettings()");
}

#warning "TODO: MainWindow::openUiSettings()"

void MainWindow::openUiSettings()
{
    /// TODO
    qDebug("TODO: MainWindow::openUiSettings()");
}

#warning "TODO: MainWindow::connectDatabase()"

void MainWindow::connectDatabase()
{
    /// TODO
    qDebug("FIXME: MainWindow::connectDatabase()");
    footer->progressStart( tr("Connection to database...") );
    if (isConnected) {
        current->disconnectDatabase();
    } else {
        if (!current->connectDatabase()) {
            QMessageBox::warning( this, tr("Error"), tr("Can't connect to database.") );
            isConnected = false;
            footer->progressStop( false );
            return;
        }
        isConnected = true;
        home->setProject( current );
    }
    actConnect->setText( (isConnected) ? tr("Disconnect...") : tr("Connect...") );
    footer->progressStop( true );
}

void MainWindow::error(int e)
{
    QString text;
    switch (e) {
    // Database
    case _ERR_DB_CONNECT:
        text = tr("Can't connect to SQL Server"); break;
    // URI
    case _ERR_URI_SYNTAX:
        text = tr("Syntax error in URI query."); break;
    // Namespace
    case _ERR_NS_SQLNOINIT:
        text = tr("SQL not be initialized."); break;
    case _ERR_NS_CNFNOINIT:
        text = tr("Config not be initialized."); break;
    case _ERR_NS_TYPE:
        text = tr("Namespace type error?!"); break;
    case _ERR_NS_NOCNFNODE:
        text = tr("Config node not found in project xml file."); break;
    case _ERR_NS_NOROOT:
        text = tr("Root node not found in project xml file."); break;
    case _ERR_NS_SYNTAX_PRO:
        text = tr("Syntax error in project xml file."); break;
    case _ERR_CANTOPEN:
        text = tr("Can't open file."); break;
    case _ERR_NO_ERROR:
    case _ERR_UNKNOW:
    default:
        text = tr("Unknown");
    }

    qDebug() << "Error: " << e;
}
