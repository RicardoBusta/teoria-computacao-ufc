#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainToolBar->addWidget(ui->toolButton_move);
    ui->mainToolBar->addWidget(ui->toolButton_create);
    ui->mainToolBar->addWidget(ui->toolButton_link);
    ui->mainToolBar->addWidget(ui->toolButton_rename);
    ui->mainToolBar->addWidget(ui->toolButton_start);
    ui->mainToolBar->addWidget(ui->toolButton_final);

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));

    connect(ui->toolButton_create,SIGNAL(clicked()),ui->widget,SLOT(setCreateTool()));
    connect(ui->toolButton_move,SIGNAL(clicked()),ui->widget,SLOT(setMoveTool()));
    connect(ui->toolButton_link,SIGNAL(clicked()),ui->widget,SLOT(setLinkTool()));
    connect(ui->toolButton_rename,SIGNAL(clicked()),ui->widget,SLOT(setRenameTool()));
    connect(ui->toolButton_start,SIGNAL(clicked()),ui->widget,SLOT(setFirstTool()));
    connect(ui->toolButton_final,SIGNAL(clicked()),ui->widget,SLOT(setHaltTool()));

    connect(ui->actionSave_SVG,SIGNAL(triggered()),ui->widget,SLOT(exportSVG()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
