#include "tmgui.h"

#include <QMouseEvent>

#include "tmguistate.h"

TMGUI::TMGUI(QWidget *parent) :
    QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);
}

TMGUI::~TMGUI()
{
}

void TMGUI::mousePressEvent(QMouseEvent *event)
{
    this->scene()->addEllipse(QRectF(event->globalPos(),QSizeF(50,50)));
}
