#include "src/core/dnamespace.h"
#include "src/core/dproject.h"
#include "src/errors.h"
#include "src/core/myincapplication.h"
#include "src/core/dauth.h"
//
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
//
DProject::DProject(QString fileName)
{
    isNew = !QFile::exists(fileName);
    isLoad = false;
    isSql = false;

    auth = new DAuth();

    filePath = fileName;

    if (isNew) {
        qDebug("Creation new project is blocked!");
        MyIncApplication::application()->quit();
    }
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug("Error: can't open project file");
    }
    data = new QDataStream( f.readAll() );
    quint32 magic = 0x00;
    data[0] >> magic;

    switch (magic) {
    case 0x08435259: // cryped
        qDebug("TODO: DProject::DProject()\n\t Cryped");
        break;
    case 0x085a4950: // siple ziped
    {
        QByteArray * buf = new QByteArray(
                    qUncompress( data->device()->readAll() ));
        delete data;
        data = new QDataStream( buf, QIODevice::ReadOnly );
    }
        break;
    default: // Native XML
        data->device()->seek(0);
    }
}
//
DProject::~DProject()
{
    disconnectDatabase();
}
//
bool DProject::load()
{
    isLoad = false;
    if (isNew)
        return false;

    dbDriver = MyIncApplication::uriNamespace()
            ->config( "Database", "Driver" ).toUpper();
    if ((dbDriver == "NULL") || (dbDriver == "ERORR"))
        return false;
    dbUser = MyIncApplication::uriNamespace()
            ->config( "Database", "Username" );
    dbName = MyIncApplication::uriNamespace()
            ->config( "Database", "Name" );
    dbPassord = MyIncApplication::uriNamespace()
            ->config( "Database", "Password" );
    dbConnectOptions = MyIncApplication::uriNamespace()
            ->config( "Database", "Options" );
    dbHost = MyIncApplication::uriNamespace()
            ->config( "Database", "Hostname" );
    dbPort = MyIncApplication::uriNamespace()
            ->config( "Database", "Port" ).toInt();
    if (MyIncApplication::uriNamespace()->config( "Auth", "Enabled")
            .trimmed().toLower() != "true")
        auth->setAuth( true );

    isLoad = true;

    return true;
}
//
bool DProject::loadSql(QDomElement docElem)
{
    sel.clear();
    del.clear();
    upd.clear();
    ins.clear();
    other.clear();

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
    return false;
}
//
bool DProject::authorized() { return auth->isAuthed(); }
//
bool DProject::getAuthorized(QWidget* parent) { return auth->setAuth(parent); }
//
bool DProject::getIsNew() { return isNew; }
//
bool DProject::getIsLoad() { return isLoad; }
//
bool DProject::getIsConnected() { return isSql; }
//
QString DProject::getLastError() { return lastError; }
//
QString DProject::getDbDriver() { return dbDriver; }
//
QString DProject::getDbUser() { return dbUser; }
//
QString DProject::getDbPassord() { return dbPassord; }
//
QString DProject::getDbConnectOptions() { return dbConnectOptions; }
//
QString DProject::getDbHost() {  return dbHost; }
//
QString DProject::getDbName() { return dbName; }
//
int DProject::getDbPort() { return dbPort; }
/*/
QString DProject::getProjectFile() { return filePath; }
/*/
int DProject::getSelectSqlQuertyCount() { return sel.count(); }
//
QString DProject::getSelectSqlQuerty(QString name) { return sel[name]; }
//
QString DProject::getSelectSqlQuerty( int pos ) { return sel.keys().at( pos ); }
//
int DProject::getInsertSqlQuertyCount() { return ins.size(); }
//
QString DProject::getInsertSqlQuerty(QString name) { return ins[name]; }
//
QString DProject::getInsertSqlQuerty( int pos ) { return ins.keys().at( pos ); }
//
int DProject::getDeleteSqlQuertyCount() { return del.size(); }
//
QString DProject::getDeleteSqlQuerty(QString name) { return del[name]; }
//
QString DProject::getDeleteSqlQuerty( int pos ) { return del.keys().at( pos ); }
//
int DProject::getUpdateSqlQuertyCount() { return upd.size(); }
//
QString DProject::getUpdateSqlQuerty(QString name) { return upd[name]; }
//
QString DProject::getUpdateSqlQuerty( int pos ) { return upd.keys().at( pos ); }
//
int DProject::getOtherSqlQuertyCount() { return other.size(); }
//
QString DProject::getOtherSqlQuerty(QString name) { return other[name]; }
//
QString DProject::getOtherSqlQuerty( int pos ) { return other.keys().at( pos ); }
//
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
//
void DProject::setDbName(QString datebaseName) { dbName = datebaseName; }
//
void DProject::setDbUser(QString user) { dbUser = user; }
//
void DProject::setDbPassord(QString password) { dbPassord = password; }
//
void DProject::setDbConnectOptions(QString connectOptions) { dbConnectOptions = connectOptions; }
//
void DProject::setDbHost(QString hostName) { dbHost = hostName; }
//
void DProject::setDbPort(int port) { dbPort = port; }
// FIXME: DProject::connectDatabase()
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
    if (!isSql) {
        lastError = db.lastError().text();
        emit error( _ERR_DB_CONNECT );
    }
    MyIncApplication::uriNamespace()->initSql();
    return isSql;
}
//
void DProject::disconnectDatabase()
{
    // FIXME: recurse closing QSqlQuery, QSqlError, QSqlDatabase etc...
    qDebug("FIXME: DProjct::disconnectDatabase()");
    QSqlDatabase::removeDatabase( "Project" );
    isSql = false;
}
//
QStringList DProject::workspace()
{
    int count = MyIncApplication::uriNamespace()->configSize( "Workspace" );
    //, "Count" ).toInt();
    QStringList list;
    for( int i = 0; i < count; i++ )
        list << MyIncApplication::uriNamespace()
                ->config( "Workspace", QString("Space_%1").arg(i) );
    return list;
}
// TODO: DProject::config(QString,QString) to Multi-threaded.
QString DProject::config(QString name, QString arrayElement)
{
    return MyIncApplication::uriNamespace()->config(name, arrayElement);
}
