#include "src/core/dnamespace.h"
#include "src/core/dproject.h"
#include "src/errors.h"
#include "src/core/myincapplication.h"
//
#include <QtXml/QDomDocument>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QFile>
#include <QRegExp>
#include <QStringList>
#include <QTextStream>
//
DNamespace::DNamespace() :
    QObject()
{
    isSql = false;
    isConfig = false;

    rx = new QRegExp("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?");
}
//
DNamespace::~DNamespace()
{
    if (isConfig) {
        delete doc;
        delete cfg;
    }
    if (isSql)
        delete query;
    delete rx;
}
//
bool DNamespace::initConfig()
{
    if (isConfig) {
        delete doc;
        delete cfg;
    }

    doc = new QDomDocument( "Project" );
    QFile file( MyIncApplication::project()->getProjectFile() );
    if (!file.open(QIODevice::ReadOnly)) {
        emit error( _ERR_CANTOPEN );
        delete doc;
        return false;
    }
    if (!doc->setContent(&file)) {
        emit error( _ERR_NS_SYNTAX_PRO );
        file.close();
        delete doc;
        return false;
    }
    file.close();

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
// FIXME: DNamespace::initSql()
bool DNamespace::initSql()
{
    if ( !MyIncApplication::project()->getIsConnected() ) {
        emit error(_ERR_DB_CONNECT);
        return false;
    }
    query = new QSqlQuery();
    /// FIXME: load sql procedures from xml
    qDebug("FIXME: DNamespace::initSql()");

    return MyIncApplication::project()->loadSql( doc->firstChildElement( "root" ) );
}
// TODO: DNamespace::saveXml()
void DNamespace::saveXml()
{
    qDebug("TODO: DNamespace::saveXml()");
    QFile f(MyIncApplication::project()->getProjectFile());
    f.open( QIODevice::ReadWrite );
    QTextStream tx( &f );
    cfg->save( tx, 0x00 );
    f.close();
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
// FIXME: DNamespace::sql(QString,QList<QVariant>)
const QSqlResult * DNamespace::sql(QString queryName, QList<QVariant> bindValue)
{
    Q_UNUSED(queryName);
    if (!isSql) {
        emit error(_ERR_NS_SQLNOINIT);
        return 0x00;
    }

    query->prepare(queryName);
    qDebug("FIXME: DNamespace::sql(QString,QList<QVariant>)\n\tSQL-Text need convert to name SQL-procedure in xml file");
    for (int i = 0; i > bindValue.count(); i++) {
        query->addBindValue( bindValue[i] );
    }
    query->exec();
    return query->result();
}
// TODO: DNamespace::uri(QString,QVariant*)
void DNamespace::uri(QString uri, QVariant * var)
{
    qDebug("TODO: DNamespace::uri(QString,QVariant*)");

    // parser string "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?"
    // example "http://www.ics.uci.edu/pub/ietf/uri/#Related"
    // 2 (myinc)
    // 4 (config|resourse|sql)
    // 5 (Database/Username)
    // 7
    // 9 (|Set)
    //

    // 1) http:
    // 2) http
    // 3) //www.ics.uci.edu
    // 4) www.ics.uci.edu
    // 5) /pub/ietf/uri/
    // 6)
    // 7)
    // 8) #Related+
    // 9) Related

    // myinc://host/Config/Database/User     // Socket
    // myinc:///Config/Database/User         // Without socket
    // myinc://localhost/sql/%name
    if (rx->indexIn(uri) != -1)
        if ( rx->cap(1) == "myinc" ) { // default
            if (rx->cap(3) == "" ) {// localhost without socket
                QStringList path = rx->cap(4).split('/', QString::SkipEmptyParts );
                // processing
                QString q = path.at(0).toLower();
                if ( q == "config" ) {
                    var->setValue( config(path[1],path[2]) );
                } else if ( q == "sql" ) {
                    qDebug("TODO: DNamespace::uri(QString,QVariant*)\n\tProcessing \"sql\"");
                } else if ( q == "system" ) {
                    qDebug("TODO: DNamespace::uri(QString,QVariant*)\n\tProcessing \"system\"");
                } else {
                    var->setValue(QString("Error"));
                    emit error(_ERR_URI_SYNTAX);
                }
            }
        }
}
