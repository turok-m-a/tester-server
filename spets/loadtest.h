#ifndef LOADTEST_H
#define LOADTEST_H
#include <qthread.h>
#include <network.h>
#include <qbytearray.h>
#include <iostream>
class loadTest : public QThread
{
public:
    int * volatile pause;
    loadTest();
    int opCode;
    double time;
    QByteArray r;
    void run();
};

#endif // LOADTEST_H
