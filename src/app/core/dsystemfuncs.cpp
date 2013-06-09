#include "dsystemfuncs.h"
//
#include "myincapplication.h"
#include "../ui/dhomescreen.h"
#include "../ui/mainwindow.h"
#include "../ui/dworkwidget.h"
//
const QString g_sNull = "NULL";
const QVariant g_vErr = QVariant("Error");
const QVariant g_vSuc = QVariant("Success");
const QVariant g_vNull= QVariant(g_sNull);
//
#define MAKE_BOOL( b ) \
    (b) ? g_vSuc : g_vErr
//
DSystemFuncs::DSystemFuncs() :
    QObject()
{

}
//
DSystemFuncs::~DSystemFuncs()
{ }
/// Widgets
//
QWidget* DSystemFuncs::findWidget(const QString &name)
{
    QString p_sName = name.trimmed();
    if (p_sName.isEmpty())
        return NULL;
    p_sName = QString("ww_").append(name);
    QWidgetList lst = MIA_APP->allWidgets();
    foreach(QWidget *w, lst) {
        if (w->objectName() == p_sName )
            return w;
    }

    return NULL;
}
// like myinc:///System/Window/Close - kill "sender" window
//      myinc:///System/Window/Close#HelloWindow - kill window with name "HelloWindow"
bool DSystemFuncs::openWidget(const QString &widget)
{
    return MIA_GLOBAL->mainWindow()->getHome()->selectWorkspace( widget );
}
//
void DSystemFuncs::closeWidget(const QString &widget)
{
    QWidget* w = DSystemFuncs::findWidget( widget );
    if (w == NULL)
        return;
    w->close();
}
//
bool DSystemFuncs::refreshWidget(const QString &widget)
{
    QWidget* w = findWidget( widget );
    if (w == NULL)
        return false;
    if (!w->inherits( "DWorkWidget" ))
        return false;
    return static_cast<DWorkWidget*>(w)->refreshUri();
}
/// Variables
//
QString DSystemFuncs::getGlobalVariable(const QString &name) const
{
    QString p_sName = QString(name.trimmed()).toLower();
    QPair<QString,QString> item;
    foreach(item, var) {
        if(item.first == p_sName)
            return item.second;
    }

    return g_sNull;
}
//
void DSystemFuncs::setGlobalVariable(const QString &name, const QString &value)
{
    QString p_sName = QString(name.trimmed()).toLower();
    // check for exists
    for(int i = 0; i < var.size(); i++)
        if (var.at(i).first == p_sName) {
            var.removeAt(i);
        }
    var.append( QPair<QString,QString>( p_sName, value ) );
}
//
QString DSystemFuncs::getVariable(const QString &widget, const QString &name)
{
    QWidget* w = findWidget( widget );
    if (w == NULL)
        return g_sNull;
    return w->property( qPrintable(QString("mib_").append(name.trimmed())) ).toString();
}
//
void DSystemFuncs::setVariable(const QString &widget,
                               const QString &name,
                               const QString &value
                               )
{
    QWidget* w = findWidget( widget );
    if (w == NULL)
        return;
    w->setProperty( qPrintable(QString("mib_").append(name.trimmed())),
                    value );
}
//
QVariant DSystemFuncs::object(const QString &window,
                              const QString &control,
                              const QString &property
                              )
{
    QWidget* w = findWidget( window );
    if (w == NULL)
        return g_vNull;
    QObject *obj = w->findChild<QObject*>( control );
    if (obj == NULL)
        return g_vNull;

    return obj->property( qPrintable(property.trimmed()) );
}
//
void DSystemFuncs::setObject(const QString &window,
                             const QString &control,
                             const QString &property,
                             const QString &value
                             )
{
    QWidget* w = findWidget( window );
    if (w == NULL)
        return;
    QObject *obj = w->findChild<QObject*>( control );
    if (obj == NULL)
        return;

    obj->setProperty( qPrintable(property.trimmed()), value );
}
//  sample:  myinc:///System/Window/Close#HelloWindow
//                  func = Window/Close
//                  arg = HelloWindow
QVariant DSystemFuncs::run(const QString &func,
                           const QStringList &arg,
                           const QObject * nativeSender
                           )
{
    QStringList f = (func.toLower()).split('/', QString::SkipEmptyParts );
    if (f.count() <= 1)
        return g_vErr;
    const QString* tmp = &f.at(0);
    if ( *tmp == QString("window") ) { /// window funcs
        tmp = &f.at(1);
        if ( *tmp == QString("close") ) {
            DSystemFuncs::closeWidget( arg.count() != 0 ?
                        arg.at(0):
                        nativeSender->objectName() );
            return g_vSuc;
        } else if ( *tmp == QString("open")) {
            return MAKE_BOOL(DSystemFuncs::openWidget( arg.at(0) ));
        } else if ( *tmp == "object") {
            if ((arg.count() == 0) || (arg.count() > 4))
                return g_vErr;
            int c = arg.count();
            return DSystemFuncs::object( c==2?nativeSender->objectName():arg.at(0),
                                         arg.at(c - 2),
                                         arg.at(c - 1) );
        } else if ( *tmp == "setobject" ) {
            if ((arg.count() == 0) || (arg.count() > 4))
                return g_vErr;
            int c = arg.count();
            DSystemFuncs::setObject( c == 3 ? nativeSender->objectName() : arg.at(0),
                                     arg.at(1), arg.at(2), arg.at(3) );
            return g_vSuc;
        } else if ( *tmp == "refresh" ) {
            return MAKE_BOOL(refreshWidget(
                                 (arg.count() == 0)?
                                     nativeSender->objectName():
                                     arg.at(0)
                                     ));
        }
    } else if ( *tmp == QString("var") ) { // variable funcs
        tmp = &f.at(1);
        if (arg.count() < 1) {
            return g_vErr;
        }
        if ( *tmp == QString("global") ) { /// this is Global Variables
            if (f.count() != 3)
                return g_vErr;
            tmp = &f.at(2);
            if (*tmp == QString("get"))
                return getGlobalVariable( arg.at(0) );
            else if (*tmp == QString("set")) {
                if (arg.count() < 2 )
                    return g_vErr;
                setGlobalVariable( arg.at(0), arg.at(1) );
                return g_vSuc;
            }
        } else if ( *tmp == QString("local")) {
            if (f.count() != 3)
                return g_vErr;
            if (arg.count() < 2 )
                return g_vErr;
            tmp = &f.at(2);
            if (*tmp == QString("get"))
                return DSystemFuncs::getVariable( arg.at(0), arg.at(1) );
            else if (*tmp == QString("set")) {
                if (arg.count() < 3 )
                    return g_vErr;
                DSystemFuncs::setVariable( arg.at(0), arg.at(1), arg.at(2) );
                return g_vSuc;
            }
        }
    } else if ( *tmp == g_sNull.toLower() )
        return g_vNull;
    else if ( *tmp == "succses" )
        return g_vSuc;
    else if ( *tmp == "error" )
        return g_vErr;

    return g_vErr;
}
//
#undef MAKE_BOOL
