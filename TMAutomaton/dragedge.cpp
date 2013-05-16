#include "dragedge.h"

DragEdge::DragEdge(DragState *e1, DragState *e2):
    e1(e1),e2(e2)
{
    type = DRAG_EDGE;
}

bool DragEdge::pick(QPoint)
{
    return false;
}
