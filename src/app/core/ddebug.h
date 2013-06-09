#ifndef DDEBUG_H
#define DDEBUG_H
//
#include <QObject>
//
#include <QDateTime>
#include <QList>
#include <QVariant>
//
#include "../core.h"
//
struct DUriQuery {
    QString uri;
    QVariant result;
    QObject* sender;
    QDateTime time;
};
//
class DDebug : public QObject
{
    Q_OBJECT
public:
    DDebug();
    virtual ~DDebug();

protected:
    QList<DUriQuery*> p_uri;
    DQ_PROPERTY_RW(int, Max, private)

public:
    const DUriQuery* getUri(int) const;
    int getUriCount() const;

signals:
    void uriEvent( DUriQuery );
    void uriError( DUriQuery );
    void uriNull( DUriQuery );
    void uriSuccessful( DUriQuery );

public slots:
    void debug(DUriQuery uri);
    void debug(QString uri, QVariant result, QObject * sender);

};
//
#endif // DDEBUG_H
