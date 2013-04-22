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

    //ui->verticalWidget->hide();
    ui->widget->hide();

    this->setWindowTitle("Simulador de Maquina de Turing - by Ricardo Bustamante");

    ui->machine_input->clear();

    ui->machine_tape->setFont(io_font::input);
    ui->machine_input->setFont(io_font::input);
    ui->machine_debug->setFont(io_font::input);
    ui->machine_debug->setStyleSheet("background-color:#fff;");

    ui->label_current_state->setStyleSheet("background-color:#fff;");
    ui->label_current_step->setStyleSheet("background-color:#fff;");
    ui->label_current_tape->setStyleSheet("background-color:#fff;");
    ui->label_current_state->setFont(io_font::input);
    ui->label_current_step->setFont(io_font::input);
    ui->label_current_tape->setFont(io_font::input);

    new TuringMachine("L","","//L Machine\nq0 a halt <\nq0 b halt <\nq0 # halt <",this);

    new TuringMachine("R","","//R machine\nq0 a halt >\nq0 b halt >\nq0 # halt >",this);

    new TuringMachine("Rblank","#aabbab","//Rblank machine\nq0 # q1 >\nq1 a q1 >\nq1 b q1 >\nq1 # halt #",this);

    TuringMachine::machine_current = "R";


    new TMSyntax(ui->machine_input);
    connect(ui->machine_input, SIGNAL(textChanged()), this, SLOT(process_text()));
    connect(ui->machine_start,SIGNAL(clicked()),this,SLOT(start_machine()));
    connect(ui->machine_step,SIGNAL(clicked()),this,SLOT(step_machine()));

    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about_clicked()));
    connect(ui->machine_log,SIGNAL(clicked()),this,SLOT(show_log()));
    connect(ui->machine_step_back,SIGNAL(clicked()),this, SLOT(back_step_machine()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::process_text()
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->process(ui->machine_input->toPlainText());
    }
}

void MainWindow::start_machine()
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->begin(ui->machine_tape->text());
    }
}

void MainWindow::step_machine()
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->step();
    }
    qDebug() << TuringMachine::machine_current;
}

void MainWindow::show_log()
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

void MainWindow::set_current_machine(QString name)
{
    if(TuringMachine::machine_map.contains(name)){
        if(TuringMachine::get()){
            disconnect(TuringMachine::get(),0,0,0);
        }
        TuringMachine::machine_current = name;
        connect(TuringMachine::get(),SIGNAL(current_state(QString)),ui->label_current_state,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_tape(QString)),ui->label_current_tape,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_step(QString)),ui->label_current_step,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_description(QString)),ui->machine_debug,SLOT(setText(QString)));

        ui->machine_input->setText(TuringMachine::get()->program);
        ui->machine_tape->setText(TuringMachine::get()->default_tape);
    }
}


void MainWindow::back_step_machine()
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->back_step();
    }
}
