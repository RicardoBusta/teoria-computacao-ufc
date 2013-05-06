#ifndef TMTEXTEDIT_H
#define TMTEXTEDIT_H

#include <QTextEdit>

class TMTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TMTextEdit(QWidget *parent = 0);
    
    void keyPressEvent(QKeyEvent *e);
signals:
    
public slots:
    
};

#endif // TMTEXTEDIT_H
