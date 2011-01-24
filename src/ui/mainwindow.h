#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>


class QCloseEvent;
// UI
class QPushButton;
class QLabel;
class QGridLayout;
class QMenuBar;

class DHomeScreen;
class DFooter;


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget * central;
    QGridLayout * l;
    QMenuBar * menuBar;
    QMenu * menFile;
    QAction * actCreateProject;
    QAction * actOpenProject;
    QAction * actSave;
    QAction * actSaveAs;
    QAction * actClose;
    QAction * actExit;
    QMenu *  menProject;
    QAction * actConnect;
    QAction * actDbSettings;
    QAction * actQuerySettings;
    QAction * actUiSettings;

    DHomeScreen * home;
    DFooter * footer;

    bool isConnected;
    bool isOpened; // to ~MainWindow

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool loadProject( QString & filename);

protected:
     void closeEvent( QCloseEvent * event );

public slots:
    void lockUI( bool );
    void createProject();
    void openProjectPush();
    void openConnectionSettings();
    void openQuerySettings();
    void openUiSettings();
    void connectDatabase();
    void error(int);

};

#endif // MAINWINDOW_H
