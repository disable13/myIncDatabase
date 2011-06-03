#ifndef DAUTH_H
#define DAUTH_H
//
#include <QObject>
//
#include <QString>
//
class DAuth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool Authed
               READ isAuthed)

private:
    QString p_current; // for user name currenty login in.
    bool p_isAuthed;

public:
    DAuth(QObject *parent = 0);
    virtual ~DAuth();
    //
    bool isUser(QString name);
    QString userPasswordHash(QString name);
    //
    bool isAuthed();
    bool setAuth(QString user, QString password);
    bool setAuth(QWidget*parentForm);
    void setAuth(bool);

};
//
#endif // DAUTH_H
