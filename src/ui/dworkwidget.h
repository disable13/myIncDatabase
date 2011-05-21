#ifndef DWORKWIDGET_H
#define DWORKWIDGET_H

#include "src/dwidget.h"
//
class QGridLayout;
class DProject;
//
class DWorkWidget : public DWidget
{
    Q_OBJECT
private:
    QWidget  * central;

    QGridLayout * l;
    QString formName;

public:
    DWorkWidget(QString formName);
    ~DWorkWidget();

    bool init();

protected:
    int errorMessage( QString more );

};
//
#endif // DWORKWIDGET_H
