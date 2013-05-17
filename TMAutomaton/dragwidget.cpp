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
    case TOOL_MOVE:
        ele = pick_element(e->pos());
        if(ele && ele->type==DRAG_STATE){
            selected = dynamic_cast<DragState*>(ele);
            selected->dragged = true;
        }
        if(!selected){
            clearSelected();
            multipleSelect = true;
            mousePos = e->pos();
        }
        if(!selectedElements.contains(selected)){
            clearSelected();
            mousePos = e->pos();
        }

        mousePrev = e->pos();
        break;
    case TOOL_LINK:
        if(selected){
            DragState *sel2 = dynamic_cast<DragState*>(pick_element(e->pos()));
            //            if(selected!=sel2){
            element.push_back(new DragEdge(selected,sel2));
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
        element.push_back(new DragState(e->pos()));
        break;
    case TOOL_MOVE:
        multipleSelect = false;
        multipleSelectRect.setRect(0,0,0,0);
        clearDragged();
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
    case TOOL_TEXT:
        element.push_back(new DragText(e->pos()));
    default:
        break;
    }

    foreach(DragElement *el, element){

    }
    this->repaint();
}


void DragWidget::mouseMoveEvent(QMouseEvent *e)
{
    switch(current_tool){
    case TOOL_MOVE:
        if(multipleSelect){
            multipleSelectRect = QRect( QPoint( qMin(mousePos.x(),e->pos().x()),qMin( mousePos.y(),e->pos().y()) ), QPoint( qMax(mousePos.x(),e->pos().x()),qMax(mousePos.y(),e->pos().y()) ) );
            foreach(DragElement* el, element){
                if( el->pos.x() < multipleSelectRect.right() && el->pos.x() > multipleSelectRect.left() && el->pos.y() > multipleSelectRect.top() && el->pos.y() < multipleSelectRect.bottom() ){
                    if(!selectedElements.contains(el)){
                        selectedElements.push_back(el);
                        el->selected=true;
                    }
                }else{
                    if(selectedElements.contains(el)){
                        selectedElements.removeOne(el);
                        el->selected=false;
                    }
                }
            }
        }
        if(!multipleSelect && !selectedElements.empty()){
            foreach(DragElement* el,selectedElements){
                if(el->selected){
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
        break;
    }
    this->repaint();
}

DragElement *DragWidget::pick_element(QPoint epos)
{
    DragElement *ele = NULL;
    foreach(DragElement *el,element){
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

    foreach(DragElement *ed,element){
        ed->render(&painter);
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::lightGray,2,Qt::DashLine));
    painter.drawRect(multipleSelectRect);

    painter.end();
}

void DragWidget::clearSelected()
{
    while(!selectedElements.empty()){
        selectedElements.takeFirst()->selected = false;
    }
}

void DragWidget::clearDragged()
{
    if(selected){
        selected->dragged = false;
        selected = NULL;
    }
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
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://create.svg"),0,0));
    this->repaint();
}

void DragWidget::setMoveTool()
{
    current_tool = TOOL_MOVE;
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://move.svg"),0,0));
    this->repaint();
}

void DragWidget::setLinkTool()
{
    current_tool = TOOL_LINK;
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://link.svg"),0,0));
    this->repaint();
}

void DragWidget::setRenameTool()
{
    current_tool = TOOL_RENAME;
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://rename.svg"),0,0));
    this->repaint();
}

void DragWidget::setFirstTool()
{
    current_tool = TOOL_START;
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://first.svg"),0,0));
    this->repaint();
}

void DragWidget::setHaltTool()
{
    current_tool = TOOL_HALT;
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://halt.svg"),0,0));
    this->repaint();
}

void DragWidget::setAddTextTool()
{
    current_tool = TOOL_TEXT;
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://text.svg"),0,0));
    this->repaint();
}

void DragWidget::setDeleteTool()
{
    current_tool = TOOL_DELETE;
    clearDragged();
    clearSelected();
    this->setCursor(QCursor(QPixmap("://delete.svg"),0,0));
    this->repaint();
}
