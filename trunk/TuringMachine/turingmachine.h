#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <QObject>

#include "exnamespace.h"
#include "tmcommand.h"
#include "tmhistory.h"
#include "tminstance.h"

class TuringMachine : public QObject
{
    Q_OBJECT
public:
    explicit TuringMachine(QString program, QObject *parent = 0);

    QString name;
    QString program;
    QString default_tape;

    bool halted;
    bool current_state_is_machine;
    QString current_state;

    QString state_first;
    QList<QString> state_list;

    void state_add(const QString name);

    QMap<QString,TMInstance> related_machines;
    void machine_add(const QString name);

    QMap< QString,QMap<QString,TMCommand> > command_map;
    QList<QStringList> command_queue_list;
    QList<int> command_queue_line;

    void command_queue_add(const QStringList s_list, const int line_defined);
    int command_add();
    int command_line_redefined;

    QList<QString> character_list;
    void character_add(const QString name);

    QList<TMHistory> history;

    //Machine Execution
    static QMap<QString,TuringMachine*> machine_map;
    static QString machine_current_machine;
    static QString machine_tape;
    static int machine_head;
    static int machine_step_count;
    static int machine_step_max;

    static TuringMachine* get();

    void clear();

    TuringMachine operator=(TuringMachine tm);

signals:
    void current_state_signal(QString);
    void current_tape_signal(QString);
    void current_step_signal(QString);

    void current_description_signal(QString);

    void debug_message(QString);

    void rename_event();
public slots:
    void process(const QString text);
    void process(const QTextDocument *document);

    bool step();
    void back_step();
};

#endif // TURINGMACHINE_H
