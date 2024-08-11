#include "mainwindow.h"
#include "./../ui/ui_mainwindow.h"
#include "processing.h"
#include <QApplication>
#include <QMessageBox>
#include "json.hpp"
using json = nlohmann::ordered_json;
#include <io/file_stream.h>
#include <QtNetwork/QtNetwork>
#include <functional>
#include <QDesktopServices>
void htmlGet(const QUrl &url, const std::function<void(const QString&)> &fun) {
    QScopedPointer<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QNetworkReply *response = manager->get(QNetworkRequest(QUrl(url)));
    QObject::connect(response, &QNetworkReply::finished, [response, fun]{
        response->deleteLater();
        response->manager()->deleteLater();
        if (response->error() != QNetworkReply::NoError) return;

        auto const html = QString::fromUtf8(response->readAll());
        fun(html); // do something with the data
    }) && manager.take();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cfg = new Config();
    ui->setupUi(this);
    connect(ui->DecodeFile, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->DecodeFolder, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->EncodeFile, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
    connect(ui->EncodeFolder, SIGNAL(clicked()),this,SLOT(BtnCLicked()));
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

int MainWindow::CheckUpdates()
{
    htmlGet({"https://raw.githubusercontent.com/lilmuff2/X-coder/images/v.json"}, [this](const QString &body){
        json v = json::parse(body.toStdString());
        int ActualVersion = v["ActualVersion"];
        int MinVersion = v["MinVersion"];
        cfg->versions(ActualVersion,MinVersion);
    });
    if(cfg->versions()[1]>21){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("UPDATE AVAILABE"));
        msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
        msgBox.setText(tr("You can find new version on <a href='https://github.com/lilmuff2/X-coder/releases'>https://github.com/lilmuff2/X-coder/releases</a>"));
        msgBox.addButton(tr("EXIT"), QMessageBox::ActionRole);
        msgBox.exec();
        return 1;
    }else if(cfg->versions()[0]>21){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("UPDATE AVAILABE"));
        msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
        msgBox.setText("You can find new version on <a href='https://github.com/lilmuff2/X-coder/releases'>https://github.com/lilmuff2/X-coder/releases</a>");
        QPushButton *btn = msgBox.addButton(tr("UPDATE"), QMessageBox::ActionRole);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        if (msgBox.clickedButton() == btn) {
            QDesktopServices::openUrl(QUrl("https://github.com/lilmuff2/X-coder/releases"));
        }
    }
    return 0;
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
    cfg->lang(langs[index]);
    qApp->exit(69);
}

