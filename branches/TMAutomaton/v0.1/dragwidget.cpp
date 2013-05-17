#include "dragwidget.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QVector2D>
#include <qmath.h>
#include <QSvgGenerator>

#include "staticresource.h"

DragWidget::DragWidget(QWidget *parent) :
    QWidget(parent),iniState(NULL),
    current_tool(TOOL_CREATE),selected(NULL),multipleSelect(false)
{
    this->setFixedSize(500,500);
    container = QRect(0,0,width(),height());
}

void DragWidget::paintEvent(QPaintEvent *)
{
    renderScene(this);
}

void DragWidget::mousePressEvent(QMouseEvent *e)
{
    DragElement *ele;

    switch(current_tool){
    case TOOL_CREATE:

        break;
    case TOOL_MOVE:
        ele = pick_element(e->pos());
        if(ele && ele->type==DRAG_STATE){
            selected = dynamic_cast<DragState*>(ele);
        }
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
            DragState *sel2 = dynamic_cast<DragState*>(pick_element(e->pos()));
            //            if(selected!=sel2){
            edge.push_back(new DragEdge(selected,sel2));
            selected = NULL;
            //            }
        }else{
            ele = pick_element(e->pos());
            if(ele && ele->type==DRAG_STATE){
                selected = static_cast<DragState*>(ele);
            }
        }
        break;
    case TOOL_RENAME:
        break;
    default:
        break;
    }
    this->repaint();
}

#include <QInputDialog>

void DragWidget::mouseReleaseEvent(QMouseEvent *e)
{
    DragElement *ele;
    QInputDialog *dialog;

    switch(current_tool){
    case TOOL_CREATE:
        state.push_back(new DragState(e->pos()));
        break;
    case TOOL_MOVE:
        multipleSelect = false;
        multipleSelectRect.setRect(0,0,0,0);
        selected = NULL;
        break;
    case TOOL_RENAME:
        ele = pick_element(e->pos());
        if(!ele) break;

        dialog = new QInputDialog(this);
        dialog->setTextValue(static_cast<DragState*>(ele)->txt);
        if(dialog->exec()==QDialog::Accepted){
            switch(ele->type){
            case DRAG_EDGE:
                //static_cast<DragEdge*>(ele)
                break;
            case DRAG_STATE:
                static_cast<DragState*>(ele)->txt = dialog->textValue();
                break;
            default:
                break;
            }
        }
        delete dialog;
        break;
    case TOOL_START:
        ele = pick_element(e->pos());
        if(!ele || ele->type!=DRAG_STATE) break;

        iniState = static_cast<DragState*>(ele);

        break;
    case TOOL_HALT:
        ele = pick_element(e->pos());
        if(!ele || ele->type!=DRAG_STATE) break;

        static_cast<DragState*>(ele)->final = !static_cast<DragState*>(ele)->final;
        break;
    default:
        break;
    }

    foreach(DragState *st, state){
        if(container.left() > st->pos.x()-st->radius){
            st->pos.setX(container.left()+(st->radius+10));
        }else if(container.right() < st->pos.x()+st->radius){
            st->pos.setX(container.right()-(st->radius+10));
        }

        if(container.top() > st->pos.y()-st->radius){
            st->pos.setY(container.top()+(st->radius+10));
        }else if(container.bottom() < st->pos.y()+st->radius){
            st->pos.setY(container.bottom()-(st->radius+10));
        }
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
            foreach(DragState* el, state){
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
    case TOOL_LINK:
        break;
    case TOOL_RENAME:
        break;
    default:
        break;
    }
    this->repaint();
}

DragElement *DragWidget::pick_element(QPoint epos)
{
    DragElement *ele = NULL;
    foreach(DragState *el,state){
        if(el->pick(epos)){
            anchor = epos-el->pos;
            ele = el;
        }
    }
    return ele;
}

void DragWidget::renderScene(QPaintDevice *device)
{
    QPainter painter;
    painter.begin(device);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);

    //White Background
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());

    const float sint = sin(-30*M_PI/180);
    const float cost = cos(-30*M_PI/180);

    foreach(DragEdge *ed,edge){
        if(ed->e1 && ed->e2){
            if(ed->e1 != ed->e2){
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
                QVector2D v1(ed->e1->pos-ed->e2->pos);

                v1.normalize();
                QVector2D v2(ed->e2->pos-ed->e1->pos);
                v2.normalize();
                QPointF p[4];
                p[0] = ed->e1->pos-(ed->e1->radius*v1.toPointF());
                p[1] = ed->e2->pos-(ed->e2->radius*v2.toPointF());
                painter.drawLine(p[0],p[1]);

                painter.drawText(((p[0]+p[1])/2)+QPointF(0,-10),"HMM");

                p[2] = p[1]+(10*QPointF(v1.x()*cost-v1.y()*sint,v1.x()*sint+v1.y()*cost));
                p[3] = p[1]+(10*QPointF(v1.x()*cost-v1.y()*(-sint),v1.x()*(-sint)+v1.y()*cost));
                painter.setBrush(Qt::black);
                painter.drawPolygon(p+1,3);
            }else{
                painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(ed->e1->pos+QPointF(0,-30),20,30);
                painter.drawText(ed->e1->pos+QPointF(0,-60),"HMM");

                QPointF p[3];
                p[0] = ed->e1->pos+QPointF(-20,-25);
                p[1] = p[0]+(10*QPointF(+sint,-cost));
                p[2] = p[0]+(10*QPointF(-sint,-cost));
                painter.setBrush(Qt::black);
                painter.drawPolygon(p,3);
            }
        }else{
            delete ed;
            edge.removeOne(ed);
        }
    }

    foreach(DragState *el,state){
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(el->pos,el->radius,el->radius);

        if(el == selected || selectedElements.contains(el)){
            painter.setPen(QPen(Qt::cyan,4,Qt::SolidLine));
        }else{
            painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
        }
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(el->pos,el->radius,el->radius);

        if(iniState==el){
            QPointF p[3];
            painter.setBrush(Qt::white);
            p[0] = el->pos+QPointF(-el->radius,0);
            p[1] = el->pos+QPointF(-el->radius-20,20);
            p[2] = el->pos+QPointF(-el->radius-20,-20);
            painter.drawPolygon(p,3);
        }

        if(el->final){
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
            painter.drawEllipse(el->pos,el->radius-5,el->radius-5);
        }

        QRectF rect = QRectF(el->pos-QPointF(el->radius,el->radius),el->pos+QPointF(el->radius,el->radius));
        painter.setPen(Qt::black);
        painter.drawText(rect,el->txt,Qt::AlignVCenter|Qt::AlignHCenter);
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::gray,2,Qt::DashLine));
    painter.drawRect(multipleSelectRect);

    painter.end();
}

