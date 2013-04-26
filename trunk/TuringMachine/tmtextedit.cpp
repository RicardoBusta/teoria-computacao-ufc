#include "tmtextedit.h"

#include "tmsyntax.h"

#include <QKeyEvent>

TMTextEdit::TMTextEdit(QWidget *parent) :
    QTextEdit(parent)
{

}

void TMTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Slash && (e->modifiers() & Qt::ControlModifier) ){
        QTextCursor tc = textCursor();
        tc.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
        if(tc.block().text().startsWith("//")){
            tc.deleteChar();
            tc.deleteChar();
        }else{
            tc.insertText("//");
        }

    }else{
        QTextEdit::keyPressEvent(e);
    }
}
