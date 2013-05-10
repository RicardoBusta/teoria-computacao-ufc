#include "dragelement.h"

DragElement::DragElement(QPointF pos):
    pos(pos),radius(30)
{
    static int count=0;
    txt = QString("TEST_%1").arg(count++);
}
