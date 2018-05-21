#ifndef GROUPQUESTIONDRAWER_H
#define GROUPQUESTIONDRAWER_H
#include <QByteArray>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <moveitem.h>
#include <QMap>
#ifdef QT_DEBUG
#include <iostream>

#endif
#include <constants.h>
class GroupQuestionDrawer
{
private:
    QByteArray question;
    QVector<MoveItem *> guiObjects;
    QMap<int,MoveItem *> guiObjectsMap;
    int xAnswerSize;
    int columns;
    QVector<qreal> columnsLastPosition;
public:
    GroupQuestionDrawer(QByteArray questionData, QGraphicsScene *scene,QVector<int> restoreSequence = QVector<int>());
    QVector<int> getSequence();
    ~GroupQuestionDrawer();
};

#endif // GROUPQUESTIONDRAWER_H
