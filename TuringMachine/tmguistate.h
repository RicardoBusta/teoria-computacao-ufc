#ifndef TMGUISTATE_H
#define TMGUISTATE_H

#include <QGraphicsItem>
#include "tmgui.h"

class TMGUIState : public QGraphicsItem
{
//    Q_OBJECT
public:
    explicit TMGUIState(int x, int y,TMGUI *tmgui = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
//private:
    TMGUI *tmgui;

//    void mousePressEvent(QGraphicsSceneMouseEvent *e);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
signals:

public slots:

};

#endif // TMGUISTATE_H
