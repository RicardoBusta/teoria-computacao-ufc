#ifndef DRAGSTATE_H
#define DRAGSTATE_H

#include "dragelement.h"

class DragState : public DragElement
{
public:
    DragState();

    DragState(QPoint pos);

    int radius;
    QString txt;

    bool final;
    bool initial;

    bool pick(QPoint epos);
    void render(QPainter *painter);
    QRect boundingBox();
};

#endif // DRAGSTATE_H
