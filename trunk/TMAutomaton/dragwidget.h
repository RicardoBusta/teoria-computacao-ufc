#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>

#include "dragelement.h"
#include "dragedge.h"
#include "dragstate.h"
#include "dragtext.h"

enum DRAG_TOOLS{
    TOOL_CREATE,
    TOOL_MOVE,
    TOOL_LINK,
    TOOL_RENAME,
    TOOL_HALT,
    TOOL_START,
    TOOL_DELETE,
    TOOL_TEXT
};

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
    QList<DragElement*> element;
    DragState *selected;

    DRAG_TOOLS current_tool;
    QPoint anchor;
    bool multipleSelect;
    QPoint mousePos;
    QRect multipleSelectRect;
    QPoint mousePrev;

    QRectF container;

    QList<DragElement*> selectedElements;

    DragElement *pick_element(QPoint epos);

    void renderScene(QPaintDevice *device);

    void clearSelected();
    void clearDragged();
signals:
    
public slots:
    void setCreateTool();
    void setMoveTool();
    void setLinkTool();
    void setRenameTool();
    void setFirstTool();
    void setHaltTool();
    void setAddTextTool();
    void setDeleteTool();

    void exportSVG();
};

#endif // DRAGWIDGET_H
