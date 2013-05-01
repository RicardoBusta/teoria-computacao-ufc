#ifndef TMINSTANCE_H
#define TMINSTANCE_H

#include <QString>
#include <QMap>

#include "tmcommand.h"

class TuringMachine;

class TMInstance
{
public:
    TMInstance(const QString alias, const TuringMachine *m);

    TMInstance operator=( const TMInstance& m);

    QString name;
    QMap< QString,QMap<QString,TMCommand> > command_map;
    QMap<QString,TMInstance> related_machines;
};

#endif // TMINSTANCE_H
