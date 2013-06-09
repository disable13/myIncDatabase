#ifndef DSQLQUERTYVIEWER_H
#define DSQLQUERTYVIEWER_H
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
    QPushButton * btnCancel;

public:
    DSqlQuertyViewer();
    virtual ~DSqlQuertyViewer();

public slots:
    void changeType(int);
    void selectQuery(QListWidgetItem*);
};
//
#endif // DSQLQUERTYVIEWER_H
