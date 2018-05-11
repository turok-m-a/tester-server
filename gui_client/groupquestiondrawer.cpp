#include "groupquestiondrawer.h"

GroupQuestionDrawer::GroupQuestionDrawer(QByteArray questionData, QGraphicsScene * scene, QVector<int> restoreSequence)
{
    QByteArray temp;
    QVector<QString> answers;
    qreal height = 0;
    int questionNumber = 0;
    int currentColumn = 0;
    columns = questionData.at(0);
    xAnswerSize = WINDOW_WIDTH / columns;
    columnsLastPosition.fill(0,columns); // начальные позиции прямоугольников в столбцах
    for(int i=1;i<questionData.size();i++){
        if (questionData[i] != 0){
        temp.push_back(questionData[i]);
        } else {
            temp.push_back(questionData[i]);
            //temp.push_back(questionData[i+1]);
            answers.push_back(QString::fromUtf8(temp));
            temp.clear();
            i+=1;
        }

    }
    if (restoreSequence.isEmpty()){
    foreach (QString str, answers) {
        MoveItem * item = new MoveItem(0,xAnswerSize,15,xAnswerSize-1);
        item->setPositionY(columnsLastPosition[currentColumn]);
        item->setPos(currentColumn*xAnswerSize, columnsLastPosition[currentColumn]);
        columnsLastPosition[currentColumn] = item->setText(str);
        // рисуем поочередно ответы слева направо
        item->setQuestionNumber(questionNumber);
        questionNumber++;
        scene->addItem(item);
        guiObjects.push_back(item);

        currentColumn++;
        if (currentColumn == columns) currentColumn = 0;
    }
    } else {
        QVector<int> lineLastPosition;
        lineLastPosition.fill(0,columnsLastPosition.size());
        for (int i=0;i<answers.size();i++){
        MoveItem * item = new MoveItem(0,xAnswerSize,15,xAnswerSize-1);
        item->setPositionY(columnsLastPosition[currentColumn]);
        item->setPos(restoreSequence[i]*xAnswerSize, columnsLastPosition[restoreSequence[i]]);
        columnsLastPosition[restoreSequence[i]] = item->setText(answers[i]);

        item->setQuestionNumber(restoreSequence[i]);
        scene->addItem(item);
        guiObjects.push_back(item);

       }
    }
    question = questionData;
}

QVector<int> GroupQuestionDrawer::getSequence()
{
    MoveItem * i;
    QVector<int> answerSequence;
    foreach (i, guiObjects) {
        int group = i->pos().x()/xAnswerSize;
        guiObjectsMap.insert(i->pos().y(),i);
        answerSequence.push_back(group);
    }

#ifdef QT_DEBUG
    int j;
    foreach (j, answerSequence) {
        std::cout<<j<<" "<<std::endl;
    }
#endif
    return answerSequence;
}

GroupQuestionDrawer::~GroupQuestionDrawer(){}
