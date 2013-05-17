#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));

    ui->widget->setMoveTool();

    connect(ui->actionCreate_State,SIGNAL(triggered()),ui->widget,SLOT(setCreateTool()));
    connect(ui->actionMove_Select,SIGNAL(triggered()),ui->widget,SLOT(setMoveTool()));
    connect(ui->actionCreate_Transition,SIGNAL(triggered()),ui->widget,SLOT(setLinkTool()));
    connect(ui->actionEdit_Text,SIGNAL(triggered()),ui->widget,SLOT(setRenameTool()));
    connect(ui->actionInitial_State,SIGNAL(triggered()),ui->widget,SLOT(setFirstTool()));
    connect(ui->actionFinal_State,SIGNAL(triggered()),ui->widget,SLOT(setHaltTool()));
    connect(ui->actionAdd_Text,SIGNAL(triggered()),ui->widget,SLOT(setAddTextTool()));
    connect(ui->actionDelete,SIGNAL(triggered()),ui->widget,SLOT(setDeleteTool()));

    connect(ui->actionSave_SVG,SIGNAL(triggered()),ui->widget,SLOT(exportSVG()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
