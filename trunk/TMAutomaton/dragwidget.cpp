#include "dragwidget.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QVector2D>
#include <qmath.h>

DragWidget::DragWidget(QWidget *parent) :
    QWidget(parent),
    current_tool(TOOL_CREATE),selected(NULL),multipleSelect(false)
{
    this->setFixedSize(1000,1000);
}

void DragWidget::paintEvent(QPaintEvent *)
{
    container = QRect(0,0,500,500);
    foreach(DragElement *el, element){
        if( (el->pos.x()-el->radius)<container.left() ){
            container.setLeft( (el->pos.x()-el->radius) );
        }
        if( (el->pos.x()+el->radius)>container.right() ){
            container.setRight( (el->pos.x()+el->radius) );
        }
        if( (el->pos.y()-el->radius)<container.top() ){
            container.setTop( (el->pos.y()-el->radius) );
        }
        if( (el->pos.y()+el->radius)>container.bottom() ){
            container.setBottom( (el->pos.y()+el->radius) );
        }
    }

    foreach(DragElement *el, element){
        el->pos = el->pos - container.topLeft();
    }

    this->setFixedSize(container.adjusted(-20,-20,20,20).size().toSize());

    QPainter painter(this);



    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(container);

    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);

    float sint = sin(-30*M_PI/180);
    float cost = cos(-30*M_PI/180);

    foreach(DragEdge *ed,edge){
        if(ed->e1 && ed->e2){
            if(ed->e1 != ed->e2){
                painter.setBrush(Qt::NoBrush);
                painter.setPen(Qt::black);
                QVector2D v1(ed->e1->pos-ed->e2->pos);
                //            int len = v1.length();
                //            float ang = atan(v1.y()/v1.x())*180/M_PI;
                v1.normalize();
                QVector2D v2(ed->e2->pos-ed->e1->pos);
                v2.normalize();
                QPointF p[4];
                p[0] = ed->e1->pos-(ed->e1->radius*v1.toPointF());
                p[1] = ed->e2->pos-(ed->e2->radius*v2.toPointF());
                painter.drawLine(p[0],p[1]);

                painter.drawText(((p[0]+p[1])/2),"HMM");

                p[2] = p[1]+(20*QPointF(v1.x()*cost-v1.y()*sint,v1.x()*sint+v1.y()*cost));
                p[3] = p[1]+(20*QPointF(v1.x()*cost-v1.y()*(-sint),v1.x()*(-sint)+v1.y()*cost));
                painter.setBrush(Qt::black);
                painter.drawPolygon(p+1,3);
            }else{
                painter.setPen(Qt::black);
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(ed->e1->pos+QPointF(0,-30),20,30);
                painter.drawText(ed->e1->pos+QPointF(0,-60),"HMM");

                QPointF p[3];
                p[0] = ed->e1->pos+QPointF(-20,-25);
                p[1] = p[0]+(20*QPointF(+sint,-cost));
                p[2] = p[0]+(20*QPointF(-sint,-cost));
                painter.setBrush(Qt::black);
                painter.drawPolygon(p,3);
            }
        }else{
            delete ed;
            edge.removeOne(ed);
        }
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::cyan);
    painter.drawRect(multipleSelectRect);

    foreach(DragElement *el,element){
        painter.setPen(Qt::NoPen);
        if(el == selected || selectedElements.contains(el)){
            painter.setBrush(Qt::cyan);
            painter.drawEllipse(el->pos,el->radius+10,el->radius+10);
        }
        painter.setBrush(Qt::black);
        painter.drawEllipse(el->pos,el->radius+3,el->radius+3);
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(el->pos,el->radius,el->radius);

        QRectF rect = QRectF(el->pos-QPointF(el->radius,el->radius),el->pos+QPointF(el->radius,el->radius));
        painter.setPen(Qt::black);
        painter.drawText(rect,el->txt,Qt::AlignVCenter|Qt::AlignHCenter);
    }

    painter.end();
}

void DragWidget::mousePressEvent(QMouseEvent *e)
{

    switch(current_tool){
    case TOOL_CREATE:
        element.push_back(new DragElement(e->pos()));
        break;
    case TOOL_MOVE:
        selected = pick_element(e->pos());
        if(!selected){
            selectedElements.clear();
            multipleSelect = true;
            mousePos = e->pos();
        }
        if(!selectedElements.contains(selected)){
            selectedElements.clear();
            mousePos = e->pos();
        }
        mousePrev = e->pos();
        break;
    case TOOL_LINK:
        if(selected){
            DragElement *sel2 = pick_element(e->pos());
            //            if(selected!=sel2){
            edge.push_back(new DragEdge(selected,sel2));
            selected = NULL;
            //            }
        }else{
            selected = pick_element(e->pos());
        }
        break;
    default:
        return;
    }
    this->repaint();
}

void DragWidget::mouseReleaseEvent(QMouseEvent *)
{
    switch(current_tool){
    case TOOL_CREATE:
        break;
    case TOOL_MOVE:
        multipleSelect = false;
        multipleSelectRect.setRect(0,0,0,0);
        selected = NULL;
        break;
    default:
        return;
    }
    this->repaint();
}


void DragWidget::mouseMoveEvent(QMouseEvent *e)
{
    switch(current_tool){
    case TOOL_CREATE:
        break;
    case TOOL_MOVE:
        if(multipleSelect){
            multipleSelectRect = QRect( QPoint( qMin(mousePos.x(),e->pos().x()),qMin( mousePos.y(),e->pos().y()) ), QPoint( qMax(mousePos.x(),e->pos().x()),qMax(mousePos.y(),e->pos().y()) ) );
            foreach(DragElement* el, element){
                if( el->pos.x() < multipleSelectRect.right() && el->pos.x() > multipleSelectRect.left() && el->pos.y() > multipleSelectRect.top() && el->pos.y() < multipleSelectRect.bottom() ){
                    if(!selectedElements.contains(el)){
                        selectedElements.push_back(el);
                    }
                }else{
                    if(selectedElements.contains(el)){
                        selectedElements.removeOne(el);
                    }
                }
            }
        }
        if(!multipleSelect && !selectedElements.empty()){
            foreach(DragElement* el,selectedElements){
                if(el!=selected){
                    el->pos = el->pos + (e->pos()-mousePrev);
                }
            }
        }
        mousePrev = e->pos();
        if(selected){
            selected->pos = e->pos() - anchor;
        }
        break;
    default:
        return;
    }
    this->repaint();
}

DragElement *DragWidget::pick_element(QPointF epos)
{
    DragElement *ele = NULL;
    foreach(DragElement *el,element){
        QPointF delta = epos - el->pos;
        if( (delta.x()*delta.x()) + (delta.y()*delta.y()) <= el->radius*el->radius ){
            anchor = delta;
            ele = el;
        }
    }
    return ele;
}

void DragWidget::setCreateTool()
{
    current_tool = TOOL_CREATE;
    selected = NULL;
    this->setCursor(Qt::CrossCursor);
}

void DragWidget::setMoveTool()
{
    current_tool = TOOL_MOVE;
    selected = NULL;
    this->setCursor(Qt::OpenHandCursor);
}

void DragWidget::setLinkTool()
{
    current_tool = TOOL_LINK;
    selected = NULL;
    this->setCursor(Qt::SizeAllCursor);
}
