#include "tmstate.h"

QMap<QString,TMState> TMState::map;
QString TMState::first_state;

TMState::TMState(QString name, bool halt)
{
    this->name = name;
    this->halt = halt;
}

void TMState::clear()
{
    map.clear();
    map.insert("h",TMState("h",true));
    map.insert("e",TMState("e",true));
    first_state = "h";
}

void TMState::add(QString name)
{
    if(!map.contains(name)){
        map.insert(name,TMState(name));
    }
}
