#ifndef DRAGEDGE_H
#define DRAGEDGE_H

#include "dragelement.h"
#include "dragstate.h"

class DragEdge:public DragElement
{
public:
    DragEdge(DragState *e1, DragState *e2);

    DragState *e1, *e2;

    bool pick(QPoint);
    void render(QPainter *painter);
    QRect boundingBox();
};

#endif // DRAGEDGE_H
