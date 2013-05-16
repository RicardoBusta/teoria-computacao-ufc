#include "dragstate.h"

DragState::DragState(QPointF pos):
    radius(30),final(false)
{
    type = DRAG_STATE;

    this->pos = pos;
    static int count=0;
    txt = QString("TEST_%1").arg(count++);
}

bool DragState::pick(QPoint epos)
{
    QPointF delta = epos - pos;
    if( (delta.x()*delta.x()) + (delta.y()*delta.y()) <= radius*radius ){

        return true;
    }
    return false;
}
