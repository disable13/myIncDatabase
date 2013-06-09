#ifndef DAUTH_H
#define DAUTH_H
//
#include <QObject>
//
#include <QString>
//
#include "../core.h"
//
class DAuth : public QObject
{
    Q_OBJECT
public:
    DAuth(QObject *parent = 0);
    virtual ~DAuth();

private:
    DQ_PROPERTY_RW(bool, Auth,    private)

public:
    bool    isUser(const QString& name) const;
    QString userPasswordHash(const QString& name);
    //
    bool setAuth(const QString& user, const QString& password);
    bool setAuth(QWidget*parentForm);

};
//
#endif // DAUTH_H
