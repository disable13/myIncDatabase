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
#include "../core.h"
//
class DSystemFuncs : public QObject
{
    Q_OBJECT
public:
    DSystemFuncs();
    virtual ~DSystemFuncs();

private:
    QList< QPair<QString,QString> > var;

protected:
    static QWidget* findWidget( const QString &name );

public:
    static bool openWidget(const QString &widget);
    static void closeWidget(const QString &widget);
    static bool refreshWidget(const QString &widget);
    // Variables
    QString getGlobalVariable(const QString &name) const;
    void    setGlobalVariable(const QString &name, const QString &value);
    //
    static QString getVariable(const QString &widget, const QString &name);
    static void    setVariable(const QString &widget,
                        const QString &name,
                        const QString &value
                        );
    //
    static QVariant object(const QString &window,
                    const QString &control,
                    const QString &property
                    );
    static void    setObject(const QString &window,
                      const QString &control,
                      const QString &property,
                      const QString &value
                      );

signals:
    void error( int );

public slots:
    QVariant run(const QString &func,
                 const QStringList &arg = QStringList(),
                 const QObject *nativeSender = NULL
            );

};
//
#endif // DSYSTEMFUNCS_H
