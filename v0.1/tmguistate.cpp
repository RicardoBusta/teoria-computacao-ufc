#include "tmguistate.h"

#include <QDebug>

#include "tmguiedge.h"

QList<TMGUIState*> TMGUIState::state_list;

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
    QBrush brush(QColor(0xff,0xff,0xff));
    QPen pen(QColor(0x00,0x00,0x00));

    QRadialGradient gradient(0, 0, 100);
    gradient.setCenter(0, 0);
    gradient.setFocalPoint(0, 0);

    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::darkYellow);

    painter->setBrush(gradient);
    painter->setPen(pen);
    //painter->setBrush(brush);
    painter->drawEllipse(-50,-50,100,100);

    painter->setBrush(brush);
    painter->drawEllipse(-40,-40,80,80);
    painter->drawText(0,0,"q0");
}

QVariant TMGUIState::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (TMGUIEdge *edge, TMGUIEdge::edge_list){
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
