#include "ddbconfig.h"
//
#include "../core/dproject.h"
//
#ifndef HAVE_QT5
 #include <QtGui/QFormLayout>
 #include <QtGui/QComboBox>
 #include <QtGui/QLineEdit>
 #include <QtGui/QSpinBox>
 #include <QtGui/QTextEdit>
 #include <QtGui/QDialogButtonBox>
 #include <QtGui/QPushButton>
 #include <QtGui/QLabel>
 //
 #include <QtGui/QMessageBox>
#else
 #include <QtWidgets/QFormLayout>
 #include <QtWidgets/QComboBox>
 #include <QtWidgets/QLineEdit>
 #include <QtWidgets/QSpinBox>
 #include <QtWidgets/QTextEdit>
 #include <QtWidgets/QDialogButtonBox>
 #include <QtWidgets/QPushButton>
 #include <QtWidgets/QLabel>
 //
 #include <QtWidgets/QMessageBox>
#endif
//
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
//
// FIXME: class DDbConfig
//
DDbConfig::DDbConfig(QWidget *parent) :
    QDialog(parent)
{
    qDebug("FIXME: class DDbConfig");
    project = 0x00;

    l = new QFormLayout(this);

    cbDriver = new QComboBox(this);
    cbDriver->addItem( tr("NULL") );
    cbDriver->insertItems(1, QSqlDatabase::drivers());
    edtDbHost = new QLineEdit(this);
// FIXME: DDbConfig::DDbConfig()\n\tFull Input check.
    qDebug("FIXME: DDbConfig::DDbConfig()\n\tFull Input check.");
    sbDbPort = new QSpinBox(this);
    sbDbPort->setMinimum(0);
    sbDbPort->setMaximum(65535);
    edtDbUser = new QLineEdit(this);
    edtDbPassword = new QLineEdit(this);
    edtDbPassword->setEchoMode( QLineEdit::Password );
    edtDbName = new QLineEdit(this);
    edtDbOptions = new QTextEdit(this);
    lStatus = new QLabel(this);
    lStatus->setVisible( false );

    l->addRow( tr("Driver"), cbDriver );
    l->addRow( tr("Hostname"), edtDbHost );
    l->addRow( tr("Port"), sbDbPort );
    l->addRow( tr("User"), edtDbUser );
    l->addRow( tr("Password"), edtDbPassword );
    l->addRow( tr("Database name"), edtDbName );
    l->addRow( tr("Options"), edtDbOptions );
    l->addWidget( lStatus );

    // Buttons
    QDialogButtonBox * btnBox = new QDialogButtonBox(this);

    QPushButton * btn = new QPushButton(tr("Ok"), this);
    connect( btn, SIGNAL(clicked()), this, SLOT(save())  );
    connect( this, SIGNAL(destroyed()), btn, SLOT(deleteLater()) );
    btnBox->addButton( btn, QDialogButtonBox::AcceptRole );

    btn = new QPushButton( tr("Cancel"), this);
    connect( btn, SIGNAL(clicked()), this, SLOT(reject())  );
    connect( this, SIGNAL(destroyed()), btn, SLOT(deleteLater()) );
    btnBox->addButton( btn, QDialogButtonBox::RejectRole );

    btn = new QPushButton( tr("Test"), this );
    connect( btn, SIGNAL(clicked()), this, SLOT(apply())  );
    connect( this, SIGNAL(destroyed()), btn, SLOT(deleteLater()) );
    btnBox->addButton( btn, QDialogButtonBox::ApplyRole );

    connect( this, SIGNAL(destroyed()), btnBox, SLOT(deleteLater()) );

    l->addWidget(btnBox);

    setWindowTitle( tr("SQL Connection settings") );
    setWindowIcon( QIcon(":/icon/configure.png") );
}
//
DDbConfig::~DDbConfig()
{
    delete cbDriver;
    delete edtDbHost;
    delete edtDbName;
    delete edtDbOptions;
    delete edtDbPassword;
    delete sbDbPort;
    delete edtDbUser;

    delete l;
}
//
void DDbConfig::setProject(DProject * pro)
{
    project = pro;

    edtDbHost->setText( project->getDbHost() );
    edtDbName->setText( project->getDbName() );
    edtDbOptions->setPlainText( project->getDbConnectOptions() );
    edtDbPassword->setText( project->getDbPassword() );
    edtDbUser->setText( project->getDbUser() );
    sbDbPort->setValue( project->getDbPort() );

    qDebug("CHECKME: void DDbConfig::setProject(DProject*)");
    QString drv = project->getDbDriver();
    cbDriver->setCurrentIndex(0);
    for(int i = 1; i < cbDriver->count(); i++ )
        if (cbDriver->itemText(i) == drv) {
            cbDriver->setCurrentIndex(i);
            break;
        }
}
//
void DDbConfig::save()
{
    if (!apply()) {
        return;
    }
    project->setDbDriver( cbDriver->currentText() );
    project->setDbConnectOptions( edtDbOptions->toPlainText() );
    project->setDbHost( edtDbHost->text() );
    project->setDbName( edtDbName->text() );
    project->setDbPassword( edtDbPassword->text() );
    project->setDbPort( sbDbPort->value() );
    project->setDbUser( edtDbUser->text() );

    qDebug("TODO: can't save new Database configuration");
    if (project->getSql())
        project->disconnectDatabase();
    project->connectDatabase();
    emit accept();
}
//
bool DDbConfig::apply()
{
    bool isSql = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase( cbDriver->currentText(), "test" );
        db.setHostName( edtDbHost->text() );
        db.setDatabaseName( edtDbName->text() );
        db.setUserName( edtDbUser->text() );
        db.setPassword( edtDbPassword->text() );
        db.setPort( sbDbPort->value() );
        db.setConnectOptions( edtDbOptions->toPlainText() );

        isSql = db.open();

        if (!isSql) { // Error
            QSqlError err = db.lastError();
            QMessageBox::warning( this, tr("Not connected"),
                                 tr("Can't create connection to SQL server.\nError text: %1")
                                 .arg(err.text()) );
        } else {
            db.close();
            QMessageBox::information( this, tr("Connection sucsessfull"),
                                      tr("Connection to data base succsessfully created.") );
        }
    }
    QSqlDatabase::removeDatabase( "test" );

    return isSql;
}
