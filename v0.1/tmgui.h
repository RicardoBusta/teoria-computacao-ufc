#ifndef TMGUI_H
#define TMGUI_H

#include <QGraphicsView>


class TMGUI : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit TMGUI(QWidget *parent = 0);
    ~TMGUI();
private:
};

#endif // TMGUI_H
