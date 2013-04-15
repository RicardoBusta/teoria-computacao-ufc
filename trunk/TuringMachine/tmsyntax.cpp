#include "tmsyntax.h"

#include <QDebug>
#include <QTextEdit>

TMSyntax::TMSyntax(QTextEdit *parent) :
    QSyntaxHighlighter(parent)
{
    QColor comment_color = QColor(0,150,0);
    QColor state_color = QColor(0,0,150);
    QColor error_color = QColor(255,0,0);
    QColor character_color = QColor(0,150,150);
    QColor command_color = QColor(150,0,150);

    // White
    // Comment
    io_format::comment.setForeground(QBrush(comment_color));
    // Error
    io_format::error.setUnderlineColor(error_color);
    io_format::error.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    // State
    io_format::state.setForeground(QBrush(state_color));
    io_format::state.setFontItalic(true);

    io_format::state_spec.setForeground(QBrush(state_color));
    io_format::state_spec.setFontWeight(99);
    // Character
    io_format::character.setForeground(QBrush(character_color));

    io_format::character_spec.setForeground(QBrush(character_color));
    // Command
    io_format::command = io_format::character;

    io_format::command_spec.setForeground(QBrush(command_color));
}

void TMSyntax::highlightBlock(const QString &text)
{
    /**/ if(io_ex::white_line.exactMatch(text)){
        setFormat(0,text.size(),io_format::blank);
    }
    else if(io_ex::comment.exactMatch(text)){
        setFormat(0,text.size(),io_format::comment);
    }
    else if(io_ex::valid_line.exactMatch(text)){
        //STATE
        int pos =0;
        pos = io_ex::state.indexIn(text,pos);
        if(io_ex::state_spec.exactMatch(text.mid(pos,io_ex::state.matchedLength()))){
            setFormat(pos,io_ex::state.matchedLength(),io_format::state_spec);
        }else{
            setFormat(pos,io_ex::state.matchedLength(),io_format::state);
        }
        pos += io_ex::state.matchedLength();

        //CHARACTER
        pos = io_ex::character.indexIn(text,pos);
        if(io_ex::character_spec.exactMatch(text.mid(pos,io_ex::character.matchedLength()))){
            setFormat(pos,io_ex::character.matchedLength(),io_format::character_spec);
        }else{
            setFormat(pos,io_ex::character.matchedLength(),io_format::character);
        }
        pos += io_ex::character.matchedLength();

        //STATE
        pos = io_ex::state.indexIn(text,pos);
        if(io_ex::state_spec.exactMatch(text.mid(pos,io_ex::state.matchedLength()))){
            setFormat(pos,io_ex::state.matchedLength(),io_format::state_spec);
        }else{
            setFormat(pos,io_ex::state.matchedLength(),io_format::state);
        }
        pos += io_ex::state.matchedLength();

        //COMMAND
        pos = io_ex::command.indexIn(text,pos);
        if(io_ex::command_spec.exactMatch(text.mid(pos,io_ex::command.matchedLength()))){
            setFormat(pos,io_ex::command.matchedLength(),io_format::command_spec);
        }else{
            setFormat(pos,io_ex::command.matchedLength(),io_format::command);
        }
    }
    else{
        setFormat(0,text.size(),io_format::error);
    }
    //    int pos = 0;
    //    pos = ex::word.indexIn(text,pos);
    //    if(pos >= 0){
    //        if(ex::state.exactMatch(text.mid(pos,ex::word.matchedLength()))){
    //            setFormat(pos, ex::state.matchedLength(), io_format::state);

    //        }else{

    //        }
    //    }

}
