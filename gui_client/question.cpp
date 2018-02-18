#include "question.h"

int Question::getAnswerNumber()
{
    int num = 0;
    for(int i =0;i<rawQuestionText.size();i++){
        int nextAnswerPosition = rawQuestionText.indexOf('$',i);
        if(nextAnswerPosition == -1) break;
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
        if (nextAnswerPosition == -1) break;
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

QString Question::getAnswerText()
{
    return answerText;
}

Question::Question(QString rawText, int _id, int _type) : id(_id), type(_type)
{
    isAnswered = false;
    rawQuestionText = rawText;
    int answerSignPosition;
    QVector<int> answerSignPositions;
    if(type == SELECT_QUESTION_TYPE || type == MATCH_QUESTION_TYPE){
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
    }
}



Question::Question()
{

}

QVector<int> Question::getSelectedAnswers()
{
    return selectedAnswers;
}

QVector<int> Question::getAnswersSequence()
{
    return answer;
}

void Question::addAnswer(int selectedAnswer)
{
    isAnswered = true;
    selectedAnswers.push_back(selectedAnswer);
}

void Question::addAnswer(QString answerInput)
{
    isAnswered = true;
    answerText = answerInput;
}

void Question::addAnswer(QVector<int> a)
{
    isAnswered = true;
    answer = a;
}

void Question::addAdvancedData(QByteArray data)
{
    advData = data;
    if(type == MATCH_QUESTION_TYPE){
        groupNumber = advData.at(0);
    }

}

void Question::resetAnswer()
{
    selectedAnswers.clear();
    answerText.clear();
    answer.clear();
    isAnswered = false;
}

int Question::getId()
{
    return id;
}

QByteArray Question::getAdvancedData()
{
    return advData;
}
