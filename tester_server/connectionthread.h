#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H
#include <QThread>
#include <winsock2.h>
#include <database.h>
#include <QByteArray>
#include <QVector>
class connectionThread : public QThread
{
private:
    SOCKET sockDescriptor;
    int userType; // 1 - student, 2 - teacher, 3 - admin
    int userId;
    int opCode;
    int exam_id;
    void processStudent();
    void processTeacher();
    void processStudentAnswers();
    QByteArray question_list;
public:
     Q_OBJECT
public:
    connectionThread(SOCKET s,QObject* parent);
    void run();
    bool authorization();

};

#endif // CONNECTIONTHREAD_H
