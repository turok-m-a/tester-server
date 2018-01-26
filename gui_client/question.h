#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QChar>
#include <QVector>
#define SELECT_QUESTION_TYPE 1  //выбор одного или нескольких вариантов
#define INPUT_QUESTION_TYPE 2   //ввод ответа с клавиатуры
#define SEQUENCE_QUESTION_TYPE 3//выбор последовательности действий
#define MATCH_QUESTION_TYPE 4   //сопоставление
class Question
{
private:
    int id;

    QString rawQuestionText;
    QVector<QString> answers;
    QVector<int> selectedAnswers; //тип вопроса 1
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
    void addAnswer(int selectedAnswer);
    void addAnswer(QString answerInput);
};

#endif // QUESTION_H
