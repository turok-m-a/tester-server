#ifndef NETWORK_H
#define NETWORK_H
#include "question.h"
#include "qvector.h"
#include "qstring.h"
class network
{
public:
    network();
    QVector<Question> getQuestionsForStudent(QString studLogin);
};

#endif // NETWORK_H
