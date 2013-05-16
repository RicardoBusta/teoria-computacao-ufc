#ifndef DRAGSTATE_H
#define DRAGSTATE_H

#include "dragelement.h"

class DragState : public DragElement
{
public:
    DragState();

    DragState(QPointF pos);

    float radius;
    QString txt;

    bool final;

    bool pick(QPoint epos);
};

#endif // DRAGSTATE_H
