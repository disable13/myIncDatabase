#include "dplugin.h"
//
#include <QLibrary>
#include <QDir>
//
DPlugin::DPlugin()
{
    libPath = QString("../lib/myinc");
}
//
DPlugin::~DPlugin()
{

}
//
bool DPlugin::load()
{
    QDir dir(QDir::toNativeSeparators(libPath));
    QStringList list = dir.entryList();
    bool check = true;
    for(int i = 0; i < list.count(); i++)
        check = check && load(list.at(i));
    return check;
}
//
bool DPlugin::load(QString lib)
{
    if (!QLibrary::isLibrary(lib))
        return false;
    QLibrary * plug = new QLibrary( lib );
    if (!plug->load())
        return false;
    MIDINIT *proc = (MIDINIT*)(plug->resolve( "midInit" ));
    if ((proc == (MIDINIT*)0x00) || (proc == (MIDINIT*)-1))
        return false;
    try {
        MIDPLUG *data = proc();
        QPair<MIDPLUG,QLibrary*> addin(data[0],plug);
        libs.push_back(addin);
    } catch (...) {
        return false;
    }


    return true;
}
