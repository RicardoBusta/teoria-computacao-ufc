#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_create,SIGNAL(clicked()),ui->widget,SLOT(setCreateTool()));
    connect(ui->pushButton_move,SIGNAL(clicked()),ui->widget,SLOT(setMoveTool()));
    connect(ui->pushButton_link,SIGNAL(clicked()),ui->widget,SLOT(setLinkTool()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
