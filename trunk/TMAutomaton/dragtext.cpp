#include "dragtext.h"

DragText::DragText(QPoint pos):
    DragElement()
{
    this->pos = pos;
    text = "HAHA";
}

bool DragText::pick(QPoint epos)
{
    return false;
}

void DragText::render(QPainter *painter)
{
    if(selected){
        painter->drawRect(boundingBox());
    }
    painter->setPen(Qt::black);
    painter->drawText(pos,text);
}

QRect DragText::boundingBox()
{
    return QRect(pos,QSize(30,30));
}
