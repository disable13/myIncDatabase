#ifndef MYINCAPPLICATION_H
#define MYINCAPPLICATION_H

#include <QApplication>

class DProject;
class DNamespace;
class DThreadPool;

class MyIncApplication : public QObject
{
    Q_OBJECT
private:
    static MyIncApplication * self;
    static QApplication * m_app;

    static DThreadPool * m_pool;
    static DNamespace * m_namespace;
    static DProject * m_project;


public:
    MyIncApplication(int &argc, char** argv);
    virtual ~MyIncApplication();

    static bool openProject( QString fileName );

    static MyIncApplication * instance() { return self; }
    static QApplication * application() { return m_app; }
    static DNamespace * uriNamespace() { return m_namespace; }
    static DProject * project() { return m_project; }
    static DThreadPool * threadPool() { return m_pool; }

};

#endif // MYINCAPPLICATION_H
