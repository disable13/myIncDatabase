#ifndef DHOMESCREEN_H
#define DHOMESCREEN_H

#include "src/dwidget.h"

class QGridLayout;
class QListWidget;
class DProject;

#include <QListWidgetItem>

class DHomeScreen : public DWidget
{
    Q_OBJECT
private:
    DProject * current;

    QGridLayout * l;

    QListWidget * lstBase;

public:
    DHomeScreen(QWidget * parent = 0);
    virtual ~DHomeScreen();

public slots:
    void clear();
    void setProject( DProject * );
    void selectWorkspace( QListWidgetItem * );

};

#endif // DHOMESCREEN_H
