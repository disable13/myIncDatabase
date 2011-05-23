#ifndef DSQLQUERTYVIEWER_H
#define DSQLQUERTYVIEWER_H
//
#include <QWidget>
//
class QGridLayout;
class QPushButton;
class QLabel;
class QComboBox;
class QListWidget;
class QLineEdit;
class QTextEdit;
//
class DSqlQuertyViewer : public QWidget
{
    Q_OBJECT
private:
    QGridLayout * l;
    // colomn 0
    QLabel * lType;
    QComboBox * cbType;
    QListWidget * lstQuery;
    // colomn 1
    QLabel * lName;
    QLineEdit * leName;
    QLabel * lData;
    QTextEdit * teData;
    //
    QPushButton * btnSave;
    QPushButton * btnCancel;

public:
    DSqlQuertyViewer();
    ~DSqlQuertyViewer();

public slots:

};
//
#endif // DSQLQUERTYVIEWER_H