#ifndef DRAGTEXT_H
#define DRAGTEXT_H

#include "dragelement.h"

#include <QString>

class DragText : public DragElement
{
public:
    DragText(QPoint pos);

    QString text;

    bool pick(QPoint epos);
    void render(QPainter *painter);
    QRect boundingBox();
};

#endif // DRAGTEXT_H
