#ifndef DRAGELEMENT_H
#define DRAGELEMENT_H

#include <QPoint>
#include <QRectF>
#include <QString>

enum DRAG_{
    DRAG_STATE,
    DRAG_EDGE,
    DRAG_TEXT
};

class DragElement{
public:
    explicit DragElement():pos(QPointF(0,0)){}

    DRAG_ type;

    QPointF pos;

    virtual bool pick(QPoint)=0;
};


#endif // DRAGELEMENT_H
