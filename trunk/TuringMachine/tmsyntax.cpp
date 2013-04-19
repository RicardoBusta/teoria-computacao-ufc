#include "tmsyntax.h"

#include <QDebug>
#include <QTextEdit>

#include "exnamespace.h"

TMSyntax::TMSyntax(QTextEdit *parent) :
    QSyntaxHighlighter(parent)
{
    QColor comment_color = QColor(0,150,0);
    QColor state_color = QColor(0,0,150);
    QColor error_color = QColor(255,0,0);
    QColor character_color = QColor(0,150,150);
    QColor command_color = QColor(150,0,150);
    QColor option_color = QColor(250,0,150);

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
    // Option
    io_format::option.setForeground(QBrush(option_color));
}

static QString token_name[] = {
    "TOKEN_ERROR",
    "TOKEN_STATE",
    "TOKEN_STATE_SPEC",
    "TOKEN_CHARACTER",
    "TOKEN_CHARACTER_SPEC",
    "TOKEN_COMMAND",
    "TOKEN_COMMAND_SPEC"
};

void TMSyntax::highlightBlock(const QString &text)
{
    if(previousBlockState()==1){
        setCurrentBlockState(1);
        setFormat(0,text.size(),io_format::error);
        return;
    }

    setCurrentBlockState(0);
    switch(io_ex::line_type(text)){

    case io_ex::LINE_OPTION:
        setFormat(0,text.size(),io_format::option);
        break;

    case io_ex::LINE_VALID:
        break_line_token(text);

        qDebug() << "===";
        qDebug() << text.mid(token_list[0].start,token_list[0].count) << token_list[0].start << token_list[0].count << token_name[token_list[0].type];
        qDebug() << text.mid(token_list[1].start,token_list[1].count) << token_list[1].start << token_list[1].count << token_name[token_list[1].type];
        qDebug() << text.mid(token_list[2].start,token_list[2].count) << token_list[2].start << token_list[2].count << token_name[token_list[2].type];
        qDebug() << text.mid(token_list[3].start,token_list[3].count) << token_list[3].start << token_list[3].count << token_name[token_list[3].type];

        if( token_list[0].type == io_ex::TOKEN_STATE_SPEC ){
            setFormat(token_list[0].start, token_list[0].count, io_format::state_spec);
        }else if( token_list[0].type == io_ex::TOKEN_STATE ){
            setFormat(token_list[0].start,token_list[0].count,io_format::state);
        }

        if(token_list[1].type == io_ex::TOKEN_CHARACTER_SPEC){
            setFormat(token_list[1].start,token_list[1].count,io_format::character_spec);
        }else if(token_list[1].type == io_ex::TOKEN_CHARACTER){
            setFormat(token_list[1].start,token_list[1].count,io_format::character);
        }

        if(token_list[2].type == io_ex::TOKEN_STATE_SPEC){
            setFormat(token_list[2].start,token_list[2].count,io_format::state_spec);
        }else if(token_list[2].type == io_ex::TOKEN_STATE){
            setFormat(token_list[2].start,token_list[2].count,io_format::state);
        }

        if(token_list[3].type == io_ex::TOKEN_COMMAND_SPEC){
            setFormat(token_list[3].start,token_list[3].count,io_format::command_spec);
        }else if(token_list[3].type == io_ex::TOKEN_COMMAND){
            setFormat(token_list[3].start,token_list[3].count,io_format::command);
        }
        break;

    case io_ex::LINE_COMMENT:
        setFormat(0,text.size(),io_format::comment);
        break;

    case io_ex::LINE_WHITE:
        setFormat(0,text.size(),io_format::blank);
        break;

    case io_ex::LINE_ERROR:
    default:
        setFormat(0,text.size(),io_format::error);
        setCurrentBlockState(1);
        break;
    }
}

