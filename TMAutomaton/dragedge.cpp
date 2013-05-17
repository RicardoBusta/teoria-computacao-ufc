#include "dragedge.h"

#include <QVector2D>
#include <qmath.h>

DragEdge::DragEdge(DragState *e1, DragState *e2):
    DragElement(),e1(e1),e2(e2)
{
    type = DRAG_EDGE;
}

bool DragEdge::pick(QPoint)
{
    return false;
}

void DragEdge::render(QPainter *painter)
{
    const float sint = sin(-30*M_PI/180);
    const float cost = cos(-30*M_PI/180);

    if(e1 && e2){
        if(e1 != e2){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(Qt::black,2,Qt::SolidLine));
            QVector2D v1(e1->pos-e2->pos);

            v1.normalize();
            QVector2D v2(e2->pos-e1->pos);
            v2.normalize();
            QPointF p[4];
            p[0] = e1->pos-(e1->radius*v1.toPointF());
            p[1] = e2->pos-(e2->radius*v2.toPointF());
            painter->drawLine(p[0],p[1]);

            QRect trect = QRect((((p[0]+p[1])/2)+QPointF(-30,-30)).toPoint(),QSize(60,30));
            painter->setPen(QPen(Qt::lightGray,2,Qt::DashLine));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(trect);

//                painter->setBrush(Qt::NoBrush);
            painter->setPen(Qt::black);
            painter->drawText(trect,"HMM",Qt::AlignVCenter|Qt::AlignHCenter);

            p[2] = p[1]+(10*QPointF(v1.x()*cost-v1.y()*sint,v1.x()*sint+v1.y()*cost));
            p[3] = p[1]+(10*QPointF(v1.x()*cost-v1.y()*(-sint),v1.x()*(-sint)+v1.y()*cost));
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::black);
            painter->drawPolygon(p+1,3);
        }else{
            painter->setPen(QPen(Qt::black,2,Qt::SolidLine));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(e1->pos+QPointF(0,-30),20,30);

            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::lightGray);
//                painter->drawEllipse(rect);

            painter->setBrush(Qt::NoBrush);
            painter->setPen(Qt::black);
            painter->drawText(e1->pos+QPointF(0,-60),"HMM");

            QPointF p[3];
            p[0] = e1->pos+QPointF(-20,-25);
            p[1] = p[0]+(10*QPointF(+sint,-cost));
            p[2] = p[0]+(10*QPointF(-sint,-cost));
            painter->setBrush(Qt::black);
            painter->setPen(Qt::NoPen);
            painter->drawPolygon(p,3);
        }
    }
}

QRect DragEdge::boundingBox()
{
    return QRect(pos,QPoint(100,100));
}
