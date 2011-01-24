#ifndef DTHREADURI_H
#define DTHREADURI_H

#include <QObject>
#include <QRunnable>

#include <QStringList>
#include <QVariant>
class DProject;

class DThreadUri : public QObject, public QRunnable
{
    Q_OBJECT
private:
    QString uri;

    QStringList path;
    int m_err;
    QVariant result;

public:
    DThreadUri(QString Uri);
    virtual ~DThreadUri();

    void run();

private:
    void system();
    void config();
    void sql();

signals:
    void error(int);
    void complete(int*,QVariant);

};

#endif // DTHREADURI_H
