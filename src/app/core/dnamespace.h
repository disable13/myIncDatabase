#ifndef DNAMESPACE_H
#define DNAMESPACE_H
//
#include <QObject>
//
#include <QVariant>
//
class QDomDocument;
class QDomNode;
class DProject;
class QRegExp;
class QSqlQuery;
class DSystemFuncs;
//
class DNamespace : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_ENUMS(SqlType)

private:
    DSystemFuncs * sys;
    //
    QDomDocument * doc;
    bool isSql; // connected?
    bool isConfig; // open?
    QSqlQuery * query;
    //
    QDomNode * cfg;

public:
    DNamespace();
    ~DNamespace();
    //
    enum Type { Sql, System, Config };
    enum SqlType { SELECT, INSERT, UPDATE, DELETE, Other };
    //
    bool initConfig();
    bool initSql();
    //
    QString config(QString name, QString arrayElement );
    int configSize(QString name);
    void setConfig(QString name, QString value, QString arrayElement );
    //
    QSqlQuery sql(SqlType type, QString queryName, QStringList bindValue);
    //
    bool report(QString name, QStringList args);

public slots:
    void uri(QString,QVariant*);

signals:
    void error(int);

};
//
#endif // DNAMESPACE_H
