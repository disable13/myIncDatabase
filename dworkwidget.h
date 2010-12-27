#ifndef DWORKWIDGET_H
#define DWORKWIDGET_H

#include <QWidget>
#include <QList>

class DProject;
class QGridLayout;

class DWorkWidget : public QWidget
{
    Q_OBJECT
private:
    DProject * current;

    QGridLayout * l;
    QWidget * central;
    QString formName;

public:
    DWorkWidget(DProject * pro, QString & formName);
    ~DWorkWidget();

    bool init();

signals:

public slots:

};

#endif // DWORKWIDGET_H
