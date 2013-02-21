#ifndef QABOUTWIDGET_H
#define QABOUTWIDGET_H
//
#ifndef HAVE_QT5
 #include <QtGui/QWidget>
#else
 #include <QtWidgets/QWidget>
#endif
//
class QGridLayout;
class QLabel;
class QPushButton;
//
class QAboutWidget : public QWidget
{
    Q_OBJECT
public:
    QAboutWidget();
    virtual ~QAboutWidget();

private:
    QGridLayout * l;
    //
    QLabel * lLogo;
    QLabel * lSource;
    QPushButton * btnOk;

private slots:
    void closeClick();

};

#endif // QABOUTWIDGET_H
