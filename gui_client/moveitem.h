#ifndef MOVEITEM_H
#define MOVEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

class MoveItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MoveItem(QObject *parent = 0, int xgrid = INT_MAX, int ygrid = 15,int xsize = 400);
    ~MoveItem();
    qreal setText(QString _text);
    void setPositionY(qreal pos);
    void setQuestionNumber(int n);
    int getQuestionNumber();
signals:

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    int questionNumber;
    QString text;
    int lines;
    qreal positionY;
    int xGrid;
    int yGrid;
    int xSize;
public slots:
};

#endif // MOVEITEM_H
