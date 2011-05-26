#include "dsqlquertyviewer.h"
//
#include "src/core/myincapplication.h"
#include "src/core/dproject.h"
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
        int count = MyIncApplication::project()->getSelectSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getSelectSqlQuerty( i ) );
            item->setData(Qt::UserRole, 1);
            //item->setIcon( QIcon(":/icons/sql/select.png") );
        }
        count = MyIncApplication::project()->getInsertSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getInsertSqlQuerty( i ) );
            item->setData(Qt::UserRole, 2);
            //item->setIcon( QIcon(":/icons/sql/insert.png") );
        }
        count = MyIncApplication::project()->getUpdateSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getUpdateSqlQuerty( i ) );
            item->setData(Qt::UserRole, 3);
            //item->setIcon( QIcon(":/icons/sql/update.png") );
        }
        count = MyIncApplication::project()->getDeleteSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getDeleteSqlQuerty( i ) );
            item->setData(Qt::UserRole, 4);
            //item->setIcon( QIcon(":/icons/sql/delete.png") );
        }
        count = MyIncApplication::project()->getOtherSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getOtherSqlQuerty( i ) );
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
    if ( !MyIncApplication::project()->getIsReadOnly() ) {
        btnSave = new QPushButton( tr("Save"), this );
        l->addWidget( btnSave, 2, 3);
    }
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
    delete btnSave;
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
        count = MyIncApplication::project()->getSelectSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getSelectSqlQuerty( i ) );
            item->setData(Qt::UserRole, 1);
            //item->setIcon( QIcon(":/icons/sql/select.png") );
        }
        count = MyIncApplication::project()->getInsertSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getInsertSqlQuerty( i ) );
            item->setData(Qt::UserRole, 2);
            //item->setIcon( QIcon(":/icons/sql/insert.png") );
        }
        count = MyIncApplication::project()->getUpdateSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getUpdateSqlQuerty( i ) );
            item->setData(Qt::UserRole, 3);
            //item->setIcon( QIcon(":/icons/sql/update.png") );
        }
        count = MyIncApplication::project()->getDeleteSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getDeleteSqlQuerty( i ) );
            item->setData(Qt::UserRole, 4);
            //item->setIcon( QIcon(":/icons/sql/delete.png") );
        }
        count = MyIncApplication::project()->getOtherSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getOtherSqlQuerty( i ) );
            item->setData(Qt::UserRole, 5);
            //item->setIcon( QIcon(":/icons/sql/other.png") );
        }
        return;
    case 1: // Select
        count = MyIncApplication::project()->getSelectSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getSelectSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/select.png") );
        }
        return;
    case 2: // Insert
        count = MyIncApplication::project()->getInsertSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getInsertSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/insert.png") );
        }
        return;
    case 3: // Update
        count = MyIncApplication::project()->getUpdateSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getUpdateSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/update.png") );
        }
        return;
    case 4: // Delete
        count = MyIncApplication::project()->getDeleteSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getDeleteSqlQuerty( i ) );
            item->setData(Qt::UserRole, type);
            //item->setIcon( QIcon(":/icons/sql/delete.png") );
        }
        return;
    case 5: // Other
        count = MyIncApplication::project()->getOtherSqlQuertyCount();
        for(int i = 0; i < count; i++ ) {
            QListWidgetItem * item = new QListWidgetItem( lstQuery );
            item->setText( MyIncApplication::project()->getOtherSqlQuerty( i ) );
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
        teData->setText( MyIncApplication::project()->getSelectSqlQuerty( name ) );
        return;
    case 2:
        teData->setText( MyIncApplication::project()->getInsertSqlQuerty( name ) );
        return;
    case 3:
        teData->setText( MyIncApplication::project()->getUpdateSqlQuerty( name ) );
        return;
    case 4:
        teData->setText( MyIncApplication::project()->getDeleteSqlQuerty( name ) );
        return;
    case 5:
        teData->setText( MyIncApplication::project()->getOtherSqlQuerty( name ) );
        return;
    }
}
