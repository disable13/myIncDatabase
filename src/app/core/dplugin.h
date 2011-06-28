#ifndef DPLUGIN_H
#define DPLUGIN_H
//
#include <QObject>
#include <QList>
#include <QPair>
//
class QLibrary;
//
extern "C" {
/**/
struct MIDPLUG {
    char type;
    char ver[3];
    char sys_name[8];
};
/**/
}
//
typedef MIDPLUG* (MIDINIT)();
typedef int (MIDMAIN)(void*);
//
class DPlugin
{
public:
    DPlugin();
    ~DPlugin();

private:
    QString libPath;
    QList< QPair<MIDPLUG,QLibrary*> > libs;

protected:

public:
    bool load();
    bool load(QString);

};
//
#endif // DPLUGIN_H
