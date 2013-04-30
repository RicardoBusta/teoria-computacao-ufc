#ifndef TMINSTANCE_H
#define TMINSTANCE_H

#include <QString>

class TuringMachine;

class TMInstance
{
public:
    TMInstance(const QString alias, const TuringMachine *m);
};

#endif // TMINSTANCE_H
