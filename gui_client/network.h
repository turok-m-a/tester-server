#ifndef NETWORK_H
#define NETWORK_H
#include "question.h"
#include "qvector.h"
#include "qstring.h"
#include "winsock2.h"
#include "constants.h"
#include <QByteArray>
class Network
{
private:
    Network();
    Network( const Network&);
    Network& operator=(Network& );
    QString serverAddress;
    QString login;
    QString password;
public:
    void setAddress(QString address);
    int currentUserType;
    QString studLogin;

    void setUser(QString _login,QString _password);
    int checkUser();
    QByteArray sendQuery(int opCode,QByteArray query);
    static Network& getInstance() {
           static Network  instance;
           return instance;
       }
    QVector<Question> getQuestionsForStudent(QString studLogin,int & loginStatus);
    float sendQuestions(QVector<Question> q);
};

#endif // NETWORK_H
