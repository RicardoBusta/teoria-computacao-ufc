#ifndef TMGUIEDGE_H
#define TMGUIEDGE_H

#include <QGraphicsItem>

class TMGUIState;

class TMGUIEdge : public QGraphicsItem
{
    //Q_OBJECT
public:
    explicit TMGUIEdge(TMGUIState *from=0, TMGUIState *to=0);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    static QList<TMGUIEdge*> list;

    void adjust();
private:
    TMGUIState *from,*to;
signals:
    
public slots:
    
};

#endif // TMGUIEDGE_H
