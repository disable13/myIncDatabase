#ifndef DNAMESPACE_H
#define DNAMESPACE_H

#include <QObject>

#include <QVariant>
class QDomDocument;
class QDomNode;
class DProject;

class DNamespace : public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)

private:
    QDomDocument * doc;
    DProject * parent;
    bool isSql; // connected?
    bool isConfig; // open?

    QDomNode * cfg;

public:
    DNamespace(DProject * parent);
    ~DNamespace();

    enum Type { Sql, System, Config };

    bool initConfig();
    QString config(QString name, QString arrayElement );
    void setConfig(QString name, QString value, QString arrayElement );

public slots:
    void function(Type,QString,QVariant*);

signals:
    void error(int);

};

#endif // DNAMESPACE_H
