#include "turingmachine.h"

#include <QDebug>
#include <QTextDocument>
#include <QTextBlock>


QMap<QString,TuringMachine*> TuringMachine::machine_map;
QString TuringMachine::machine_current_machine;
QString TuringMachine::machine_tape;
int TuringMachine::machine_head;
int TuringMachine::machine_step_max;

TuringMachine::TuringMachine(QString program, QObject *parent) :
    QObject(parent)
{
    root_machine = true;
    this->name = "New";
    this->alias = "Root_Machine";
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
    machine_step_max = 1000;
    machine_map.insert(name,this);
}

TuringMachine::TuringMachine(QString alias, TuringMachine *tm)
{
    root_machine = false;
    this->name = "New";
    this->alias = alias;
    this->default_tape = "";

    this->process(tm->gen_resulting_code());
    machine_step_max = 1000;

//    this->setParent(tm);
//    this->name = tm->name;
//    this->alias = alias;
//    this->program = tm->program;
//    this->state_first = tm->state_first;
//    //qDebug() << state_first << tm->state_first;
//    this->character_list = tm->character_list;
//    this->state_list = tm->state_list;
//    this->command_map = tm->command_map;
//    foreach(TuringMachine *rm,tm->related_machines){
//        this->related_machines.insert(rm->alias,new TuringMachine(rm->alias,this));
//    }
//    //process(program);
}

//State
void TuringMachine::clear()
{
    //qDebug() << alias << "cleared";
    machine_step_count = 0;
    halted = false;
    current_state_is_machine = false;
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
                command_map[state][character].set(state,character,TMCOM_RIGHT,character,state,-2);
            }else{
                command_map[state][character].set(state,character,TMCOM_ERROR,character,state,-2);
            }
        }
    }

    //clear machine copies
    foreach(TuringMachine *tm,related_machines){
        disconnect(tm,0,0,0);
        delete tm;
    }
    related_machines.clear();

    //History
    //history.clear();
}

void TuringMachine::reset_execution()
{
    machine_step_count = 0;
    //qDebug() << name << "execution reset";
    halted = false;
    current_state = state_first;
    if(io_ex::machine.exactMatch(state_first)){
        current_state_is_machine = true;
    }else{
        current_state_is_machine = false;
    }
}

void TuringMachine::state_add(const QString name)
{
    if(!state_list.contains(name)){
        state_list.push_back(name);
    }

}

void TuringMachine::machine_add(const QString alias, const QString name)
{
    if(!related_machines.contains(alias)){
        TuringMachine *child = new TuringMachine(alias,machine_map[name]);
        //connect(child,SIGNAL(current_tape_signal(QString)),this,SIGNAL(current_tape_signal(QString)));
        related_machines.insert(alias, child);
    }
}

//Command
void TuringMachine::command_queue_add(const QStringList s_list, const int line_defined)
{
    //qDebug() << "add command:" << s_list[0] << s_list[1] << s_list[2] << s_list[3];
    command_queue_list.push_back(s_list);
    command_queue_line.push_back(line_defined);
}

int TuringMachine::command_add()
{
    while(command_queue_list.size()>0){
        QStringList command = command_queue_list.takeFirst();

        int line = command_queue_line.takeFirst();
        TMCOM_TYPE t;
        if(io_ex::machine_instance.exactMatch(command[2])){
            t = TMCOM_EXEC;
        }else if(command[3]==io_ex::right_command){
            t = TMCOM_RIGHT;
        }else if(command[3]==io_ex::left_command){
            t = TMCOM_LEFT;
        }else{
            t = TMCOM_WRITE;
        }

        //State to command
        if(command[1]==io_ex::all_character){
            foreach(QString c,character_list){
                if(command_map[command[0]][c].line_defined==-1){
                    if(command[3]!=io_ex::all_character){
                        command_map[command[0]][c].set(command[0],c,t,command[3],command[2],-1);
                    }else{
                        command_map[command[0]][c].set(command[0],c,t,c,command[2],-1);
                    }
                }else{
                    //Leave as it is
                }
            }
        }else{
            if(command_map[command[0]][command[1]].line_defined==-1){
                if(command[3]!=io_ex::all_character){
                    command_map[command[0]][command[1]].set(command[0],command[1],t,command[3],command[2],line);
                }else{
                    command_map[command[0]][command[1]].set(command[0],command[1],t,command[1],command[2],line);
                }
                //qDebug() << command_map[command[0]][command[1]].goto_state << command_map[command[0]][command[1]].write_char;
            }else{
                int res = command_map[command[0]][command[1]].line_defined;
                command_line_redefined = line;
                command_queue_list.clear();
                command_queue_line.clear();
                return res;
            }

        }
    }
    return -1;
}

