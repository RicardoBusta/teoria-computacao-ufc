#include "tmcharacter.h"


QMap<QString,TMCharacter> TMCharacter::map;

TMCharacter::TMCharacter(QString name)
{
    this->name = name;
}

void TMCharacter::clear()
{
    map.clear();
    //Must keep default characters on
    map.insert("@",TMCharacter("@"));
    map.insert("#",TMCharacter("#"));
}

void TMCharacter::add(QString name)
{
    if(!map.contains(name)){
        map.insert(name,TMCharacter(name));
    }
}
