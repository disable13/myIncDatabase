#ifndef MYINCAPPLICATION_H
#define MYINCAPPLICATION_H
//
#include <QApplication>
//
#include "../core.h"
//
class DProject;
class DNamespace;
class MainWindow;
class DDebug;
//
class MyIncApplication
{
public:
    MyIncApplication(int &argc, char** argv);
    virtual ~MyIncApplication();

private:
    static MyIncApplication *   self;
    QApplication* m_app;
    //
    DNamespace* m_namespace;
    DProject*   m_project;
    DDebug*     m_debug;
    // UI
    MainWindow* m_mainWindow;
    // global settings
    D_PROPERTY_RW(bool, Debug, private)

public:
    bool openProject( const QString &fileName );
    bool closeProject();
    //
    inline static MyIncApplication* instance()
    { Q_ASSERT(self); return self; }
    inline QApplication * application() const
    { return m_app; }
    inline DNamespace * uriNamespace() const
    { return m_namespace; }
    inline DProject * project() const
    { return m_project; }
    inline DDebug * debug() const
    { return m_debug; }
    inline MainWindow * mainWindow() const
    { return m_mainWindow; }
    //
    void compressXmlProject(const QString &fileName);
    void unCompressXmlProject(const QString &fileName);

};
//
#define MIA_GLOBAL      MyIncApplication::instance()
//
#define MIA_PROJECT     MIA_GLOBAL->project()
#define MIA_NAMESPACE   MIA_GLOBAL->uriNamespace()
#define MIA_APP         MIA_GLOBAL->application()
#define MIA_DEBUG       MIA_GLOBAL->debug()
//
#define MIA_FOCUS       MIA_APP->focusWidget()
//
#endif // MYINCAPPLICATION_H
