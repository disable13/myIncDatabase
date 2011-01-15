#include "dworkwidget.h"

#include <QGridLayout>
#include <QtUiTools/QUiLoader>
#include <QFile>

#include "dproject.h"

DWorkWidget::DWorkWidget(DProject * pro, QString & formName) :
        QWidget(), current(pro), formName(formName)
{
    l = new QGridLayout( this );
    setLayout( l );
}

DWorkWidget::~DWorkWidget()
{
    delete central;
    delete l;
}

bool DWorkWidget::init()
{
    QUiLoader loader;
    QFile f( QString("%1.ui").arg(formName) );
    if (!f.open(QFile::ReadOnly))
        return false;
    central = loader.load(&f, this);
    f.close();

    l->addWidget( central );
    return true;
}
