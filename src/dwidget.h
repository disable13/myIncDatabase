#ifndef DWIDGET_H
#define DWIDGET_H

#include <QWidget>
#include <QVariant>

#include <QPair>
#include <QList>

class DWidget : public QWidget
{
    Q_OBJECT

    friend class DWorkWidget;

private:
    QList< QPair<QWidget*,int*> > threadList;
    bool isThreadStart;

public:
    DWidget(QWidget *parent = 0x00);
    virtual ~DWidget();

private:
    void threadStart(QString uri,QWidget* widget = 0x00);

public slots:
    void threadStarted(int*);
    void threadEnd(int*,QVariant);

};

#endif // DWIDGET_H
