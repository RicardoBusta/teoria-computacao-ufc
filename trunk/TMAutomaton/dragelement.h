#ifndef DRAGELEMENT_H
#define DRAGELEMENT_H

#include <QPoint>
#include <QRect>
#include <QString>
#include <QPainter>

enum DRAG_{
    DRAG_STATE,
    DRAG_EDGE,
    DRAG_TEXT
};

class DragElement{
public:
    explicit DragElement():pos(QPoint(0,0)),selected(false),dragged(false){}

    DRAG_ type;

    QPoint pos;

    bool dragged;
    bool selected;

    virtual bool pick(QPoint)=0;
    virtual void render(QPainter *painter)=0;
    virtual QRect boundingBox()=0;
    virtual void keepInside(QRect container){
        QRect bb = boundingBox();
        if(container.left() > bb.left()){
            pos.setX(container.left()+bb.width()/2);
        }else if(container.right() < bb.right()){
            pos.setX(container.right()-bb.width()/2);
        }

        if(container.top() > bb.top()){
            pos.setY(container.top()+bb.height()/2);
        }else if(container.bottom() < bb.bottom()){
            pos.setY(container.bottom()-bb.height()/2);
        }
    }
};


#endif // DRAGELEMENT_H
