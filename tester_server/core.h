#ifndef CORE_H
#define CORE_H
#include <QtCore>



#include <database.h>
#include <QString>
#include <connection.h>
using namespace std;
class core : public QObject
{
    Q_OBJECT
public:
    core(QObject *parent = 0) : QObject(parent) {}

public slots:
    void run();

signals:
    void finished();
};

#endif // CORE_H
