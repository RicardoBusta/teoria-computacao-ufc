#ifndef EXNAMESPACE_H
#define EXNAMESPACE_H

#include <QRegExp>
#include <QFont>
#include <QTextCharFormat>
#include <QDebug>

namespace io_ex{

enum TOKEN{
    TOKEN_ERROR,
    TOKEN_STATE,
    TOKEN_STATE_SPEC,
    TOKEN_CHARACTER,
    TOKEN_CHARACTER_SPEC,
    TOKEN_COMMAND,
    TOKEN_COMMAND_SPEC
};

enum LINE{
    LINE_ERROR,
    LINE_WHITE,
    LINE_VALID,
    LINE_OPTION,
    LINE_COMMENT
};

//Characters
const static QString halt_state = QString("HALT");
const static QString error_state = QString("ERROR");
const static QString blank_character = QString("#");
const static QString begin_character = QString("@");
const static QString left_command = QString("<");
const static QString right_command = QString(">");

//Tokens
const static QRegExp token = QRegExp("(\\S+)");
const static QRegExp state_spec = QRegExp("("+halt_state+")");
const static QRegExp state = QRegExp("[a-zA-Z][a-zA-Z0-9_]*");
const static QRegExp character_spec = QRegExp("["+blank_character+begin_character+"]");
const static QRegExp character = QRegExp("[a-zA-Z0-9]");
const static QRegExp command_spec = QRegExp("["+left_command+right_command+"]");
const static QRegExp command = QRegExp("[a-zA-Z0-9]");

const static QRegExp option = QRegExp("name|NAME");
const static QRegExp option_line = QRegExp("^#("+option.pattern()+")(\\s+)(\\S+)(\\s*)$");
//Lines
const static QRegExp comment = QRegExp("^((//(.*))|(/\\*(.*)\\*/))$"/*|(#(.*))*/);
const static QRegExp white_line = QRegExp("^(\\s*)$");
const static QRegExp valid_line = QRegExp("^(\\s*)("+io_ex::state.pattern()+"|"+state_spec.pattern()+")"
                                          +"(\\s+)("+io_ex::character.pattern()+"|"+io_ex::character_spec.pattern()+")"
                                          +"(\\s+)("+io_ex::state.pattern()+"|"+state_spec.pattern()+")"
                                          +"(\\s+)("+io_ex::command.pattern()+"|"+command_spec.pattern()+"|"+character_spec.pattern()+")(\\s*)$");


static inline TOKEN token_type(const QString s){
    if(state_spec.exactMatch(s)){
        return TOKEN_STATE_SPEC;
    }else if(state.exactMatch(s)){
        return TOKEN_STATE;
    }else if(character_spec.exactMatch(s)){
        return TOKEN_CHARACTER_SPEC;
    }else if(character.exactMatch(s)){
        return TOKEN_CHARACTER;
    }
    return TOKEN_ERROR;
}
static inline LINE line_type(const QString s){
    if(white_line.exactMatch(s)){
        return LINE_WHITE;
    }else if(option.exactMatch(s)){
        return LINE_OPTION;
    }else if(comment.exactMatch(s)){
        return LINE_COMMENT;
    }else if(valid_line.exactMatch(s)){
        return LINE_VALID;
    }
    return LINE_ERROR;
}

}

namespace io_format{
static QTextCharFormat blank;
static QTextCharFormat comment;
static QTextCharFormat option;
static QTextCharFormat error;
static QTextCharFormat state;
static QTextCharFormat state_spec;
static QTextCharFormat character;
static QTextCharFormat character_spec;
static QTextCharFormat command;
static QTextCharFormat command_spec;
}

namespace io_font{
static QFont input = QFont("consolas",12);
}

#endif // EXNAMESPACE_H
