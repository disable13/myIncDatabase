#include "dnamespace.h"
#include "dproject.h"

#include <QFile>
#include <QtXml/QDomDocument>
#include <QtSql/QSqlDatabase>

DProject::DProject(QString fileName)
{
    //nspace = new DNamespace( this );
    isNew = !QFile::exists(fileName);
    isLoad = !isLoad;
    filePath = fileName;

    nspace = new DNamespace( this );

    connect( nspace, SIGNAL(error(int)), this, SIGNAL(error(int)) );
}

DProject::~DProject()
{
    /// FIXME: close database connection;
    qDebug( "FIXME: DProject::~DProject()" );
    disconnectDatabase();
    delete nspace;
}

bool DProject::save()
{
    nspace->setConfig( "Database", dbDriver, "Driver" );
    nspace->setConfig( "Database", dbUser, "Username" );
    nspace->setConfig( "Database", dbPassord, "Password" );
    nspace->setConfig( "Database", dbConnectOptions, "Options" );
    nspace->setConfig( "Database", dbHost, "Hostname" );
    nspace->setConfig( "Database", dbName, "Name" );
    nspace->setConfig( "Database", QString( dbPort ), "Driver" );

    isNew = false;
    return true;
}

bool DProject::load()
{
    isLoad = false;
    if (isNew)
        return false;
    nspace->initConfig();

    dbDriver = QString("Q").append( nspace->config( "Database", "Driver" ).toUpper() );
    if ((dbDriver == "QNULL") || (dbDriver == "QERORR"))
        return false;
    dbUser = nspace->config( "Database", "Username" );
    dbName = nspace->config( "Database", "Name" );
    dbPassord = nspace->config( "Database", "Password" );
    dbConnectOptions = nspace->config( "Database", "Options" );
    dbHost = nspace->config( "Database", "Hostname" );
    dbPort = nspace->config( "Database", "Port" ).toInt();

    isLoad = true;

    return true;
}

bool DProject::loadSql()
{
    /// FIXME
    qDebug("FIXME: DProject::loadSql()");
    /*
    sel.clear();
    del.clear();
    upd.clear();
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

    if (docElem.tagName().toLower() != "sql-list")
        return 0x01;
    for ( QDomNode n = docElem.firstChildElement( "sql" ); !n.isNull(); n = n.nextSiblingElement("sql") ) {
        QDomElement e = n.toElement();
        if ( !e.isNull() ) {
            QString type = n.toElement().attribute( "type", "null" ).toLower();
            for ( QDomNode nn = e.firstChild(); !nn.isNull(); nn = nn.nextSiblingElement( "query" ) ) {
                QDomElement ee = nn.toElement();
                if ( !ee.isNull() ) {

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
                }
            }
        }
    }
    */
    return true;
}

bool DProject::getIsNew() { return isNew; }

bool DProject::getIsLoad() { return isLoad; }

QString DProject::getDbDriver() { return dbDriver; }

QString DProject::getDbUser() { return dbUser; }

QString DProject::getDbPassord() { return dbPassord; }

QString DProject::getDbConnectOptions() { return dbConnectOptions; }

QString DProject::getDbHost() {  return dbHost; }

QString DProject::getDbName() { return dbName; }

int DProject::getDdbPort() { return dbPort; }

QString DProject::getProjectFile() { return filePath; }

QString DProject::getSelectSqlQuerty(QString &name) { return sel[name]; }

QString DProject::getInsertSqlQuerty(QString &name) { return ins[name]; }

QString DProject::getDeleteSqlQuerty(QString &name) { return del[name]; }

QString DProject::getUpdateSqlQuerty(QString &name) { return upd[name]; }

QString DProject::getOtherSqlQuerty(QString &name) { return other[name]; }

bool DProject::setDbDriver(QString & nameDriver)
{
    nameDriver = nameDriver.trimmed().toUpper() ;
    if (nameDriver == "")
        return false;

    int n = QSqlDatabase::drivers().count();
    for (int i = 0; i < n; i++)
        if (nameDriver == QSqlDatabase::drivers().at(i) ) {
        dbDriver = nameDriver;
        return true;
    }
    return false;
}

void DProject::setDbName(QString &datebaseName) { dbName = datebaseName; }

void DProject::setDbUser(QString & user) { dbUser = user; }

void DProject::setDbPassord(QString & password) { dbPassord = password; }

void DProject::setDbConnectOptions(QString & connectOptions) { dbConnectOptions = connectOptions; }

void DProject::setDbHost(QString & hostName) { dbHost = hostName; }

void DProject::setDdbPort(int & port) { dbPort = port; }

bool DProject::connectDatabase()
{
    if ( !isLoad )
        return false;

    QSqlDatabase db = QSqlDatabase::addDatabase( dbDriver );
    db.setHostName( dbHost );
    db.setDatabaseName( dbName );
    db.setUserName( dbUser );
    db.setPassword( dbPassord );
    db.setPort( dbPort );
    db.setConnectOptions( dbConnectOptions );

    return db.open();
}

void DProject::disconnectDatabase()
{
    /// TODO
    qDebug("TODO: DProjct::disconnectDatabase()");
    //QSqlDatabase::removeDatabase();

}

QStringList DProject::workTables()
{
    /// FIXME: !!!!!!!
    qDebug("FIXME: DProject::workTables()");
    return QStringList();
}
