#include "dproject.h"

#include <QStringList>
#include <QSettings>
#include <QFile>
#ifndef NO_SQL
#include <QtSql/QSqlDatabase>
#endif

DProject::DProject(QString fileName)
{
    isNew = !QFile::exists(fileName);
    isLoad = !isLoad;

    set = new QSettings( fileName, QSettings::IniFormat );
}

bool DProject::save()
{
    set->setValue( "Database/Driver", dbDriver );
    set->setValue( "Database/Username", dbUser );
    set->setValue( "Database/Password", dbPassord );
    set->setValue( "Database/Options", dbConnectOptions );
    set->setValue( "Database/Hostname", dbHost );
    set->setValue( "Database/Driver", dbPort );

    isNew = false;
    return true;
}

bool DProject::load()
{
    if (isNew)
        return false;
#ifndef NO_SQL
    dbDriver = set->value( "Database/Driver", QVariant("NULL") ).toString();
    if (dbDriver == "NULL")
        return false;
    dbUser = set->value( "Database/Username", QVariant("") ).toString();
    dbPassord = set->value( "Database/Password", QVariant("") ).toString();
    dbConnectOptions = set->value( "Database/Options", QVariant("") ).toString();
    dbHost = set->value( "Database/Hostname", QVariant("") ).toString();
    dbPort = set->value( "Database/Driver", QVariant(-1) ).toInt();
#endif
    return true;
}

bool DProject::getIsNew()
{
    return isNew;
}

bool DProject::getIsLoad()
{
    return isLoad;
}

QString DProject::getDbDriver()
{
    return dbDriver;
}

QString DProject::getDbUser()
{
    return dbUser;
}

QString DProject::getDbPassord()
{
    return dbPassord;
}

QString DProject::getDbConnectOptions()
{
    return dbConnectOptions;
}

QString DProject::getDbHost()
{
    return dbHost;
}

QString DProject::getDbName()
{
    return dbName;
}

int DProject::getDdbPort()
{
    return dbPort;
}

bool DProject::setDbDriver(QString & nameDriver)
{
    nameDriver = nameDriver.trimmed().toUpper() ;
    if (nameDriver == "")
        return false;
#ifndef NO_SQL
    int n = QSqlDatabase::drivers().count();
    for (int i = 0; i < n; i++)
        if (nameDriver == QSqlDatabase::drivers().at(i) ) {
            dbDriver = nameDriver;
            return true;
    }
#endif
    return false;
}

void DProject::setDbName(QString &datebaseName)
{
    dbName = datebaseName;
}

void DProject::setDbUser(QString & user)
{
    dbUser = user;
}

void DProject::setDbPassord(QString & password)
{
    dbPassord = password;
}

void DProject::setDbConnectOptions(QString & connectOptions)
{
    dbConnectOptions = connectOptions;
}

void DProject::setDbHost(QString & hostName)
{
    dbHost = hostName;
}

void DProject::setDdbPort(int & port)
{
    dbPort = port;
}

bool DProject::connectDatabase()
{
    if ( !isNew || !isLoad)
        return false;
#ifndef NO_SQL
    QSqlDatabase db = QSqlDatabase::addDatabase( dbDriver );
    db.setHostName( dbHost );
    db.setDatabaseName( dbName );
    db.setUserName( dbUser );
    db.setPassword( dbPassord );
    db.setPort( dbPort );
    db.setConnectOptions( dbConnectOptions );
    return db.open();
#else
    return false;
#endif
}
