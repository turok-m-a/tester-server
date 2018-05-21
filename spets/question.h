#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QChar>
#include <QVector>
#include <constants.h>
class Question
{
private:
    int id;

    QString rawQuestionText;
    QVector<QString> answers;   //варианты ответов
    QVector<int> selectedAnswers; //тип вопроса 1
    QString answerText;      //тип 2
    QVector<int> answer;       //тип 3
    //QVector<QPair<QString,int>> groupAnswers; //тип 4
    QByteArray advData;
    int groupNumber;
public:
    int type;
    bool isAnswered;
    int getAnswerNumber();
    QString getQuestionText();
    QString getAnswerText(int i);
    QString getAnswerText();
    Question(QString rawText,int _id,int _type);
    Question();
     QVector<int> getSelectedAnswers();
      QVector<int> getAnswersSequence();
    void addAnswer(int selectedAnswer);
    void addAnswer(QString answerInput);
    void addAnswer(QVector<int> a);
    void addAdvancedData(QByteArray data);
    void resetAnswer();
    int getId();
    QByteArray getAdvancedData();
};

#endif // QUESTION_H
