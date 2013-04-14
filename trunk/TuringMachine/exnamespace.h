#ifndef EXNAMESPACE_H
#define EXNAMESPACE_H

#include <QRegExp>
#include <QFont>
#include <QTextCharFormat>

namespace io_ex{
static QRegExp state_spec = QRegExp("(h|e)");
static QRegExp state = QRegExp("[a-zA-Z][a-zA-Z0-9_-]*");
static QRegExp character_spec = QRegExp("[#@]");
static QRegExp character = QRegExp("[a-zA-Z0-9#@]");
static QRegExp command_spec = QRegExp("[><]");
static QRegExp command = QRegExp("[a-zA-Z0-9#><]");
static QRegExp comment = QRegExp("^((//(.*))|(/\\*(.*)\\*/)|(#(.*)))$");
static QRegExp blank = QRegExp("^(\\s*)$");
static QRegExp line = QRegExp("^(\\s*)"+io_ex::state.pattern()
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
