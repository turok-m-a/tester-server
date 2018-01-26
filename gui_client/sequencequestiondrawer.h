#ifndef SEQUENCEQUESTIONDRAWER_H
#define SEQUENCEQUESTIONDRAWER_H
#include <QByteArray>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <moveitem.h>
class SequenceQuestionDrawer
{
private:
    QByteArray question;
    QVector<QGraphicsItem *> guiObjects;
public:
    SequenceQuestionDrawer(QByteArray questionData, QGraphicsScene *scene);

};

#endif // SEQUENCEQUESTIONDRAWER_H
