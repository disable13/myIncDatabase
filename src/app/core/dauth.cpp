#include "dauth.h"
//
#include "myincapplication.h"
#include "dnamespace.h"
//
#include <QtCore/QCryptographicHash>
// GUI
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>
//
DAuth::DAuth(QObject *parent) :
    QObject(parent), p_isAuthed(false)
{
    setObjectName("Auth");
}
//
DAuth::~DAuth()
{

}
//
bool DAuth::isUser(QString name)
{
    name = name.trimmed().toLower();
    int count = MyIncApplication::uriNamespace()->configSize( "Users" );
    for (int i = 0; i < count; i++)
        if ( MyIncApplication::uriNamespace()
                ->config( "Users", QString("User_%1").arg(i)).toLower() == name ) {
            if ((name == "error") || (name == "null")) // block system names
                return false;
            return true;
        }
    return false;
}
//
QString DAuth::userPasswordHash(QString name)
{
    name = name.trimmed().toLower();
    if (!isUser(name)) // if user not exist then no hash
        return QByteArray();
    return MyIncApplication::uriNamespace()->config( name, "Password" );
}
//
bool DAuth::isAuthed()
{
    return p_isAuthed;
}
//
bool DAuth::setAuth(QString user, QString password)
{
    QByteArray hash = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Md5);
    hash = hash.toHex();
    p_isAuthed = ( QString(hash) == userPasswordHash( user ).toLower() );

    return p_isAuthed;
}
//
bool DAuth::setAuth(QWidget *parentForm)
{
    QDialog d(parentForm, Qt::Dialog);
    QGridLayout l(&d);
    d.setLayout( &l );
    d.setWindowTitle( tr("Authentication") );

    QLabel lUser( tr("User"), &d );
    l.addWidget( &lUser );
    QLabel lPass( tr("Password"), &d);
    l.addWidget( &lPass, 1, 0);
    QLineEdit leUser( &d );
    leUser.setMaxLength( 16 );
    l.addWidget(&leUser, 0, 1, 1, 2);
    QLineEdit lePass( &d );
    lePass.setMaxLength( 32 );
    lePass.setEchoMode( QLineEdit::Password );
    l.addWidget( &lePass, 1, 1, 1, 2);
    QPushButton bOk( tr("Login"), &d );
    bOk.setDefault( true );
    l.addWidget( &bOk, 2, 1 );
    QPushButton bCancel( tr("Cancel"), &d );
    l.addWidget( &bCancel, 2, 2 );
    connect( &bOk, SIGNAL(clicked()),
            &d, SLOT(accept()) );
    connect( &bCancel, SIGNAL(clicked()),
            &d, SLOT(reject()) );

    int trying = 5;
    while ( !p_isAuthed && (trying != 0)) {
        if (d.exec() == QDialog::Rejected) {
            p_isAuthed = false;
            break;
        }
        if (leUser.text().trimmed().length() < 4) {
            leUser.setFocus();
            QMessageBox::warning( &d, tr("Authentication"),
                                 tr("User name too short") );
            continue;
        }
        if (lePass.text().trimmed().length() < 4)  {
            lePass.clear();
            lePass.setFocus();
            QMessageBox::warning( &d, tr("Authentication"),
                                 tr("User name too short") );
            continue;
        }
        trying--;
        if (!setAuth(leUser.text().trimmed().toLower(),
                lePass.text().trimmed())) {
            lePass.clear();
            QMessageBox::warning( &d, tr("Authentication"),
                                 tr("Bad user name or wrong password.") );
        }
    }

    return p_isAuthed;
}
//
void DAuth::setAuth(bool a)
{
    p_isAuthed = a;
}
