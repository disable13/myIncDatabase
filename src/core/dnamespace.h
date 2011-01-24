#ifndef DNAMESPACE_H
#define DNAMESPACE_H

#include <QObject>

#include <QVariant>
class QDomDocument;
class QDomNode;
class DProject;
class QRegExp;
class QSqlQuery;
class QSqlResult;

class DNamespace : public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)

private:
    QDomDocument * doc;
    bool isSql; // connected?
    bool isConfig; // open?
    QRegExp * rx; // uri
    QSqlQuery * query;

    QDomNode * cfg;

public:
    DNamespace();
    ~DNamespace();

    enum Type { Sql, System, Config };

    bool initConfig();
    bool initSql();

    void saveXml();

    QString config(QString name, QString arrayElement );
    void setConfig(QString name, QString value, QString arrayElement );

    const QSqlResult * sql(QString queryName, QList<QVariant> bindValue);

public slots:
    void uri(QString,QVariant*);

signals:
    void error(int);

};

#endif // DNAMESPACE_H
