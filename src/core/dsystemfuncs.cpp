#include "dsystemfuncs.h"
//
#include "core/myincapplication.h"
#include "ui/dhomescreen.h"
#include "ui/mainwindow.h"
#include "ui/dworkwidget.h"
//
DSystemFuncs::DSystemFuncs() :
    QObject()
{

}
//
DSystemFuncs::~DSystemFuncs()
{

}
/// Widgets
//
QWidget* DSystemFuncs::findWidget(QString name)
{
    QWidgetList lst = MyIncApplication::application()->allWidgets();
    for(int i = 0; i < lst.count(); i++)
        if (lst.at(i)->objectName() == name )
            return lst.at(i);
    return 0x00;
}
// like myinc:///System/Window/Close - kill "sender" window
//      myinc:///System/Window/Close#HelloWindow - kill window with name "HelloWindow"
bool DSystemFuncs::openWidget(QString widget)
{
    return MyIncApplication::mainWindow()->getHome()->selectWorkspace( widget );
}
//
void DSystemFuncs::closeWidget(QString widget)
{
    QWidget* w = findWidget( widget );
    if (w == 0x00)
        return;
    w->close();
}
/// Variables
//
QString DSystemFuncs::getGlobalVariable(QString name)
{
    name = name.toLower();
    for(int i = 0; i < var.size(); i++)
        if (var.at(i).first == name)
            return var.at(i).second;

    return QString("");
}
//
void DSystemFuncs::setGlobalVariable(QString name, QString value)
{
    // check for exists
    for(int i = 0; i < var.size(); i++)
        if (var.at(i).first == name) {
            var.removeAt(i);
        }
    var.append( QPair<QString,QString>( name, value ) );
}
//
QString DSystemFuncs::getVariable(QString widget, QString name)
{
    QWidget* w = findWidget( widget );
    if (w == 0x00)
        return QString::null;
    return w->property( qPrintable(QString("mib_").append(name)) ).toString();
}
//
void DSystemFuncs::setVariable(QString widget, QString name, QString value)
{
    QWidget* w = findWidget( widget );
    if (w == 0x00)
        return;
    w->setProperty( qPrintable(QString("mib_").append(name)),
                   value );
}
//
QVariant DSystemFuncs::object(QString window, QString control, QString property)
{
    QWidget* w = findWidget( window );
    if (w == 0x00)
        return QVariant("NULL");
    QObject *obj = w->findChild<QObject*>( control );
    if (obj == 0x00)
        return QVariant("NULL");

    return obj->property( qPrintable(property) );
}
//
void DSystemFuncs::setObject(QString window, QString control,
                             QString property, QString value)
{
    QWidget* w = findWidget( window );
    if (w == 0x00)
        return;
    QObject *obj = w->findChild<QObject*>( control );
    if (obj == 0x00)
        return;

     obj->setProperty( qPrintable(property), value );
}
//  sample:  myinc:///System/Window/Close#HelloWindow
//                  func = Window/Close
//                  arg = HelloWindow
QVariant DSystemFuncs::run(QString func, QStringList arg, QObject * nativeSender )
{
    func = func .toLower();
    QStringList f = func.split('/', QString::SkipEmptyParts );
    if (f.count() <= 1)
        return QVariant("Error");
    QString tmp = f.at(0).toLower();
    if ( tmp == QString("window") ) { /// window funcs
        tmp = f.at(1).toLower();
        if ( tmp == QString("close") ) {
            if (arg.count() == 0)
                arg.append( nativeSender->objectName() );
            closeWidget( arg.at(0) );
        } else if (tmp == QString("open")) {
            openWidget( arg.at(0) );
        } else if ( tmp == "object") {
            if ((arg.count() == 0) || (arg.count() > 4))
                return QVariant("Error");
            if (arg.count() == 2)
                arg.insert(0, nativeSender->objectName() );
            return object( arg.at(0), arg.at(1), arg.at(2) );
        } else if ( tmp == "setobject" ) {
            if ((arg.count() == 0) || (arg.count() > 4))
                return QVariant("Error");
            if (arg.count() == 3)
                arg.insert(0, nativeSender->objectName() );
            setObject( arg.at(0), arg.at(1), arg.at(2), arg.at(3) );
            return QVariant("Succses");
        }
    } else if ( tmp == QString("var") ) { // variable funcs
        tmp = f.at(1).toLower();
        if (arg.count() < 1) {
            return QVariant("Error");
        }
        if (tmp == QString("global") ) { /// this is Global Variables
            if (f.count() != 3)
                return QVariant("Error");
            tmp = f.at(2).toLower();
            if (tmp == QString("get"))
                return getGlobalVariable( arg.at(0) );
            else if (tmp == QString("set")) {
                if (arg.count() < 2 )
                    return QVariant("Error");
                setGlobalVariable( arg.at(0),arg.at(1) );
                return QVariant("Succses");
            }
        }
    } else if ( tmp == "null" )
        return QVariant("NULL");
    else if ( tmp == "succses" )
        return QVariant("Succses");
    else if ( tmp == "error" )
        return QVariant("Error");

    return QVariant("Error");
}
