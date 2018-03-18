#ifndef CMDPROCESS_H
#define CMDPROCESS_H
#include <winsock2.h>
#include <constants.h>
#include <QVector>
#include <database.h>
#include <QDataStream>
class CmdProcess
{
public:
    CmdProcess();
    CmdProcess(int opCode,SOCKET _sock);
private:
    void sendStudList();
    void addStudToList();
    void removeStudentFromList();
    void addSubject();
    void findSubject();
    void removeSubject();
    void getQuestionList();
    SOCKET sock;
};

#endif // CMDPROCESS_H
