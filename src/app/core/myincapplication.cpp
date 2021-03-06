#include "myincapplication.h"
//
#include "dnamespace.h"
#include "dproject.h"
#include "ddebug.h"
#include "../ui/mainwindow.h"
//
#include <QFile>
#include <QTimer>
#include <QTranslator>
#include <QLocale>
#include <QtSql/QSqlDatabase>
#include <QDataStream>
//
MyIncApplication* MyIncApplication::self = NULL;
//
#define TR QApplication::instance()->tr
//
MyIncApplication::MyIncApplication(int &argc, char** argv)
    : m_app(new QApplication(argc,argv)), m_namespace(NULL), m_project(NULL),
      m_debug(NULL), m_mainWindow(NULL), m_Debug(false)
{
    m_app->setApplicationName( "myIncDatabase" );
    m_app->setApplicationVersion( "0.1a" );
    m_app->setOrganizationDomain( "http://github.com/disable13/myIncDatabase" );
    m_app->setOrganizationName( "disable13" );
    m_app->setWindowIcon( QIcon( ":/icon/qrcode.png" ));
    Q_ASSERT_X(!self, "MyIncApplication",
               "there should be only one application object");
    MyIncApplication::self = this;

    QTranslator *myTranslator = new QTranslator();
    myTranslator->load(QLocale::system().name());
    m_app->installTranslator(myTranslator);

    m_namespace = new DNamespace();

    bool isCompress = false;
    // Check arguments
    if (argc > 1)
        for(int i = 0; i < argc; i++) {
            QString arg = QString(argv[i]);
            if (arg.startsWith("--")) {
                if (arg == QString("--help") ) { // give help
                    qDebug(qPrintable(TR("Usage: myinc [--drivers|--debug|--help] [Project]")));
                    qDebug(qPrintable(TR("Usage: myinc --compress [[Project1.xml] [[Project2.xml] [..]]\n")));
                    qDebug(qPrintable(TR("--drivers\tshow aviable SQL drivers")));
                    qDebug(qPrintable(TR("--help\tshow this message")));
                    qDebug(qPrintable(TR("--compress\tCompressing XML project")));
                    qDebug(qPrintable(TR("--debug\tAdding specialy menu for developing and testing projects")));
                    QTimer::singleShot( 50, m_app, SLOT(quit()) );
                    return;
                } else if (arg == "--drivers") { // show aviable SQL drivers
                    qDebug( qPrintable(TR("Aviable SQL drivers in system:\n")) );
                    QStringList drv = QSqlDatabase::drivers();
                    for(int i = 0; i < drv.count(); i++)
                        qDebug( qPrintable(QString("\t").append(drv.at(i))) );
                    QTimer::singleShot( 50, m_app, SLOT(quit()) );
                    return;
                } else if (arg=="--compress") {
                    isCompress = true;
                } else if (arg=="--debug") {
                    m_Debug = true;
                    if (!m_debug)
                        m_debug = new DDebug();
                }
            } else if (QFile::exists(arg) &&
                       (arg.endsWith(".xml") || arg.endsWith(".xml.flate")) ) { // is file ?
                if (isCompress) { // compressing
                    qDebug(qPrintable(TR("Compressing %1 file...").arg(arg)));
                    compressXmlProject(arg);
                } else { // opening
                    m_mainWindow = new MainWindow();
                    if (m_mainWindow->loadProject(arg)) {
                        m_mainWindow->lockUI( false );
                        m_mainWindow->connectDatabase();
                        if (!m_mainWindow->openManualWorkspace())
                            qDebug(qPrintable(TR("Please set manual form for project")));
                        else
                            m_mainWindow->close();

                    } else
                        QTimer::singleShot( 50, m_app, SLOT(quit()) );
                    return;
                }
            }
        }
    if (isCompress) {
        QTimer::singleShot( 50, m_app, SLOT(quit()) );
        return;
    }
    m_mainWindow =  new MainWindow();
}
//
MyIncApplication::~MyIncApplication()
{
    FREE_MEM(m_mainWindow);
    FREE_MEM(m_project);
    FREE_MEM(m_namespace);
    FREE_MEM(m_debug);

    QTimer::singleShot( 50, m_app, SLOT(quit()) );
}
//
bool MyIncApplication::openProject(const QString &fileName )
{
    Q_ASSERT_X(!m_project, "Project",
               "You don't able to open more then one project.");
    MyIncApplication::m_project = new DProject(fileName);
    QObject::connect( m_namespace, SIGNAL(error(int)),
                      m_project, SIGNAL(error(int)) );

    return m_namespace->initConfig();
}
//
bool MyIncApplication::closeProject()
{
    FREE_MEM(m_project);

    return true;
}
//
void MyIncApplication::compressXmlProject(const QString &fileName)
{
    QFile in(fileName);
    if (!in.open(QIODevice::ReadOnly)) {
        qDebug("Error: can't open project file");
    }
    QDataStream data( in.readAll() );
    //in.close();
    data.device()->seek(0);
    quint32 magic = 0x00;
    data >> magic;
    data.device()->seek(0);

    if (magic != 0x085a4950) {
        QByteArray buf = qCompress( data.device()->readAll() );
        QString sStuff = fileName;
        QFile out( sStuff.append(".flate") );
        if (!out.open(QIODevice::WriteOnly)) {
            qDebug("Error: can't write compressed file");
        }
        QDataStream dataOut(&out);
        dataOut << (quint32)0x085a4950
                << buf;
        dataOut.device()->close();
    } else {
        qDebug(qPrintable(TR("This file already compressed. Skipping...")));
    }
}
// TODO: finish this
void MyIncApplication::unCompressXmlProject(const QString &)
{
    qDebug("TODO: void MyIncApplication::unCompressXmlProject(QString)");
}
//
#undef TR
