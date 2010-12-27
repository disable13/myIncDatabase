#ifndef DPROJECT_H
#define DPROJECT_H

#include <QObject>

#include <QStringList>
#include <QHash>

class DNamespace;
class QSettings;

class DProject : public QObject
{
    Q_OBJECT
private:
    QSettings * set;
    DNamespace * nspace;

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
    QString dbUi;
    QStringList dbTables; // список рабочих таблиц
    // список полей для рабочих таблиц

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
    QString getUiFile();

    QString getSelectSqlQuerty( QString & name);
    QString getInsertSqlQuerty( QString & name);
    QString getDeleteSqlQuerty( QString & name);
    QString getUpdateSqlQuerty( QString & name);
    QString getOtherSqlQuerty( QString & name);


    bool setDbDriver(QString & nameDriver);
    void setDbName(QString & datebaseName);
    void setDbUser(QString & user);
    void setDbPassord(QString & password);
    void setDbConnectOptions(QString & connectOptions);
    void setDbHost(QString & hostName);
    void setDdbPort(int & port);
    void setDbSqlListFile( QString & filename );
    void setUiListFile( QString & filename );

    bool connectDatabase();
    void disconnectDatabase();

    QStringList workTables();


signals:
    void error();

public slots:

};

#endif // DPROJECT_H
