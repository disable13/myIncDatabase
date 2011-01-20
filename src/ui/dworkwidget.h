#ifndef DWORKWIDGET_H
#define DWORKWIDGET_H

#include <QWidget>
//#include <QList>

class QGridLayout;
class DProject;

class DWorkWidget : public QWidget
{
    Q_OBJECT
private:
    QWidget  * central;
    DProject * current;

    QGridLayout * l;
    QString formName;

public:
    DWorkWidget( QString formName);
    ~DWorkWidget();

    bool init();
    void setProject(DProject * );

};

#endif // DWORKWIDGET_H
