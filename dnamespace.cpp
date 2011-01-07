#include "dnamespace.h"
#include "dproject.h"
#include "errors.h"

#include <QtXml/QDomDocument>
#include <QFile>

DNamespace::DNamespace(DProject * parent) :
        QObject(), parent(parent)
{
    isSql = false;
    isConfig = false;
}

DNamespace::~DNamespace()
{
    if (isConfig) {
        delete doc;
        delete cfg;
    }
}

void DNamespace::function(Type t, QString query, QVariant * out)
{
    qDebug("FIXME: DNodespace::function(Type,QString,QVariant)");

    Q_UNUSED(query);
    Q_UNUSED(out);

    switch (t) {
    case Sql:
        break;
    case Config:
        break;
    case System:
        break;
    default:
        emit error( _ERR_NS_TYPE );
    }
}

bool DNamespace::initConfig()
{
    isConfig = false;
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
    qDebug("TODO: DNamespace::initSql()");


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
        emit error( _ERR_NS_CNFNOLOADED );
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
        emit error( _ERR_NS_CNFNOLOADED );
        return;
    }

    QDomElement child = cfg->firstChildElement( name );
    if (child.childNodes().count() != 0 )
        child.firstChildElement( arrayElement ).attributeNode( "value" ).setValue( value );
    else
        child.attributeNode("value").setValue( value );
}
