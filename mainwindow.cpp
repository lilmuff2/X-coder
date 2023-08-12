#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "utils.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Utils utils;
    utils.getPermission();
    connect(ui->Decode, SIGNAL(clicked()),this,SLOT(ShowMenu()));
    connect(ui->DecodeFolder, SIGNAL(clicked()),this,SLOT(ShowMenu()));
    connect(ui->Encode, SIGNAL(clicked()),this,SLOT(ShowMenu()));
    connect(ui->EncodeFolder, SIGNAL(clicked()),this,SLOT(ShowMenu()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowMenu()
{
    QPushButton *action = (QPushButton *)sender();
    DecodeMenu = new Decode;
    DecodeMenu->show();
    DecodeMenu->changetitel(action->text());
}


void MainWindow::on_About_clicked()
{
    QMessageBox::about(this,"About","Author: lilmuff1\nDesigner: rst1k\nHelper: Danil SV");
}

