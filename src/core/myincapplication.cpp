#include "myincapplication.h"
//
#include "src/core/dnamespace.h"
#include "src/core/dthreadpool.h"
#include "src/core/dproject.h"
#include "src/ui/mainwindow.h"
//
MyIncApplication*   MyIncApplication::self          = 0x00;
QApplication*       MyIncApplication::m_app         = 0x00;
DNamespace*         MyIncApplication::m_namespace   = 0x00;
DThreadPool*        MyIncApplication::m_pool        = 0x00;
DProject*           MyIncApplication::m_project     = 0x00;
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

    new MainWindow();
}
//
MyIncApplication::~MyIncApplication()
{

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
    m_namespace->initSql();

    return true;
}
//
bool MyIncApplication::closeProject()
{
    delete m_project;
    m_project = 0x00;

    return true;
}
