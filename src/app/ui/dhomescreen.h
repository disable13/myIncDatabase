#ifndef DHOMESCREEN_H
#define DHOMESCREEN_H
//
#ifndef HAVE_QT5
# include <QtGui/QWidget>
# include <QtGui/QListWidgetItem>
#else
# include <QtWidgets/QWidget>
# include <QtWidgets/QListWidgetItem>
#endif
//
class QGridLayout;
class QListWidget;
//
class DProject;
class DWorkWidget;
//
class DHomeScreen : public QWidget
{
    Q_OBJECT
public:
    DHomeScreen(QWidget * parent = 0);
    virtual ~DHomeScreen();

private:
    DProject * current;
    QList<DWorkWidget*> listWidget;
    //
    QGridLayout * l;
    //
    QListWidget * lstBase;

public:
    bool selectManualWorkspace();
    bool selectWorkspace(const QString &name);

public slots:
    void clear();
    void setProject( DProject * );
    void selectWorkspace( QListWidgetItem * );

};
//
#endif // DHOMESCREEN_H
