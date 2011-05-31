#ifndef DHOMESCREEN_H
#define DHOMESCREEN_H
//
#include <QWidget>
//
class QGridLayout;
class QListWidget;
class DProject;
//
#include <QListWidgetItem>
//
class DWorkWidget;
//
class DHomeScreen : public QWidget
{
    Q_OBJECT
private:
    DProject * current;
    QList<DWorkWidget*> listWidget;
    //
    QGridLayout * l;
    //
    QListWidget * lstBase;

public:
    DHomeScreen(QWidget * parent = 0);
    virtual ~DHomeScreen();
    //
    bool selectWorkspace(QString name);

public slots:
    void clear();
    void setProject( DProject * );
    void selectWorkspace( QListWidgetItem * );

};
//
#endif // DHOMESCREEN_H
