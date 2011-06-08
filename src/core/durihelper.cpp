#include "durihelper.h"
//
#include <QtCore/QRegExp>
//
#include <QStringList>
#include <QDebug>
//
QString DUriHelper::uriMask =
QString("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?");
//
DUriHelper::DUriHelper(QString  query) :
    QObject()
{
    rx = new QRegExp( uriMask );
    rx->indexIn( query );

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
    qDebug("TODO: void DUriHelper::initArg()");
    p_args = new QStringList(
                rx->cap(9).split('&', QString::SkipEmptyParts) );
}
//
bool DUriHelper::isUri()
{
    return (rx->captureCount() != 0);
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
