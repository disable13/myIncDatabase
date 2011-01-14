#include "src/dnamespace.h"
#include "src/dproject.h"
#include "src/errors.h"

#include <QtXml/QDomDocument>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QFile>
#include <QRegExp>
#include <QStringList>

DNamespace::DNamespace(DProject * parent) :
    QObject(), parent(parent)
{
    isSql = false;
    isConfig = false;

    rx = new QRegExp("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?");
}

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

bool DNamespace::initConfig()
{
    if (isConfig)
        return true;

    doc = new QDomDocument( "Project" );
    QFile file( parent->getProjectFile() );
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

bool DNamespace::initSql()
{
    if (isSql)
        return true;

    if ( !parent->getIsConnected() )
        if ( !parent->connectDatabase() ) {
            emit error(_ERR_DB_CONNECT);
            return false;
        }
    query = new QSqlQuery();
    /// FIXME: load sql procedures from xml
    qDebug("FIXME: DNamespace::initSql()");

    return false;
}


// <config>
//      <name value="value1" /> <!-- single -->     // cfg::name = 'value1'
// to set cfg::name(new_value)
//      <name1 >   <!-- Array -->                   // cfg::name1 = 'Array'
//          <name0 value="val" />    // cfg::name1[name0] = 'val'
//      </name1>
// </config>
QString DNamespace::config(QString name, QString arrayElement)
{
    if (!isConfig) {
        emit error( _ERR_NS_CNFNOINIT );
        return "Error";
    }
    QDomElement child = cfg->firstChildElement( name );
    if (child.childNodes().count() != 0 )  {// check array
        if (arrayElement == "")
            return "Array";

        child = child.firstChildElement( arrayElement );
        return child.attribute( "value", "NULL" );
    } else
        return child.attribute( "value", "NULL" );
}

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

void DNamespace::uri(QString uri, QVariant * var)
{
    qDebug("TODO: DNamespace::uri(QString,QVariant*)");

    // parser string "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?"
    // example "http://www.ics.uci.edu/pub/ietf/uri/#Related"
    // группа 2 — схема, (myinc)
    // группа 4 — источник, (config|resourse|sql)
    // группа 5 — путь, (Database/Username)
    // группа 7 — запрос,
    // группа 9 — фрагмент. (|Set)
    //

    // 1) http:
    // 2) http
    // 3) //www.ics.uci.edu
    // 4) www.ics.uci.edu
    // 5) /pub/ietf/uri/
    // 6) нет результата
    // 7) нет результата
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
