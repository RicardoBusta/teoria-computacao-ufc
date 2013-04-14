#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "tmsyntax.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TMSyntax *syntax = new TMSyntax(ui->textEdit);
    ex_valid_program = syntax->get_valid_exp();

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(compile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::compile()
{
    qDebug() << ex_valid_program.pattern();
    qDebug() << ui->textEdit->toPlainText();
    if (ex_valid_program.exactMatch(ui->textEdit->toPlainText())) {
        ui->label->setText("valid");
        ui->label->setStyleSheet("color: #0f0");
    }else{
        ui->label->setText("invalid");
        ui->label->setStyleSheet("color: #f00");
    }
}
