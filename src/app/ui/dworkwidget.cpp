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
#ifndef HAVE_QT5
#include <QtGui>
#else
#include <QtWidgets>
#endif
//
#include "../core/myincapplication.h"
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

    setObjectName( QString("ww_").append(central->objectName()) );

    initUri();

    setWindowTitle( central->windowTitle() );
    setWindowIcon( central->windowIcon() );

    l->addWidget( central );
    return true;
}
//
bool DWorkWidget::initUri()
{
    try {
        QVariant  v;
        QVariant value = central->property( "InitMe" );
        if (value.isValid()) {
            emit uri( value.toString().trimmed(), &v  );
        }
    } catch (...) {
        errorMessage("Can't be initialized");
    }

    const QObjectList childs = central->children();
    for(int i = 0; i < childs.count(); i++ ) {
        QWidget * obj = static_cast<QWidget*>(childs.at(i));

        if (obj->inherits("QAbstractButton" )) { /// Button
            if (!initLabel(obj) || !initButton(obj))
                errorMessage( tr("Can't create button %1").arg(obj->objectName()));
        } else if (obj->inherits("QLabel")) { /// Label
            if (!initLabel(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QAbstractItemView")) { /// tables and etc
            if (!initList(static_cast<QAbstractItemView*>(obj)))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QLineEdit") ) { /// edit
            if (!initLabel(obj) || !initChangeText(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QTextEdit") ) { /// text edit
            if (!initLabel(obj) || !initChangeText(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QComboBox") ) { /// combo box
            if (!initComboBoxItems(static_cast<QComboBox*>(obj)) ||
                    !initComboBoxChange(obj) ||
                    !initChangeText(obj) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QDateTimeEdit") ) { /// edit controls for time
            if (!initDateTime(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if (obj->inherits( "QSpinBox") || obj->inherits("QDoubleSpinBox")) { /// numbers edit
            if (!initSpinBox(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QCheckBox")) { /// check box
            if (!initLabel(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        }/* else if (obj->inherits("QProgressBar")) { /// progress bar

        }*/
    }
    return true;
}
//
bool DWorkWidget::refreshUri()
{
    const QObjectList childs = central->children();
    for(int i = 0; i < childs.count(); i++ ) {
        QWidget * obj = static_cast<QWidget*>(childs.at(i));

        if (obj->inherits("QAbstractButton" )) { /// Button
            if ( !initLabel(obj) )
                errorMessage( tr("Can't create button %1").arg(obj->objectName()));
        } else if (obj->inherits("QLabel")) { /// Label
            if ( !initLabel(obj))
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QAbstractItemView")) { /// tables and etc
            if ( !initList(static_cast<QAbstractItemView*>(obj)) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QLineEdit") ) { /// edit
            if ( !initLabel(obj) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QTextEdit") ) { /// text edit
            if ( !initLabel(obj) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QComboBox") ) { /// combo box
            if ( !initComboBoxItems(static_cast<QComboBox*>(obj)) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QDateTimeEdit") ) { /// edit controls for time
            if ( !initDateTime(obj) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if (obj->inherits( "QSpinBox") || obj->inherits("QDoubleSpinBox")) { /// numbers edit
            if ( !initSpinBox(obj) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        } else if ( obj->inherits("QCheckBox")) { /// check box
            if ( !initLabel(obj) )
                errorMessage( tr("Can't assign a value in the %1").arg(obj->objectName()) );
        }/* else if (obj->inherits("QProgressBar")) { /// progress bar

        }*/
    }
    return true;
}
// TODO: need finish
bool DWorkWidget::initButton(QWidget * w)
{
    QAbstractButton * b = static_cast<QAbstractButton *>(w);
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

again:
            switch(static_cast<QMetaType::Type>(v.type())) {
            case QMetaType::VoidStar: {
                QSqlQuery *q = ((QSqlQuery*)v.value<void*>());
                if (!q->first())
                    q->next();
                v = q->value(0);
                goto again;
            }
            case QMetaType::Int:
            case QMetaType::Long:
            case QMetaType::Double:
            case QMetaType::Float:
            case QMetaType::Short:
            case QMetaType::UInt:
            case QMetaType::ULong:
            case QMetaType::ULongLong:
            case QMetaType::UShort:
                if (!v.convert(QVariant::String))
                    return false;
            case QMetaType::QString:
                w->setProperty( "text", v );
                break;
            case QMetaType::QStringList:
                qDebug( "TODO: bool DWorkWidget::initLabel(QWidget *)\n\tAdd StringList support" );
                break;
            case QMetaType::QDate:
                w->setProperty( "text", v.toDate().toString() );
                break;
            case QMetaType::QTime:
                w->setProperty( "text", v.toTime().toString() );
                break;
            case QMetaType::QDateTime:
                w->setProperty( "text", v.toDateTime().toString() );
                break;
            default:
                return false;
            }
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
            if (static_cast<QMetaType::Type>(v.type()) == QMetaType::VoidStar) {
                QSqlQuery *q = ((QSqlQuery*)v.value<void*>());
                if (!q->first())
                    q->next();
                v = q->value(0);
            }
            if (!v.convert( w->inherits("QDoubleSpinBox") ?
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

again:
            switch(static_cast<QMetaType::Type>(v.type())) {
            case QMetaType::VoidStar: {
                QSqlQuery *q = ((QSqlQuery*)v.value<void*>());
                if (!q->first())
                    q->next();
                v = q->value(0);
                goto again;
            }
            case QMetaType::Int:
            case QMetaType::Long:
            case QMetaType::Double:
            case QMetaType::Float:
            case QMetaType::Short:
            case QMetaType::UInt:
            case QMetaType::ULong:
            case QMetaType::ULongLong:
            case QMetaType::UShort:
                return false;
            case QMetaType::QString:
                v = QDateTime::fromString(v.toString(), "yyyy-MM-dd");
                goto again;
            case QMetaType::QDate:
            case QMetaType::QTime:
            case QMetaType::QDateTime:
                w->setProperty( "dateTime", v );
                return true;
            default:
                return false;
            }
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
bool DWorkWidget::initListItemChange(QAbstractItemView * w)
{
    QVariant value = w->property( "OnSelectItem" );
    try {
        if (value.isValid()) {
            connect( w, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(listItemChange(QModelIndex)) );
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
            else if ( w->inherits("QLineEdit"))
                connect( w, SIGNAL(textChanged()),
                         this, SLOT(changeTextEvent()) );
            else if (w->inherits("QComboBox"))
                connect( w, SIGNAL(currentIndexChanged(QString)),
                         this, SLOT(changeItemIndex(QString)) );
            else
                return false;
        }
    } catch (...) {
        return false;
    }

    return true;
}
//
bool DWorkWidget::initComboBoxItems(QComboBox * w)
{
    QVariant  v;
    QVariant value = w->property( "Model" );
    try {
        if (value.isValid()) {
            emit uri( value.toString().trimmed(), &v  );

            w->clear();
            switch (static_cast<QMetaType::Type>(v.type())) {
            case QMetaType::QStringList: //
                w->addItems( v.toStringList() );
                break;
            case QMetaType::VoidStar: // SQL result
            {
                QSqlQuery *q = ((QSqlQuery*)v.value<void*>());

                while (q->next())
                    w->addItem( q->value(0).toString() );
            }
            break;
            default:
                return false;
            }
        }
    } catch (...) {
        return false;
    }

    return true;
}
//
bool DWorkWidget::initComboBoxChange(QWidget * w)
{
    QVariant value = w->property( "OnChangeIndex" );
    try {
        if (value.isValid()) {
            connect( w, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(changeItemIndex(int)) );
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
//
void DWorkWidget::changeItemIndex(QString text)
{
    emit uri( sender()->property( "OnChangeText" ).toString()
              .arg( text  ), new QVariant() );
}
//
void DWorkWidget::changeItemIndex(int index)
{
    emit uri( sender()->property( "OnChangeIndex" ).toString()
              .arg( index ), new QVariant() );
}
//
void DWorkWidget::listItemChange(QModelIndex)
{

}