//Character
void TuringMachine::character_add(const QString name)
{
    if(name!=io_ex::all_character && !character_list.contains(name)){
        character_list.push_back(name);
    }
}

TuringMachine *TuringMachine::get()
{
    return machine_map[machine_current_machine];
}

bool TuringMachine::step()
{
    //qDebug() << alias << "step" << machine_step_count;
    TMCommand current_command = command_map[current_state][QString(machine_tape[machine_head])];

    if(machine_step_count>=machine_step_max){
        emit debug_message( "ERROR: max number of steps reached" );
        return false;
    }

    machine_step_count++;
    if(current_state==io_ex::error_state){
        emit debug_message( "ERROR: machine stopped" );
        return false;
    }

    if(halted){
        emit debug_message( "HALT: " + QString(machine_tape).insert(machine_head,'_') );
        return false;
    }

    //Current command is a running machine
    if( current_state_is_machine ){
        //qDebug() << "executing internal machine" << current_state;
        //if( TuringMachine::machine_map[current_state] && !TuringMachine::machine_map[current_state]->halted ){
        if( related_machines.contains(current_state) && !related_machines[current_state]->halted ){
            bool res = related_machines[current_state]->step();
            emit current_state_signal(current_state+": "+related_machines[current_state]->current_state);
            emit current_tape_signal(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
            emit current_step_signal(QString::number(machine_step_count));
            return res;
        }else{
            current_state_is_machine = false;
        }
    }

    //qDebug() << alias << current_state <<" executing: " << current_command.goto_state << current_command.write_char << current_state_is_machine;
    //qDebug() << alias << "first state: " << state_first;

    //CURRENT STATE IS NOT HALTING, SO GO TO NEXT
    //history.push_back(TMHistory(current_state,machine_head,machine_tape));

    current_state = current_command.goto_state;

    if( current_state == io_ex::halt_state ){
        halted = true;
    }

    //AND EXECUTE COMMAND (left, right, write or error(go to error state))
    switch(current_command.type){
    case TMCOM_EXEC:
        //qDebug() << "transforming into machine execution";
        current_state_is_machine = true;
        related_machines[current_state]->reset_execution();
        emit current_state_signal(current_state+": "+related_machines[current_state]->current_state);
        emit current_tape_signal(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
        emit current_step_signal(QString::number(machine_step_count));
        return true;

    case TMCOM_LEFT:
        if(machine_head>0){
            machine_head--;
        }else{
            emit debug_message( "ERROR: accessing data out of tape. aborting." );
            current_state = io_ex::error_state;
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
    default:
        emit debug_message( "ERROR: no operation defined for this command. aborting." );
        current_state = io_ex::error_state;
        break;
    }

    emit current_tape_signal(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
    emit current_state_signal(current_state);
    emit current_step_signal(QString::number(machine_step_count));

    return true;
}

void TuringMachine::back_step()
{
    //    if(!history.empty() && machine_step_count>0){
    //        current_state = history.last().state;
    //        machine_tape = history.last().tape;
    //        machine_head = history.last().head;
    //        machine_step_count--;

    //        emit current_tape_signal(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
    //        emit current_state_signal(current_state);
    //        emit current_step_signal(QString::number(machine_step_count));

    //        history.pop_back();
    //    }
}

void TuringMachine::process(const QString text)
{
    QTextDocument *doc = new QTextDocument(text);
    process(doc);
    delete doc;
}

#define launch_error(e,s) valid=false;errorline=(e);errorstring=(s);

void TuringMachine::process(const QTextDocument *document)
{
    program = document->toPlainText();
    clear();
    bool name_defined = false;
    bool valid = true;
    bool tape_defined = false;
    bool initial_state_defined = false;
    int errorline = -1;
    QString errorstring = "";
    QStringList arg;

    QTextBlock block = document->firstBlock();
    while(block!=document->lastBlock().next()){
        QString line = block.text();

        switch(io_ex::line_type(line)){

        //Options like #name, #tape, #init, #halt etc. usually arg[0] is the #command and arg[1] is the value.
        case io_ex::LINE_OPTION:
            arg = line.split(QRegExp("(\\s+)"),QString::SkipEmptyParts);
            // #name - defines machine name. only one allowed per program.
            if(io_ex::name_option.exactMatch(arg[0])){
                if(!name_defined || !root_machine){
                    name_defined = true;
                    if(TuringMachine::machine_map.contains(arg[1]) && TuringMachine::machine_map[arg[1]]!=this){
                        qDebug() << "this machine already exists";
                        launch_error(block.blockNumber(),"this machine already exists");
                        break;
                    }
                    if(arg[1]!=name){
                        machine_map.remove(this->name);
                        this->name = arg[1];
                        machine_map.insert(this->name,this);
                        machine_current_machine = this->name;
                        emit rename_event();
                    }
                }else{
                    launch_error(block.blockNumber(),"name already defined");
                }
                // #init - defines the machine initial state. only one allowed per program.
            }else if(io_ex::init_option.exactMatch(arg[0])){
                if(initial_state_defined){
                    launch_error(block.blockNumber(),"the first state was already defined");
                }
                initial_state_defined=true;
                state_first = arg[1];
                current_state = arg[1];
                switch(io_ex::token_type_s(current_state)){
                case io_ex::TOKEN_MACHINE:
                    current_state_is_machine = true;
                    break;
                case io_ex::TOKEN_STATE:
                case io_ex::TOKEN_STATE_SPEC:
                    current_state_is_machine = false;
                    break;
                default:
                    break;
                }
                // #halt defines one of the possible halting states
            }else if(io_ex::halt_option.exactMatch(arg[0])){

                // #tape defines the machine tape. only one allowed per program.
            }else if(io_ex::tape_option.exactMatch(arg[0])){
                if(!tape_defined){
                    default_tape = arg[1];
                    tape_defined = true;
                    for(int i=0;i<default_tape.size();i++){
                        character_add(default_tape.mid(i,1));
                    }
                }else{
                    launch_error(block.blockNumber(),"tape defined twice");
                }
                // invalid option.
            }else if(io_ex::include_option.exactMatch(arg[0])){
                if(machine_map.contains(arg[2])){
                    if(this->name != arg[2]){
                        if(!related_machines.contains(arg[1])){
                            machine_add(arg[1],arg[2]);
                        }else{
                            launch_error(block.blockNumber(),"alias already defined");
                        }
                    }else{
                        launch_error(block.blockNumber(),"recursive inclusion");
                    }
                }else{
                    launch_error(block.blockNumber(),"unexistant machine");
                }
            }else{
                launch_error(block.blockNumber(),"unexistent option");
            }
            break;
            // options that define a machine operation (transition). they are either  "state/machine condition state operatior" or "state/machine condition machine"
            // machine as state means any of the machine halting states. machine as operation means the machine to be executed.
            // arg[0] is machine/state, arg[1] is a character, arg[2] is a machine or a state, and arg[3] will only exist as a command for an arg[2] state.
        case io_ex::LINE_VALID_MACHINE:
        case io_ex::LINE_VALID_STATE:
            arg = line.split(QRegExp("(\\s+)"),QString::SkipEmptyParts);
            //if there isn't a first state/machine defined, define it now. first state/machine ever read. if a machine is the initial state, it will be executed first.
            if(!initial_state_defined){
                initial_state_defined = true;
                state_first = arg[0];
                current_state = arg[0];
                switch(io_ex::token_type_s(current_state)){
                case io_ex::TOKEN_MACHINE:
                    current_state_is_machine = true;
                    break;
                case io_ex::TOKEN_STATE:
                case io_ex::TOKEN_STATE_SPEC:
                    current_state_is_machine = false;
                    break;
                default:
                    break;
                }
            }
            // add args in the corresponding lists.
            if(io_ex::state.exactMatch(arg[0]) || io_ex::state_spec.exactMatch(arg[0])){
                state_add(arg[0]);
            }else{
                if(!related_machines.contains(arg[0])){
                    launch_error(block.blockNumber(),"unexistent machine instance (first argument)");
                    break;
                }
            }

            character_add(arg[1]);

            if(io_ex::state.exactMatch(arg[2]) || io_ex::state_spec.exactMatch(arg[2])){
                state_add(arg[2]);
            }else{
                if(!related_machines.contains(arg[2])){
                    launch_error(block.blockNumber(),"unexistent machine instance (second argument)");
                    break;
                }
            }
            if(arg.size()>3){
                if(arg[3]!=io_ex::left_command && arg[3]!=io_ex::right_command){
                    character_add(arg[3]);
                }
                command_queue_add(arg,block.blockNumber());
            }
            break;
        case io_ex::LINE_COMMENT:
        case io_ex::LINE_WHITE:
            // Do nothing for comments and white lines
            break;

        case io_ex::LINE_ERROR:
        default:
            //            block.setUserState(2);
            launch_error(block.blockNumber(),"bad syntax");
            break;
        }

        if(!valid) break;

        block = block.next();
    }
    if(!tape_defined){
        default_tape = "";
    }

    int def_line=-1;
    if((def_line = command_add())!=-1){
        if(def_line!=-2){
            launch_error(command_line_redefined, QString("previously defined at line %1").arg(def_line));
        }else{
            launch_error(command_line_redefined, QString("string predefined by machine"));
        }
    }


    //Start Debug Output

    QString outstr = "";

    if (valid) {
        outstr += "<font color='#0a0'><b>[valid]</b></font><br>";
    }else{
        outstr += QString("<font color='#a00'><b>[invalid:line %1] %2</b></font><br>").arg(errorline).arg(errorstring);
    }

    outstr += "<hr>";

    outstr += "<br><b>M</b><sub>(Formal Machine)</sub> = { Q, &Gamma;, b, &Sigma;, &delta;, s, F }<br>";

    outstr += "<b>Q</b><sub>(Machine States)</sub> = { ";
    foreach(QString state,state_list){
        outstr += (state+", ");
    }
    outstr += "}<br>";

    outstr += "<b>&Gamma;</b><sub>(Machine Alphabet)</sub> = { ";
    foreach(QString character,character_list){
        outstr += character+", ";
    }
    outstr += "}<br>";

    outstr += "<b>b</b><sub>(Blank Character)</sub> = { "+io_ex::blank_character+", }<br>";

    outstr += "<b>&Sigma;</b><sub>(Input Alphabet)</sub> = { ";
    foreach(QString character,character_list){
        if(character!=io_ex::blank_character && character!=io_ex::begin_character){
            outstr += character+", ";
        }
    }
    outstr += "}<br>";

    outstr += "<b>&delta;</b><sub>(Transition Function)</sub> = <i>see the table</i><br>";

    outstr += QString("<b>s</b><sub>(Initial State)</sub> = { %1, }<br>").arg(state_first);

    outstr += QString("<b>F</b><sub>(Final States)</sub> = { "+io_ex::halt_state+", }<br>");

    outstr += "Available Machines = { ";
    foreach(TuringMachine *m, machine_map){
        if(m!=NULL){
            outstr += m->name+", ";
        }
    }
    outstr +="}<br>";

    outstr += "Related Machines = {";
    foreach(TuringMachine *tm, related_machines){
        tm->reset_execution();
        outstr += tm->alias+"("+tm->name+"):"+tm->state_first+", ";
    }
    outstr +="}";

    //history.clear();
    machine_step_count=0;
    if(default_tape!=""){
        this->machine_tape = io_ex::begin_character+default_tape;
    }else{
        this->machine_tape = io_ex::begin_character+"#";
    }
    machine_head = 1;
    current_state = state_first;

    emit current_tape_signal(QString(this->machine_tape).insert(machine_head+1,"</b></font>").insert(machine_head,"<font color='#f00'><b>"));
    emit current_state_signal(current_state);
    emit current_step_signal(QString::number(machine_step_count));
    emit current_description_signal(outstr);

    qDebug() << "finished processing" << alias << name << state_first;
    qDebug() << program;
    qDebug() << "=======================" << gen_resulting_code();
}

QString TuringMachine::gen_machine_code()
{
    QString code;
    QTextDocument *document = new QTextDocument(program);
    QTextBlock block = document->firstBlock();
    while(block!=document->lastBlock().next()){
        code += block.text()+"\\n";
        block = block.next();
    }
    delete document;
    return code;
}

QString TuringMachine::gen_resulting_code()
{
    QString output;
    QMap<QString,TMCommand> m;

    if(root_machine){
    output += "#name "+this->name+"\r\n";
    output += "#tape "+this->default_tape+"\r\n";
    output += "#init "+this->state_first+"\r\n";
    output += "\r\n";
    }

    foreach(TuringMachine *m,related_machines){
        output += "//"+m->alias+"("+m->name+") code\r\n";
        output += "//program:\r\n"+m->program+"\r\n//program_end\r\n";
        output += m->gen_resulting_code();
    }

    if(root_machine){
        output += "//Root_Machine code\r\n";
    }
    foreach(m,command_map){
        foreach(TMCommand command,m){
            if(command.state != io_ex::error_state && command.state != io_ex::halt_state && command.character != io_ex::begin_character ){
                QString from_state;
                QString to_state;
                if(io_ex::machine.exactMatch(command.goto_state)){
                    to_state = related_machines[command.goto_state]->state_first+"_"+command.goto_state;
                }else{
                    if(root_machine){
                        to_state = command.goto_state;
                    }else{
                        to_state = command.goto_state+"_"+alias;
                    }
                }

                if(io_ex::machine.exactMatch(command.state)){
                    from_state = "halt_"+command.state;
                }else{
                    if(root_machine){
                    from_state = command.state;
                    }else{
                        from_state = command.state+"_"+alias;
                    }
                }

                output += from_state +" "+ command.character +" "+ to_state +" "+ command.write_char + "\r\n";
            }
        }
    }
    return output;
}

#undef launch_error
