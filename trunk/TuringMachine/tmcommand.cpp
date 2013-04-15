#include "tmcommand.h"

#include "tmstate.h"
#include "tmcharacter.h"

#include <QDebug>
#include <QStringList>

QMap< QString,QMap<QString,TMCommand> > TMCommand::map;
QList<QStringList> TMCommand::queue_list;
QList<int> TMCommand::queue_line;
int TMCommand::line_redefined = -1;

TMCommand::TMCommand()
{
    this->type = TMCOM_NULL;
    line_defined = -1;
}

void TMCommand::set(TMCOM_TYPE type, QString write_char, QString goto_state, int line_defined)
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
            if(c.name=="@"){
                map[s.name][c.name].set(TMCOM_RIGHT,c.name,s.name,-2);
            }else{
                map[s.name][c.name].set(TMCOM_NULL,c.name,s.name,-2);
            }
        }
    }
}

void TMCommand::queue_add(QStringList s_list, int line_defined)
{
    queue_list.push_back(s_list);
    queue_line.push_back(line_defined);
}

int TMCommand::add()
{
    while(queue_list.size()>0){
        TMCOM_TYPE t;
        if(queue_list.first()[3]==">"){
            t = TMCOM_RIGHT;
        }else if(queue_list.first()[3]=="<"){
            t = TMCOM_LEFT;
        }else{
            t = TMCOM_WRITE;
        }
//        qDebug() << queue_list.first()[0] << queue_list.first()[1]<< map[queue_list.first()[0]][queue_list.first()[1]].type;
//        qDebug() << queue_list.first()[0] << queue_list.first()[1]<< map[queue_list.first()[0]][queue_list.first()[1]].line_defined;
        if(map[queue_list.first()[0]][queue_list.first()[1]].type==TMCOM_NULL){
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
