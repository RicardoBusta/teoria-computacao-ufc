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
    QColor machine_color = QColor(150,150,0);

    // White
    // Comment
    io_format::comment.setForeground(QBrush(comment_color));

    // Error
    io_format::error.setUnderlineColor(error_color);
    io_format::error.setUnderlineStyle(QTextCharFormat::WaveUnderline);

    // State
    io_format::state.setForeground(QBrush(state_color));
    io_format::state.setFontItalic(true);
    // spec
    io_format::state_spec.setForeground(QBrush(state_color));
    io_format::state_spec.setFontWeight(99);

    // Character
    io_format::character.setForeground(QBrush(character_color));
    // spec
    io_format::character_spec.setForeground(QBrush(character_color));

    // Command
    io_format::command.setForeground(QBrush(command_color));

    // Option
    io_format::option.setForeground(QBrush(option_color));

    // Machine
    io_format::machine.setForeground(QBrush(machine_color));
    io_format::machine.setFontItalic(true);
    // spec
    io_format::machine_spec.setForeground(QBrush(machine_color));
    io_format::machine_spec.setFontWeight(99);
}

void TMSyntax::highlightBlock(const QString &text)
{
    if(previousBlockState()==1){
        //setCurrentBlockState(1);
        setFormat(0,text.size(),io_format::error);
        return;
    }

    //setCurrentBlockState(0);

    switch(io_ex::line_type(text)){
    case io_ex::LINE_OPTION:
        setFormat(0,text.size(),io_format::option);
        break;

    case io_ex::LINE_VALID:
        break_line_token(text);

        switch(token_list[0].type){
        case io_ex::TOKEN_STATE_SPEC:
             setFormat(token_list[0].start, token_list[0].count, io_format::state_spec);
            break;
        case io_ex::TOKEN_STATE:
            setFormat(token_list[0].start,token_list[0].count,io_format::state);
            break;
        case io_ex::TOKEN_MACHINE_SPEC:
            setFormat(token_list[0].start,token_list[0].count,io_format::machine_spec);
            break;
        case io_ex::TOKEN_MACHINE:
            setFormat(token_list[0].start,token_list[0].count,io_format::machine);
            break;
        default:
            break;
        }

        switch(token_list[1].type){

        case io_ex::TOKEN_CHARACTER_SPEC:
            setFormat(token_list[1].start,token_list[1].count,io_format::character_spec);
            break;
        case io_ex::TOKEN_CHARACTER:
            setFormat(token_list[1].start,token_list[1].count,io_format::character_spec);
            break;
        default:
            break;
        }

        switch (token_list[2].type) {
        case io_ex::TOKEN_STATE_SPEC:
            setFormat(token_list[2].start,token_list[2].count,io_format::state_spec);
            break;
        case io_ex::TOKEN_STATE:
            setFormat(token_list[2].start,token_list[2].count,io_format::state);
            break;
        case io_ex::TOKEN_MACHINE_SPEC:
            setFormat(token_list[2].start,token_list[2].count,io_format::machine_spec);
            break;
        case io_ex::TOKEN_MACHINE:
            setFormat(token_list[2].start,token_list[2].count,io_format::machine);
            break;
        default:
            break;
        }

        switch (token_list[3].type) {
        case io_ex::TOKEN_COMMAND:
            setFormat(token_list[3].start,token_list[3].count,io_format::command);
            break;
        case io_ex::TOKEN_CHARACTER_SPEC:
            setFormat(token_list[3].start,token_list[3].count,io_format::character_spec);
            break;
        case io_ex::TOKEN_CHARACTER:
            setFormat(token_list[3].start,token_list[3].count,io_format::character);
            break;
        default:
            break;
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
        //setCurrentBlockState(1);
        break;
    }
}

