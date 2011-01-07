#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

// UI
class QPushButton;
class QLabel;
class QGridLayout;
class QMenuBar;

class DProject;
class DHomeScreen;
class DFooter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DProject * current;

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

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DProject * getProject();

    bool loadProject( QString & filename);

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
