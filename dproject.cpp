#include "dproject.h"

#include <QStringList>
#include <QSettings>
#include <QFile>
#include <QDomDocument>

#ifndef NO_SQL
#include <QtSql/QSqlDatabase>
#endif

DProject::DProject(QString fileName)
{
    isNew = !QFile::exists(fileName);
    isLoad = !isLoad;
    //isSqlLoad = !isSqlLoad;

    set = new QSettings( fileName, QSettings::IniFormat );
}

DProject::~DProject()
{
    /// FIXME: close database connection;
    delete set;
}

bool DProject::save()
{
    set->setValue( "Database/Driver", dbDriver );
    set->setValue( "Database/Username", dbUser );
    set->setValue( "Database/Password", dbPassord );
    set->setValue( "Database/Options", dbConnectOptions );
    set->setValue( "Database/Hostname", dbHost );
    set->setValue( "Database/Driver", dbPort );
    set->setValue( "Database/Query File", dbSqlList );

    isNew = false;
    return true;
}

bool DProject::load()
{
    if (isNew)
        return false;

    dbDriver = set->value( "Database/Driver", QVariant("NULL") ).toString();
    if (dbDriver == "NULL")
        return false;
    dbUser = set->value( "Database/Username", QVariant("") ).toString();
    dbPassord = set->value( "Database/Password", QVariant("") ).toString();
    dbConnectOptions = set->value( "Database/Options", QVariant("") ).toString();
    dbHost = set->value( "Database/Hostname", QVariant("") ).toString();
    dbPort = set->value( "Database/Driver", QVariant(-1) ).toInt();
    dbSqlList = set->value( "Database/Query File", QVariant("") ).toString();

    return true;
}

bool DProject::loadSql()
{
    sel.clear();
    del.clear();
    ins.clear();
    other.clear();

    QDomDocument doc("SQL");
    QFile file( dbSqlList );
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    if (docElem.tagName() != "sql-list")
        return 0x01;
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if ( !e.isNull() && (e.tagName() == "sql")) {
            QDomNode nn = e.firstChild();
            while(!nn.isNull()) {
                QDomElement ee = nn.toElement();
                if ( !ee.isNull() && (ee.tagName() == "query")) {
                    QString type = n.toElement().attribute( "type", "null" ).toLower();

                    if (type == "select") {
                        sel.insert( ee.attribute( "name", "null"), ee.text() );
                    } else if (type == "insert") {
                        ins.insert( ee.attribute( "name", "null"), ee.text() );
                    } else if (type == "delete") {
                        del.insert( ee.attribute( "name", "null"), ee.text() );
                    } else if ( type == "update") {
                        upd.insert( ee.attribute( "name", "null"), ee.text() );
                    } else if ( type == "other") {
                        other.insert( ee.attribute( "name", "null"), ee.text() );
                    }
                    //qDebug() << "Tag name " << ee.tagName() << ". Type " << n.toElement().attribute( "type", "null" )
                    //        << ". Name: " << ee.attribute( "name", "null") << ". Text: " << ee.text();
                }
                nn = nn.nextSibling();
            }
        }
        n = n.nextSibling();
    }

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

QString DProject::getDbSqlListFile()
{
    return dbSqlList;
}

QString DProject::getSelectSqlQuerty(QString &name)
{
    return sel[name];
}

QString DProject::getInsertSqlQuerty(QString &name)
{
    return ins[name];
}

QString DProject::getDeleteSqlQuerty(QString &name)
{
    return del[name];
}

QString DProject::getOtherSqlQuerty(QString &name)
{
    return other[name];
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

void DProject::setDbSqlListFile(QString &filename)
{
    dbSqlList = filename;
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

void DProject::disconnectDatabase()
{

}

QStringList DProject::workTables()
{
#ifndef NO_SQL

#else
    return QStringList();
#endif
}
