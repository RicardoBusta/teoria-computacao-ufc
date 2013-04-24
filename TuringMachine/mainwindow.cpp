#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include "tmsyntax.h"

#include "tmguistate.h"

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Simulador de Maquina de Turing - by Ricardo Bustamante");
    this->resize(800,600);
    ui->machine_input->clear();

    io_font::input.setFamily("Courier");
    io_font::input.setStyleHint(QFont::Monospace);
    io_font::input.setFixedPitch(true);
    io_font::input.setPointSize(12);

    ui->machine_input->setFont(io_font::input);
    ui->machine_debug->setFont(io_font::input);

    ui->label_current_state->setFont(io_font::input);
    ui->label_current_step->setFont(io_font::input);
    ui->label_current_tape->setFont(io_font::input);
    ui->line_number->setFont(io_font::input);

    create_machine("//Empty Machine");
    create_machine("//Empty Machine");
    create_machine("//Empty Machine");

    create_machine("#name L\n#tape #\n//L Machine\nq0 a halt <\nq0 b halt <\nq0 # halt <");

    create_machine("#name R\n#tape #\n//R machine\nq0 a halt >\nq0 b halt >\nq0 # halt >");

    create_machine("#name RBlank\n#tape #aabbba\n//RBlank machine\nq0 # q1 >\nq1 a q1 >\nq1 b q1 >\nq1 # halt #");

    set_current_machine(ui->comboBox->currentText());


    new TMSyntax(ui->machine_input);
    connect(ui->machine_input->document(), SIGNAL(contentsChanged()), this, SLOT(process_text()));
    connect(ui->machine_start,SIGNAL(clicked()),this,SLOT(start_machine()));
    connect(ui->machine_step,SIGNAL(clicked()),this,SLOT(step_machine()));

    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about_clicked()));
    connect(ui->machine_log,SIGNAL(clicked()),this,SLOT(show_log()));
    connect(ui->machine_step_back,SIGNAL(clicked()),this, SLOT(back_step_machine()));

    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(set_current_machine(QString)));

    connect(ui->machine_input->verticalScrollBar(),SIGNAL(valueChanged(int)),ui->line_number->verticalScrollBar(),SLOT(setValue(int)));
    connect(ui->line_number->verticalScrollBar(),SIGNAL(valueChanged(int)),ui->machine_input->verticalScrollBar(),SLOT(setValue(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::process_text() const
{
    int c = ui->machine_input->document()->lineCount();
    ui->line_number->clear();
    for(int i=0;i<c;i++){
        ui->line_number->append(QString("%1.").arg(i));
    }
    int l;
    ui->line_number->setFixedWidth(QFontMetrics(io_font::input).width(QString("%1..").arg(c-1)));

    TuringMachine *m = TuringMachine::get();
    if(m){
        m->process(ui->machine_input->document());
    }
}

void MainWindow::start_machine() const
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->begin();
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
