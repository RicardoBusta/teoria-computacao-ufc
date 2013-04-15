#ifndef EXNAMESPACE_H
#define EXNAMESPACE_H

#include <QRegExp>
#include <QFont>
#include <QTextCharFormat>

namespace io_ex{
const static QString halt_state = QString("HALT");
const static QString error_state = QString("ERROR");
const static QString blank_character = QString("#");
const static QString begin_character = QString("@");
const static QString left_command = QString("<");
const static QString right_command = QString(">");

const static QRegExp state_spec = QRegExp("("+halt_state+"|"+error_state+")");
const static QRegExp state = QRegExp("[a-zA-Z][a-zA-Z0-9_-]*");
const static QRegExp character_spec = QRegExp("["+blank_character+begin_character+"]");
const static QRegExp character = QRegExp("[a-zA-Z0-9"+blank_character+begin_character+"]");
const static QRegExp command_spec = QRegExp("["+left_command+right_command+"]");
const static QRegExp command = QRegExp("[a-zA-Z0-9"+blank_character+left_command+right_command+"]");
const static QRegExp comment = QRegExp("^((//(.*))|(/\\*(.*)\\*/)|(#(.*)))$");
const static QRegExp white_line = QRegExp("^(\\s*)$");
const static QRegExp valid_line = QRegExp("^(\\s*)"+io_ex::state.pattern()
                              +"(\\s+)"+io_ex::character.pattern()
                              +"(\\s+)"+io_ex::state.pattern()
                              +"(\\s+)"+io_ex::command.pattern()+"(\\s*)$");
}

namespace io_format{
static QTextCharFormat blank;
static QTextCharFormat comment;
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
