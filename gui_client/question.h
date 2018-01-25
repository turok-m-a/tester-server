#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QChar>
#include <QVector>
class Question
{
private:
    int id;

    QString rawQuestionText;
    QVector<QString> answers;
    int selectedAnswer = -1; //тип вопроса 1
    QString answerText;      //тип 2
    QByteArray answer;       //тип 3,4
    QByteArray advData;
public:
    int type;
    int getAnswerNumber();
    QString getQuestionText();
    QString getAnswerText(int i);
    Question(QString rawText,int _id,int _type);
    Question();
    void setAdvData(QByteArray data);
};

#endif // QUESTION_H
