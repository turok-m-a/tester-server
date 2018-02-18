#include "moveitem.h"

MoveItem::MoveItem(QObject *parent, int xgrid, int ygrid, int xsize) :
     QObject(parent),QGraphicsItem()
{
    xGrid = xgrid;
    yGrid = ygrid;
    xSize = xsize;
    //setText("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n\n222");
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
    painter->drawRect(0,0,xSize,15*lines);
    //this->setTextWidth(60.0);
    //this->setFlags(GraphicsItemFlag::ItemIsMovable|GraphicsItemFlag::ItemIsSelectable);
    painter->drawText(1,1,xSize,15*lines,Qt::TextWrapAnywhere|Qt::AlignVCenter | Qt::AlignHCenter,text);
    //29
    //15 на строку
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF p(event->pos());
   // p.rx() = (qreal)(((int)p.rx()/xGrid)*xGrid);
   // p.ry() = (qreal)(((int)p.ry()/yGrid)*yGrid);

    this->setPos(mapToScene(p));
     Q_UNUSED(event);
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
    double xx=x();
    double yy=y();
    if((int)xx / xGrid != xx/(double)xGrid) xx=(double)xGrid*round(xx/(double)xGrid);
    if((int)yy / yGrid != yy/(double)yGrid) yy=(double)yGrid*round(yy/(double)yGrid);
        setPos(xx,yy);
    Q_UNUSED(event);

}

void MoveItem::setQuestionNumber(int n)
{
    questionNumber = n;
}

int MoveItem::getQuestionNumber()
{
    return questionNumber;
}

qreal MoveItem::setText(QString _text)
{
   lines = 0;
   int symbolCounter = xSize/14;
   text = _text;
   for(int i=0;i<_text.size();i++){
       if(_text[i] == "\n" || symbolCounter == 0){
           symbolCounter =  xSize/14;
           lines++;
       } else {
           symbolCounter--;
       }
   }
   if(_text.size()< xSize/14 && lines == 0){
       lines = 1;
   }
   return positionY + lines*15 + 15;
}

void MoveItem::setPositionY(qreal pos)
{
    positionY = pos;
}

