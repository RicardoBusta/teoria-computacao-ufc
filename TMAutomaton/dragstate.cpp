#include "dragstate.h"

DragState::DragState(QPoint pos):
    DragElement(),radius(30),final(false),initial(false)
{
    type = DRAG_STATE;

    this->pos = pos;
    static int count=0;
    txt = QString("q%1").arg(count++);
}

bool DragState::pick(QPoint epos)
{
    QPointF delta = epos - pos;
    if( (delta.x()*delta.x()) + (delta.y()*delta.y()) <= radius*radius ){

        return true;
    }
    return false;
}

void DragState::render(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    if(dragged){
        painter->setBrush(QColor(Qt::yellow).darker(120));
    }else{
        painter->setBrush(Qt::yellow);
    }
    painter->drawEllipse(pos,radius,radius);

    if(selected){
        painter->setPen(QPen(Qt::cyan,4,Qt::SolidLine));
    }else{
        painter->setPen(QPen(Qt::black,2,Qt::SolidLine));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(pos,radius,radius);

    if(initial){
        QPointF p[3];
        painter->setBrush(Qt::white);
        p[0] = pos+QPointF(-radius,0);
        p[1] = pos+QPointF(-radius-20,20);
        p[2] = pos+QPointF(-radius-20,-20);
        painter->drawPolygon(p,3);
    }

    if(final){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::black,2,Qt::SolidLine));
        painter->drawEllipse(pos,int(radius-5),int(radius-5));
    }

    QRectF rect = QRectF(pos-QPointF(radius,radius),pos+QPointF(radius,radius));
    painter->setPen(Qt::black);
    painter->drawText(rect,txt,Qt::AlignVCenter|Qt::AlignHCenter);
}

QRect DragState::boundingBox()
{
    return QRect(pos-QPoint(radius,radius),pos+QPoint(radius,radius));
}
