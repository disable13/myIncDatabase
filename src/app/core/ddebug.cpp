#include "ddebug.h"
//
#include "myincapplication.h"
//
DDebug::DDebug() :
    QObject(),p_maximum(200)
{
    // Can't create without '--debug' param
    Q_ASSERT( MyIncApplication::isDebug() );

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
        delete p_uri.back();
        p_uri.removeLast();
    }
}
//
DUriQuery* DDebug::getUri(int i)
{
    if (i < p_uri.count())
        return  p_uri.at(i);
    return NULL;
}
//
int DDebug::getUriCount()
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

    if (p_maximum > p_uri.count()) {
        delete p_uri.last();
        p_uri.removeLast();
    }
}
//
void DDebug::debug(QString uri, QVariant result, QObject* sender)
{
    DUriQuery q;
    q.uri = uri;
    q.result = result;
    q.sender = sender;
    q.time = QDateTime::currentDateTime();
    debug( q );
}
