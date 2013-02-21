#ifndef DDEBUGWIDGET_H
#define DDEBUGWIDGET_H
//
#ifndef HAVE_QT5
 #include <QtGui/QWidget>
 #include <QtGui/QListWidgetItem>
#else
 #include <QtWidgets/QWidget>
 #include <QtWidgets/QListWidgetItem>
#endif
//
#include "../core/ddebug.h"
//
class QGridLayout;
class QListWidget;
class QMenu;
class QAction;
//
class DDebugWidget : public QWidget
{
    Q_OBJECT
public:
    DDebugWidget(QWidget *parent = 0);
    virtual ~DDebugWidget();

private:
    int max;
    QGridLayout * l;
    //
    QListWidget * lstUri;
    QMenu * popMenu;
    QAction * actOpenUri;
    QAction * actobjView;
    QAction * actShError;
    QAction * actShNull;
    QAction * actShSuccessful;
    //
    QPoint last;

private slots:
    void openUri();
    void openObjView();
    //
    void shError();
    void shSuccssesful();
    void shNull();
    void openPopup(QPoint);

public slots:
    void uriError( DUriQuery );
    void uriNull( DUriQuery );
    void uriSuccessful( DUriQuery );

};

#endif // DDEBUGWIDGET_H
