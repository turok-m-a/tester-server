#include "sequencequestiondrawer.h"

SequenceQuestionDrawer::SequenceQuestionDrawer(QByteArray questionData, QGraphicsScene * scene)
{

    question = questionData;
    MoveItem * item = new MoveItem();        // Создаём графический элемента

    //QGraphicsTextItem *textItem = new QGraphicsTextItem("testtesttest",item);
    //textItem->setFlags(QGraphicsItem::ItemIsMovable);
    //textItem->setTextWidth(60);
    scene->addItem(item);   // Добавляем элемент на графическую сцену
    item->setPos(10,10);
    scene->addItem(new MoveItem());
}
