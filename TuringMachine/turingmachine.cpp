#include "turingmachine.h"

#include <QDebug>

QMap<QString,TuringMachine*> TuringMachine::machine_map;
QString TuringMachine::machine_current_machine;
QString TuringMachine::machine_current_state;
QString TuringMachine::machine_tape;
int TuringMachine::machine_head;
int TuringMachine::machine_step_count;
int TuringMachine::machine_step_max;

TuringMachine::TuringMachine(QString program, QObject *parent) :
    QObject(parent)
{
    this->name = "New";
    this->program = program;
    this->default_tape = "";
    if(machine_map.contains(name)){
        int count=0;
        while(machine_map.contains(QString("%1_%2").arg(name).arg(count))){
            count++;
        }
        name = QString("%1_%2").arg(name).arg(count);
    }
    emit debug_message( "adding " + name);
    process(program);
    begin(default_tape);
    machine_step_max = 1000;
    machine_map.insert(name,this);
}

//State
void TuringMachine::clear()
{
    //Clear State
    state_list.clear();
    state_list.push_back(io_ex::halt_state);
    state_list.push_back(io_ex::error_state);
    state_first = io_ex::halt_state;

    //Clear Character
    character_list.clear();
    //Must keep default characters on
    character_list.push_back(io_ex::begin_character);
    character_list.push_back(io_ex::blank_character);

    //Clear Command
    command_map.clear();
    //must keep the map with all transitions from all states, even if null(default).
    foreach(QString state, state_list){
        command_map.insert(state,QMap<QString,TMCommand>());
        foreach(QString character,character_list){
            command_map[state].insert(character,TMCommand());
            if(character==io_ex::begin_character){
                command_map[state][character].set(TMCOM_RIGHT,character,state,-2);
            }else{
                command_map[state][character].set(TMCOM_ERROR,character,state,-2);
            }
        }
    }

    //History
    history.clear();
}

void TuringMachine::state_add(const QString name)
{
    if(!state_list.contains(name)){
        state_list.push_back(name);
    }
}

//Command
void TuringMachine::command_queue_add(const QStringList s_list, const int line_defined)
{
    command_queue_list.push_back(s_list);
    command_queue_line.push_back(line_defined);
}

int TuringMachine::command_add()
{
    while(command_queue_list.size()>0){
        TMCOM_TYPE t;
        if(command_queue_list.first()[3]==io_ex::right_command){
            t = TMCOM_RIGHT;
        }else if(command_queue_list.first()[3]==io_ex::left_command){
            t = TMCOM_LEFT;
        }else{
            t = TMCOM_WRITE;
        }
        if(command_map[command_queue_list.first()[0]][command_queue_list.first()[1]].line_defined==-1){
            command_map[command_queue_list.first()[0]][command_queue_list.first()[1]].set(t,command_queue_list.first()[3],command_queue_list.first()[2],command_queue_line.first());
        }else{
            int res = command_map[command_queue_list.first()[0]][command_queue_list.first()[1]].line_defined;
            command_line_redefined = command_queue_line.first();
            command_queue_list.clear();
            command_queue_line.clear();
            return res;
        }
        command_queue_list.pop_front();
        command_queue_line.pop_front();
    }
    return -1;
}

//Character
void TuringMachine::character_add(const QString name)
{
    if(!character_list.contains(name)){
        character_list.push_back(name);
    }
}

TuringMachine *TuringMachine::get()
{
    return machine_map[machine_current_machine];
}

