//#include "tmguistate.h"

//#include <QDebug>

//TMGUIState::TMGUIState(TMGUI *tmgui)
//{
//    this->tmgui = tmgui;
////    setFlag(ItemIsMovable);
////    setFlag(ItemSendsGeometryChanges);
////    setCacheMode(DeviceCoordinateCache);
////    setZValue(-1);
//}

//void TMGUIState::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    QBrush brush(QColor(0xff,0xff,0xff));
//    QPen pen(QColor(0x00,0x00,0x00));

//    painter->setPen(pen);
//    painter->setBrush(brush);
//    painter->drawEllipse(0,0,50,50);

//    painter->end();
//}

//QRectF TMGUIState::boundingRect() const
//{
//    return QRectF(-50,-50,100,100);
//}

//QPainterPath TMGUIState::shape() const
//{
//    QPainterPath path;
//    path.addEllipse(-50,-50,100,100);
//    return path;
//}

////void TMGUIState::mousePressEvent(QGraphicsSceneMouseEvent *e)
////{
////    update();
////    QGraphicsItem::mousePressEvent(e);
////}

////void TMGUIState::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
////{
////    update();
////    QGraphicsItem::mouseReleaseEvent(e);
////}
