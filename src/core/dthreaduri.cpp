#include "dthreaduri.h"
//
#include "src/core/myincapplication.h"
//
#include <QRegExp>
#include <QtXml/QDomDocument>
#include <QFile>
#include "dproject.h"
#include "src/errors.h"
//
#include <QDebug>
//
DThreadUri::DThreadUri(QString Uri) :
    QObject(), uri(Uri)
{

}
//
DThreadUri::~DThreadUri()
{

}
//
void DThreadUri::run()
{
    QRegExp rx("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?");

    if (rx.indexIn( uri ) == -1) {
        error( _TRD_ERR_RX_SYNTAX );
#warning "Always Error!!!!!"
        return;
    }

    if (rx.cap(1)=="myinc") {
        if (rx.cap(3)=="") { // localhost without socket
            path = rx.cap(4).
                    split('/', QString::SkipEmptyParts );
            QString type = path.at(0).toLower();
            qDebug() << (type);
            if (type=="system")
                system();
            else if (type=="config")
                config();
            else if (type=="sql")
                sql();
            else {
                // Unknow type
                error( _TRD_ERR_TYPE );
            }
        } else { // need TCP Connection to rx.cap(3)

        }
    } else {
        /// TODO file,ftp,http
    }
}
//
void inline DThreadUri::system()
{

}
//
#warning "TODO DThreadUri::config()"
//
void inline DThreadUri::config()
{
    QString name, arrayElement;
    QDomDocument doc( "Project" );
    QFile file( MyIncApplication::project()->getProjectFile() );
    if (!file.open(QIODevice::ReadOnly )) {
        error( _TRD_ERR_CONF_CANTOPEN );
        return;
    }
    if (!doc.setContent(&file)) {
        error( _TRD_ERR_NS_SYNTAX_PRO );
        file.close();
        return;
    }
    file.close();

    if (doc.documentElement().tagName().toLower() != "root") {
        error( _TRD_ERR_NS_ROOT );
        return;
    }
    /// TODO: Set config node;
    QDomNode cfg( doc.documentElement().firstChildElement( "config" ) );
    if ( cfg.isNull() ) {
        error( _TRD_ERR_NS_NOCNFNODE );
        return;
    }
    QDomElement child = cfg.firstChildElement( name );
    if (child.childNodes().count() != 0 )  {// check array
        if (arrayElement == "") {
             complete( (int*)this, "Array" );
            return;
        }
        child = child.firstChildElement( arrayElement );
        complete( (int*)this, child.attribute( "value", "NULL" ) );
        return;
    } else {
        complete( (int*)this, child.attribute( "value", "NULL" ) );
        return;
    }
}
//
void inline DThreadUri::sql()
{
    complete( (int*)this, QVariant("SQL not finished") );
}
//
void inline DThreadUri::error(int e ) { m_err = e; }
