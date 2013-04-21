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

  ui->machine_input->setText("//R machine\nq0 # q1 >\nq1 a q1 >\nq1 b q1 >\nq1 # halt #");
  ui->machine_tape->setText("ababab");

  TuringMachine::machine_map.insert("M",new TuringMachine(this));
  TuringMachine::machine_current = "M";

  new TMSyntax(ui->machine_input);
  connect(ui->machine_input, SIGNAL(textChanged()), this, SLOT(process_text()));
  connect(ui->machine_start,SIGNAL(clicked()),this,SLOT(start_machine()));
  connect(ui->machine_step,SIGNAL(clicked()),this,SLOT(step_machine()));
  connect(TuringMachine::machine_map[TuringMachine::machine_current],SIGNAL(current_state(QString)),ui->label_current_state,SLOT(setText(QString)));
  connect(TuringMachine::machine_map[TuringMachine::machine_current],SIGNAL(current_tape(QString)),ui->label_current_tape,SLOT(setText(QString)));
  connect(TuringMachine::machine_map[TuringMachine::machine_current],SIGNAL(current_step(QString)),ui->label_current_step,SLOT(setText(QString)));
  connect(ui->machine_step_back,SIGNAL(clicked()),TuringMachine::machine_map[TuringMachine::machine_current], SLOT(back_step()));
  connect(TuringMachine::machine_map[TuringMachine::machine_current],SIGNAL(current_description(QString)),ui->machine_debug,SLOT(setText(QString)));

  connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about_clicked()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::process_text()
{
    TuringMachine::machine_map[TuringMachine::machine_current]->process(ui->machine_input->toPlainText());
}

void MainWindow::start_machine()
{
  TuringMachine::machine_map[TuringMachine::machine_current]->begin(ui->machine_tape->text());
}

void MainWindow::step_machine()
{
  TuringMachine::machine_map[TuringMachine::machine_current]->step();
  qDebug() << TuringMachine::machine_current;
}

void MainWindow::about_clicked()
{
    QMessageBox *about = new QMessageBox(QMessageBox::Information,"Sobre","Programa desenvolvido por Ricardo Bustamante (ricardobqueiroz@gmail.com)\nCom auxilio de Vanessa Karla",QMessageBox::NoButton,this);
    about->exec();
}
