#include "dnamespace.h"
#include "dproject.h"
#include "../errors.h"
#include "myincapplication.h"
#include "dauth.h"
#include "dxml.h"
//
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
//
const QString g_sDatabase = "Database";
const QString g_sNull =     "NULL";
//
DProject::DProject(const QString &fileName)
    : m_Load(false), m_Sql(false), m_FilePath(fileName)
{
    m_New = !QFile::exists(m_FilePath);

    if (m_New) {
        qDebug("Creation new project is blocked!");
        MIA_APP->quit();
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
    if (m_Load)
        return true;
    if (m_New)
        return false;

    m_DbDriver = MIA_NAMESPACE->config( g_sDatabase, "Driver" ).toUpper();
    if ((m_DbDriver == "NULL") || (m_DbDriver == "ERORR"))
        return false;
    m_DbUser =          MIA_NAMESPACE->config( g_sDatabase, "Username" );
    m_DbName =          MIA_NAMESPACE->config( g_sDatabase, "Name" );
    m_DbPassword =      MIA_NAMESPACE->config( g_sDatabase, "Password" );
    m_DbConnectOptions = MIA_NAMESPACE->config( g_sDatabase, "Options" );
    m_DbHost =          MIA_NAMESPACE->config( g_sDatabase, "Hostname" );
    m_DbPort =          MIA_NAMESPACE->config( g_sDatabase, "Port" ).toInt();

    if (MIA_NAMESPACE->config( "Auth", "Enabled").trimmed().toLower() != "true")
        auth.setAuth( true );

    m_Load = true;

    return true;
}
//
bool DProject::loadSql(DXml *docElem)
{
    sel.clear();
    del.clear();
    upd.clear();
    ins.clear();
    other.clear();

    docElem->selectRoot();
    if (!docElem->selectElement("myinc.sql"))
        return false;
    do {
        QString attr = docElem->elementAttr( "type" );
        DNamespace::SqlType eType;
        if (attr == "select") {
            eType = DNamespace::ST_SELECT;
        } else if (attr == "insert") {
            eType = DNamespace::ST_INSERT;
        } else if (attr == "delete") {
            eType = DNamespace::ST_DELETE;
        } else if ( attr == "update") {
            eType = DNamespace::ST_UPDATE;
        } else /*if ( attr == "other")*/ {
            eType = DNamespace::ST_Other;
        }
        docElem->selectElement("query");
        do {
            QString name = docElem->elementAttr("name");
            switch (eType) {
            case DNamespace::ST_SELECT:
                sel.insert(name, docElem->element()); break;
            case DNamespace::ST_INSERT:
                ins.insert(name, docElem->element()); break;
            case DNamespace::ST_DELETE:
                del.insert(name, docElem->element()); break;
            case DNamespace::ST_UPDATE:
                upd.insert(name, docElem->element()); break;
            default: other.insert(name, docElem->element());
            }
        } while(!docElem->selectNext());

        if (!docElem->selectParent()) break;
    } while(!docElem->selectNext());

    return true;
}
//
bool DProject::authorized() const
{ return auth.getAuth(); }
//
bool DProject::getAuthorized(QWidget* parent)
{ return auth.setAuth(parent); }
//
int DProject::getSelectSqlQuertyCount() const
{ return sel.count(); }
//
QString DProject::getSelectSqlQuerty(const QString &name) const
{ return sel[name]; }
//
QString DProject::getSelectSqlQuerty(const int &pos) const
{ return sel.keys().at( pos ); }
//
int DProject::getInsertSqlQuertyCount() const
{ return ins.size(); }
//
QString DProject::getInsertSqlQuerty(const QString &name) const
{ return ins[name]; }
//
QString DProject::getInsertSqlQuerty(const int &pos) const
{ return ins.keys().at( pos ); }
//
int DProject::getDeleteSqlQuertyCount() const
{ return del.size(); }
//
QString DProject::getDeleteSqlQuerty(const QString &name) const
{ return del[name]; }
//
QString DProject::getDeleteSqlQuerty(const int &pos) const
{ return del.keys().at( pos ); }
//
int DProject::getUpdateSqlQuertyCount() const
{ return upd.size(); }
//
QString DProject::getUpdateSqlQuerty(const QString &name) const
{ return upd[name]; }
//
QString DProject::getUpdateSqlQuerty( const int &pos ) const
{ return upd.keys().at( pos ); }
//
int DProject::getOtherSqlQuertyCount() const
{ return other.size(); }
//
QString DProject::getOtherSqlQuerty(const QString &name) const
{ return other[name]; }
//
QString DProject::getOtherSqlQuerty(const int &pos ) const
{ return other.keys().at( pos ); }
//
bool DProject::setDbDriver(const QString &driver)
{
    QString nameDriver = driver.trimmed().toUpper();
    if (nameDriver.isEmpty())
        return false;

    foreach(QString driver, QSqlDatabase::drivers()) {
        if (nameDriver == driver) {
            m_DbDriver = driver;
            return true;
        }
    }
    //    int n = QSqlDatabase::drivers().count();
    //    for (int i = 0; i < n; i++)
    //        if (nameDriver == QSqlDatabase::drivers().at(i) ) {
    //            m_DbDriver = nameDriver;
    //            return true;
    //        }

    return false;
}
// FIXME: DProject::connectDatabase()
bool DProject::connectDatabase()
{
    if (!m_Load)
        return false;
    if (m_Sql) {
        qDebug("DProject::connectDatabase() Already connected");
        return true;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase( m_DbDriver, "Project" );
    if (m_DbHost != g_sNull)
        db.setHostName( m_DbHost );
    if (m_DbName != g_sNull)
        db.setDatabaseName( m_DbName );
    if (m_DbUser != g_sNull)
        db.setUserName( m_DbUser );
    if (m_DbPassword != g_sNull)
        db.setPassword( m_DbPassword );
    if (m_DbPort != 0)
        db.setPort( m_DbPort );
    if (m_DbConnectOptions != g_sNull)
        db.setConnectOptions( m_DbConnectOptions );

    m_Sql = db.open();
    if (!m_Sql) {
        m_LastError = db.lastError().text();
        emit error( _ERR_DB_CONNECT );
    }
    MIA_NAMESPACE->initSql();

    return m_Sql;
}
//
void DProject::disconnectDatabase()
{
    // FIXME: recurse closing QSqlQuery, QSqlError, QSqlDatabase etc...
    qDebug("FIXME: DProjct::disconnectDatabase()");
    QSqlDatabase::removeDatabase( "Project" );
    m_Sql = false;
}
//
QStringList DProject::workspace()
{
    int count = MIA_NAMESPACE->configSize( "Workspace" );
    //, "Count" ).toInt();
    QStringList list;
    for( int i = 0; i < count; i++ )
        list << MIA_NAMESPACE->config( "Workspace", QString("Space_%1").arg(i) );
    return list;
}
// TODO: DProject::config(QString,QString) to Multi-threaded.
QString DProject::config(const QString &name, const QString &arrayElement) const
{
    return MIA_NAMESPACE->config(name, arrayElement);
}
