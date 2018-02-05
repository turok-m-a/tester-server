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
    QVector<QString> answers;   //варианты ответов
    QVector<int> selectedAnswers; //тип вопроса 1
    QString answerText;      //тип 2
    QVector<int> answer;       //тип 3,4
    QByteArray advData;
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
    void setAdvData(QByteArray data);
    void addAnswer(int selectedAnswer);
    void addAnswer(QString answerInput);
    void addAnswer(QVector<int> a);
    void addAdvancedData(QByteArray data);
    void resetAnswer();
    int getId();
    QByteArray getAdvancedData();
};

#endif // QUESTION_H
