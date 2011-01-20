#ifndef DTHREADURI_H
#define DTHREADURI_H

#include <QRunnable>

#include <QStringList>
class DProject;



class DThreadUri : public QRunnable
{
private:
    QString uri;
    DProject * project;

    QStringList path;

    char type[2];
    char * result;
    int resultLength;

public:
    DThreadUri(QString Uri, DProject *);

    void run();

    void system();
    void config();
    void sql();

    void error(int);
};

#endif // DTHREADURI_H
