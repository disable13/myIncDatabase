#ifndef DDBCONFIG_H
#define DDBCONFIG_H

#include <QDialog>

class DProject;

class QFormLayout; class QComboBox; class QLineEdit; class QTextEdit; class
QSpinBox; class QLabel;

class DDbConfig : public QDialog
{
    Q_OBJECT
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
    DDbConfig(QWidget *parent = 0);
    ~DDbConfig();

    void setProject(DProject *);

public slots:
    void save();
    bool apply();

};

#endif // DDBCONFIG_H
