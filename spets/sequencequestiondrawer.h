#ifndef SEQUENCEQUESTIONDRAWER_H
#define SEQUENCEQUESTIONDRAWER_H
#include <QByteArray>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <moveitem.h>
#include <QMap>
#ifdef QT_DEBUG
#include <iostream>
#endif
class SequenceQuestionDrawer
{
private:
    QByteArray question;
    QVector<MoveItem *> guiObjects;
    QMap<int,MoveItem *> guiObjectsMap;
public:
    SequenceQuestionDrawer(QByteArray questionData, QGraphicsScene *scene,QVector<int> restoreSequence = QVector<int>());
    QVector<int> getSequence();
    ~SequenceQuestionDrawer();
};

#endif // SEQUENCEQUESTIONDRAWER_H
