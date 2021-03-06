#ifndef DDBCONFIG_H
#define DDBCONFIG_H
//
#ifndef HAVE_QT5
# include <QtGui/QDialog>
#else
# include <QtWidgets/QDialog>
#endif
//
class DProject;
//
class QFormLayout;
class QComboBox;
class QLineEdit;
class QTextEdit;
class QSpinBox;
class QLabel;
//
class DDbConfig : public QDialog
{
    Q_OBJECT
public:
    DDbConfig(QWidget *parent = 0);
    virtual ~DDbConfig();

private:
    DProject * project;
    // UI
    QFormLayout * l;

    QComboBox * cbDriver;
    QLineEdit * edtDbName;
    QLineEdit * edtDbHost;
    QSpinBox  * sbDbPort;
    QLineEdit * edtDbUser;
    QLineEdit * edtDbPassword;
    QTextEdit * edtDbOptions;
    QLabel    * lStatus;

public:
    void setProject(DProject *);

public slots:
    void save();
    bool apply();

};
//
#endif // DDBCONFIG_H
