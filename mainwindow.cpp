#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "processing.h"

#include <QMessageBox>

#include "json.hpp"
using json = nlohmann::ordered_json;
#include <io/file_stream.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->DecodeFile, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->DecodeFolder, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->EncodeFile, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->EncodeFolder, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    std::vector<QString> langs = {"en","ru"};
    for(int i = 0;i<langs.size();i++){
        if(langs[i]==tr("en")){
            ui->comboBox->setCurrentIndex(i);
            break;
        }
    }
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
    msgBox.setWindowTitle(tr("ABOUT"));
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Author: <a href='tg://resolve?domain=lilmuff1'>lilmuff1</a>\n<br>THX FOR HELP: <a href='https://github.com/Daniil-SV'>Danil SV</a>"
                   "<br>DISCORD: <a href='https://discord.com/invite/yNajwpBe'>discord.com/invite/yNajwpBe</a>"
                   "<br>TG: <a href='tg://resolve?domain=XcoderBS'>t.me/XcoderBS</a>"
                   "<br>GITHUB: <a href='https://github.com/lilmuff2/X-coder'>github.com/lilmuff2/X-coder</a>");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    std::vector<std::string> langs = {"en","ru"};
    std::filesystem::path settings = "settings.json";
    #if defined Q_OS_ANDROID
    settings = "/sdcard/Android/data/lilmuff1.xcoder/settings.json";
    #endif
    std::ifstream cfile(settings);
    json config = json::parse(cfile);
    config["language"]=langs[index];
    sc::OutputFileStream file_info(settings);
    file_info.write(config.dump(4).data(), config.dump(4).size());
    qApp->exit(69);
 }

