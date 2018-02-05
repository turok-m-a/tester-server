#ifndef NETWORK_H
#define NETWORK_H
#include "question.h"
#include "qvector.h"
#include "qstring.h"
#include "winsock2.h"
#
class Network
{
private:
    Network();
    Network( const Network&);
    Network& operator=(Network& );
public:
    int currentUserType;
    QString studLogin;
    static Network& getInstance() {
           static Network  instance;
           return instance;
       }
    QVector<Question> getQuestionsForStudent(QString studLogin,int & loginStatus);
    int sendQuestions(QVector<Question> q);
};

#endif // NETWORK_H
