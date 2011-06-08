#ifndef DWORKWIDGET_H
#define DWORKWIDGET_H
//
#include <QWidget>
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
private:
    QWidget  * central;
    //
    QGridLayout * l;
    QString formName;

public:
    DWorkWidget(QString formName);
    virtual ~DWorkWidget();
    //
    bool init();
    bool initUri();

private:
    bool inline initButton(QWidget*);
    bool inline initLabel(QWidget*);
    bool inline initSpinBox(QWidget*);
    bool inline initDateTime(QWidget*);
    bool inline initList(QAbstractItemView*);
    bool inline initListItemChange(QAbstractItemView*);
    bool inline initChangeText(QWidget*);
    bool inline initComboBoxItems(QComboBox*);
    bool inline initComboBoxChange(QWidget*);

protected:
    int errorMessage( QString more );

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
