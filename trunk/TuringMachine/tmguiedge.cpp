#include "tmguiedge.h"

#include "tmguistate.h"

 QList<TMGUIEdge*> TMGUIEdge::edge_list;

TMGUIEdge::TMGUIEdge(TMGUIState *from, TMGUIState *to) :
    from(from),to(to)
{
    edge_list.push_back(this);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(0);
}

QRectF TMGUIEdge::boundingRect() const
{
    if (!from || !to)
        return QRectF();
    return QRectF(from->pos(),to->pos());
}

void TMGUIEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!from || !to)
        return;
    painter->drawLine(from->pos(),to->pos());
}

void TMGUIEdge::adjust()
{
    update();
}
