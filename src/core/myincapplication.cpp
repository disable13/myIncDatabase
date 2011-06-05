#include "myincapplication.h"
//
#include "src/core/dnamespace.h"
#include "src/core/dproject.h"
#include "src/ui/mainwindow.h"
//
#include <QFile>
#include <QTimer>
#include <QTranslator>
#include <QLocale>
#include <QtSql/QSqlDatabase>
#include <QDataStream>
//
MyIncApplication*   MyIncApplication::self          = 0x00;
QApplication*       MyIncApplication::m_app         = 0x00;
DNamespace*         MyIncApplication::m_namespace   = 0x00;
DProject*           MyIncApplication::m_project     = 0x00;
MainWindow*         MyIncApplication::m_mainWindow  = 0x00;
//
MyIncApplication::MyIncApplication(int &argc, char** argv) :
    QObject()
{
    MyIncApplication::m_app = new QApplication(argc,argv);
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
                    qDebug(qPrintable(tr("Usage: myinc [--drivers|--help] [Project]")));
                    qDebug(qPrintable(tr("Usage: myinc --compress [[Project1.xml] [[Project2.xml] [..]]\n")));
                    qDebug(qPrintable(tr("--drivers\tshow aviable SQL drivers")));
                    qDebug(qPrintable(tr("--help\tshow this message")));
                    qDebug(qPrintable(tr("--compress\tCompressing XML project")));
                    QTimer::singleShot( 50, m_app, SLOT(quit()) );
                    return;
                } else if (arg == "--drivers") { // show aviable SQL drivers
                    qDebug( qPrintable(tr("Aviable SQL drivers in system:\n")) );
                    QStringList drv = QSqlDatabase::drivers();
                    for(int i = 0; i < drv.count(); i++)
                        qDebug( qPrintable(QString("\t").append(drv.at(i))) );
                    QTimer::singleShot( 50, m_app, SLOT(quit()) );
                    return;
                } else if (arg=="--compress") {
                    isCompress = true;
                }
            } else if (QFile::exists(arg) &&
                       (arg.endsWith(".xml") || arg.endsWith(".xml.flate")) ) { // is file ?
                if (isCompress) { // compressing
                    qDebug(qPrintable(tr("Compressing %1 file...").arg(arg)));
                    compressXmlProject(arg);
                } else { // opening
                    m_mainWindow = new MainWindow();
                    if (m_mainWindow->loadProject(arg)) {
                        m_mainWindow->lockUI( false );
                        m_mainWindow->connectDatabase();
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
    delete m_mainWindow;
    delete m_project;
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

    return true;
}
//
bool MyIncApplication::closeProject()
{
    delete m_project;
    m_project = 0x00;

    return true;
}
//
void MyIncApplication::compressXmlProject(QString fileName)
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
        QFile out( fileName.append(".flate") );
        if (!out.open(QIODevice::WriteOnly)) {
            qDebug("Error: can't write compressed file");
        }
        QDataStream dataOut(&out);
        dataOut << (quint32)0x085a4950
                << buf;
        dataOut.device()->close();
    } else {
        qDebug(qPrintable(tr("This file already compressed. Skipping...")));
    }
}
// TODO: finish this
void MyIncApplication::unCompressXmlProject(QString)
{
    qDebug("TODO: void MyIncApplication::unCompressXmlProject(QString)");
}
