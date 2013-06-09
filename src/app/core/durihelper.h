#ifndef DURIHELPER_H
#define DURIHELPER_H
//
#include <QObject>
//
#include <QStringList>
//
#include "../core.h"
//
class DUriHelper : public QObject
{
    Q_OBJECT
public:
    DUriHelper(const QString &query);
    virtual ~DUriHelper();

private:
    QRegExp* rx;
    //
    static const QString uriMask;
    //
    QStringList* p_path;
    QStringList* p_args;

protected:
    void initPath();
    void initArg();

public:
    bool    isUri() const;
    QString protocol() const;
    QString host() const;
    QString path() const;
    quint32 pathItemsCount();
    QString path(const int&);
    QStringList args();
    QString arg(const int&);
    quint32 argCount();

signals:
    void uriArgument(QString,QVariant*);

};
//
#endif // DURIHELPER_H
