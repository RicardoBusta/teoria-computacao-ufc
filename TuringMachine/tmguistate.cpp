#include "tmguistate.h"

#include <QDebug>

#include "tmguiedge.h"

TMGUIState::TMGUIState(int x, int y, TMGUI *tmgui)
{
    this->setPos(x,y);
    this->tmgui = tmgui;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}

void TMGUIState::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(QColor(0xff,0xff,0x00));
    QPen pen(QColor(0x00,0x00,0x00));

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(-50,-50,100,100);
    painter->drawText(0,0,"q0");

    painter->end();
}

QVariant TMGUIState::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (TMGUIEdge *edge, TMGUIEdge::list){
            edge->adjust();
        }
        break;
    default:
        break;
    };

    update();
    return QGraphicsItem::itemChange(change, value);
}

QRectF TMGUIState::boundingRect() const
{
    return QRectF(-50,-50,100,100);
}

QPainterPath TMGUIState::shape() const
{
    QPainterPath path;
    path.addEllipse(-50,-50,100,100);
    return path;
}
