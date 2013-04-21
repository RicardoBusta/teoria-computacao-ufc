#include "tmgui.h"

#include <QMouseEvent>

#include "tmguistate.h"
#include "tmguiedge.h"

#include <QDebug>

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

    TMGUIState *from,*to;
    scene->addItem(from = new TMGUIState(0,0,this));
    scene->addItem(to = new TMGUIState(100,100,this));
    scene->addItem(new TMGUIEdge(from,to));
}

TMGUI::~TMGUI()
{
}

void TMGUI::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "gui pressed";
    QGraphicsView::mousePressEvent(event);
}
