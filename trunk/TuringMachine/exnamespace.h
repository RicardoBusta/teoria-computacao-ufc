/**
 *  Máquina de Turing - Ricardo Bustamante de Queiroz
 *  Copyright (C) <year>  <name of author>
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 */

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
    TOKEN_MACHINE
};

enum LINE{
    LINE_ERROR,
    LINE_WHITE,
    LINE_VALID_STATE,
    LINE_VALID_MACHINE,
    LINE_OPTION,
    LINE_COMMENT
};

//Characters
const static QString halt_state = QString("halt");
const static QString error_state = QString("error");
const static QString all_character = QString("*");
const static QString blank_character = QString("#");
const static QString begin_character = QString("@");
const static QString left_command = QString("<");
const static QString right_command = QString(">");

//Tokens
const static QRegExp token = QRegExp("(\\S+)");
const static QRegExp name_option = QRegExp("(#(n|N)(a|A)(m|M)(e|E))");
const static QRegExp init_option = QRegExp("(#(i|I)(n|N)(i|I)(t|T))");
const static QRegExp halt_option = QRegExp("(#(h|H)(a|A)(l|L)(t|T))");
const static QRegExp step_option = QRegExp("(#(s|S)(t|T)(e|E)(p|P))");
const static QRegExp tape_option = QRegExp("(#(t|T)(a|A)(p|P)(e|E))");
const static QRegExp include_option = QRegExp("(#(i|I)(n|N)(c|C)(l|L)(u|U)(d|D)(e|E))");
const static QRegExp machine = QRegExp("[A-Z]([a-zA-Z0-9_]*)");
const static QRegExp machine_instance = QRegExp("("+machine.pattern()+")");
const static QRegExp state_spec = QRegExp("("+halt_state+")");
const static QRegExp state = QRegExp("[a-z]([a-zA-Z0-9_]*)");
const static QRegExp character_spec = QRegExp("["+blank_character+begin_character+all_character+"]");
const static QRegExp character = QRegExp("[a-zA-Z0-9]");
const static QRegExp command = QRegExp("["+left_command+right_command+"]");

const static QRegExp option_line = QRegExp(QString("^(\\s*)(")
                                           +"("+name_option.pattern()+")(\\s+)(\\S+)(\\s*)|"
                                           +"("+init_option.pattern()+")(\\s+)(\\S+)(\\s*)|"
                                           +"("+halt_option.pattern()+")(\\s+)(\\S+)(\\s*)|"
                                           +"("+tape_option.pattern()+")(\\s+)(\\S+)(\\s*)|"
                                           +"("+include_option.pattern()+")(\\s+)(\\S+)(\\s+)(\\S+)(\\s*)|"
                                           +")$");
//Lines
const static QRegExp comment = QRegExp("^((//(.*))|(/\\*(.*)\\*/))$"/*|(#(.*))*/);
const static QRegExp white_line = QRegExp("^(\\s*)$");
const static QRegExp valid_line_state = QRegExp("^(\\s*)("+state.pattern()+"|"+machine_instance.pattern()+")"
                                                +"(\\s+)("+character.pattern()+"|"+character_spec.pattern()+")"
                                                +"(\\s+)("+state.pattern()+"|"+state_spec.pattern()+")"
                                                +"(\\s+)("+command.pattern()+"|"+character.pattern()+"|"+character_spec.pattern()+")(\\s*)$");
const static QRegExp valid_line_machine = QRegExp("^(\\s*)("+state.pattern()+"|"+machine_instance.pattern()+")"
                                                  +"(\\s+)("+character.pattern()+"|"+character_spec.pattern()+")"
                                                  +"(\\s+)("+machine_instance.pattern()+")"
                                                  +"(\\s+)("+character.pattern()+"|"+character_spec.pattern()+")(\\s*)$");


static inline TOKEN token_type_s(const QString s){
    if(state_spec.exactMatch(s)){
        return TOKEN_STATE_SPEC;
    }else if(state.exactMatch(s)){
        return TOKEN_STATE;
    }else if(machine_instance.exactMatch(s)){
        return TOKEN_MACHINE;
    }else if(character_spec.exactMatch(s)){
        return TOKEN_CHARACTER_SPEC;
    }else if(character.exactMatch(s)){
        return TOKEN_CHARACTER;
    }else if(command.exactMatch(s)){
        return TOKEN_COMMAND;
    }
    return TOKEN_ERROR;
}
static inline TOKEN token_type_c(const QString s){
    if(character_spec.exactMatch(s)){
        return TOKEN_CHARACTER_SPEC;
    }else if(character.exactMatch(s)){
        return TOKEN_CHARACTER;
    }else if(state_spec.exactMatch(s)){
        return TOKEN_STATE_SPEC;
    }else if(state.exactMatch(s)){
        return TOKEN_STATE;
    }else if(machine.exactMatch(s)){
        return TOKEN_MACHINE;
    }else if(command.exactMatch(s)){
        return TOKEN_COMMAND;
    }
    return TOKEN_ERROR;
}
static inline LINE line_type(const QString s){
    if(white_line.exactMatch(s)){
        return LINE_WHITE;
    }else if(option_line.exactMatch(s)){
        return LINE_OPTION;
    }else if(comment.exactMatch(s)){
        return LINE_COMMENT;
    }else if(valid_line_state.exactMatch(s)){
        return LINE_VALID_STATE;
    }else if(valid_line_machine.exactMatch(s)){
        return LINE_VALID_MACHINE;
    }
    return LINE_ERROR;
}

}

namespace io_format{
static QTextCharFormat blank;
static QTextCharFormat comment;
static QTextCharFormat option;
static QTextCharFormat syntax_error;
static QTextCharFormat semantic_error;
static QTextCharFormat state;
static QTextCharFormat state_spec;
static QTextCharFormat character;
static QTextCharFormat character_spec;
static QTextCharFormat command;
static QTextCharFormat machine;
static QTextCharFormat machine_spec;
}

namespace io_font{
static QFont input;// = QFont("consolas",12);
}

#endif // EXNAMESPACE_H
