#ifndef DFOOTER_H
#define DFOOTER_H
//
#include <QWidget>
//
class QGridLayout;
class QProgressIndicator;
class QProgressBar;
class QLabel;
//
class DFooter : public QWidget
{
    Q_OBJECT
public:
    DFooter(QWidget *parent = 0);
    ~DFooter();

private:
    QGridLayout * l;
    //
    QProgressIndicator * progressInicator;
    QProgressBar * progressBar;
    QLabel * lText;
    //
    int max;

public slots:
    void setText( QString );
    void progressStart(); // for indicator
    void progressStart(int); // max
    void progressStart(QString);
    void progressStart(QString,int);
    void progressChange(int); // now value. to stop set max value
    void progressStop(); // for indicator (unknow result)
    void progressStop(bool); // for good or bad result

};
//
#endif // DFOOTER_H
