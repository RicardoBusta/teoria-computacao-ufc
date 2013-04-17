#include "tmcommand.h"

#include "tmstate.h"
#include "tmcharacter.h"
#include "exnamespace.h"

#include <QDebug>
#include <QStringList>

QMap< QString,QMap<QString,TMCommand> > TMCommand::map;
QList<QStringList> TMCommand::queue_list;
QList<int> TMCommand::queue_line;
int TMCommand::line_redefined = -1;

TMCommand::TMCommand()
{
    this->type = TMCOM_ERROR;
    line_defined = -1;
}

void TMCommand::set(const TMCOM_TYPE type, const QString write_char, const QString goto_state, const int line_defined)
{
    this->type = type;
    this->write_char = write_char;
    this->goto_state = goto_state;
    this->line_defined = line_defined;
}

void TMCommand::clear()
{
    map.clear();
    //must keep the map with all transitions from all states, even if null(default).
    foreach(TMState s,TMState::map){
        map.insert(s.name,QMap<QString,TMCommand>());
        foreach(TMCharacter c,TMCharacter::map){
            map[s.name].insert(c.name,TMCommand());
            if(c.name==io_ex::begin_character){
                map[s.name][c.name].set(TMCOM_RIGHT,c.name,s.name,-2);
            }else{
                map[s.name][c.name].set(TMCOM_ERROR,c.name,s.name,-2);
            }
        }
    }
}

void TMCommand::queue_add(const QStringList s_list, const int line_defined)
{
    queue_list.push_back(s_list);
    queue_line.push_back(line_defined);
}

int TMCommand::add()
{
    while(queue_list.size()>0){
        TMCOM_TYPE t;
        if(queue_list.first()[3]==io_ex::right_command){
            t = TMCOM_RIGHT;
        }else if(queue_list.first()[3]==io_ex::left_command){
            t = TMCOM_LEFT;
        }else{
            t = TMCOM_WRITE;
        }
        if(map[queue_list.first()[0]][queue_list.first()[1]].line_defined==-2){
            map[queue_list.first()[0]][queue_list.first()[1]].set(t,queue_list.first()[3],queue_list.first()[2],queue_line.first());
        }else{
            int res = map[queue_list.first()[0]][queue_list.first()[1]].line_defined;
            line_redefined = queue_line.first();
            queue_list.clear();
            queue_line.clear();
            return res;
        }
        queue_list.pop_front();
        queue_line.pop_front();
    }
    return -1;
}

TMCommand TMCommand::operator =(TMCommand command)
{
    this->type = command.type;
    this->write_char = command.write_char;
    this->goto_state = command.goto_state;
    this->line_defined = command.line_defined;
    return *this;
}
