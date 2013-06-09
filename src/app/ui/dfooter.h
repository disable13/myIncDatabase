#ifndef DFOOTER_H
#define DFOOTER_H
//
#ifndef HAVE_QT5
# include <QtGui/QWidget>
#else
# include <QtWidgets/QWidget>
#endif
//
class QGridLayout;
class QProgressIndicator;
class QProgressBar;
class QLabel;
//
#include "../core.h"
//
class DFooter : public QWidget
{
    Q_OBJECT
public:
    DFooter(QWidget *parent = 0);
    virtual ~DFooter();

private:
    QGridLayout * l;
    //
    QProgressIndicator * progressInicator;
    QProgressBar * progressBar;
    QLabel * lText;
    //
    int max;

public slots:
    void setText( const QString &);
    void progressStart(); // for indicator
    void progressStart(const int &); // max
    void progressStart(const QString &);
    void progressStart(const QString &, const int &);
    void progressChange(const int&); // now value. to stop set max value
    void progressStop(); // for indicator (unknow result)
    void progressStop(const bool&); // for good or bad result

};
//
#endif // DFOOTER_H
