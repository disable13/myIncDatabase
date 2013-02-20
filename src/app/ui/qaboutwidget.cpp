#include "qaboutwidget.h"
//
#include "../core/myincapplication.h"
//
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
//
QAboutWidget::QAboutWidget() :
    QWidget(MIA_FOCUS, Qt::Dialog )
{
    l = new QGridLayout( this );
    setLayout( l );

    lLogo = new QLabel( this );
    lLogo->setPixmap( QPixmap(":/icon/qrcode.png") );
    l->addWidget( lLogo, 0, 0, 2, 1, Qt::AlignTop );

    lSource = new QLabel( this );
    lSource->setText(
                QString("MyIncDatabase %1\n\n"
                        "Author:\t%2\n"
                        "Site:\t%3\n"
                        "License:\tLGPL")
                .arg(MIA_APP->applicationVersion())
                .arg(MIA_APP->organizationName())
                .arg(MIA_APP->organizationDomain())
                );

    l->addWidget( lSource, 0, 1, Qt::AlignTop );

    btnOk = new QPushButton( tr("Close"), this );
    btnOk->setMaximumSize(75,23);
    connect( btnOk, SIGNAL(clicked()),
             this, SLOT(closeClick()) );
    l->addWidget( btnOk, 1, 1, Qt::AlignBottom | Qt::AlignRight );

    setWindowTitle( "About programm..." );
    setWindowIcon( QIcon(":/icon/info.png") );
}
//
QAboutWidget::~QAboutWidget()
{
    delete btnOk;
    delete lSource;
    delete lLogo;
    delete l;
}
//
void QAboutWidget::closeClick()
{
    close();
    deleteLater();
}
