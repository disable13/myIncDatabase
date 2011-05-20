#include "dhomescreen.h"
//
#include "src/core/dproject.h"
#include "src/ui/dworkwidget.h"
//
#include <QGridLayout>
#include <QListWidget>
//
DHomeScreen::DHomeScreen(QWidget *parent) :
    DWidget(parent)
{
    l = new QGridLayout( this );

    lstBase = new QListWidget( this );
    l->addWidget( lstBase, 0, 0);

    connect( lstBase, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectWorkspace(QListWidgetItem*)) );
}
//
DHomeScreen::~DHomeScreen()
{
    delete lstBase;
    delete l;
}
//
void DHomeScreen::clear()
{
    lstBase->clear();
}
//
void DHomeScreen::setProject( DProject * project )
{
    clear();

    current = project;
    QStringList list = current->workspace();
    for (int i = 0; i < list.count(); i ++) {
        QString temp = current->config( list[i], "Title");
        if ( temp == QString("NULL") ) continue;
        QListWidgetItem * item =
                new QListWidgetItem( temp, lstBase );
        item->setData( Qt::UserRole, list[i] );
    }
}
// TODO: DHomeScreen::selectWorkspace()
void DHomeScreen::selectWorkspace(QListWidgetItem* item)
{
    qDebug("FIXME: DHomeScreen::selectWorkspace()");

    DWorkWidget * widget =
            new DWorkWidget(
                current->config(item->data(Qt::UserRole).toString(), "ui") );
    if (widget->init())
        widget->show();    
}
