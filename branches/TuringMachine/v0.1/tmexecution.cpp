#include "tmexecution.h"

#include "exnamespace.h"

#include <QDebug>

TMExecution::TMExecution(QObject *parent):
    QObject(parent)
{
    max_step_number = 1000;
}

void TMExecution::begin(const QString tape)
{
    step_count=0;
    this->tape = io_ex::begin_character+io_ex::blank_character+tape+io_ex::blank_character;
    header_index = 1;
    state = TMState::first_state;
    qDebug() << "machine built.\n execute for: "+io_ex::begin_character+io_ex::blank_character+tape+io_ex::blank_character;
    emit current_tape(QString(this->tape).insert(header_index+1,"</b></font>").insert(header_index,"<font color='#f00'><b>"));
    emit current_state(state);
    //qDebug()  << QString(this->tape).insert(header_index,'_');

    //while(step()){}
}

bool TMExecution::step()
{
    TMCommand current_command = TMCommand::map[state][QString(tape[header_index])];
    //qDebug() << "executing:" << state << tape[header_index] << current_command.goto_state << current_command.write_char;

    if(step_count>=max_step_number){
        qDebug() << "ERROR: max number of steps reached";
        return false;
    }
    step_count++;
    if(state==io_ex::error_state){
        qDebug() << "ERROR: machine stopped";
        return false;
    }
    if(state==io_ex::halt_state){
        qDebug() << "HALT:" << QString(tape).insert(header_index,'_');
        return false;
    }

    //CURRENT STATE IS NOT HALTING, SO GO TO NEXT
    state = current_command.goto_state;

    //AND EXECUTE COMMAND (left, right, write or error(go to error state))
    switch(current_command.type){
    case TMCOM_LEFT:
        if(header_index>0){
            header_index--;
        }else{
            qDebug() << "ERROR: accessing data out of tape. aborting.";
            state = io_ex::error_state;
        }
        break;
    case TMCOM_RIGHT:
        header_index++;
        break;
    case TMCOM_WRITE:
        tape[header_index] = current_command.write_char[0];
        break;
    case TMCOM_ERROR:
        qDebug() << "ERROR: no operation defined for this command. aborting.";
        state = io_ex::error_state;
        break;
    }

    //qDebug()  << QString(tape).insert(header_index,'_');
    emit current_tape(QString(this->tape).insert(header_index+1,"</b></font>").insert(header_index,"<font color='#f00'><b>"));
    emit current_state(state);

    return true;
}
