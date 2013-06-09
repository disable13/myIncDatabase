#ifndef DWORKWIDGET_H
#define DWORKWIDGET_H
//
#ifndef HAVE_QT5
# include <QtGui/QWidget>
#else
# include <QtWidgets/QWidget>
#endif
//
#include <QVariant>
#include <QModelIndex>
//
class QGridLayout;
class DProject;
class QAbstractItemView;
class QComboBox;
//
class DWorkWidget : public QWidget
{
    Q_OBJECT
public:
    DWorkWidget(const QString &formName);
    virtual ~DWorkWidget();

private:
    QWidget  * central;
    //
    QGridLayout * l;
    QString formName;

public:
    bool init();
    bool initUri();

public slots:
    bool refreshUri();

private:
    bool initButton(QWidget*);
    bool initLabel(QWidget*);
    bool initSpinBox(QWidget*);
    bool initDateTime(QWidget*);
    bool initList(QAbstractItemView*);
    bool initListItemChange(QAbstractItemView*);
    bool initChangeText(QWidget*);
    bool initComboBoxItems(QComboBox*);
    bool initComboBoxChange(QWidget*);

protected:
    static int errorMessage( const QString &more );

private slots:
    void clickEvent();
    void changeTextEvent();
    void changePlainTextEvent();
    void changeItemIndex(QString);
    void changeItemIndex(int);
    void listItemChange(QModelIndex);

signals:
    void uri(QString,QVariant*);

};
//
#endif // DWORKWIDGET_H
