#include "tminstance.h"

#include "turingmachine.h"

TMInstance::TMInstance(const QString alias, const TuringMachine *m)
{
    this->name = m->name;
    this->command_map = m->command_map;
    this->related_machines = m->related_machines;
}

TMInstance TMInstance::operator =(const TMInstance& m)
{
    this->name = m.name;
    this->command_map = m.command_map;
    this->related_machines = m.related_machines;
}
