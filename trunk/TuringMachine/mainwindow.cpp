#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "tmsyntax.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->machine_tape->setFont(io_font::input);
    ui->machine_input->clear();
    ui->machine_input->setFont(io_font::input);
    ui->machine_debug->setFont(io_font::input);

    new TMSyntax(ui->machine_input);
    connect(ui->machine_input, SIGNAL(textChanged()), this, SLOT(process_text()));
    connect(ui->machine_start,SIGNAL(clicked()),this,SLOT(start_machine()));
    connect(ui->machine_step,SIGNAL(clicked()),this,SLOT(step_machine()));
    connect(&tmexec,SIGNAL(current_state(QString)),ui->label,SLOT(setText(QString)));
    connect(&tmexec,SIGNAL(current_tape(QString)),ui->label_2,SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::process_text()
{
    TMState::clear();
    TMCharacter::clear();
    bool valid = true;
    bool first_defined = false;
    int errorline = -1;
    QString errorstring = "";
    //qDebug() << ex_valid_program.pattern();
    //qDebug() << ui->textEdit->toPlainText();
    QStringList text = ui->machine_input->toPlainText().split('\n');
    for(int i=0;i<text.size();i++){
        QString line = text.at(i);
        if(io_ex::valid_line.exactMatch(line)){
            QStringList arg = line.split(QRegExp("(\\s+)"));
            if(!first_defined){
                first_defined = true;
                TMState::first_state = arg[0];
            }
            TMState::add(arg[0]);
            TMCharacter::add(arg[1]);
            TMState::add(arg[2]);
            if(arg[3]!=io_ex::left_command and arg[3]!=io_ex::right_command){
                TMCharacter::add(arg[3]);
            }
            TMCommand::queue_add(arg,i);
        }else if(io_ex::white_line.exactMatch(line) || io_ex::comment.exactMatch(line)){
            //ignore comments and white lines
        }else{
            valid = false;
            errorline = i;
            errorstring = "bad syntax";
            break;
        }
    }
    TMCommand::clear();
    qDebug() << "\n\n\n\n\n\n\n\nclear" << "=========";
    QMap<QString,TMCommand> m;
    foreach(m,TMCommand::map){
        foreach(TMCommand com, m){
            qDebug()<<com.type<<com.goto_state<<com.write_char;
        }
    }

    int def_line=-1;
    if((def_line = TMCommand::add())!=-1){
        valid = false;
        errorline = TMCommand::line_redefined;
        if(def_line!=-2){
            errorstring = QString("previously defined at line %1").arg(def_line);
        }else{
            errorstring = QString("string predefined by machine");
        }
    }


    //Start Debug Output
    ui->machine_debug->clear();
    QString outstr = "";

    if (valid) {
        outstr += "<font color='#0a0'><b>[valid]</b></font><br>";
    }else{
        outstr += QString("<font color='#a00'><b>[invalid:line %1] %2</b></font><br>").arg(errorline).arg(errorstring);
    }

    outstr += "M(Machine) = { Q, &Gamma;, b, &Sigma;, &delta;, s, F }<br>";

    outstr += "Q(Machine States) = { ";
    foreach(TMState state,TMState::map){
        outstr += (state.name+", ");
    }
    outstr += "}<br>";

    outstr += "&Gamma;(Machine Alphabet) = { ";
    foreach(TMCharacter character,TMCharacter::map){
        outstr += character.name+", ";
    }
    outstr += "}<br>";

    outstr += "b(Blank Character) = { "+io_ex::blank_character+", }<br>";

    outstr += "&Sigma;(Input Alphabet) = { ";
    foreach(TMCharacter character,TMCharacter::map){
        if(character.name!=io_ex::blank_character and character.name!=io_ex::begin_character){
            outstr += character.name+", ";
        }
    }
    outstr += "}<br>";

    outstr += "&delta;(Transition Function) = <i>see the table</i><br>";

    outstr += QString("s(Initial State) = { %1, }<br>").arg(TMState::first_state);

    outstr += QString("F(Final States) = { "+io_ex::halt_state+", }<br>");


    ui->machine_debug->setHtml(outstr);
}

void MainWindow::start_machine()
{
    //ui->machine_input->setText("//R machine\nq0 # q1 >\nq1 a q1 >\nq1 b q1 >\nq1 # HALT #");
    //ui->machine_tape->setText("ababab");
    //ui->machine_programming->setEnabled(!ui->machine_programming->isEnabled());
    tmexec.begin(ui->machine_tape->text());
}

void MainWindow::step_machine()
{
    tmexec.step();
}
