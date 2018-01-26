#include "moveitem.h"

MoveItem::MoveItem(QObject *parent) :
     QObject(parent),QGraphicsItem()
{
    setText("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n\n222");
}

MoveItem::~MoveItem()
{

}

QRectF MoveItem::boundingRect() const
{
    return QRectF (0,0,400,15*lines);
}

void MoveItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(QColor(240,240,255));
    painter->drawRect(0,0,400,15*lines);
    //this->setTextWidth(60.0);
    //this->setFlags(GraphicsItemFlag::ItemIsMovable|GraphicsItemFlag::ItemIsSelectable);
    painter->drawText(1,1,400,15*lines,Qt::TextWrapAnywhere|Qt::AlignVCenter | Qt::AlignHCenter,text);
    //29
    //15 на строку
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
    p.rx() = (qreal)(((int)p.rx()/15)*15);
    p.ry() = (qreal)(((int)p.ry()/15)*15);
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

void MoveItem::setText(QString _text)
{
   lines = 0;
   int symbolCounter = 29;
   text = _text;
   for(int i=0;i<_text.size();i++){
       if(_text[i] == "\n" || symbolCounter == 0){
           symbolCounter = 29;
           lines++;
       } else {
           symbolCounter--;
       }
   }
   if(_text.size()<29 && lines == 0){
       lines = 1;
   }
}
