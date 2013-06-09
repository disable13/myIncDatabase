#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//
#ifndef HAVE_QT5
# include <QtGui/QMainWindow>
#else
# include <QtWidgets/QMainWindow>
#endif
//
class QCloseEvent;
// UI
class QPushButton;
class QLabel;
class QGridLayout;
class QMenuBar;
//
class DHomeScreen;
class DFooter;
class DSqlQuertyViewer;
class DDebugWidget;
class QSystemTrayIcon;
//
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
    //QAction * actSave;
    //QAction * actSaveAs;
    QAction * actClose;
    QAction * actExit;
    QMenu *  menProject;
    QAction * actConnect;
    QAction * actDbSettings;
    QAction * actQuerySettings;
    QAction * actUiSettings;
    QMenu *  menHelp;
    QAction * actHelp;
    QAction * actAbout;
    QAction * actAboutQt;
    // Tray
    QSystemTrayIcon * tray;
    QMenu * trayMenu;
    //
    DHomeScreen * home;
    DFooter * footer;
    DDebugWidget * debug;
    DSqlQuertyViewer * sqlQuertyViewer;
    //
    bool isConnected;
    bool isOpened; // to ~MainWindow

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    //
    bool loadProject( QString & filename);
    DHomeScreen * getHome();
    bool openManualWorkspace();

protected:
     void closeEvent(QCloseEvent *);
     void showEvent(QShowEvent *);

public slots:
    void lockUI( bool );
    void showHideMain();
    void createProject();
    void openProjectPush();
    void openConnectionSettings();
    void openQuerySettings();
    void openUiSettings();
    void openDebug();
    void connectDatabase();
    void closeProject();
    void error(int);
    void about();

};
//
#endif // MAINWINDOW_H
