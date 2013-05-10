#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>

#include "dragelement.h"
#include "dragedge.h"

enum DRAG_TOOLS{TOOL_CREATE,TOOL_MOVE,TOOL_LINK};

class DragWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = 0);
    
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    QList<DragElement*> element;
    QList<DragEdge*> edge;
    DragElement *selected;

    DRAG_TOOLS current_tool;
    QPointF anchor;

    DragElement *pick_element(QPointF epos);
signals:
    
public slots:
    void setCreateTool();
    void setMoveTool();
    void setLinkTool();
};

#endif // DRAGWIDGET_H
