#ifndef DRAGTEXT_H
#define DRAGTEXT_H

#include "dragelement.h"

#include <QString>

class DragText : public DragElement
{
public:
    DragText();

    QString text;

    bool pick(QPoint epos);

    QRect textRect();
};

#endif // DRAGTEXT_H
