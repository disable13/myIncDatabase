#include "dthreadpool.h"
//
#include "dthreaduri.h"
#include "dproject.h"
#include <QObject>
//
DThreadPool::DThreadPool() :
     QThreadPool(QThreadPool::globalInstance())
{

}
//
DThreadPool::~DThreadPool()
{

}
//
void DThreadPool::start(DWidget*sender,QString uri)
{
    DThreadUri * thread = new DThreadUri( uri );

    (sender)->threadStarted( (int*)thread );

    connect( thread, SIGNAL(complete(int*,QVariant)),
            (sender), SLOT(threadEnd(int*,QVariant)) );

    ((QThreadPool*)this)->start( (QRunnable* )thread );
}
