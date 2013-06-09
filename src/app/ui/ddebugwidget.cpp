#include "ddebugwidget.h"
//
#include "../core/myincapplication.h"
#include "../core/ddebug.h"
//
#ifndef HAVE_QT5
 #include <QtGui/QListWidget>
 #include <QtGui/QGridLayout>
 #include <QtGui/QMenu>
 #include <QtGui/QAction>
#else
 #include <QtWidgets/QListWidget>
 #include <QtWidgets/QGridLayout>
 #include <QtWidgets/QMenu>
 #include <QtWidgets/QAction>
#endif
// TODO: upload previos debug data
DDebugWidget::DDebugWidget(QWidget *parent) :
    QWidget(parent, Qt::Tool )
{
    qDebug("DDebugWidget::DDebugWidget(QWidget *)");

    setObjectName( "DebugWidget" );
    setWindowTitle( tr("Debug tool") );
    setWindowIcon( QIcon(":/icon/develop.png") );

    max = MIA_DEBUG->getMax();

    l = new QGridLayout( this );
    setLayout( l );

    lstUri = new QListWidget( this );
    lstUri->setContextMenuPolicy( Qt::CustomContextMenu );
    l->addWidget(lstUri);

    popMenu = new QMenu( "URI" );

    actOpenUri = popMenu->addAction( tr("Open in URI viewer"), this,
                        SLOT(openUri()) );
    actobjView = popMenu->addAction( tr(" Open in object viewer"), this,
                        SLOT(openObjView()) );
    popMenu->addSeparator();
    actShError = popMenu->addAction( QIcon(":/icon/cancel.png"),
                        tr("Show/Hide URI with ").append("error"),
                        this, SLOT(shError()) );
    actShNull = popMenu->addAction( QIcon(":/icon/info.png"),
                        tr("Show/Hide URI with ").append("NULL"),
                        this, SLOT(shNull()) );
    actShSuccessful = popMenu->addAction( QIcon(":/icon/apply.png"),
                        tr("Show/Hide URI with ").append("successful"),
                        this, SLOT(shSuccssesful()) );

    actShError->setCheckable( true );
    actShNull->setCheckable( true );
    actShSuccessful->setCheckable( true );

    connect( MIA_DEBUG, SIGNAL(uriError(DUriQuery)),
             this, SLOT(uriError(DUriQuery)) );
    connect( MIA_DEBUG, SIGNAL(uriNull(DUriQuery)),
             this, SLOT(uriNull(DUriQuery)) );
    connect( MIA_DEBUG, SIGNAL(uriSuccessful(DUriQuery)),
             this, SLOT(uriSuccessful(DUriQuery)) );
    connect( lstUri, SIGNAL(customContextMenuRequested(QPoint)),
             this, SLOT(openPopup(QPoint)) );

}
//
DDebugWidget::~DDebugWidget()
{
    FREE_MEM(lstUri);
    FREE_MEM(l);

    FREE_MEM(popMenu);
}
// TODO: create form
void DDebugWidget::openObjView()
{
    qDebug("void DDebugWidget::openObjView()");
}
//
void DDebugWidget::openUri()
{
    qDebug("void DDebugWidget::openUri()");
}
//
void DDebugWidget::shError()
{
    for(int i = 0; i < lstUri->count(); i++) {
        QVariant v = lstUri->item(i)->data(Qt::UserRole);
        DUriQuery * q = static_cast<DUriQuery *>(v.value<void*>());
        if (q->result.toString()== "Error") {
            QAction * act = static_cast<QAction*>(sender());
            lstUri->item(i)->setHidden( ! act->isChecked() );
            act->setChecked( ! act->isChecked() );
        }
    }
}
//
void DDebugWidget::openPopup(QPoint p)
{
    last = p;
    popMenu->popup( lstUri->mapToGlobal(p) );
    QListWidgetItem * item = lstUri->itemAt( p );
    actOpenUri->setEnabled( item != NULL);
    actobjView->setEnabled( item != NULL);
}

//
void DDebugWidget::shNull()
{
    for(int i = 0; i < lstUri->count(); i++) {
        QVariant v = lstUri->item(i)->data(Qt::UserRole);
        DUriQuery * q = static_cast<DUriQuery *>(v.value<void*>());
        if (q->result.toString()== "NULL") {
            QAction * act = static_cast<QAction*>(sender());
            lstUri->item(i)->setHidden( ! act->isChecked() );
            act->setChecked( ! act->isChecked() );
        }
    }
}
//
void DDebugWidget::shSuccssesful()
{
    for(int i = 0; i < lstUri->count(); i++) {
        QVariant v = lstUri->item(i)->data(Qt::UserRole);
        DUriQuery * q = static_cast<DUriQuery *>(v.value<void*>());
        if (q->result.toString()== "NULL") {
            QAction * act = static_cast<QAction*>(sender());
            lstUri->item(i)->setHidden( ! act->isChecked() );
            act->setChecked( ! act->isChecked() );
        }
    }
}
//
void DDebugWidget::uriError(DUriQuery u)
{
    QListWidgetItem * item = new QListWidgetItem();
    item->setText( QString("%2 - %1").arg(u.sender->objectName())
                   .arg(u.time.toString("hh:mm:ss.zzz")) );
    item->setIcon( QIcon(":/icon/cancel.png") );
    QVariant v;
    v.setValue(static_cast<void*>(
                   new DUriQuery(u))
               );
    item->setData(Qt::UserRole, v);
    lstUri->insertItem(0, item);
    if (max < lstUri->count())
        lstUri->removeItemWidget(lstUri->item(lstUri->count() - 1));

}
//
void DDebugWidget::uriNull(DUriQuery u)
{
    QListWidgetItem * item = new QListWidgetItem();
    item->setText( QString("%2 - %1").arg(u.sender->objectName())
                   .arg(u.time.toString("hh:mm:ss.zzz")) );
    item->setIcon( QIcon(":/icon/info.png") );
    QVariant v;
    v.setValue(static_cast<void*>(
                   new DUriQuery(u))
               );
    item->setData(Qt::UserRole, v);
    lstUri->insertItem(0, item);
    if (max < lstUri->count())
        lstUri->removeItemWidget(lstUri->item(lstUri->count() - 1));
}
//
void DDebugWidget::uriSuccessful(DUriQuery u)
{
    QListWidgetItem * item = new QListWidgetItem();
    item->setText( QString("%2 - %1").arg(u.sender->objectName())
                   .arg(u.time.toString("hh:mm:ss.zzz")) );
    item->setIcon( QIcon(":/icon/apply.png") );
    QVariant v;
    v.setValue(static_cast<void*>(
                   new DUriQuery(u))
               );
    item->setData(Qt::UserRole, v);
    lstUri->insertItem(0, item);
    if (max < lstUri->count())
        lstUri->removeItemWidget(lstUri->item(lstUri->count() - 1));
}
