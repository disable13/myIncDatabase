#ifndef DTHREADURI_H
#define DTHREADURI_H

#include <QRunnable>

#include <QStringList>
#include <QVariant>
class DProject;



class DThreadUri : public QRunnable
{
private:
    QString uri;
    DProject * project;

    QStringList path;
    int m_err;
    QVariant result;

public:
    DThreadUri(QString Uri, DProject *);

    void run();

    void system();
    void config();
    void sql();

    void error(int);
};

#endif // DTHREADURI_H
