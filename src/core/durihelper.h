#ifndef DURIHELPER_H
#define DURIHELPER_H
//
#include <QObject>
//
#include <QStringList>
//
class DUriHelper : public QObject
{
    Q_OBJECT
private:
    QRegExp * rx;
    //
    static QString uriMask;
    //
    QStringList * p_path;
    QStringList * p_args;

public:
    DUriHelper(QString query);
    ~DUriHelper();

private:
    void initPath();
    void initArg();

public:
    bool isUri();
    QString protocol();
    QString host();
    QString path();
    QString path(int);
    unsigned int pathItemsCount();
    QStringList args();
    QString arg(int);
    unsigned int argCount();

signals:
    void uriArgument(QString,QVariant*);

};
//
#endif // DURIHELPER_H
