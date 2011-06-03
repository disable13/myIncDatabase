#include "src/core/dnamespace.h"
#include "src/core/dproject.h"
#include "src/errors.h"
#include "src/core/myincapplication.h"
#include "src/core/dsystemfuncs.h"
#include "src/core/durihelper.h"
//
#include <QtXml/QDomDocument>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QFile>
#include <QRegExp>
#include <QStringList>
#include <QTextStream>
#include <QtGui/QMessageBox>
//
DNamespace::DNamespace() :
    QObject()
{
    isSql = false;
    isConfig = false;

    setObjectName( "Namespace" );

    sys = new DSystemFuncs();
}
//
DNamespace::~DNamespace()
{
    if (isConfig) {
        delete doc;
        delete cfg;
    }
    if (isSql) {
        delete query;
    }
    delete sys;
}
//
bool DNamespace::initConfig()
{
    if (isConfig) {
        delete doc;
        delete cfg;
    }

    doc = new QDomDocument( "Project" );

    if (!doc->setContent(
                MyIncApplication::project()->getData()->device())) {
        emit error( _ERR_NS_SYNTAX_PRO );
        delete doc;
        return false;
    }

    QDomElement docElem = doc->documentElement();

    if (docElem.tagName().toLower() != "root") {
        emit error( _ERR_NS_NOROOT );
        delete doc;
        return false;
    }
    /// TODO: Set config node;
    cfg = new QDomNode( docElem.firstChildElement( "config" ) );
    if ( cfg->isNull() ) {
        emit error( _ERR_NS_NOCNFNODE );
        delete doc;
        delete cfg;
        return false;
    }

    isConfig = true;
    return true;
}
//
bool DNamespace::initSql()
{
    if ( !MyIncApplication::project()->getIsConnected() ) {
        emit error(_ERR_DB_CONNECT);
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database("Project");
    query = new QSqlQuery(db);

    isSql = true;

    return MyIncApplication::project()->loadSql( doc->firstChildElement( "root" ) );
}
// <config>
//      <name value="value1" /> <!-- single -->     //
// to set
//      <name1 >   <!-- Array -->                   //
//          <name0 value="val" />    //
//      </name1>
// </config>
QString DNamespace::config(QString name, QString arrayElement)
{
    if (!isConfig) {
        emit error( _ERR_NS_CNFNOINIT );
        return "Error";
    }
    QDomElement child = cfg->firstChildElement( name );
    if (child.childNodes().count() != 0 )  { // check array
        if (arrayElement == "")
            return "Array";

        child = child.firstChildElement( arrayElement );
        return child.attribute( "value", "NULL" );
    } else
        return child.attribute( "value", "NULL" );
}
// BUG : return comments too
int DNamespace::configSize( QString name )
{
    if (!isConfig) {
        emit error( _ERR_NS_CNFNOINIT );
        return 0;
    }
    return cfg->firstChildElement( name ).childNodes().count();
}
//
void DNamespace::setConfig(QString name, QString value, QString arrayElement)
{
    if (!isConfig) {
        emit error( _ERR_NS_CNFNOINIT );
        return;
    }

    QDomElement child = cfg->firstChildElement( name );
    if (child.childNodes().count() != 0 )
        child.firstChildElement( arrayElement ).attributeNode( "value" ).setValue( value );
    else
        child.attributeNode("value").setValue( value );
}
// FIXME: DNamespace::sql
QSqlQuery DNamespace::sql(SqlType type, QString queryName, QStringList bindValue)
{
    if (!isSql) {
        emit error(_ERR_NS_SQLNOINIT);
        return 0x00;
    }
    QString src;
    switch (type) {
    case SELECT:
        src = MyIncApplication::project()->getSelectSqlQuerty(queryName);
        break;
    case INSERT:
        MyIncApplication::project()->getInsertSqlQuerty(queryName);
        break;
    case UPDATE:
        MyIncApplication::project()->getUpdateSqlQuerty(queryName);
        break;
    case DELETE:
        MyIncApplication::project()->getDeleteSqlQuerty(queryName);
        break;
    default:
        MyIncApplication::project()->getOtherSqlQuerty(queryName);
    }
    query->prepare(src);
    for (int i = 0; i > bindValue.count(); i++) {
        query->addBindValue( bindValue.at(i) );
    }
    query->exec();

    // for future
    return QSqlQuery( query[0] );
}
// TODO: DNamespace::uri(QString,QVariant*)
void DNamespace::uri(QString uri, QVariant * var)
{
    if (!MyIncApplication::project()->authorized()) {
        QMessageBox::critical( NULL, tr("Authentication"),
                              tr("Your not be authorized. Running the procedure will be terminated") );
        return;
    }
    DUriHelper ps(uri);

    if ( ps.isUri() )
        if ( ps.protocol() == "myinc" ) { // default
            if (ps.host() == "" ) {// localhost without socket
                // processing
                QString q = ps.path(0).toLower();
                if ( q == "config" ) {
                    qDebug("FIXME: void DNamespace::uri(QString,QVariant*)\n\tConfig node.Index out in array.");
                    var->setValue( config(ps.path(1), ps.path(2) );
                } else if ( q == "sql" ) { /// myinc:///sql/%type/%name#arg1&arg2
                    q = ps.path(1).toLower();
                    SqlType t;
                    if (q == "select")
                        t = SELECT;
                    else if (q == "insert")
                        t = INSERT;
                    else if (q == "update")
                        t = UPDATE;
                    else if (q == "delete")
                        t = DELETE;
                    else
                        t = Other;
                    sql(t, ps.path(2),
                        ps.args() );
                    var->setValue( (void*)query );
                } else if ( q == "system" ) {
                    qDebug("FIXME: void DNamespace::uri(QString,QVariant*)\n\tSystem node.Remove 'system' word from start.");
                    QStringList pathFix = ps.path().split( '/', QString::SkipEmptyParts );
                    pathFix.removeFirst();
                    q = QString::null;
                    for(int i = 0; i < pathFix.count(); i++)
                        q.append("/").append(pathFix.at(i));
                    q.remove(0,1);
                    var = new QVariant(
                                sys->run( q,
                                         ps.args(), sender() )
                                );
                } else {
                    var->setValue(QString("Error"));
                    emit error(_ERR_URI_SYNTAX);
                }
            }
        }
}
