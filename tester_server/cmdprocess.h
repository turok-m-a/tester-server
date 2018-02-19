#ifndef CMDPROCESS_H
#define CMDPROCESS_H
#include <winsock2.h>
#include <constants.h>
#include <QVector>
#include <database.h>
class CmdProcess
{
public:
    CmdProcess();
    CmdProcess(int opCode,SOCKET _sock);
private:
    void sendStudList();
    SOCKET sock;
};

#endif // CMDPROCESS_H
