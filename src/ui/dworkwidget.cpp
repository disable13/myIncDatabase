#include "dworkwidget.h"
//
#include <QGridLayout>
#include <QtUiTools/QUiLoader>
#include <QMessageBox>
#include <QVariant>
#include <QFile>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
//
#include <QtGui>
//
#include "src/core/myincapplication.h"
//
DWorkWidget::DWorkWidget(QString formName) :
    QWidget(), formName(formName)
{
    central = 0x00;
    l = new QGridLayout( this );
    setLayout( l );
}
//
DWorkWidget::~DWorkWidget()
{
    if (central)
        delete central;
    delete l;
}
//
bool DWorkWidget::init()
{
    // TODO: DWorkWidget::init()
    qDebug( "TODO: DWorkWidget::init() " );

    if (MyIncApplication::project() == 0x00) {
        errorMessage( tr("Project not be opened") );
        return false;
    }
    try {
        QUiLoader loader;
        QFile f( formName );
        if (!f.open(QFile::ReadOnly)){
            errorMessage( tr("Can't read UI file ").append(formName) );
            return false;
        }
        central = loader.load(&f, this);
        f.close();
    } catch (...) {
        errorMessage( tr("Bad UI file") );
    }

    setObjectName( formName );

    initUri();

    l->addWidget( central );
    return true;
}
//
bool DWorkWidget::initUri()
{
    const QObjectList childs = central->children();
    for(int i = 0; i < childs.count(); i++ ) {
        QWidget * obj = (QWidget*)childs.at(i);

        if (obj->inherits("QAbstractButton" )) { /// Button
            if (!initLabel(obj) || !initButton(obj))
                errorMessage( tr("Can't create button %1").arg(obj->objectName()));
        } else if (obj->inherits("QLabel")) { /// Label
            initLabel(obj);
        } else if ( obj->inherits("QAbstractItemView")) { /// tables and etc
            if (!initList(static_cast<QAbstractItemView*>(obj)))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QLineEdit") ) { /// edit
            initLabel(obj); initChangeText(obj);
        } else if ( obj->inherits("QTextEdit") ) { /// text edit
            initLabel(obj); initChangeText(obj);
        } else if ( obj->inherits("QComboBox") ) { /// combo box

        } else if ( obj->inherits("QDateTimeEdit") ) { /// edit controls for time
            if (!initDateTime(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if (obj->inherits( "QSpinBox") || obj->inherits("QDoubleSpinBox")) { /// numbers edit
            if (!initSpinBox(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QCheckBox")) { /// check box
            initLabel(obj);
        }/* else if (obj->inherits("QProgressBar")) { /// progress bar

        }*/
    }
    return true;
}
// TODO: need finish
bool DWorkWidget::initButton(QWidget * w)
{
    QAbstractButton * b = (QAbstractButton *)w;
    QVariant value;
    value = w->property( "OnClick" );
    if (!value.isNull())
        connect( b, SIGNAL(clicked()),
                this, SLOT(clickEvent()) );

    return true;
}
//
bool DWorkWidget::initLabel(QWidget * w)
{
    QVariant  v;
    QVariant value = w->property( "TextValue" );
    try {
        if (value.isValid()) {
            emit uri( value.toString().trimmed(), &v  );
            w->setProperty( "text", v );
        }
    } catch (...) {
        return false;
    }

    return true;
}
bool DWorkWidget::initSpinBox(QWidget * w)
{
    QVariant  v;
    QVariant value;
    try {
        value = w->property( "NumValue" );
        if (value.isValid()) {
            emit uri( value.toString().trimmed(), &v  );
            if (v.convert( w->inherits("QDoubleSpinBox") ?
                          QVariant::Int : QVariant::Double ))
                return false;
            w->setProperty( "value", v );
        }
    } catch (...) {
        return false;
    }

    return true;
}
//
bool DWorkWidget::initDateTime(QWidget * w )
{
    QVariant  v; // to result
    QVariant value;
    try {
        value = w->property( "DateValue" );
        if (value.isValid()) {
            emit uri( value.toString().trimmed(), &v  );
            if (v.type() == QVariant::String) {
                v = QDateTime::fromString(v.toString());
                if (!v.toDateTime().isValid())
                    return false;
            }
            if ((v.type() == QVariant::DateTime)||(v.type() == QVariant::Date)
                    ||(v.type()==QVariant::Time))
                w->setProperty( "dateTime", v );
            else
                return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}
//
bool DWorkWidget::initList(QAbstractItemView * w)
{
    QVariant  v;
    QVariant value = w->property( "Model" );
    try {
        if (value.isValid()) {
            emit uri( value.toString().trimmed(), &v  );
            QSqlQueryModel * model = new QSqlQueryModel();

            model->setQuery( ((QSqlQuery*)v.value<void*>())[0] );

            w->setModel( static_cast<QAbstractItemModel*>(model) );
        }
    } catch (...) {
        return false;
    }

    return true;
}
//
bool DWorkWidget::initChangeText(QWidget * w)
{
    QVariant value = w->property( "OnChangeText" );
    try {
        if (value.isValid()) {
            if (w->inherits("QTextEdit"))
                connect( w, SIGNAL(textChanged()),
                        this, SLOT(changePlainTextEvent()) );
            else
                connect( w, SIGNAL(textChanged()),
                        this, SLOT(changeTextEvent()) );
        }
    } catch (...) {
        return false;
    }

    return true;
}
//
bool DWorkWidget::initComboBoxItems(QWidget * w)
{
    QVariant  v;
    QVariant value = w->property( "Model" );
    try {
        if (value.isValid()) {
            emit uri( value.toString().trimmed(), &v  );

            switch (v.type()) {
            case QVariant::StringList:
                break;
            case QMetaType::VoidStar:
                break;
            }
        }
    } catch (...) {
        return false;
    }

    return true;
}
//
int DWorkWidget::errorMessage(QString more)
{
    QMessageBox msg;
    msg.setIcon( QMessageBox::Critical );
    msg.setStandardButtons( QMessageBox::Retry | QMessageBox::Cancel
                           | QMessageBox::Ignore);
    msg.setDefaultButton( QMessageBox::Cancel );
    msg.setText( tr("Work widget error") );
    msg.setDetailedText( more );

    return msg.exec();
}
//
void DWorkWidget::clickEvent()
{
    emit uri( sender()->property( "OnClick" ).toString(), new QVariant() );
}
//
void DWorkWidget::changeTextEvent()
{
    emit uri( sender()->property( "OnChangeText" ).toString()
             .arg(sender()->property( "text" ).toString() ), new QVariant() );
}
//
void DWorkWidget::changePlainTextEvent()
{
    emit uri( sender()->property( "OnChangeText" ).toString()
             .arg(sender()->property( "plainText" ).toString() ), new QVariant() );
}
