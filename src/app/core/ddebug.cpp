#include "ddebug.h"
//
#include "myincapplication.h"
//
DDebug::DDebug() :
    QObject(), m_Max(200)
{
    // Can't create without '--debug' param
    Q_ASSERT( MIA_GLOBAL->getDebug() );

    setObjectName( "Debug" );

    connect( this, SIGNAL(uriError(DUriQuery)),
             this, SIGNAL(uriEvent(DUriQuery)) );
    connect( this, SIGNAL(uriNull(DUriQuery)),
             this, SIGNAL(uriEvent(DUriQuery)) );
    connect( this, SIGNAL(uriSuccessful(DUriQuery)),
             this, SIGNAL(uriEvent(DUriQuery)) );
}
//
DDebug::~DDebug()
{
    while (!p_uri.isEmpty()) {
        FREE_MEM(p_uri.back());
        p_uri.removeLast();
    }
}
//
const DUriQuery *DDebug::getUri(int i) const
{
    if (i < p_uri.count())
        return  p_uri.at(i);
    return NULL;
}
//
int DDebug::getUriCount() const
{
    return p_uri.count();
}
//
void DDebug::debug(DUriQuery uri)
{
    uri.time = QDateTime::currentDateTime();
    p_uri.append( new DUriQuery(uri) );

    if (uri.result.toString() == "Error")
        emit uriError( uri );
    else if (uri.result.toString() == "NULL")
        emit uriNull( uri );
    else
        emit uriSuccessful( uri );

    if (m_Max > p_uri.count()) {
        FREE_MEM(p_uri.last());
        p_uri.removeLast();
    }
}
//
void DDebug::debug(QString uri, QVariant result, QObject* sender)
{
    DUriQuery q = { uri, result, sender, QDateTime::currentDateTime() };
    debug( q );
}
