#include "sequencequestiondrawer.h"

SequenceQuestionDrawer::SequenceQuestionDrawer(QByteArray questionData, QGraphicsScene * scene)
{
    QByteArray temp;
    qreal nextPosition = 0;
    int questionNumber = 0;
    for(int i=0;i<questionData.size();i++){
        if (questionData[i] != 0){
        temp.push_back(questionData[i]);
        } else {
            temp.push_back(questionData[i]);
            temp.push_back(questionData[i+1]);
            i+=2;
            MoveItem * item = new MoveItem();
            item->setPositionY(nextPosition);
            nextPosition = item->setText(QString::fromUtf8(temp));
            item->setPos(0,nextPosition);
            item->setQuestionNumber(questionNumber);
            questionNumber++;
            scene->addItem(item);
            guiObjects.push_back(item);
            temp.clear();
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

SequenceQuestionDrawer::~SequenceQuestionDrawer()
{
    MoveItem * i;
    foreach (i, guiObjects) {
        delete i;
    }
}
