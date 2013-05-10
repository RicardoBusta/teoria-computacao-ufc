#ifndef DRAGEDGE_H
#define DRAGEDGE_H

#include "dragelement.h"

class DragEdge
{
public:
    DragEdge(DragElement *e1, DragElement *e2);

    DragElement *e1, *e2;
};

#endif // DRAGEDGE_H
