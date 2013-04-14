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

    ui->machine_tape->setFont(io_font::input);
    ui->machine_input->clear();
    ui->machine_input->setFont(io_font::input);
    ui->machine_debug->setFont(io_font::input);

    new TMSyntax(ui->machine_input);
    connect(ui->machine_input, SIGNAL(textChanged()), this, SLOT(compile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::compile()
{
    TMState::clear();
    TMCharacter::clear();
    bool valid;
    bool first_defined = false;
    int errorline = -1;
    QString errorstring = "";
    //qDebug() << ex_valid_program.pattern();
    //qDebug() << ui->textEdit->toPlainText();
    QStringList text = ui->machine_input->toPlainText().split('\n');
    for(int i=0;i<text.size();i++){
        QString line = text.at(i);
        if(io_ex::line.exactMatch(line)){
            QStringList arg = line.split(QRegExp("(\\s+)"));
            if(!first_defined){
                first_defined = true;
                TMState::first_state = arg[0];
            }
            TMState::add(arg[0]);
            TMCharacter::add(arg[1]);
            TMState::add(arg[2]);
            if(arg[3]!="<" and arg[3]!=">"){
                TMCharacter::add(arg[3]);
            }
            TMCommand::queue_add(arg,i);
        }else if(io_ex::blank.exactMatch(line) || io_ex::comment.exactMatch(line)){
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
    if((def_line = TMCommand::add())>=0){
        valid = false;
        errorline = def_line;
        errorstring = QString("previously defined at line %1").arg(def_line);
    }


    //Start Debug Output
    ui->machine_debug->clear();
    if (valid) {
        ui->machine_debug->append("<font color='#0a0'><b>[valid]</b></font>");
    }else{
        ui->machine_debug->append(QString("<font color='#a00'><b>[invalid:line %1] %2</b></font>").arg(errorline).arg(errorstring));
    }

    QString alphabet_str = "";
    alphabet_str += "Σ = { ";
    foreach(TMCharacter character,TMCharacter::map){
        alphabet_str += character.name+", ";
    }
    alphabet_str += "}";
    ui->machine_debug->append(alphabet_str);

    QString state_str = "";
    state_str += "Q = { ";
    foreach(TMState state,TMState::map){
        state_str += (state.name+", ");
    }
    state_str += "}";
    ui->machine_debug->append(state_str);
}
