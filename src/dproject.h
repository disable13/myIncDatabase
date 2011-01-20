#ifndef DPROJECT_H
#define DPROJECT_H

#include <QObject>

#include <QStringList>
#include <QHash>

class DNamespace;

class DProject : public QObject
{
    Q_OBJECT
private:
    DNamespace * nspace;

    bool isNew;
    bool isLoad;
    bool isSql;

    QString filePath;
    QString dbDriver;
    QString dbName;
    QString dbUser;
    QString dbPassord;
    QString dbConnectOptions;
    QString dbHost;
    int     dbPort;
    QStringList dbTables; // список рабочих таблиц
    // список полей для рабочих таблиц


    // FIXME:
    QHash<QString, QString> sel;
    QHash<QString, QString> ins;
    QHash<QString, QString> del;
    QHash<QString, QString> upd;
    QHash<QString, QString> other;

public:
    DProject( QString fileName );
    ~DProject();

    void save();
    bool load();
    bool loadSql();
    // propertes
    bool getIsNew();
    bool getIsLoad();
    bool getIsConnected();
    QString getDbDriver();
    QString getDbName();
    QString getDbUser();
    QString getDbPassord();
    QString getDbConnectOptions();
    QString getDbHost();
    int     getDbPort();
    QString getProjectFile();
    DNamespace * getNamespace();

    QString getSelectSqlQuerty( QString name);
    QString getInsertSqlQuerty( QString name);
    QString getDeleteSqlQuerty( QString name);
    QString getUpdateSqlQuerty( QString name);
    QString getOtherSqlQuerty( QString name);

    bool setDbDriver(QString nameDriver);
    void setDbName(QString datebaseName);
    void setDbUser(QString user);
    void setDbPassord(QString password);
    void setDbConnectOptions(QString connectOptions);
    void setDbHost(QString hostName);
    void setDbPort(int port);

    bool connectDatabase();
    void disconnectDatabase();

    QStringList workspace();

    QString config(QString, QString);

    int uri(QString);

signals:
    void error(int);

};

#endif // DPROJECT_H
