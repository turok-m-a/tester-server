#include "moveitem.h"

MoveItem::MoveItem(QObject *parent) :
     QObject(parent),QGraphicsItem()
{

}

MoveItem::~MoveItem()
{

}

QRectF MoveItem::boundingRect() const
{
    return QRectF (0,0,180,60);
}

void MoveItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(QColor(240,240,255));
    painter->drawRect(0,0,180,60);
    //this->setTextWidth(60.0);
    //this->setFlags(GraphicsItemFlag::ItemIsMovable|GraphicsItemFlag::ItemIsSelectable);
    painter->drawText(1,1,180,60,Qt::TextWrapAnywhere|Qt::AlignVCenter | Qt::AlignHCenter,"45011114501111111111111112345");//29

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /* Устанавливаем позицию графического элемента
     * в графической сцене, транслировав координаты
     * курсора внутри графического элемента
     * в координатную систему графической сцены
     * */
    QPointF p(event->pos());
    p.rx() = (qreal)(((int)p.rx()/10)*10);
    p.ry() = (qreal)(((int)p.ry()/10)*10);
    this->setPos(mapToScene(p));
}

void MoveItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /* При нажатии мышью на графический элемент
     * заменяем курсор на руку, которая держит этот элемента
     * */
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void MoveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    /* При отпускании мышью элемента
     * заменяем на обычный курсор стрелку
     * */
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}
