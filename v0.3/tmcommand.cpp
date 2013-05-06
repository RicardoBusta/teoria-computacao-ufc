#include "tmcommand.h"


#include "exnamespace.h"

#include <QDebug>
#include <QStringList>


TMCommand::TMCommand()
    :type(TMCOM_ERROR),line_defined(-1)
{
}

void TMCommand::set(const QString state, const QString character, const TMCOM_TYPE type, const QString write_char, const QString goto_state, const int line_defined)
{
    this->state = state;
    this->character = character;
    this->type = type;
    this->write_char = write_char;
    this->goto_state = goto_state;
    this->line_defined = line_defined;
}



TMCommand TMCommand::operator =(TMCommand command)
{
    this->type = command.type;
    this->write_char = command.write_char;
    this->goto_state = command.goto_state;
    this->line_defined = command.line_defined;
    return *this;
}
