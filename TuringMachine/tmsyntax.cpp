#include "tmsyntax.h"

#include <QDebug>
#include <QTextEdit>

TMSyntax::TMSyntax(QTextEdit *parent) :
    QSyntaxHighlighter(parent)
{
    // Font
    font = QFont("consolas",15);

    //Set Parent
    parent->clear();
    parent->setFont(QFont("consolas",15));

    // White
    //format_white.setFont(font);
    // Comment
    format_comment.setForeground(QBrush(QColor(0,150,0)));
    //format_comment.setFont(font);
    // Error
    format_error.setUnderlineColor(QColor(255,0,0));
    //format_error.setFont(font);
    format_error.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    // State
    format_state.setForeground(QBrush(QColor(0,0,150)));
    //format_state.setFont(font);
    // Character
    format_character.setForeground(QBrush(QColor(0,150,150)));
    //format_character.setFont(font);
    // Command
    format_command.setForeground(QBrush(QColor(150,0,150)));
    //format_command.setFont(font);

    ex_state = QRegExp("[a-zA-Z][a-zA-Z0-9]*");
    ex_alphabet = QRegExp("[a-zA-Z#@]");
    ex_command = QRegExp("[a-zA-Z#><]");

    ex_line = QRegExp("^(\\s*)"+ex_state.pattern()
                      +"(\\s+)"+ex_alphabet.pattern()
                      +"(\\s+)"+ex_state.pattern()
                      +"(\\s+)"+ex_command.pattern()+"(\\s*)$");

    ex_comment = QRegExp("^\
                         ((//(.*))\
                         |(/\\*(.*)\\*/)\
                         |(#(.*)))$");
    ex_white = QRegExp("^(\\s*)$");
}

void TMSyntax::highlightBlock(const QString &text)
{

    /**/ if(ex_white.exactMatch(text)){
        setFormat(0,text.size(),format_white);
    }
    else if(ex_comment.exactMatch(text)){
        setFormat(0,text.size(),format_comment);
    }
    else if(ex_line.exactMatch(text)){
        int pos =0;
        setFormat(0,text.size(),format_white);
        pos = ex_state.indexIn(text,pos);
        setFormat(pos,ex_state.matchedLength(),format_state);
        pos += ex_state.matchedLength();
        pos = ex_alphabet.indexIn(text,pos);
        setFormat(pos,ex_alphabet.matchedLength(),format_character);
        pos += ex_alphabet.matchedLength();
        pos = ex_state.indexIn(text,pos);
        setFormat(pos,ex_state.matchedLength(),format_state);
        pos += ex_state.matchedLength();
        pos = ex_command.indexIn(text,pos);
        setFormat(pos,ex_command.matchedLength(),format_command);
    }
    else{
        setFormat(0,text.size(),format_error);
    }
    //    int pos = 0;
    //    pos = ex_word.indexIn(text,pos);
    //    if(pos >= 0){
    //        if(ex_state.exactMatch(text.mid(pos,ex_word.matchedLength()))){
    //            setFormat(pos, ex_state.matchedLength(), format_state);

    //        }else{

    //        }
    //    }

}

QRegExp TMSyntax::get_valid_exp()
{
    return QRegExp("^\
                   ((\\s*)\\n)*\
                   $");
}
