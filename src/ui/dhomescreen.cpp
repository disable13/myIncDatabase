#include "dhomescreen.h"
//
#include "core/dproject.h"
#include "ui/dworkwidget.h"
#include "core/myincapplication.h"
#include "core/dnamespace.h"
//
#include <QGridLayout>
#include <QListWidget>
//
DHomeScreen::DHomeScreen(QWidget *parent) :
    QWidget(parent)
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
    clear();
    delete lstBase;
    delete l;
}
//
void DHomeScreen::clear()
{
    lstBase->clear();
    for(int i = 0; i < listWidget.size(); i++ ) {
        listWidget.at(i)->close();
        delete listWidget.at(i);
    }
    listWidget.clear();
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
//
bool DHomeScreen::selectWorkspace( QString name )
{
    name = name.toLower();
    for(int i = 0; i < lstBase->count(); i++ )
        if (lstBase->item(i)->text().toLower() == name ) {
            selectWorkspace( lstBase->item(i) );
            return true;
    }
    return false;
}
//
void DHomeScreen::selectWorkspace(QListWidgetItem* item)
{
    qDebug("FIXME: void DHomeScreen::selectWorkspace(QListWidgetItem*)\n\tFix Name");
    QString name = current->config(item->data(Qt::UserRole).toString(), "ui");
    for(int i = 0; i < listWidget.count(); i++)
        if (listWidget.at(i)->objectName() == name) {
            listWidget.at(i)->show();
            return;
    }
    DWorkWidget * widget =
            new DWorkWidget(
                name );
    connect( widget, SIGNAL(uri(QString,QVariant*)),
             MIA_NAMESPACE, SLOT(uri(QString,QVariant*)) );
    if (widget->init()) {
        widget->setObjectName( name );
        listWidget.append( widget );
        widget->show();
    } else {
        delete widget;
    }
}
