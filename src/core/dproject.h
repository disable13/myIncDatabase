#ifndef DPROJECT_H
#define DPROJECT_H
//
#include <QObject>
//
#include <QStringList>
#include <QHash>
#include <QtXml/QDomElement>
#include <QDataStream>
//
class DProject : public QObject
{
    Q_OBJECT
private:
    bool isNew;
    bool isLoad;
    bool isSql;
    //
    QDataStream * data;
    //
    QString filePath;
    QString dbDriver;
    QString dbName;
    QString dbUser;
    QString dbPassord;
    QString dbConnectOptions;
    QString dbHost;
    int     dbPort;
    QStringList dbTables;
    //
    QString lastError;
    //
    QHash<QString, QString> sel;
    QHash<QString, QString> ins;
    QHash<QString, QString> del;
    QHash<QString, QString> upd;
    QHash<QString, QString> other;

public:
    DProject( QString fileName );
    ~DProject();
    //
    void save();
    bool load();
    bool loadSql(QDomElement);
    // propertes
    bool getIsNew();
    bool getIsLoad();
    bool getIsConnected();
    QString getLastError();
    QDataStream * getData() { return data; }
    QString getDbDriver();
    QString getDbName();
    QString getDbUser();
    QString getDbPassord();
    QString getDbConnectOptions();
    QString getDbHost();
    int     getDbPort();
    QString getProjectFile();
    //
    int     getSelectSqlQuertyCount();
    QString getSelectSqlQuerty( QString name );
    QString getSelectSqlQuerty( int pos );
    //
    int     getInsertSqlQuertyCount();
    QString getInsertSqlQuerty( QString name );
    QString getInsertSqlQuerty( int pos );
    //
    int     getDeleteSqlQuertyCount();
    QString getDeleteSqlQuerty( QString name );
    QString getDeleteSqlQuerty( int pos );
    //
    int     getUpdateSqlQuertyCount();
    QString getUpdateSqlQuerty( QString name );
    QString getUpdateSqlQuerty( int pos );
    //
    int     getOtherSqlQuertyCount();
    QString getOtherSqlQuerty( QString name );
    QString getOtherSqlQuerty( int pos );
    //
    bool setDbDriver(QString nameDriver);
    void setDbName(QString datebaseName);
    void setDbUser(QString user);
    void setDbPassord(QString password);
    void setDbConnectOptions(QString connectOptions);
    void setDbHost(QString hostName);
    void setDbPort(int port);
    //
    bool connectDatabase();
    void disconnectDatabase();
    //
    QStringList workspace();
    //
    QString config(QString, QString);

signals:
    void error(int);

};
//
#endif // DPROJECT_H
