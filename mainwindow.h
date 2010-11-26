#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

// UI
class QPushButton;
class QLabel;
class QGridLayout;
class QMenuBar;

class DProject;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DProject * current;

    QGridLayout * l;
    QMenuBar * menuBar;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
