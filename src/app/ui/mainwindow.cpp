#include "mainwindow.h"
//
#include "../core/dproject.h"
#include "../core/myincapplication.h"
#include "../core/dnamespace.h"
#include "dhomescreen.h"
#include "dfooter.h"
#include "ddbconfig.h"
#include "dsqlquertyviewer.h"
#include "qaboutwidget.h"
#include "ddebugwidget.h"
#include "../errors.h"
//
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
//#include <QCloseEvent>
#include <QStatusBar>
#include <QSystemTrayIcon>
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
    debug = 0x00;

    central = new QWidget();
    l = new QGridLayout( central );

    setCentralWidget( central );
    setWindowIcon( QIcon( ":/icon/home.png") );

    menuBar = new QMenuBar( central );
    setMenuWidget( menuBar );
    menFile = menuBar->addMenu( tr("File") );
//    actCreateProject = menFile->addAction( tr("Create Project..."),
//                                          this, SLOT(createProject()) );
//    menFile->addSeparator();
    actOpenProject = menFile->addAction( QIcon(":/icon/folder.png"), tr("Open Project..."),
                                        this, SLOT(openProjectPush()));
    //actSave = menFile->addAction( tr("Save") );
    //actSaveAs = menFile->addAction( tr("Save as...") );
    menFile->addSeparator();
    actClose = menFile->addAction( tr("Close"), this, SLOT(closeProject()) );
    actExit = menFile->addAction( QIcon(":/icon/halt.png"), tr("Exit"), qApp, SLOT(quit()) );

    menProject = menuBar->addMenu( tr("Project") );
    actConnect = menProject->addAction( tr("Connect..."),
                                       this, SLOT(connectDatabase()) );
    menProject->addSeparator();
    actDbSettings = menProject->addAction( QIcon(":/icon/configure.png"), tr("Connection settings"),
                                          this, SLOT(openConnectionSettings()) );
    actQuerySettings = menProject->addAction( tr("Query settings"),
                                             this, SLOT(openQuerySettings()) );
