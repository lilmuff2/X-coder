#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "processing.h"

#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->DecodeFile, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->DecodeFolder, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->EncodeFile, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->EncodeFolder, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BtnCLicked()
{
    Processing *processWindow = new Processing();
    processWindow->show();
    QString action = sender()->objectName();
    processWindow->ChangeTitle(action);
}

void MainWindow::on_About_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("ABOUT");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Author: <a href='tg://resolve?domain=lilmuff1'>lilmuff1</a>\n<br>THX FOR HELP: <a href='https://github.com/Daniil-SV'>Danil SV</a>"
                   "<br>DISCORD: <a href='https://discord.com/invite/yNajwpBe'>discord.com/invite/yNajwpBe</a>"
                   "<br>TG: <a href='tg://resolve?domain=XcoderBS'>t.me/XcoderBS</a>"
                   "<br>GITHUB: <a href='https://github.com/lilmuff2/X-coder'>github.com/lilmuff2/X-coder</a>");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

}

