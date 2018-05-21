#include "sequencequestiondrawer.h"

SequenceQuestionDrawer::SequenceQuestionDrawer(QByteArray questionData, QGraphicsScene * scene, QVector<int> restoreSequence)
{
    QByteArray temp;
    QVector<QString> answers;
    qreal nextPosition = 0;
    int questionNumber = 0;
    int qdSize = questionData.size();
    for(int i=0;i< qdSize;i++){
        if (questionData[i] != 0){
        temp.push_back(questionData[i]);
        } else {
            temp.push_back(questionData[i]);
            //temp.push_back(questionData[i+1]);
            answers.push_back(QString::fromUtf8(temp));
            temp.clear();
            i++;

            temp.clear();
        }

    }
    if (restoreSequence.isEmpty()){
    foreach (QString str, answers) {
        MoveItem * item = new MoveItem();
        item->setPositionY(nextPosition);
        nextPosition = item->setText(str);
        item->setPos(0,nextPosition);
        item->setQuestionNumber(questionNumber);
        questionNumber++;
        scene->addItem(item);
        guiObjects.push_back(item);
    }
    } else {    //восстанавливаем в требуемом порядке
        for (int i=0;i<answers.size();i++){
        MoveItem * item = new MoveItem();
        item->setPositionY(nextPosition);
        nextPosition = item->setText(answers[restoreSequence[i]]);
        item->setPos(0,nextPosition);
        item->setQuestionNumber(restoreSequence[i]);
        scene->addItem(item);
        guiObjects.push_back(item);
        }
    }
    question = questionData;
}

QVector<int> SequenceQuestionDrawer::getSequence()
{
    guiObjectsMap.clear();
    MoveItem * i;
    int sequencePosition;
    foreach (i, guiObjects) {
        guiObjectsMap.insert(i->pos().y(),i);
        sequencePosition++;
    }
    QMapIterator<int,MoveItem *> iter(guiObjectsMap);
    QVector<int> answerSequence;
    while (iter.hasNext()){
        iter.next();
        answerSequence.push_back(iter.value()->getQuestionNumber());
    }
#ifdef QT_DEBUG
    int j;
    foreach (j, answerSequence) {
        std::cout<<j<<" "<<std::endl;
    }
#endif
    return answerSequence;
}

SequenceQuestionDrawer::~SequenceQuestionDrawer(){}
