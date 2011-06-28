#ifndef DSYSTEMFUNCS_H
#define DSYSTEMFUNCS_H
//
#include <QObject>
//
class DWorkWidget;
//
#include <QPair>
#include <QList>
#include <QStringList>
#include <QVariant>
//
class DSystemFuncs : public QObject
{
    Q_OBJECT

private:
    QList< QPair<QString,QString> > var;
    //
    QWidget* findWidget( QString name );
    //
    QVariant inline returnBool(bool);

public:
    DSystemFuncs();
    virtual ~DSystemFuncs();
    //
    bool openWidget(QString widget);
    void closeWidget(QString widget);
    bool refreshWidget(QString widget);
    // Variables
    QString getGlobalVariable(QString name);
    void setGlobalVariable(QString name,
                           QString value);
    QString getVariable(QString widget,
                        QString name);
    void setVariable(QString widget,
                     QString name, QString value);
    QVariant object(QString window, QString control, QString property);
    void setObject(QString window, QString control,  QString property ,QString value);

signals:
    void error( int );

public slots:
    QVariant run( QString func,
            QStringList arg = QStringList(), QObject * nativeSender = 0x00 );

};
//
#endif // DSYSTEMFUNCS_H
