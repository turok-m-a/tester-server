#include "connectionthread.h"

void connectionThread::processStudent()
{
    database db = dataBase.getInstance();
    db.getStudentStatus(userId);

}

connectionThread::connectionThread(SOCKET s, QObject *parent) :
    QThread(parent), sockDescriptor(s),person (0) {}

void connectionThread::run()
{
    if(!authorization()) {
        send(sockDescriptor,"NOTAUTH",8,0);
        closesocket(s);
        return;
    }
    switch (userType) {
    case 1:
        processStudent();
        break;
    case 2:
        processTeacher();
        break;
    default:
        break;
    }
}

bool connectionThread::authorization()
{
    recv(sockDescriptor,&userType,sizeof(int),0);
    char buf[40];
    recv(sockDescriptor,&buf,sizeof(buf),0);
    QString userName(buf);
    database db = dataBase.getInstance();
    userID = db.getUserId(userName);
    if (userId < 0)
        return false;
    else
        return true;
}
