#ifndef DNAMESPACE_H
#define DNAMESPACE_H
//
#include <QObject>
//
#include <QVariant>
//
class DXml;
class QSqlQuery;
class DSystemFuncs;
//
class DNamespace : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_ENUMS(SqlType)

public:
    DNamespace();
    virtual ~DNamespace();

private:
    DSystemFuncs * sys;
    //
    DXml* doc;
    QSqlQuery * query;

public:
    enum Type   { T_Sql, T_System, T_Config };
    enum SqlType{ ST_SELECT, ST_INSERT, ST_UPDATE, ST_DELETE, ST_Other };
    //
    bool initConfig();
    bool initSql();
    //
    QString config(const QString &name,
                   const QString &arrayElement = QString()
            ) const;
    quint32 configSize(const QString &name) const;
    void    setConfig(const QString &name, const QString &value,
                      const QString &arrayElement = QString() );
    //
    QSqlQuery sql(SqlType type, QString queryName, QStringList bindValue);
    //
    bool report(const QString &name, const QStringList &args) const;

public slots:
    void uri(QString,QVariant*);

signals:
    void error(int) const;

};
//
#endif // DNAMESPACE_H
