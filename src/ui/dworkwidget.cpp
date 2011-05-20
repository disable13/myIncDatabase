#include "dworkwidget.h"
//
#include <QGridLayout>
#include <QtUiTools/QUiLoader>
#include <QMessageBox>
#include <QVariant>
#include <QFile>
//
#include "src/core/myincapplication.h"
//
DWorkWidget::DWorkWidget(QString formName) :
        DWidget(), formName(formName)
{
    central = 0x00;
    l = new QGridLayout( this );
    setLayout( l );
}
//
DWorkWidget::~DWorkWidget()
{
    if (central)
        delete central;
    delete l;
}
//
bool DWorkWidget::init()
{
    // TODO: DWorkWidget::init() <===<<
    qDebug( "TODO: DWorkWidget::init() " );

    if (MyIncApplication::project() == 0x00) {
        errorMessage( tr("Project not be opened") );
        return false;
    }

    QUiLoader loader;
    QFile f( formName );
    if (!f.open(QFile::ReadOnly)){
        errorMessage( tr("Can't read UI file ").append(formName) );
        return false;
    }
    central = loader.load(&f, this);
    f.close();

    const QObjectList childs = central->children();
    for(int i = 0; i < childs.count(); i++ ) {
        QWidget * obj = (QWidget*)childs.at(i);

        // need "URI" to myinc URI DNamespace::uri
        // also "PropertyValue" need for set value to this property

        QVariant v = obj->property( "URI" );
        if (!v.isNull())
            threadStart( v.toString(), obj );
    }

    l->addWidget( central );
    return true;
}
//
int DWorkWidget::errorMessage(QString more)
{
    QMessageBox msg;
    msg.setIcon( QMessageBox::Critical );
    msg.setStandardButtons( QMessageBox::Retry | QMessageBox::Cancel );
    msg.setDefaultButton( QMessageBox::Cancel );
    msg.setText( tr("Work widget error") );
    msg.setDetailedText( more );

    return msg.exec();
}