//    actUiSettings = menProject->addAction( tr("User interface"),
//                                          this, SLOT(openUiSettings()) );
    if (MIA_GLOBAL->getDebug())
        menProject->addAction( QIcon(":/icon/develop.png"), tr("Debug tool"),
                           this, SLOT(openDebug()) );

    menHelp = menuBar->addMenu( tr("Help") );
    actHelp = menHelp->addAction( QIcon(":/icon/question.png"), tr("Help") );
    actAbout = menHelp->addAction( QIcon(":/icon/info.png"), tr("About programm..."),
                                   this, SLOT(about()) );
    actAboutQt = menHelp->addAction( QIcon(":/icon/qt-logo.png"), tr("About Qt..."),
                                     MIA_APP, SLOT(aboutQt()) );
    // Tray
    trayMenu = new QMenu(tr("Tray menu"), this);
    trayMenu->addAction( tr("Show/Hide programm"), this, SLOT(showHideMain()) );
    trayMenu->addSeparator();
    trayMenu->addAction( tr("Exit"), qApp, SLOT(quit()) );

    tray = new QSystemTrayIcon( QIcon(":/icon/qrcode.png") );
    tray->setObjectName( "tray" );
    tray->setContextMenu( trayMenu );

    home = new DHomeScreen( central );
    l->addWidget( home, 0, 0);

    QStatusBar * sb = new QStatusBar( this );
    setStatusBar( sb );
    footer = new DFooter( central );
    sb->addWidget( footer );

    footer->setText( tr("Open MyInc Project File") );
    setWindowTitle( tr("No project").append(" - MyIncDatabase ver. ")
                    .append(MIA_APP->applicationVersion()) );

    lockUI( true );

    show();
}
//
MainWindow::~MainWindow()
{
    FREE_MEM(sqlQuertyViewer);
    FREE_MEM(debug);
    FREE_MEM(home);
    FREE_MEM(footer);
    FREE_MEM(actExit);
    FREE_MEM(actClose);
    //FREE_MEM(actSaveAs);
    //FREE_MEM(actSave);
    FREE_MEM(actOpenProject);
    //FREE_MEM(actCreateProject);
    FREE_MEM(actConnect);
    FREE_MEM(actDbSettings);
    FREE_MEM(actQuerySettings);
    //FREE_MEM(actUiSettings);
    FREE_MEM(menProject);
    FREE_MEM(menFile);
    FREE_MEM(menuBar);
    FREE_MEM(trayMenu);
    FREE_MEM(tray);
    FREE_MEM(l);
    FREE_MEM(central);
}
//
bool MainWindow::loadProject(const QString &fileName)
{
    MyIncApplication::instance()->openProject( fileName );
    connect( MIA_PROJECT, SIGNAL(error(int)),
            this, SLOT(error(int)) );
    isOpened = true;
    bool tmp = MIA_PROJECT->load();
    if (tmp) {
        if (!MIA_PROJECT->authorized())
            if (!MIA_PROJECT->getAuthorized( this )) {
                MyIncApplication::instance()->closeProject();
                tmp = false;
            }
        setWindowTitle( MIA_NAMESPACE->config("Title", "").append(" - MyIncDatabase ver. ")
                        .append(MIA_APP->applicationVersion()) );
    }
    return tmp;
}
//
DHomeScreen * MainWindow::getHome()
{
    return home;
}
bool MainWindow::openManualWorkspace()
{
    if (!isOpened || !isConnected)
        return false;
    return home->selectManualWorkspace();
}
//
void MainWindow::closeEvent(QCloseEvent *)
{
    tray->show();
}
//
void MainWindow::showEvent(QShowEvent *)
{
    tray->hide();
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
    //actUiSettings->setEnabled( lo );
}
//
void MainWindow::showHideMain()
{
    setHidden( ! isHidden() );
}
// FIXME: MainWindow::createProject()
void MainWindow::createProject()
{
    qDebug("FIXME: MainWindow::createProject()");

    QFileDialog f(this, tr("Select MyInc Project"), "", "XML files (*.xml)" );
    if (f.exec()) {
        MIA_GLOBAL->openProject( f.selectedFiles()[0] );
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
    c.setProject( MIA_PROJECT );
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
void MainWindow::openDebug()
{
    if (debug == 0x00)
        debug = new DDebugWidget( this );
    debug->show();
}
//
void MainWindow::connectDatabase()
{
    footer->progressStart( tr("Connection to database...") );
    if (isConnected) {
        home->clear();
        MIA_PROJECT->disconnectDatabase();
        isConnected = false;
    } else {
        if (!MIA_PROJECT->connectDatabase()) {
            QMessageBox msg;
            msg.setIcon( QMessageBox::Critical );
            //msg.setWindowTitle( app.tr("This is Title") );
            msg.setStandardButtons( QMessageBox::Retry | QMessageBox::Cancel );
            msg.setDefaultButton( QMessageBox::Cancel );
            msg.setText( tr("Can't connect to SQL Server") );
            msg.setDetailedText( MIA_PROJECT->getLastError() );

            if ( msg.exec() == QMessageBox::Retry ) // recurcive
                this->connectDatabase();

            isConnected = false;
            footer->progressStop( false );
            return;
        }
        isConnected = true;
        home->setProject( MIA_PROJECT );
    }
    actConnect->setText( (isConnected) ? tr("Disconnect...") : tr("Connect...") );
    footer->progressStop( true );
}
// BUGME: second close with crash
void MainWindow::closeProject()
{
    qDebug("BUGME: void MainWindow::closeProject()");
    if (sqlQuertyViewer)
        delete sqlQuertyViewer;
    if (isConnected)
        connectDatabase(); // to disconnect from database
    setWindowTitle( tr("No project").append(" - MyIncDatabase ver. ")
                    .append(MIA_APP->applicationVersion()) );
    MIA_GLOBAL->closeProject();

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
//
void MainWindow::about()
{
    QAboutWidget * w = new QAboutWidget();

    w->show();
}
