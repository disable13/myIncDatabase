#include "myincapplication.h"
//
#include "src/core/dnamespace.h"
#include "src/core/dthreadpool.h"
#include "src/core/dproject.h"
#include "src/ui/mainwindow.h"
//
#include <QFile>
#include <QTimer>
//
MyIncApplication*   MyIncApplication::self          = 0x00;
QApplication*       MyIncApplication::m_app         = 0x00;
DNamespace*         MyIncApplication::m_namespace   = 0x00;
DThreadPool*        MyIncApplication::m_pool        = 0x00;
DProject*           MyIncApplication::m_project     = 0x00;
MainWindow*         MyIncApplication::m_mainWindow  = 0x00;
bool                MyIncApplication::m_useThreads  = false;
//
MyIncApplication::MyIncApplication(int &argc, char** argv) :
    QObject()
{
    MyIncApplication::m_app = new QApplication(argc,argv);
    Q_ASSERT_X(!self, "MyIncApplication",
               "there should be only one application object");
    MyIncApplication::self = this;

    m_namespace = new DNamespace();
    m_pool = new DThreadPool();

    // Check arguments
    if (argc > 1)
        for(int i = 0; i < argc; i++) {
            QString arg = QString(argv[i]);
            if (arg.startsWith("--")) {
                if (arg == QString("--help") ) { // give help
                    qDebug(qPrintable(tr("--drivers\tshow aviable SQL drivers")));
                    qDebug(qPrintable(tr("--help\tshow this message")));
                    QTimer::singleShot( 50, m_app, SLOT(quit()) );
                    return;
                }
                if (arg == "--drivers") { // show aviable SQL drivers
                    qDebug("QSQLITE ^^,");
                    QTimer::singleShot( 50, m_app, SLOT(quit()) );
                    return;
                }
            } else if (QFile::exists(arg) && arg.endsWith(".xml")) { // is file ?
               m_mainWindow = new MainWindow();
               m_mainWindow->loadProject(arg);
               m_mainWindow->lockUI( false );
               m_mainWindow->connectDatabase();
               return;
            }
        }

    m_mainWindow =  new MainWindow();
}
//
MyIncApplication::~MyIncApplication()
{
    delete m_mainWindow;
    delete m_project;
    delete m_pool;
    delete m_namespace;
    QTimer::singleShot( 50, m_app, SLOT(quit()) );
}
//
bool MyIncApplication::openProject( QString fileName )
{
    Q_ASSERT_X(!m_project, "Project",
               "You don't able to open more then one project.");
    MyIncApplication::m_project = new DProject(fileName);
    connect( m_namespace, SIGNAL(error(int)),
            m_project, SIGNAL(error(int)) );
    m_namespace->initConfig();
    //m_namespace->initSql();

    return true;
}
//
bool MyIncApplication::closeProject()
{
    delete m_project;
    m_project = 0x00;

    return true;
}
