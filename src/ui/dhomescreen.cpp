#include "dhomescreen.h"

#include "dproject.h"
#include "dworkwidget.h"

#include <QGridLayout>
#include <QListWidget>

DHomeScreen::DHomeScreen(QWidget *parent) :
    QWidget(parent)
{
    l = new QGridLayout( this );

    lstBase = new QListWidget( this );
    l->addWidget( lstBase, 0, 0);

    connect( lstBase, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectWorkspace(QListWidgetItem*)) );

}

DHomeScreen::~DHomeScreen()
{
    delete lstBase;
    delete l;
}

void DHomeScreen::clear()
{
    lstBase->clear();
}

void DHomeScreen::setProject( DProject * project )
{
    clear();

    current = project;
    QStringList list = current->workspace();
    for (int i = 0; i < list.count(); i ++) {
        QListWidgetItem * item =
                new QListWidgetItem( current->config( list[i], "Title"), lstBase );
        item->setData( Qt::UserRole, list[i] );
    }
}

#warning "TODO: DHomeScreen::selectWorkspace()"

void DHomeScreen::selectWorkspace(QListWidgetItem* item)
{
    qDebug("FIXME: DHomeScreen::selectWorkspace()");

    DWorkWidget * widget =
            new DWorkWidget(
                   current->config(item->data(Qt::UserRole).toString(),"ui") );
    widget->setProject( current );
    if (widget->init())
        widget->show();
    else
        qDebug("sad dsb");
}
