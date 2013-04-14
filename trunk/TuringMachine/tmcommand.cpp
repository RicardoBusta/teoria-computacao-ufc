#include "tmcommand.h"

#include "tmstate.h"
#include "tmcharacter.h"

#include <QDebug>

QMap< QString,QMap<QString,TMCommand> > TMCommand::map;

TMCommand::TMCommand()
{
    this->type = TMCOM_ERROR;
}

void TMCommand::set(TMCOM_TYPE type, QString write_char, QString goto_state)
{
    this->type = type;
    this->write_char = write_char;
    this->goto_state = goto_state;
}

void TMCommand::clear()
{
    map.clear();
    //must keep the map with all transitions from all states, even if null(default).
    foreach(TMState s,TMState::map){
        map.insert(s.name,QMap<QString,TMCommand>());
        foreach(TMCharacter c,TMCharacter::map){
            map[s.name].insert(c.name,TMCommand());
        }
    }
}

bool TMCommand::add(QString from_state, QString from_character, QString goto_state, QString goto_command)
{
    TMCOM_TYPE t;
    if(goto_command==">"){
        t = TMCOM_RIGHT;
    }else if(goto_command=="<"){
        t = TMCOM_LEFT;
    }else{
        t = TMCOM_WRITE;
    }
    map[from_state][from_character].set(t,goto_command,goto_state);
}
