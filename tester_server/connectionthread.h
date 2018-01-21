#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H
#include <QThread>
#include <winsock2.h>
#include <database.h>
class connectionThread : QThread
{
     Q_OBJECT
    SOCKET sockDescriptor;
    int userType; // 1 - student, 2 - teacher, 3 - admin
    int userId;
    void processStudent();
    void processTeacher();
public:
    connectionThread(SOCKET s = NULL,QObject* parent);
    void run();
    bool authorization();

};

#endif // CONNECTIONTHREAD_H
