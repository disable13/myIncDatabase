#ifndef DHOMESCREEN_H
#define DHOMESCREEN_H

#include <QWidget>

class QGridLayout;
class QListWidget;
class DProject;

#include <QListWidgetItem>

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

public slots:
    void clear();
    void setProject( DProject * );
    void selectWorkspace(QListWidgetItem*);

};

#endif // DHOMESCREEN_H
