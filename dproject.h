#ifndef DPROJECT_H
#define DPROJECT_H

#include <QObject>
class QSettings;

class DProject : public QObject
{
    Q_OBJECT
private:
    QSettings * set;
    bool isNew;
    bool isLoad;

    QString dbDriver;
    QString dbName;
    QString dbUser;
    QString dbPassord;
    QString dbConnectOptions;
    QString dbHost;
    int     dbPort;

public:
    DProject( QString fileName );

    bool save();
    bool load();
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

    bool setDbDriver(QString & nameDriver);
    void setDbName(QString & datebaseName);
    void setDbUser(QString & user);
    void setDbPassord(QString & password);
    void setDbConnectOptions(QString & connectOptions);
    void setDbHost(QString & hostName);
    void setDdbPort(int & port);

    bool connectDatabase();


signals:

public slots:

};

#endif // DPROJECT_H
