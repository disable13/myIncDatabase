#include "dhomescreen.h"

#include "dproject.h"

#include <QGridLayout>
#include <QListWidget>

DHomeScreen::DHomeScreen(QWidget *parent) :
    QWidget(parent)
{
    l = new QGridLayout( this );

    lstBase = new QListWidget( this );
    l->addWidget( lstBase, 0, 0);

    connect( lstBase, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectTable(QListWidgetItem*)) );

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
    lstBase->addItems( current->workTables() );
}

#warning "TODO: DHomeScreen::selectTable()"

void DHomeScreen::selectTable(QListWidgetItem* item)
{
    Q_UNUSED(item);
    qDebug("TODO: DHomeScreen::selectTable()");
}
