#ifndef LOADTEST_H
#define LOADTEST_H
#include <qthread.h>
#include <network.h>
#include <qbytearray.h>
#include <qdebug.h>
class loadTest : public QThread
{
public:
    int * volatile pause;
    loadTest();
    int opCode;
    QByteArray r;
    void run();
};

#endif // LOADTEST_H
