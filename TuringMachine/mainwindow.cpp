#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "tmsyntax.h"
#include "tmstate.h"
#include "tmcharacter.h"
#include "tmcommand.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit->setFont(io_font::input);
    ui->textEdit->clear();
    ui->textEdit->setFont(io_font::input);

    new TMSyntax(ui->textEdit);
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(compile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::compile()
{
    TMState::clear();
    TMCharacter::clear();
    TMCommand::clear();
    bool valid;
    int errorline = -1;
    QString errorstring = "";
    //qDebug() << ex_valid_program.pattern();
    //qDebug() << ui->textEdit->toPlainText();
    QStringList text = ui->textEdit->toPlainText().split('\n');
    for(int i=0;i<text.size();i++){
        QString line = text.at(i);
        if(io_ex::line.exactMatch(line)){
            QStringList arg = line.split(QRegExp("(\\s+)"));
            TMState::add(arg[0]);
            TMCharacter::add(arg[1]);
            TMState::add(arg[2]);
            if(arg[3]!="<" and arg[3]!=">"){
                TMCharacter::add(arg[3]);
            }
            TMCommand::add(arg[0],arg[1],arg[2],arg[3]);
        }else if(io_ex::blank.exactMatch(line) || io_ex::comment.exactMatch(line)){
            //ignore comments and white lines
        }else{
            valid = false;
            errorline = i;
            errorstring = "bad syntax";
            break;
        }
    }

    if (valid) {
        ui->label_valid->setText("valid");
        ui->label_valid->setStyleSheet("color: #0f0");
    }else{
        ui->label_valid->setText(QString("invalid: first error line %1").arg(errorline));
        ui->label_valid->setStyleSheet("color: #f00");
    }

    QString alphabet_str = "";
    alphabet_str += "Σ = { ";
    foreach(TMCharacter character,TMCharacter::map){
        alphabet_str += character.name+", ";
    }
    alphabet_str += "}";
    ui->label_alphabet->setText(alphabet_str);

    QString state_str = "";
    state_str += "Q = { ";
    foreach(TMState state,TMState::map){
        state_str += (state.name+", ");
    }
    state_str += "}";
    ui->label_states->setText(state_str);
}
