#include "dnamespace.h"
#include "dproject.h"
#include "../errors.h"
#include "myincapplication.h"
#include "dsystemfuncs.h"
#include "durihelper.h"
#include "ddebug.h"
#include "dxml.h"
//
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QFile>
#include <QRegExp>
#include <QStringList>
#include <QTextStream>
//
#ifndef HAVE_QT5
# include <QtGui/QMessageBox>
# include <QtGui/QFileDialog>
#else
# include <QtWidgets/QMessageBox>
# include <QtWidgets/QFileDialog>
#endif
//
const QString g_sNull   = "NULL";
const QString g_sValue  = "value";
const QString g_sError  = "Error";
//
const QString g_sMyInc  = "myinc.";
const QString g_sConfig = g_sMyInc + "config.";
//
DNamespace::DNamespace() :
    QObject(), sys(new DSystemFuncs()), doc(NULL), query(NULL)
{
    setObjectName( "Namespace" );
}
//
DNamespace::~DNamespace()
{
    FREE_MEM(doc);
    FREE_MEM(query);
    FREE_MEM(sys);
}
//
bool DNamespace::initConfig()
{
    FREE_MEM(doc);
    doc = new DXml();

    if (!doc->open(MIA_PROJECT->getFilePath()) ) {
        emit error( _ERR_NS_SYNTAX_PRO );
        FREE_MEM(doc);
        return false;
    }

    return doc->selectRoot();
}
//
bool DNamespace::initSql()
{
    if ( !MIA_PROJECT->getSql() ) {
        emit error(_ERR_DB_CONNECT);
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database("Project");
    query = new QSqlQuery(db);

    return MIA_PROJECT->loadSql( doc );
}
// <config>
//      <name value="value1" /> <!-- single -->     //
// to set
//      <name1 >   <!-- Array -->                   //
//          <name0 value="val" />    //
//      </name1>
// </config>
QString DNamespace::config(const QString &name,
                           const QString &arrayElement /*= ""*/) const
{
    if (doc == NULL) {
        emit error( _ERR_NS_CNFNOINIT );
        return g_sError;
    }

    if (!doc->selectRoot())
        return g_sError;
    if (!doc->selectElement( g_sConfig + name ))
        return g_sNull;
    if (!arrayElement.isEmpty()) {
        if(!doc->selectElement( arrayElement ))
            return g_sNull;
    }

    return doc->elementAttr( g_sValue );

    //    QDomElement child = cfg->firstChildElement( name );
    //    if (child.childNodes().count() != 0 )  { // check array
    //        if (arrayElement == "")
    //            return "Array";
    //
    //        child = child.firstChildElement( arrayElement );
    //        return child.attribute( g_sValue, g_sNull );
    //    } else
    //        return child.attribute( g_sValue, g_sNull );
}
// BUG : return comments too
quint32 DNamespace::configSize( const QString &name ) const
{
    if (doc == NULL) {
        emit error( _ERR_NS_CNFNOINIT );
        return 0;
    }
    doc->selectRoot();
    if (!doc->selectElement( g_sConfig + name )) {
        emit error( _ERR_NS_CNFNOINIT );
        return 0;
    }

    return doc->countElements();
    //    return cfg->firstChildElement( name ).childNodes().count();
}
//
void DNamespace::setConfig(const QString &name, const QString &value,
                           const QString &arrayElement/* = ""*/)
{
    //    if (!isConfig) {
    Q_UNUSED(name);
    Q_UNUSED(value);
    Q_UNUSED(arrayElement);
    emit error( _ERR_NS_CNFNOINIT );
    return;
    //    }
    //
    //    QDomElement child = cfg->firstChildElement( name );
    //    if (child.childNodes().count() != 0 )
    //        child.firstChildElement( arrayElement ).attributeNode( g_sValue ).setValue( value );
    //    else
    //        child.attributeNode(g_sValue).setValue( value );
}
// FIXME: DNamespace::sql
QSqlQuery DNamespace::sql(SqlType type, QString queryName, QStringList bindValue)
{
    if (query == NULL) {
        emit error(_ERR_NS_SQLNOINIT);
        return QSqlQuery(); // FIXME: Qt5.. now is bad.
    }
    QString src;
    switch (type) {
    case ST_SELECT:
        src = MIA_PROJECT->getSelectSqlQuerty(queryName);
        break;
    case ST_INSERT:
        MIA_PROJECT->getInsertSqlQuerty(queryName);
        break;
    case ST_UPDATE:
        MIA_PROJECT->getUpdateSqlQuerty(queryName);
        break;
    case ST_DELETE:
        MIA_PROJECT->getDeleteSqlQuerty(queryName);
        break;
    default:
        MIA_PROJECT->getOtherSqlQuerty(queryName);
    }
    query->prepare(src);
    for (int i = 0; i < bindValue.count(); i++) {
        query->addBindValue( bindValue.at(i) );
    }
    query->exec();
    // for future
    return QSqlQuery( query[0] );
}
//
bool DNamespace::report(const QString &, const QStringList &) const
{
    qDebug("FIXME: bool DNamespace::report(QString,QStringList)");
    QMessageBox::information( 0, tr("Report error"), tr("Report system not be installed") );
    return false;
}
// TODO: DNamespace::uri(QString,QVariant*)
void DNamespace::uri(QString uri, QVariant * var)
{
    if (!MIA_PROJECT->authorized()) {
        QMessageBox::critical( MIA_FOCUS, tr("Authentication"),
                               tr("Your not be authorized. Running the procedure will be terminated") );
        return;
    }
    DUriHelper ps(uri);

    if ( ps.isUri() ) {
        if ( ps.protocol() == "myinc" ) { // default
            if (ps.host() == "" ) {// localhost without socket
                // processing
                QString q = ps.path(0).toLower();
                if ( q == "config" ) {
                    qDebug("FIXME: void DNamespace::uri(QString,QVariant*)\n\tConfig node.Index out in array.");
                    var->setValue( config(ps.path(1), ps.path(2)) );
                } else if ( q == "sql" ) { /// myinc:///sql/%type/%name#arg1&arg2
                    q = ps.path(1).toLower();
                    SqlType t;
                    if (q == "select")
                        t = ST_SELECT;
                    else if (q == "insert")
                        t = ST_INSERT;
                    else if (q == "update")
                        t = ST_UPDATE;
                    else if (q == "delete")
                        t = ST_DELETE;
                    else
                        t = ST_Other;
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
                    /// TODO: fix for 4.5.x
                    var->setValue(
                                sys->run( q,
                                          ps.args(), sender() )
                                );
                } else if (q == "report") {
                    if (report( ps.path(1).toLower(), ps.args() ))
                        var->setValue( QString("Success") );
                    else
                        var->setValue( QString(g_sError) );
                } else {
                    var->setValue(QString(g_sError));
                    emit error(_ERR_URI_SYNTAX);
                }
            } else var->setValue(QString(g_sError));
        } else var->setValue(QString(g_sError));
    } else var->setValue(QString(g_sError));

    if (MIA_GLOBAL->getDebug())
        MIA_DEBUG->debug( uri, var[0], sender() );
}
