#include "dthreaduri.h"

#include <QRegExp>
#include <QtXml/QDomDocument>
#include <QFile>
#include "dproject.h"
#include "errors.h"

DThreadUri::DThreadUri(QString Uri, DProject * pro) :
    uri(Uri), project(pro)
{
    result = 0x00;
}

void DThreadUri::run()
{
    QRegExp rx("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?");

    if (rx.indexIn( uri ) == -1) {
        error( _TRD_ERR_RX_SYNTAX );
        return;
    }

    if (rx.cap(1)=="myinc") {
        if (rx.cap(3)=="") { // localhost without socket
            path = rx.cap(4).
                    split('/', QString::SkipEmptyParts );
            QString type = path.at(0).toLower();
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

void inline DThreadUri::system()
{

}

#warning "TODO DThreadUri::config()"

void inline DThreadUri::config()
{
    QString name, arrayElement;
    QDomDocument doc( "Project" );
    QFile file( project->getProjectFile() );
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
     // getConfig(name, )
    QDomElement child = cfg.firstChildElement( name );
    if (child.childNodes().count() != 0 )  {// check array
        if (arrayElement == "") {
            ;//return "Array";
        }

        child = child.firstChildElement( arrayElement );
        //return child.attribute( "value", "NULL" );
    } else {
       ;// return child.attribute( "value", "NULL" );
    }
}

void inline DThreadUri::sql()
{

}

void inline DThreadUri::error(int e)
{
    type = QT_VERSION_CHECK(((char*)&e)[0],
                            ((char*)&e)[1],0x00);
    /*
    resultLength = 2;
    result = new char[2];
    result[0] = ((char*)&e)[0];
    result[1] = ((char*)&e)[1];
    FLAG {
    0, // no
    2, // on
    4, // off
    8, // qwe
    16 // 12eswq
    }
     2 | 4
    10

    */
}
