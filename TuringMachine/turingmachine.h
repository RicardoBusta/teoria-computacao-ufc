#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <QObject>

#include "exnamespace.h"
#include "tmcommand.h"
#include "tmstate.h"
#include "tmcharacter.h"

class TuringMachine : public QObject
{
    Q_OBJECT
public:
    explicit TuringMachine(QObject *parent = 0);
    
    QString tape;
    int header_index;
    int step_count;
    int max_step_number;
    QString state;

    static QString state_first;
    static QMap<QString,TMState> state_map;
    static void state_clear();
    static void state_add(const QString name);

    static QList<QStringList> command_queue_list;
    static QList<int> command_queue_line;
    static void command_clear();
    static void command_queue_add(const QStringList s_list, const int line_defined);
    static int command_add();
    static int command_line_redefined;

    static QMap<QString,TMCharacter> character_map;
    static void character_clear();
    static void character_add(const QString name);
signals:
    
public slots:
    
};

#endif // TURINGMACHINE_H
