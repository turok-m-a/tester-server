#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H
#include <QThread>
#include <winsock2.h>
#include <database.h>
#include <QByteArray>
class connectionThread : public QThread
{
private:
    SOCKET sockDescriptor;
    int userType; // 1 - student, 2 - teacher, 3 - admin
    int userId;
    void processStudent();
    void processTeacher();
public:
     Q_OBJECT
public:
    connectionThread(SOCKET s,QObject* parent);
    void run();
    bool authorization();

};

#endif // CONNECTIONTHREAD_H
