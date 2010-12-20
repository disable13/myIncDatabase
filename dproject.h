#ifndef DPROJECT_H
#define DPROJECT_H

#include <QObject>

#include <QHash>

class QSettings;

class DProject : public QObject
{
    Q_OBJECT
private:
    QSettings * set;
    bool isNew;
    bool isLoad;
    //bool isSqlLoad;

    QString dbDriver;
    QString dbName;
    QString dbUser;
    QString dbPassord;
    QString dbConnectOptions;
    QString dbHost;
    int     dbPort;
    QString dbSqlList;

    QHash<QString, QString> sel;
    QHash<QString, QString> ins;
    QHash<QString, QString> del;
    QHash<QString, QString> upd;
    QHash<QString, QString> other;

public:
    DProject( QString fileName );
    ~DProject();

    bool save();
    bool load();
    bool loadSql();
    // propertes
    bool getIsNew();
    bool getIsLoad();
    QString getDbDriver();
    QString getDbName();
    QString getDbUser();
    QString getDbPassord();
    QString getDbConnectOptions();
    QString getDbHost();
    int     getDdbPort();
    QString getDbSqlListFile();

    QString getSelectSqlQuerty( QString & name);
    QString getInsertSqlQuerty( QString & name);
    QString getDeleteSqlQuerty( QString & name);
    QString getOtherSqlQuerty( QString & name);


    bool setDbDriver(QString & nameDriver);
    void setDbName(QString & datebaseName);
    void setDbUser(QString & user);
    void setDbPassord(QString & password);
    void setDbConnectOptions(QString & connectOptions);
    void setDbHost(QString & hostName);
    void setDdbPort(int & port);
    void setDbSqlListFile( QString & filename );

    bool connectDatabase();
    void disconnectDatabase();

    QStringList workTables();


signals:

public slots:

};

#endif // DPROJECT_H