void TuringMachine::begin(const QString tape)
{
    history.clear();
    machine_step_count=0;
    if(tape!=""){
        this->machine_tape = io_ex::begin_character+tape;
    }else{
        this->machine_tape = io_ex::begin_character+"#";
    }
    machine_head = 1;
    machine_current_state = state_first;

    emit debug_message( "machine built.\n execute for: "+io_ex::begin_character+tape );

    emit current_tape(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
    emit current_state(machine_current_state);
    emit current_step(QString::number(machine_step_count));

}

bool TuringMachine::step()
{

    TMCommand current_command = command_map[machine_current_state][QString(machine_tape[machine_head])];

    if(machine_step_count>=machine_step_max){
        emit debug_message( "ERROR: max number of steps reached" );
        return false;
    }

    history.push_back(TMHistory(machine_current_state,machine_head,machine_tape));

    machine_step_count++;
    if(machine_current_state==io_ex::error_state){
        emit debug_message( "ERROR: machine stopped" );
        return false;
    }
    if(machine_current_state==io_ex::halt_state){
        emit debug_message( "HALT: " + QString(machine_tape).insert(machine_head,'_') );
        return false;
    }

    //CURRENT STATE IS NOT HALTING, SO GO TO NEXT
    machine_current_state = current_command.goto_state;

    //AND EXECUTE COMMAND (left, right, write or error(go to error state))
    switch(current_command.type){
    case TMCOM_LEFT:
        if(machine_head>0){
            machine_head--;
        }else{
            emit debug_message( "ERROR: accessing data out of tape. aborting." );
            machine_current_state = io_ex::error_state;
        }
        break;
    case TMCOM_RIGHT:
        if(machine_head==machine_tape.size()-1){
            machine_tape.append(io_ex::blank_character);
        }
        machine_head++;
        break;
    case TMCOM_WRITE:
        machine_tape[machine_head] = current_command.write_char[0];
        break;
    case TMCOM_ERROR:
        emit debug_message( "ERROR: no operation defined for this command. aborting." );
        machine_current_state = io_ex::error_state;
        break;
    }

    emit current_tape(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
    emit current_state(machine_current_state);
    emit current_step(QString::number(machine_step_count));

    return true;
}

void TuringMachine::back_step()
{
    if(!history.empty() && machine_step_count>0){
        machine_current_state = history.last().state;
        machine_tape = history.last().tape;
        machine_head = history.last().head;
        machine_step_count--;

        emit current_tape(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
        emit current_state(machine_current_state);
        emit current_step(QString::number(machine_step_count));

        history.pop_back();
    }
}

#define launch_error(e,s) valid=false;errorline=(e);errorstring=(s)

void TuringMachine::process(const QString input)
{
    this->program = input;
    clear();
    bool valid = true;
    bool first_defined = false;
    int errorline = -1;
    QString errorstring = "";
    QStringList arg;

    QStringList text = input.split('\n');
    for(int i=0;i<text.size();i++){
        QString line = text.at(i);

        switch(io_ex::line_type(line)){

        case io_ex::LINE_OPTION:
            arg = line.split(QRegExp("(\\s+)"));
            if(io_ex::name_option.exactMatch(arg[0])){
                if(TuringMachine::machine_map.contains(arg[1]) && TuringMachine::machine_map[arg[1]]!=this){
                    launch_error(i,"this machine already exists");
                    break;
                }
                if(arg[1]!=name){
                    machine_map.remove(this->name);
                    this->name = arg[1];
                    machine_map.insert(this->name,this);
                    machine_current_machine = this->name;
                    emit rename_event();
                }
            }else if(io_ex::init_option.exactMatch(arg[0])){
                if(first_defined){
                    launch_error(i,"the first state was already defined");
                }
                first_defined=true;
                state_first = arg[1];
            }else if(io_ex::halt_option.exactMatch(arg[0])){

            }else{
                launch_error(i,"unexistent option");
            }
            break;

        case io_ex::LINE_VALID:
            arg = line.split(QRegExp("(\\s+)"));

            if(!first_defined){
                first_defined = true;
                state_first = arg[0];
            }
            state_add(arg[0]);
            character_add(arg[1]);
            state_add(arg[2]);
            if(arg[3]!=io_ex::left_command && arg[3]!=io_ex::right_command){
                character_add(arg[3]);
            }
            command_queue_add(arg,i);
            break;

        case io_ex::LINE_COMMENT:
        case io_ex::LINE_WHITE:
            // Do nothing for comments and white lines
            break;

        case io_ex::LINE_ERROR:
        default:
            launch_error(i,"bad syntax");
            break;
        }

        if(!valid) break;
    }

    int def_line=-1;
    if((def_line = command_add())!=-1){
        valid = false;
        errorline = command_line_redefined;
        if(def_line!=-2){
            errorstring = QString("previously defined at line %1").arg(def_line);
        }else{
            errorstring = QString("string predefined by machine");
        }
    }


    //Start Debug Output

    QString outstr = "";

    if (valid) {
        outstr += "<font color='#0a0'><b>[valid]</b></font><br>";
    }else{
        outstr += QString("<font color='#a00'><b>[invalid:line %1] %2</b></font><br>").arg(errorline).arg(errorstring);
    }


    outstr += "<br>M(Machine) = { Q, &Gamma;, b, &Sigma;, &delta;, s, F }<br>";

    outstr += "Q(Machine States) = { ";
    foreach(QString state,state_list){
        outstr += (state+", ");
    }
    outstr += "}<br>";

    outstr += "&Gamma;(Machine Alphabet) = { ";
    foreach(QString character,character_list){
        outstr += character+", ";
    }
    outstr += "}<br>";

    outstr += "b(Blank Character) = { "+io_ex::blank_character+", }<br>";

    outstr += "&Sigma;(Input Alphabet) = { ";
    foreach(QString character,character_list){
        if(character!=io_ex::blank_character && character!=io_ex::begin_character){
            outstr += character+", ";
        }
    }
    outstr += "}<br>";

    outstr += "&delta;(Transition Function) = <i>see the table</i><br>";

    outstr += QString("s(Initial State) = { %1, }<br>").arg(state_first);

    outstr += QString("F(Final States) = { "+io_ex::halt_state+", }<br>");

    outstr += "Available Machines = { ";
    foreach(TuringMachine *m, machine_map){
        if(m!=NULL){
            outstr += m->name+", ";
        }
    }
    outstr +="}";


    emit current_description(outstr);
}

#undef launch_error