void DragWidget::exportSVG()
{
    QSvgGenerator svgGen;

    svgGen.setFileName( "test.svg" );
    svgGen.setSize(this->size());
    svgGen.setViewBox(this->rect());
    svgGen.setTitle(tr("SVG Test"));
    svgGen.setDescription(tr(" Test "
                             " Uh huh "));

    renderScene(&svgGen);
}

void DragWidget::setCreateTool()
{
    current_tool = TOOL_CREATE;
    selected = NULL;
    this->setCursor(QCursor(QPixmap("://create.svg"),0,0));
}

void DragWidget::setMoveTool()
{
    current_tool = TOOL_MOVE;
    selected = NULL;
    this->setCursor(QCursor(QPixmap("://move.svg"),0,0));
}

void DragWidget::setLinkTool()
{
    current_tool = TOOL_LINK;
    selected = NULL;
    this->setCursor(QCursor(QPixmap("://link.svg"),0,0));
}

void DragWidget::setRenameTool()
{
    current_tool = TOOL_RENAME;
    selected = NULL;
    this->setCursor(QCursor(QPixmap("://rename.svg"),0,0));
}

void DragWidget::setFirstTool()
{
    current_tool = TOOL_START;
    selected = NULL;
    this->setCursor(QCursor(QPixmap("://first.svg"),0,0));
}

void DragWidget::setHaltTool()
{
    current_tool = TOOL_HALT;
    selected = NULL;
    this->setCursor(QCursor(QPixmap("://halt.svg"),0,0));

}
