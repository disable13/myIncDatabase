#include "src/dnamespace.h"
#include "src/dproject.h"
#include "src/errors.h"

#include <QFile>
#include <QtXml/QDomDocument>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

DProject::DProject(QString fileName)
{
    //nspace = new DNamespace( this );
    isNew = !QFile::exists(fileName);
    isLoad = false;
    isSql = false;

    filePath = fileName;

    nspace = new DNamespace( this );

    connect( nspace, SIGNAL(error(int)), this, SIGNAL(error(int)) );
}

DProject::~DProject()
{
    disconnectDatabase();
    delete nspace;
}

void DProject::save()
{
    qDebug("FIXME: bool DProject::save()");
    nspace->setConfig( "Database", dbDriver, "Driver" );
    nspace->setConfig( "Database", dbUser, "Username" );
    nspace->setConfig( "Database", dbPassord, "Password" );
    nspace->setConfig( "Database", dbConnectOptions, "Options" );
    nspace->setConfig( "Database", dbHost, "Hostname" );
    nspace->setConfig( "Database", dbName, "Name" );
    nspace->setConfig( "Database", QString( dbPort ), "Driver" );

    isNew = false;
}

bool DProject::load()
{
    isLoad = false;
    if (isNew)
        return false;
    nspace->initConfig();

    dbDriver = nspace->config( "Database", "Driver" ).toUpper();
    if ((dbDriver == "NULL") || (dbDriver == "ERORR"))
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

#warning "FIXME: DProject::loadSql()"

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

bool DProject::getIsConnected() { return isSql; }

QString DProject::getDbDriver() { return dbDriver; }

QString DProject::getDbUser() { return dbUser; }

QString DProject::getDbPassord() { return dbPassord; }

QString DProject::getDbConnectOptions() { return dbConnectOptions; }

QString DProject::getDbHost() {  return dbHost; }

QString DProject::getDbName() { return dbName; }

int DProject::getDbPort() { return dbPort; }

QString DProject::getProjectFile() { return filePath; }

DNamespace * DProject::getNamespace() { return nspace; }

QString DProject::getSelectSqlQuerty(QString name) { return sel[name]; }

QString DProject::getInsertSqlQuerty(QString name) { return ins[name]; }

QString DProject::getDeleteSqlQuerty(QString name) { return del[name]; }

QString DProject::getUpdateSqlQuerty(QString name) { return upd[name]; }

QString DProject::getOtherSqlQuerty(QString name) { return other[name]; }

bool DProject::setDbDriver(QString nameDriver)
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

void DProject::setDbName(QString datebaseName) { dbName = datebaseName; }

void DProject::setDbUser(QString user) { dbUser = user; }

void DProject::setDbPassord(QString password) { dbPassord = password; }

void DProject::setDbConnectOptions(QString connectOptions) { dbConnectOptions = connectOptions; }

void DProject::setDbHost(QString hostName) { dbHost = hostName; }

void DProject::setDbPort(int port) { dbPort = port; }

#warning "FIXME: DProject::connectDatabase()"

bool DProject::connectDatabase()
{
    if ( !isLoad || isSql )
        return false;

    QSqlDatabase db = QSqlDatabase::addDatabase( dbDriver, "Project" );
    if (dbHost != "NULL")
        db.setHostName( dbHost );
    if (dbName!="NULL")
        db.setDatabaseName( dbName );
    if (dbUser!="NULL")
        db.setUserName( dbUser );
    if (dbPassord!="NULL")
        db.setPassword( dbPassord );
    if (dbPort!=0)
        db.setPort( dbPort );
    if (dbConnectOptions!="NULL")
        db.setConnectOptions( dbConnectOptions );

    isSql = db.open();
    if (!isSql)
        emit error( _ERR_DB_CONNECT );
    qDebug("FIXME: DProject::connectDatabase()");
    //    qDebug(db.lastError().text());
    return isSql;
}

#warning "FIXME: DProject::disconnectDatabase()"

void DProject::disconnectDatabase()
{
    /// FIXME: recurse closing QSqlQuery, QSqlError, QSqlDatabase etc...
    qDebug("FIXME: DProjct::disconnectDatabase()");
    QSqlDatabase::removeDatabase( "Project" );
    isSql = false;
}

QStringList DProject::workTables()
{
    int count = nspace->config( "WorkTables", "Count" ).toInt();
    QStringList list;
    for( int i = 0; i < count; i++ )
        list << nspace->config( "WorkTables", QString("Table%1").arg(i) );
    return list;
}
