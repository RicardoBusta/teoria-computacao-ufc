#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include "tmsyntax.h"

#include "tmguistate.h"

#include <QScrollBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Simulador de Maquina de Turing - by Ricardo Bustamante");
    this->resize(800,600);

    io_font::input = QFont("consolas",12);
    io_font::input.setFamily("Courier");
    io_font::input.setStyleHint(QFont::Monospace);
    io_font::input.setFixedPitch(true);
    io_font::input.setPointSize(12);

    ui->machine_input->clear();
    ui->line_number->clear();

    ui->machine_input->document()->setDefaultFont(io_font::input);
    ui->line_number->document()->setDefaultFont(io_font::input);

    ui->machine_input->setFont(io_font::input);
    ui->machine_debug->setFont(io_font::input);
    ui->line_number->setFont(io_font::input);
    ui->label_current_state->setFont(io_font::input);
    ui->label_current_step->setFont(io_font::input);
    ui->label_current_tape->setFont(io_font::input);


    ui->comboBox->addItem("--");

    create_machine("#name HELP\n\n//HELP MACHINE");

    create_machine("#name L\n#tape #\n//L Machine\nq0 a halt <\nq0 b halt <\nq0 # halt <");

    create_machine("#name R\n#tape #\n//R machine\nq0 a halt >\nq0 b halt >\nq0 # halt >");

    create_machine("#name C\n#tape #ab\n//Composite test machine\nq0 * q1 *\nq1 * R *\nR * L *\nL * halt *");

    create_machine("#name RBlank\n#tape #aabbba\n//RBlank machine\nq0 # q1 >\nq1 a q1 >\nq1 b q1 >\nq1 # halt #");

    create_machine("#name ER\n#tape #aaaabbb\n\nq0 a q1 >\nq0 b q1 >\nq0 # q1 >\nq1 a q1 >\nq1 b q1 >\nq1 # q2 <\nq2 a q3 #\nq3 a q2 <\nq2 b q3 #\nq3 b q2 <\nq2 # halt *\nq3 # q2 <");

    set_current_machine("--");

    new TMSyntax(ui->machine_input);

    connect(ui->machine_step,SIGNAL(clicked()),this,SLOT(step_machine()));
    connect(ui->machine_input->document(), SIGNAL(contentsChanged()), this, SLOT(process_text()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about_clicked()));
    connect(ui->machine_log,SIGNAL(clicked()),this,SLOT(show_log()));
    connect(ui->machine_step_back,SIGNAL(clicked()),this, SLOT(back_step_machine()));

    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(set_current_machine(QString)));

    connect(ui->machine_input->verticalScrollBar(),SIGNAL(valueChanged(int)),ui->line_number->verticalScrollBar(),SLOT(setValue(int)));
    connect(ui->line_number->verticalScrollBar(),SIGNAL(valueChanged(int)),ui->machine_input->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->push_save,SIGNAL(clicked()),this,SLOT(save_machine()));
    connect(ui->push_load,SIGNAL(clicked()),this,SLOT(load_machine()));

    connect(ui->push_add,SIGNAL(clicked()),this,SLOT(add_machine()));
    connect(ui->push_remove,SIGNAL(clicked()),this,SLOT(remove_machine()));
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
    ui->line_number->setFixedWidth(QFontMetrics(io_font::input).width(QString("%1AB").arg(c-1)));

    if(TuringMachine::get()){
        TuringMachine *m = TuringMachine::get();
        if(m){
            m->process(ui->machine_input->document());
        }
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

QString MainWindow::create_machine(const QString program)
{
    TuringMachine *m = new TuringMachine(program,this);
    ui->comboBox->addItem(m->name);
    return m->name;
}

void MainWindow::set_current_machine(const QString name)
{
    if(TuringMachine::machine_map.contains(name)){
        if(TuringMachine::get()){
            disconnect(TuringMachine::get(),0,0,0);
        }

        TuringMachine::machine_current_machine = name;
        connect(TuringMachine::get(),SIGNAL(current_state_signal(QString)),ui->label_current_state,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_tape_signal(QString)),ui->label_current_tape,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_step_signal(QString)),ui->label_current_step,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(current_description_signal(QString)),ui->machine_debug,SLOT(setText(QString)));
        connect(TuringMachine::get(),SIGNAL(debug_message(QString)),this,SLOT(debug_message(QString)));
        connect(TuringMachine::get(),SIGNAL(rename_event()),this,SLOT(machine_rename_handler()));

        ui->label_current_state->setEnabled(true);
        ui->label_current_tape->setEnabled(true);
        ui->label_current_step->setEnabled(true);
        ui->machine_debug->setEnabled(true);
        ui->machine_input->setEnabled(true);
        ui->machine_input->setText(TuringMachine::get()->program);
    }else{
        if(TuringMachine::get()){
            disconnect(TuringMachine::get(),0,0,0);
        }

        TuringMachine::machine_current_machine = "";

        ui->label_current_state->clear();
        ui->label_current_state->setEnabled(false);
        ui->label_current_tape->clear();
        ui->label_current_tape->setEnabled(false);
        ui->label_current_step->clear();
        ui->label_current_step->setEnabled(false);
        ui->machine_debug->clear();
        ui->machine_debug->setEnabled(false);
        ui->machine_input->clear();
        ui->machine_input->setEnabled(false);
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

void MainWindow::save_machine()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this,"Save File","","Text File (*.txt);;Turing Machine (*.mt)");
    if( filename=="" ) return;
    QFile outfile(filename);
    if( !outfile.open( QIODevice::Append | QIODevice::Text ) ) return;
    QTextStream out(&outfile);
    out << ui->machine_input->toPlainText() << endl;
}

void MainWindow::load_machine()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,"Open File","","Text File (*.txt);;Turing Machine (*.mt)");
    if( filename == "" ) return;
    QFile infile(filename);
    if( !infile.open( QIODevice::ReadOnly | QIODevice::Text )) return;
    QTextStream in(&infile);
    ui->machine_input->setText( in.readAll() );
}

void MainWindow::add_machine()
{
    QString m = create_machine("");
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(m));
    set_current_machine(m);

}

void MainWindow::remove_machine()
{
    if(ui->comboBox->currentIndex()!=0){
        ui->comboBox->removeItem(ui->comboBox->currentIndex());
        set_current_machine(ui->comboBox->currentText());
    }
}

void MainWindow::back_step_machine() const
{
    TuringMachine *m = TuringMachine::get();
    if(m){
        m->back_step();
    }
}
