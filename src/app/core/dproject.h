#ifndef DPROJECT_H
#define DPROJECT_H
//
#include <QObject>
//
#include <QStringList>
#include <QHash>
//
#include "../core.h"
//
#include "dauth.h"
//
class DXml;
//
class DProject : public QObject
{
    Q_OBJECT
public:
    DProject(const QString &fileName );
    virtual ~DProject();

private:
    DAuth auth;
    //
    DQ_PROPERTY_R (bool, New,   private) //bool isNew;
    DQ_PROPERTY_R (bool, Load,  private) //bool isLoad;
    DQ_PROPERTY_R (bool, Sql,   private) //bool isSql;
    //
    DQ_PROPERTY_R (QString, FilePath,   private) //QString filePath;
    DQ_PROPERTY_R (QString, DbDriver,   private) //QString dbDriver;
    DQ_PROPERTY_RW(QString, DbName,     private) //QString dbName;
    DQ_PROPERTY_RW(QString, DbUser,     private) //QString dbUser;
    DQ_PROPERTY_RW(QString, DbPassword, private) //QString dbPassord;
    DQ_PROPERTY_RW(QString, DbConnectOptions, private) //QString dbConnectOptions;
    DQ_PROPERTY_RW(QString, DbHost,     private) //QString dbHost;
    DQ_PROPERTY_RW(unsigned int, DbPort,private) //int     dbPort;
    DQ_PROPERTY_R (QStringList, DbTables, private)//QStringList dbTables;
    //
    DQ_PROPERTY_R (QString, LastError, protected) //QString lastError;
    //
    QHash<QString, QString> sel;
    QHash<QString, QString> ins;
    QHash<QString, QString> del;
    QHash<QString, QString> upd;
    QHash<QString, QString> other;

public:
    void save();
    bool load();
    bool loadSql(DXml*);
    bool authorized() const;
    bool getAuthorized(QWidget*);
    // propertes
    int     getSelectSqlQuertyCount() const;
    QString getSelectSqlQuerty(const QString &name) const;
    QString getSelectSqlQuerty(const int &pos) const;
    //
    int     getInsertSqlQuertyCount() const;
    QString getInsertSqlQuerty(const QString &name) const;
    QString getInsertSqlQuerty(const int &pos) const;
    //
    int     getDeleteSqlQuertyCount() const;
    QString getDeleteSqlQuerty(const QString &name) const;
    QString getDeleteSqlQuerty(const int &pos) const;
    //
    int     getUpdateSqlQuertyCount() const;
    QString getUpdateSqlQuerty(const QString &name) const;
    QString getUpdateSqlQuerty(const int &pos) const;
    //
    int     getOtherSqlQuertyCount() const;
    QString getOtherSqlQuerty(const QString &name) const;
    QString getOtherSqlQuerty(const int &pos) const;
    //
    bool setDbDriver(const QString &nameDriver);
    //
    bool connectDatabase();
    void disconnectDatabase();
    //
    QStringList workspace();
    //
    QString config(const QString&, const QString&) const;

signals:
    void error(int);

};
//
#endif // DPROJECT_H
