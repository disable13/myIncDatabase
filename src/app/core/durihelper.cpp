#include "durihelper.h"
//
#include "myincapplication.h"
#include "dnamespace.h"
//
#include <QtCore/QRegExp>
#include <QtSql/QSqlQuery>
#include <QStringList>
//
QString DUriHelper::uriMask =
QString("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?");
//
DUriHelper::DUriHelper(QString  query) :
    QObject()
{
    rx = new QRegExp( uriMask );
    rx->indexIn( query );

    connect( this, SIGNAL(uriArgument(QString,QVariant*)),
             MIA_NAMESPACE, SLOT(uri(QString,QVariant*)) );

    p_path = 0x00;
    p_args = 0x00;
}
//
DUriHelper::~DUriHelper()
{
    delete rx;
    if (p_path == 0x00)
        delete p_path;
    if (p_args == 0x00)
        delete p_args;
}
//
void DUriHelper::initPath()
{
    p_path = new QStringList(
                rx->cap(5).split('/', QString::SkipEmptyParts) );
}
// TODO: see Global TODO (1).
void DUriHelper::initArg()
{
    QStringList arg(
                rx->cap(9).split('&', QString::SkipEmptyParts) );

    p_args = new QStringList();
    QString buf = "";
    int open = 0;
    for(int i = 0; i < arg.count(); i++) {
        QString tmp = arg.at(i).trimmed();
        for( int ii = 0; ii < tmp.length(); ii++) // get count of '{}'
            if ( tmp.at(ii) == '{' )
                open++;
            else if ( tmp.at(ii) == '}')
                open--;
        if (buf.size() != 0)
            tmp = buf.append("&").append(tmp);
        if (open == 0) {
            if (tmp.startsWith('{')) {
                tmp.remove(0, 1);
                tmp.resize(tmp.size() - 1);
            }
            DUriHelper uri(tmp);
            if (uri.isUri())
                if (uri.protocol() == "myinc") {

                    uri.pathItemsCount();
                    uri.args();
                    QVariant v;
                    emit uriArgument(tmp, &v);
agais:
                    switch (static_cast<QMetaType::Type>(v.type())) {
                    case QMetaType::QString:
                        tmp = v.toString();
                        break;
                    case QMetaType::VoidStar: {
                        QSqlQuery *q = ((QSqlQuery*)v.value<void*>());
                        if (!q->first())
                            q->next();
                        v = q->value(0);
                        goto agais;
                    }
                    default:
                        if (!v.convert(QVariant::String))
                            tmp = "Error";
                        else
                            tmp = v.toString();
                    }
                }
        } else {
            buf = tmp;
            continue;
        }
        buf = "";
        p_args->append( tmp ); // add in args
    }
    if (open != 0){ // check for too more '{}'
        if (p_args == 0x00)
            delete p_args;
        p_args = new QStringList( "Error" );
        return;
    }
}
// TODO: fix for 4.5.x
bool DUriHelper::isUri()
{
    return (rx->captureCount() != -1);
}
//
QString DUriHelper::protocol()
{
    if (!isUri())
        return QString();
    return rx->cap(2).trimmed().toLower();
}
//
QString DUriHelper::host()
{
    if (!isUri())
        return QString();
    return rx->cap(4).trimmed().toLower();
}
//
QString DUriHelper::path()
{
    if (!isUri())
        return QString();

    return rx->cap(5).trimmed();
}
//
unsigned int DUriHelper::pathItemsCount()
{
    if (!isUri())
        return 0x00;
    if (p_path == 0x00)
        initPath();

    return p_path->count();
}
//
QString DUriHelper::path(int i)
{
    if (!isUri())
        return QString();
    if (p_path == 0x00)
        initPath();

    return p_path->at(i);
}
//
QStringList DUriHelper::args()
{
    if (!isUri())
        return QStringList();
    if (p_args == 0x00)
        initArg();

    return p_args[0];
}
//
QString DUriHelper::arg(int i)
{
    if (!isUri())
        return QString();
    if (p_args != 0x00)
        initArg();

    return p_args->at(i);
}
//
unsigned int DUriHelper::argCount()
{
    if (!isUri())
        return 0x00;
    if (p_args == 0x00)
        initArg();

    return p_args->count();
}
