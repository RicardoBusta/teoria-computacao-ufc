#ifndef DRAGELEMENT_H
#define DRAGELEMENT_H

#include <QPoint>
#include <QRectF>
#include <QString>

class DragElement{
public:
    explicit DragElement(QPointF pos);

    QPointF pos;
    QRectF rect;
    float radius;
    QString txt;
};


#endif // DRAGELEMENT_H
