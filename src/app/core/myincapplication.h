#ifndef MYINCAPPLICATION_H
#define MYINCAPPLICATION_H
//
#include <QApplication>
//
class DProject;
class DNamespace;
class MainWindow;
class DDebug;
//
class MyIncApplication
{
private:
    static MyIncApplication * self;
    static QApplication * m_app;
    //
    static DNamespace * m_namespace;
    static DProject * m_project;
    static DDebug * m_debug;
    // UI
    static MainWindow * m_mainWindow;
    // global settings
    static bool m_isDebug;

public:
    MyIncApplication(int &argc, char** argv);
    virtual ~MyIncApplication();
    //
    static bool openProject( QString fileName );
    static bool closeProject();
    //
    static MyIncApplication * instance() { return self; }
    static QApplication * application() { return m_app; }
    static DNamespace * uriNamespace() { return m_namespace; }
    static DProject * project() { return m_project; }
    static DDebug * debug() { return m_debug; }
    static MainWindow * mainWindow() { return m_mainWindow; }
    //
    static void compressXmlProject(QString fileName);
    static void unCompressXmlProject(QString fileName);
    // Settings
    static bool isDebug() { return m_isDebug; }

};
//
#define MIA_FOCUS MyIncApplication::application()->focusWidget()
#define MIA_PROJECT MyIncApplication::project()
#define MIA_NAMESPACE MyIncApplication::uriNamespace()
#define MIA_APP MyIncApplication::application()
#define MIA_DEBUG MyIncApplication::debug()
//
#endif // MYINCAPPLICATION_H
