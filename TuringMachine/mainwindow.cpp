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

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(compile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::compile()
{
    if (ex_valid_program.exactMatch(ui->textEdit->toPlainText())) {
        qDebug() << "valid";
        qDebug() << ex_valid_program.matchedLength();
    }else{
        qDebug() <<"invalid";
        qDebug() << ex_valid_program.matchedLength();
    }
}
