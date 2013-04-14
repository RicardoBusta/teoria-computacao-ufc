#include "tmstate.h"

QMap<QString,TMState> TMState::map;

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
}

void TMState::add(QString name)
{
    if(!map.contains(name)){
        map.insert(name,TMState(name));
    }
}
