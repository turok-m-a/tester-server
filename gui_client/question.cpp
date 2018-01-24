#include "question.h"

int Question::getAnswerNumber()
{
    int num = 0;
    for(int i =0;i<rawQuestionText.size();i++){
        int nextAnswerPosition = rawQuestionText.indexOf('$',i);
        if(rawQuestionText[nextAnswerPosition-1] != QChar('#')){
                num++;

        }
        i = nextAnswerPosition+1;
    }
    return num;
}

QString Question::getQuestionText()
{

    QString text = rawQuestionText;
    for(int i =0;i<text.size();i++){
        if(text[i] == QChar('#')){
            text.remove(i,1);//удаляем экранирующую #
        }
        i++; //пропускаем экранируемый символ
    }
    for(int i =0;i<text.size();i++){
        int nextAnswerPosition = text.indexOf('$',i);
        if(text[nextAnswerPosition-1] != QChar('#')){
            text.truncate(nextAnswerPosition);
            break;
        }
    }
    return text;
}

QString Question::getAnswerText(int i)
{
    return answers[i];
}

Question::Question(QString rawText, int _id, int _type) : id(_id), type(_type)
{
    rawQuestionText = rawText;
    int answerSignPosition;
    QVector<int> answerSignPositions;
    for(int i =0;i<rawQuestionText.size();i++){
        answerSignPosition = rawQuestionText.indexOf('$',i);
        if (answerSignPosition == -1) break;
        if(rawQuestionText[answerSignPosition-1] != QChar('#')){
            answerSignPositions.push_back(answerSignPosition);
        }
        i = answerSignPosition+1;
    }
    for(int i =0;i<answerSignPositions.size()-1;i++){
        answers.push_back(rawQuestionText.mid(answerSignPositions[i],answerSignPositions[i+1]-answerSignPositions[i]));
    }
    answers.push_back(rawQuestionText.mid(answerSignPositions.last()));
    selectedAnswer = -1;
}

Question::Question()
{

}
