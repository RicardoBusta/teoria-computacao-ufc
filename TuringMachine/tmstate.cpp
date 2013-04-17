#include "tmstate.h"

#include "exnamespace.h"

QMap<QString,TMState> TMState::map;
QString TMState::first_state;

TMState::TMState(const QString name, const bool halt)
{
    this->name = name;
    this->halt = halt;
}

void TMState::clear()
{
    map.clear();
    map.insert(io_ex::halt_state,TMState(io_ex::halt_state,true));
    map.insert(io_ex::error_state,TMState(io_ex::error_state,true));
    first_state = io_ex::halt_state;
}

void TMState::add(const QString name)
{
    if(!map.contains(name)){
        map.insert(name,TMState(name));
    }
}
