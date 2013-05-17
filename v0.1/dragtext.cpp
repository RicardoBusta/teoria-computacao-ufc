#include "dragtext.h"

#include <QFontMetrics>
#include <QFont>

DragText::DragText()
{
}

bool DragText::pick(QPoint epos)
{
    return false;
}

QRect DragText::textRect()
{
    QFont font;
    QFontMetrics fontmetrics(font);
    return QRect(pos.x(),pos.y(),fontmetrics.width(text),fontmetrics.height());
}
