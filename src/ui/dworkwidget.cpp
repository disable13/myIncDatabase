#include "dworkwidget.h"

#include <QGridLayout>
#include <QtUiTools/QUiLoader>
#include <QVariant>
#include <QFile>

#include "src/core/myincapplication.h"

DWorkWidget::DWorkWidget(QString formName) :
        DWidget(), formName(formName)
{
    central = 0x00;
    l = new QGridLayout( this );
    setLayout( l );
}

DWorkWidget::~DWorkWidget()
{
    if (central)
        delete central;
    delete l;
}

bool DWorkWidget::init()
{
    if (MyIncApplication::project() == 0x00)
        return false;

    QUiLoader loader;
    QFile f( QString("/home/dsb/Desktop/%1")
            .arg(formName) );
    if (!f.open(QFile::ReadOnly))
        return false;
    central = loader.load(&f, this);
    f.close();

    const QObjectList childs = central->children();
    for(int i = 0; i < childs.count(); i++ ) {
        QWidget * obj = (QWidget*)childs.at(i);

// TODO: DWorkWidget::init() !!!!!!!!!!!
        // need "URI" to myinc URI DNamespace::uri
        // also "PropertyValue" need for set value to this property

        QVariant v = obj->property( "URI" );
        if (!v.isNull())
            threadStart( v.toString(), obj );
    }

    l->addWidget( central );
    return true;
}
