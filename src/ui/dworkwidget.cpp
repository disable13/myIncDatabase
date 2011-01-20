#include "dworkwidget.h"

#include <QGridLayout>
#include <QtUiTools/QUiLoader>
#include <QVariant>
#include <QFile>

//#include "dproject.h"

DWorkWidget::DWorkWidget(QString formName) :
        QWidget(), formName(formName)
{
    central = 0x00;
    current = 0x00;
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
    if (current == 0x00)
        return false;

    QUiLoader loader;
    QFile f( QString("/home/dsb/Sources/Qt/myinc/res/templates/%1")
            .arg(formName) );
    if (!f.open(QFile::ReadOnly))
        return false;
    central = loader.load(&f, this);
    f.close();

    const QObjectList childs = central->children();
    for(int i = 0; i < childs.count(); i++ ) {
        QObject * obj = childs.at(i);

#warning "TODO: DWorkWidget::init() !!!!!!!!!!!"
        // need "URI" to myinc URI DNamespace::uri
        // also "PropertyValue" need for set value to this property

        //obj->set
        //obj->property( "URI" ).toString();
        //
        //obj->property("PropertyValue").toString()
        obj->setProperty( obj->property("PropertyValue").toString().toLocal8Bit().data(),
                         obj->property( "URI" ) );
    }

    l->addWidget( central );
    return true;
}

void DWorkWidget::setProject(DProject * pro)
{
    current = pro;
}
