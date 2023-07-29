#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(500,200);
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



