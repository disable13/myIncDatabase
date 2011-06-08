#include "dsqlquertyviewer.h"
//
#include "core/myincapplication.h"
#include "core/dproject.h"
//
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
//
DSqlQuertyViewer::DSqlQuertyViewer( ) :
    QWidget( )
{
    l = new QGridLayout( this );
    setLayout( l );
    //
    lType = new QLabel( tr("Type"), this);
    l->addWidget( lType );
    //
    cbType = new QComboBox( this );
    cbType->addItem( tr("All") );   // 0
    cbType->addItem( "SELECT" );    // 1
    cbType->addItem( "INSERT" );    // 2
    cbType->addItem( "UPDATE" );    // 3
    cbType->addItem( "DELETE" );    // 4
    cbType->addItem( tr("Other") ); // 5
    l->addWidget( cbType, 0, 1);
    //
    lstQuery = new QListWidget( this );
    {
        // TODO: add icons
        qDebug(" DSqlQuertyViewer::DSqlQuertyViewer( )");
        int count = MIA_PROJECT->getSelectSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getSelectSqlQuerty( i ) );
            item->setData(Qt::UserRole, 1);
            //item->setIcon( QIcon(":/icons/sql/select.png") );
        }
        count = MIA_PROJECT->getInsertSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getInsertSqlQuerty( i ) );
            item->setData(Qt::UserRole, 2);
            //item->setIcon( QIcon(":/icons/sql/insert.png") );
        }
        count = MIA_PROJECT->getUpdateSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getUpdateSqlQuerty( i ) );
            item->setData(Qt::UserRole, 3);
            //item->setIcon( QIcon(":/icons/sql/update.png") );
        }
        count = MIA_PROJECT->getDeleteSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getDeleteSqlQuerty( i ) );
            item->setData(Qt::UserRole, 4);
            //item->setIcon( QIcon(":/icons/sql/delete.png") );
        }
        count = MIA_PROJECT->getOtherSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getOtherSqlQuerty( i ) );
            item->setData(Qt::UserRole, 5);
            //item->setIcon( QIcon(":/icons/sql/other.png") );
        }
    }
    l->addWidget( lstQuery, 1, 0, 1, 2 );
    //
    lName = new QLabel( tr("Name"), this );
    l->addWidget( lName, 0, 2 );
    //
    leName = new QLineEdit( this );
    l->addWidget( leName, 0, 3, 1, 2 );
    //
    lData = new QLabel( tr("Source"), this );
    l->addWidget( lData, 1, 2);
    //
    teData = new QTextEdit( this );
    l->addWidget(teData, 1, 3, 1, 2 );
    //
    btnCancel = new QPushButton( tr("Close"), this );
    btnCancel->setDefault( true );
    l->addWidget( btnCancel, 2, 4);
    // Events
    connect( cbType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeType(int)) );
    connect( lstQuery, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(selectQuery(QListWidgetItem*)) );
    connect( btnCancel, SIGNAL(clicked()),
            this, SLOT(close()) );
}
//
DSqlQuertyViewer::~DSqlQuertyViewer()
{
    delete btnCancel;
    delete teData;
    delete lData;
    delete leName;
    delete lName;
    delete lstQuery;
    delete cbType;
    delete lType;
    delete l;
}
//
void DSqlQuertyViewer::changeType(int type)
{
    int count = 0;
    lstQuery->clear();
    switch (type) {
    case 0: // All
        count = MIA_PROJECT->getSelectSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getSelectSqlQuerty( i ) );
            item->setData(Qt::UserRole, 1);
            //item->setIcon( QIcon(":/icons/sql/select.png") );
        }
        count = MIA_PROJECT->getInsertSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getInsertSqlQuerty( i ) );
            item->setData(Qt::UserRole, 2);
            //item->setIcon( QIcon(":/icons/sql/insert.png") );
        }
        count = MIA_PROJECT->getUpdateSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getUpdateSqlQuerty( i ) );
            item->setData(Qt::UserRole, 3);
            //item->setIcon( QIcon(":/icons/sql/update.png") );
        }
        count = MIA_PROJECT->getDeleteSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getDeleteSqlQuerty( i ) );
            item->setData(Qt::UserRole, 4);
            //item->setIcon( QIcon(":/icons/sql/delete.png") );
        }
        count = MIA_PROJECT->getOtherSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getOtherSqlQuerty( i ) );
            item->setData(Qt::UserRole, 5);
            //item->setIcon( QIcon(":/icons/sql/other.png") );
        }
        return;
    case 1: // Select
        count = MIA_PROJECT->getSelectSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getSelectSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/select.png") );
        }
        return;
    case 2: // Insert
        count = MIA_PROJECT->getInsertSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getInsertSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/insert.png") );
        }
        return;
    case 3: // Update
        count = MIA_PROJECT->getUpdateSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getUpdateSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/update.png") );
        }
        return;
    case 4: // Delete
        count = MIA_PROJECT->getDeleteSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getDeleteSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/delete.png") );
        }
        return;
    case 5: // Other
        count = MIA_PROJECT->getOtherSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MIA_PROJECT->getOtherSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/other.png") );
        }
        return;
    }
}
//
void DSqlQuertyViewer::selectQuery(QListWidgetItem * item)
{
    QString name = item->text();
    leName->setText( name );
    switch (item->data(Qt::UserRole).toInt() ) {
    case 1:
        teData->setText( MIA_PROJECT->getSelectSqlQuerty( name ) );
        return;
    case 2:
        teData->setText( MIA_PROJECT->getInsertSqlQuerty( name ) );
        return;
    case 3:
        teData->setText( MIA_PROJECT->getUpdateSqlQuerty( name ) );
        return;
    case 4:
        teData->setText( MIA_PROJECT->getDeleteSqlQuerty( name ) );
        return;
    case 5:
        teData->setText( MIA_PROJECT->getOtherSqlQuerty( name ) );
        return;
    }
}
