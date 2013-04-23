#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include "tmsyntax.h"

#include "tmguistate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Simulador de Maquina de Turing - by Ricardo Bustamante");
    this->resize(800,600);
    ui->machine_input->clear();

    ui->machine_tape->setFont(io_font::input);
    ui->machine_input->setFont(io_font::input);
    ui->machine_debug->setFont(io_font::input);

    ui->label_current_state->setFont(io_font::input);
    ui->label_current_step->setFont(io_font::input);
    ui->label_current_tape->setFont(io_font::input);

    create_machine("//Empty Machine");
    create_machine("//Empty Machine");
    create_machine("//Empty Machine");

    create_machine("#name L\n//L Machine\nq0 a halt <\nq0 b halt <\nq0 # halt <");

    create_machine("#name R\n//R machine\nq0 a halt >\nq0 b halt >\nq0 # halt >");

    create_machine("#name RBlank\n//RBlank machine\n//Rblank machine\nq0 # q1 >\nq1 a q1 >\nq1 b q1 >\nq1 # halt #");

    set_current_machine(ui->comboBox->currentText());


    new TMSyntax(ui->machine_input);
    connect(ui->machine_input, SIGNAL(textChanged()), this, SLOT(process_text()));
    connect(ui->machine_start,SIGNAL(clicked()),this,SLOT(start_machine()));
    connect(ui->machine_step,SIGNAL(clicked()),this,SLOT(step_machine()));

    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about_clicked()));
    connect(ui->machine_log,SIGNAL(clicked()),this,SLOT(show_log()));
    connect(ui->machine_step_back,SIGNAL(clicked()),this, SLOT(back_step_machine()));

    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(set_current_machine(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::process_text() const
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->process(ui->machine_input->toPlainText());
    }
}

void MainWindow::start_machine() const
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->begin(ui->machine_tape->text());
    }
}

void MainWindow::step_machine() const
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->step();
    }
}

void MainWindow::show_log() const
{
    QString out;
    foreach(TMHistory h,TuringMachine::get()->history){
        out.append(h.tape.insert(h.head+1,"</b></font>").insert(h.head,"<font color='#f00'><b>"));
        out.append("<br>");
    }
    ui->label_current_tape->setText(out);
}

void MainWindow::about_clicked()
{
    QMessageBox *about = new QMessageBox(QMessageBox::Information,"Sobre","Programa desenvolvido por Ricardo Bustamante (ricardobqueiroz@gmail.com)\nCom auxilio de Vanessa Karla",QMessageBox::NoButton,this);
    about->exec();
}

void MainWindow::create_machine(const QString program)
{
    TuringMachine *m = new TuringMachine(program,this);
    ui->comboBox->addItem(m->name);
}

void MainWindow::set_current_machine(const QString name) const
{
    if(TuringMachine::machine_map.contains(name) && name!=TuringMachine::get()->name){
        if(TuringMachine::get()){
            disconnect(TuringMachine::get(),0,0,0);
        }
        TuringMachine::machine_current_machine = name;
        connect(TuringMachine::get(),SIGNAL(current_state(QString)),ui->label_current_state,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_tape(QString)),ui->label_current_tape,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_step(QString)),ui->label_current_step,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_description(QString)),ui->machine_debug,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(debug_message(QString)),this,SLOT(debug_message(QString)));
        connect(TuringMachine::get(),SIGNAL(rename_event()),this,SLOT(machine_rename_handler()));

        ui->machine_input->setText(TuringMachine::get()->program);

        if(TuringMachine::get()->default_tape!=""){
            ui->machine_tape->setText(TuringMachine::get()->default_tape);
        }
    }
}

void MainWindow::debug_message(const QString s) const
{
    ui->debug_text->append( s );
}

void MainWindow::machine_rename_handler()
{   
    ui->comboBox->setItemText(ui->comboBox->currentIndex(), TuringMachine::machine_current_machine);
}


void MainWindow::back_step_machine() const
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->back_step();
    }
}
