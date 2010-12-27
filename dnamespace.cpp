#include "dnamespace.h"
#include "dproject.h"

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
    if (doc)
        delete doc;
    if (cfg)
        delete cfg;
}

void DNamespace::function(Type t, QString query, QVariant * out)
{
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
        emit error( "Bad type" );
    }
}

bool DNamespace::initConfig()
{
    isConfig = false;
    doc = new QDomDocument( "Project" );
    QFile file( "/home/dsb/my.xml" /*parent->getProjectPath();*/ );
    if (!file.open(QIODevice::ReadOnly)) {
        emit error( tr("Can't open project file") );
        return false;
    }
    if (!doc->setContent(&file)) {
        emit error( tr("Can't parsing project file") );
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc->documentElement();

    if (docElem.tagName().toLower() != "root") {
        emit error( tr("Not found root-node in project") );
        return false;
    }
    /// TODO: Set config node;
    cfg = new QDomNode( docElem.firstChildElement( "config" ) );
    if ( cfg->isNull() ) {
        emit error( tr("Not fount config-node in project file") );
        return false;
    }

    isConfig = true;
    return true;
}


// <config>
//      <name value="value1" /> <!-- single -->     // cfg::name = value
// to set cfg::name(new_value)
//      <name1 >   <!-- Array -->                   // cfg::name1 = 'Array'
//          <element name="name0" value="val" />    // cfg::name1[name0] = elem value
//      </name1>
// </config>
QString DNamespace::config(QString query, QVariant * out)
{
    if (!isConfig) {
        emit error( "Configuration not loaded" );
        return;
    }

    QDomElement child = cfg->firstChildElement( query );
    if (child.childNodes().count() != 0 )  {// check array
        Q_UNUSED(out);
    } else {
        out->setValue( child.attribute("value", "NULL") );
    }
}
