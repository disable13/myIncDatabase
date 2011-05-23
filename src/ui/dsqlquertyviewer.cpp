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
    cbType->addItem( tr("All") );
    cbType->addItem( "SELECT" );
    cbType->addItem( "INSERT" );
    cbType->addItem( "UPDATE" );
    cbType->addItem( "DELETE" );
    cbType->addItem( tr("Other") );
    l->addWidget( cbType, 0, 1);
    //
    lstQuery = new QListWidget( this );
    // fill SELECT
    int count = MyIncApplication::project()->getSelectSqlQuertyCount();
    for(int i = 0; i < count; i++ ) {
        QListWidgetItem * item = new QListWidgetItem( lstQuery );
        item->setText( MyIncApplication::project()->getSelectSqlQuerty( i ) );
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
