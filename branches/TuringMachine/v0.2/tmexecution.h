#ifndef TMEXECUTION_H
#define TMEXECUTION_H

#include <QObject>
#include <QString>

#include "exnamespace.h"
#include "tmcommand.h"
#include "tmstate.h"
#include "tmcharacter.h"

class TMExecution: public QObject
{
    Q_OBJECT
public:
    QString tape;
    int header_index;
    int step_count;
    int max_step_number;
    QString state;

    TMExecution(QObject *parent=0);
signals:
    void current_state(QString);
    void current_tape(QString);
public slots:
    void begin(const QString tape);
    bool step();
};

#endif // TMEXECUTION_H
