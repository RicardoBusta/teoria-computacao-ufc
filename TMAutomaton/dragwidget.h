#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>

#include "dragelement.h"
#include "dragedge.h"
#include "dragstate.h"

enum DRAG_TOOLS{TOOL_CREATE,TOOL_MOVE,TOOL_LINK,TOOL_RENAME};

class DragWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = 0);
    
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *);

    DragState *iniState;
    QList<DragState*> state;
    QList<DragEdge*> edge;
    DragState *selected;

    DRAG_TOOLS current_tool;
    QPointF anchor;
    bool multipleSelect;
    QPoint mousePos;
    QRect multipleSelectRect;
    QPoint mousePrev;

    QRectF container;

    QList<DragElement*> selectedElements;

    DragElement *pick_element(QPoint epos);
signals:
    
public slots:
    void setCreateTool();
    void setMoveTool();
    void setLinkTool();
    void setRenameTool();
};

#endif // DRAGWIDGET_H
