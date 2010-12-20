#ifndef DHOMESCREEN_H
#define DHOMESCREEN_H

#include <QWidget>

class QGridLayout;
class QListWidget;
class DProject;

class DHomeScreen : public QWidget
{
    Q_OBJECT
private:
    DProject * current;

    QGridLayout * l;

    QListWidget * lstBase;

public:
    DHomeScreen(QWidget *parent = 0);
    ~DHomeScreen();

signals:

public slots:
    void clear();
    void setProject( DProject * );

};

#endif // DHOMESCREEN_H
