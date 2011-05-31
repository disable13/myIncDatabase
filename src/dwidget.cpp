#include "dwidget.h"
//
#include "src/core/myincapplication.h"
#include "src/core/dthreadpool.h"
//
DWidget::DWidget(QWidget *parent) :
    QWidget(parent)
{
    isThreadStart = false;
}
// FIXME: DWidget::~DWidget()\n\tKill runed threads.
DWidget::~DWidget()
{
    qDebug("FIXME: DWidget::~DWidget()\n\tKill runed threads.");
}
//
void DWidget::threadStart(QString uri,QWidget * widget)
{
    isThreadStart = false;

    QPair<QWidget*,int*> d;
    d.first = widget;
    threadList.push_back( d );
    MyIncApplication::threadPool()->start(this, uri);
    while (!isThreadStart) // wait for getting result
        MyIncApplication::application()->processEvents();
}
//
void DWidget::threadStarted(int * thread)
{
    isThreadStart = true;
    threadList.last().second = thread;
}
//
void DWidget::threadEnd(int* thread, QVariant result)
{
    for (int i = 0; i < threadList.count(); i++)
        if (threadList[i].second == thread) {
            // TODO: get Widget value and set to him result;
            if (threadList[i].first != 0x00)
                threadList[i].first->setProperty(
    qPrintable(threadList[i].first->property("PropertyValue").toString()),
                        result);
            threadList.removeAt(i);
            break;
        }
}
