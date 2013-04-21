#include "tmcharacter.h"

#include "exnamespace.h"

QMap<QString,TMCharacter> TMCharacter::map;

TMCharacter::TMCharacter(QString name)
{
    this->name = name;
}

void TMCharacter::clear()
{
    map.clear();
    //Must keep default characters on
    map.insert(io_ex::begin_character,TMCharacter(io_ex::begin_character));
    map.insert(io_ex::blank_character,TMCharacter(io_ex::blank_character));
}

void TMCharacter::add(const QString name)
{
    if(!map.contains(name)){
        map.insert(name,TMCharacter(name));
    }
}
