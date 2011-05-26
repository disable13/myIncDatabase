#include "mainwindow.h"
//
#include "src/core/dproject.h"
#include "src/core/myincapplication.h"
#include "src/ui/dhomescreen.h"
#include "src/ui/dfooter.h"
#include "src/ui/ddbconfig.h"
#include "src/ui/dsqlquertyviewer.h"
#include "src/errors.h"
//
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
//
// FIXME: class MainWindow. include qDebug()
#include <QDebug>
//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    isConnected = false;
    isOpened = false;
    sqlQuertyViewer = 0x00;

    central = new QWidget();
    l = new QGridLayout( central );

    setCentralWidget( central );

    menuBar = new QMenuBar( central );
    setMenuWidget( menuBar );
    menFile = menuBar->addMenu( tr("File") );
    actCreateProject = menFile->addAction( tr("Create Project..."),
                                          this, SLOT(createProject()) );
    menFile->addSeparator();
    actOpenProject = menFile->addAction( tr("Open Project..."),
                                        this, SLOT(openProjectPush()));
    //actSave = menFile->addAction( tr("Save") );
    //actSaveAs = menFile->addAction( tr("Save as...") );
    menFile->addSeparator();
    actClose = menFile->addAction( tr("Close"), this, SLOT(closeProject()) );
    actExit = menFile->addAction( tr("Exit"), qApp, SLOT(quit()) );

    menProject = menuBar->addMenu( tr("Project") );
    actConnect = menProject->addAction( tr("Connect..."),
                                       this, SLOT(connectDatabase()) );
    menProject->addSeparator();
    actDbSettings = menProject->addAction( tr("Connection settings"),
                                          this, SLOT(openConnectionSettings()) );
    actQuerySettings = menProject->addAction( tr("Query settings"),
                                             this, SLOT(openQuerySettings()) );
    actUiSettings = menProject->addAction( tr("User interface"),
                                          this, SLOT(openUiSettings()) );

    home = new DHomeScreen( central );
    l->addWidget( home, 0, 0);

    footer = new DFooter( central );
    l->addWidget( footer, 1, 0);

    footer->setText( tr("Open MyInc Project File") );

    lockUI( true );

    show();
}
//
MainWindow::~MainWindow()
{
    if (sqlQuertyViewer)
        delete sqlQuertyViewer;
    delete home;
    delete footer;
    delete actExit;
    delete actClose;
    //delete actSaveAs;
    //delete actSave;
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
//
bool MainWindow::loadProject( QString & fileName)
{
    MyIncApplication::instance()->openProject( fileName );
    connect( MyIncApplication::project(), SIGNAL(error(int)),
            this, SLOT(error(int)) );
    isOpened = true;
    return MyIncApplication::project()->load();
}
//
void MainWindow::closeEvent(QCloseEvent *)
{
    MyIncApplication::application()->exit( 0x00 );
}
//
void MainWindow::lockUI(bool lo)
{
    lo = !lo;

    //actSave->setEnabled( lo );
    //actSaveAs->setEnabled( lo );
    actClose->setEnabled( lo );
    actConnect->setEnabled( lo );
    actDbSettings->setEnabled( lo );
    actQuerySettings->setEnabled( lo );
    actUiSettings->setEnabled( lo );
}
// FIXME: MainWindow::createProject()
void MainWindow::createProject()
{
    qDebug("FIXME: MainWindow::createProject()");

    QFileDialog f(this, tr("Select MyInc Project"), "", "XML files (*.xml)" );
    if (f.exec()) {
        MyIncApplication::openProject( f.selectedFiles()[0] );
        lockUI( false );
    }
    qDebug("TODO: Run Wizard");
}
//
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
//
void MainWindow::openConnectionSettings()
{
    DDbConfig c(this);
    c.setProject( MyIncApplication::project() );
    c.exec();
}
//
void MainWindow::openQuerySettings()
{
    if (!isConnected)
        connectDatabase();
    if ( sqlQuertyViewer ) { // created
        sqlQuertyViewer->show();
        qDebug("FIXME: MainWindow::openQuerySettings()");
        // FIXME: why without focus
        sqlQuertyViewer->setFocus();
    } else {
        sqlQuertyViewer = new DSqlQuertyViewer();
        sqlQuertyViewer->show();
    }
}
// TODO: MainWindow::openUiSettings()
void MainWindow::openUiSettings()
{
    qDebug("TODO: MainWindow::openUiSettings()");
}
//
void MainWindow::connectDatabase()
{
    footer->progressStart( tr("Connection to database...") );
    if (isConnected) {
        home->clear();
        MyIncApplication::project()->disconnectDatabase();
        isConnected = false;
    } else {
        if (!MyIncApplication::project()->connectDatabase()) {
            QMessageBox msg;
            msg.setIcon( QMessageBox::Critical );
            //msg.setWindowTitle( app.tr("This is Title") );
            msg.setStandardButtons( QMessageBox::Retry | QMessageBox::Cancel );
            msg.setDefaultButton( QMessageBox::Cancel );
            msg.setText( tr("Can't connect to SQL Server") );
            msg.setDetailedText( MyIncApplication::project()->getLastError() );

            if ( msg.exec() == QMessageBox::Retry ) // recurcive
                this->connectDatabase();

            isConnected = false;
            footer->progressStop( false );
            return;
        }
        isConnected = true;
        home->setProject( MyIncApplication::project() );
    }
    actConnect->setText( (isConnected) ? tr("Disconnect...") : tr("Connect...") );
    footer->progressStop( true );
}
//
void MainWindow::closeProject()
{
    if (sqlQuertyViewer)
        delete sqlQuertyViewer;
    if (isConnected)
        connectDatabase(); // to disconnect from database
    MyIncApplication::closeProject();
    lockUI( true );
}
//
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

    qDebug() << "ERROR: " << text;
}
