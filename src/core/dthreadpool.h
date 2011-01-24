#ifndef DTHREADPOOL_H
#define DTHREADPOOL_H

#include <QThreadPool>
#include <QVariant>
#include "src/dwidget.h"
class DThreadUri;
class DProject;

class DThreadPool : public QThreadPool
{
    Q_OBJECT
public:
    DThreadPool();
    virtual ~DThreadPool();

signals:
    void started(int);
    void finished(int,QVariant);

public slots:
    void start(DWidget*,QString uri);

};

#endif // DTHREADPOOL_H
