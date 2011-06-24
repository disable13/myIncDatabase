#ifndef DDEBUG_H
#define DDEBUG_H
//
#include <QObject>
//
#include <QDateTime>
#include <QList>
#include <QVariant>
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
    int p_maximum;

public:
    DUriQuery* getUri(int);
    int getUriCount();
    int maximum() { return p_maximum; }

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
