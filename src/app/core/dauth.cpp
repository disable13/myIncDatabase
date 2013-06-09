#include "dauth.h"
//
#include "myincapplication.h"
#include "dnamespace.h"
//
#include <QtCore/QCryptographicHash>
// GUI
#ifndef HAVE_QT5
# include <QtGui/QDialog>
# include <QtGui/QLineEdit>
# include <QtGui/QLabel>
# include <QtGui/QPushButton>
# include <QtGui/QGridLayout>
# include <QtGui/QMessageBox>
#else
# include <QtWidgets/QDialog>
# include <QtWidgets/QLineEdit>
# include <QtWidgets/QLabel>
# include <QtWidgets/QPushButton>
# include <QtWidgets/QGridLayout>
# include <QtWidgets/QMessageBox>
#endif
//
const QString g_sUsers = "Users";
//
DAuth::DAuth(QObject *parent) :
    QObject(parent), m_Auth(false)
{
    setObjectName("Auth");
}
//
DAuth::~DAuth()
{ }
//
bool DAuth::isUser(const QString &inName) const
{
    QString name = inName.trimmed().toLower();
    int count = MIA_NAMESPACE->configSize( g_sUsers );
    for (int i = 0; i < count; i++)
        if ( MIA_NAMESPACE->config( g_sUsers,
                                    QString("User_%1").arg(i) ).toLower()
             == name ) {
            if ((name == "error") || (name == "null")) // block system names
                return false;
            return true;
        }
    return false;
}
//
QString DAuth::userPasswordHash(const QString& inName)
{
    QString name = inName.trimmed().toLower();
    if (!isUser(name)) // if user not exist then no hash
        return QByteArray();
    return MIA_NAMESPACE->config( name, "Password" );
}
//
bool DAuth::setAuth(const QString& user,
                    const QString& password)
{
    QByteArray hash = QCryptographicHash::hash(password.toLocal8Bit(), QCryptographicHash::Md5);
    hash = hash.toHex();
    m_Auth = ( QString(hash) == userPasswordHash( user ).toLower() );

    return m_Auth;
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
    while ( !m_Auth && (trying != 0)) {
        if (d.exec() == QDialog::Rejected) {
            m_Auth = false;
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

    return m_Auth;
}
