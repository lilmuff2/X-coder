#include "decode.h"
#include "ui_decode.h"
#include <QFileDialog>
#include <SupercellFlash.h>
#include "nlohmann/json.hpp"
#include "opencv2/opencv.hpp"
#include <thread>
namespace fs = std::filesystem;

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

Decode::Decode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Decode)
{
    ui->setupUi(this);
}

Decode::~Decode()
{
    delete ui;
}

void Decode::on_Menu_clicked()
{
    this->hide();
}
void Decode::changetitel(QString titel)
{
    ui->Titel->setText(titel);
    ui->ActionButton->setText(titel);
    QWidget::setWindowTitle(titel);
    if(titel=="Encode"){
        ui->Input1->setPlaceholderText("Input folder with pngs path");
        ui->Input2->setPlaceholderText("Input out _tex.sc path");
    }else if(titel=="Encode Folder"){
        ui->Input1->setPlaceholderText("Input folder with folders with pngs path");
        ui->Input2->setPlaceholderText("Input out _tex.sc folder path");
    }else if(titel=="Decode Folder"){
        ui->Input1->setPlaceholderText("Input folder with _tex.sc path");
        ui->Input2->setPlaceholderText("Input out folders folder path");
    }

}

void Decode::on_select1_clicked()
{
    QString text = ui->Input1->placeholderText();
    if(text.contains("tex.sc")&&!text.contains("folder")){
        ui->Input1->setText(QFileDialog::getOpenFileName(this,"Select _tex.sc file",NULL,"Supercell Flash File(*.sc)"));
    }else{
        ui->Input1->setText(QFileDialog::getExistingDirectory(this,"Select folder",NULL));
    }

}


void Decode::on_select2_clicked()
{
    QString text = ui->Input2->placeholderText();
    if(text.contains("tex.sc")&&!text.contains("folder")){
        if(text.contains("out")){
            ui->Input2->setText(QFileDialog::getSaveFileName(this,"Select out _tex.sc file path ",NULL,"Supercell Flash File(*.sc)"));
        }else{
            ui->Input2->setText(QFileDialog::getOpenFileName(this,"Select _tex.sc file",NULL,"Supercell Flash File(*.sc)"));
        }
    }else{
        ui->Input2->setText(QFileDialog::getExistingDirectory(this,"Select folder",NULL));
    }
}
void Decode::print(QString string){
    ui->Console->setText(ui->Console->text()+"\n"+string);
}
bool Decode::hasEnding(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}
void Decode::on_ActionButton_clicked()
{
    ui->Console->setText("Ouput: ");
    QString action = ui->ActionButton->text();
    ui->ActionButton->setEnabled(false);
    if(action == "Decode"){
        std::thread th(
            [this](){
                decode(ui->Input1->text(),ui->Input2->text());
                ui->ActionButton->setEnabled(true);
            }
            );
        th.detach();
    }
    else if(action == "Encode"){
        std::thread th(
            [this](){
                encode(ui->Input1->text(),ui->Input2->text());
                ui->ActionButton->setEnabled(true);
            }
            );
        th.detach();
    }
    else if(action == "Decode Folder"){
        std::thread th(
            [this](){
                decodefolder(ui->Input1->text(),ui->Input2->text());
                ui->ActionButton->setEnabled(true);
            }
            );
        th.detach();
    }
    else if(action == "Encode Folder"){
        std::thread th(
            [this](){
                encodefolder(ui->Input1->text(),ui->Input2->text());
                ui->ActionButton->setEnabled(true);
            }
            );
        th.detach();
    }
}
string Decode::time(time_point<high_resolution_clock> start,time_point<high_resolution_clock> end){
    milliseconds msTime = duration_cast<milliseconds>(end - start);
    if(msTime.count() >= 60000){
        minutes minTime = duration_cast<minutes>(msTime);
        seconds secTime = duration_cast<seconds>(msTime);
        return(to_string(minTime.count())+"min. " + to_string(secTime.count()-60*minTime.count())+"sec. "+ to_string(msTime.count()-60000*minTime.count())+"ms.");
    }else if(msTime.count() >= 1000){
        seconds secTime = duration_cast<seconds>(msTime);
        return(to_string(secTime.count())+"sec. "+ to_string(msTime.count()-1000*secTime.count())+"ms.");
    }else{
        return(to_string(msTime.count())+"ms.");
    }
}
void Decode::decode(QString filepathstring, QString folderpathstring)
{
    if(filepathstring==""){
        print("ERROR: file path is empty");
        return;
    }
    fs::path filepath = filepathstring.toStdString();
    print(QString::fromStdString("Processing "+filepath.stem().string()+"..."));
    print("Loading file...");
    time_point startTime = high_resolution_clock::now();
    sc::SupercellSWF swf;
    swf.textures.resize(255);
    try {
        swf.loadInternal(filepath, true);
    }
    catch (const std::exception& err) {
        print("ERROR in loading file:");
        print(err.what());
        return;
    }
    time_point LoadingEndTime = high_resolution_clock::now();
    print(QString::fromStdString("Loading took "+time(startTime,LoadingEndTime)));
    print("Decoding...");
    time_point DecidingStartTime = high_resolution_clock::now();
    fs::path folderpath = folderpathstring.toStdString();
    if(folderpathstring==""){
        print("Folder path is empty, it will be chosen automatically");
        folderpath = filepath.parent_path()/filepath.stem();
        if (!fs::exists(folderpath)) {
            fs::create_directories(folderpath);
        }
    }else if (!fs::exists(folderpath)) {
        fs::create_directories(folderpath);
    }
    json data = json::array();
    for (uint32_t i = 0; swf.textures.size() > i; i++) {
        sc::pSWFTexture texture = swf.textures[i];
        if (texture == nullptr) break;
        json texturedata = {
            {"PixelFormat",(uint8_t)texture->pixelFormat()},
            {"Filter",(uint8_t)texture->textureFilter()},
            {"Linear",texture->linear()},
            {"Downscaling",texture->downscaling()},
            {"IsKhronos",(bool)texture->textureEncoding()}
        };
        data.push_back(texturedata);
        fs::path imagePath = folderpath / filepath.stem().concat("_"+to_string(i) + ".png");
        texture->textureEncoding(sc::SWFTexture::TextureEncoding::Raw);
        texture->pixelFormat(sc::SWFTexture::PixelFormat::RGBA8);
        texture->linear(true);
        cv::Mat image = cv::Mat(cv::Size(texture->width(), texture->height()), CV_8UC4, texture->textureData.data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
        cv::imwrite(imagePath.string(), image);
    }
    fs::path jsonPath = folderpath / "data.json";
    ofstream file(jsonPath);
    file << data.dump(1);
    file.close();
    time_point EndTime = high_resolution_clock::now();
    print(QString::fromStdString("Decoding took "+time(DecidingStartTime,EndTime)));
    print(QString::fromStdString("All took "+time(startTime,EndTime)));
}
void Decode::decodefolder(QString infolderpathstring, QString outfolderpathstring)
{
    if(infolderpathstring==""){
        print("ERROR: folder with sc path is empty");
        return;
    }
    fs::path infolderpath = infolderpathstring.toStdString();
    fs::path outfolderpath = outfolderpathstring.toStdString();
    if(outfolderpathstring==""){
        print("folder with pngs path is empty.\nIt will be same as folder with sc path");
        outfolderpath = infolderpath;
    }
    time_point startTime = high_resolution_clock::now();
    for (auto file : fs::directory_iterator(infolderpath)) {
        if (hasEnding(file.path().string(), "_tex.sc")) {
            decode(QString::fromStdString(file.path().string()), QString::fromStdString((outfolderpath / file.path().stem()).string()));
        }
    }
    time_point EndTime = high_resolution_clock::now();
    print(QString::fromStdString("All took "+time(startTime,EndTime)));
}
void Decode::encode(QString folderpathstring, QString filepathstring){
    if(folderpathstring==""){
        print("ERROR: folder path is empty");
        return;
    }
    fs::path folderpath = folderpathstring.toStdString();
    print(QString::fromStdString("Processing "+folderpath.stem().string()+"..."));
    time_point startTime = high_resolution_clock::now();
    print("Reading and encoding pngs...");
    sc::SupercellSWF swf;

    fs::path jsonPath = folderpath / "data.json";
    json TexturesData = json::array();
    if(fs::exists(jsonPath)){
        ifstream f(jsonPath);
        TexturesData = json::parse(f);
    }

    int TextureIndex = 0;
    for (auto file : fs::directory_iterator(folderpath)) {
        string path = file.path().string();
        if (hasEnding(path, ".png")) {
            json TextureData = TexturesData[TextureIndex];
            if (TextureData.empty()) {
                TextureData = {
                    {"PixelFormat",0},
                    {"Filter",0},
                    {"Linear",true},
                    {"Downscaling",true},
                    {"IsKhronos",true}
                };
            }
            cv::Mat image = cv::imread(path,cv::IMREAD_UNCHANGED);
            auto texture = new sc::SWFTexture();
            texture->width(image.cols);
            texture->height(image.rows);
            cv::cvtColor(image, image, cv::COLOR_RGBA2BGRA);
            if (image.channels() != 4) {
                print(QString::fromStdString("Error. Not correct channel count: "+image.channels()));
                return;
            }
            texture->textureData = std::vector<uint8_t>(image.cols * image.rows * 4);
            memcpy(texture->textureData.data(), image.data, texture->textureData.size());
            texture->downscaling(TextureData["Downscaling"]);
            texture->textureFilter(TextureData["Filter"]);
            texture->linear(TextureData["Linear"]);
            texture->pixelFormat(TextureData["PixelFormat"]);
            if (TextureData["IsKhronos"]) {
                texture->textureEncoding(sc::SWFTexture::TextureEncoding::KhronosTexture);
            }
            sc::pSWFTexture tex = sc::pSWFTexture(texture);
            swf.textures.push_back(tex);
            TextureIndex++;
        }
    }
    time_point encodingEndTime = high_resolution_clock::now();
    print(QString::fromStdString("Encoding took "+time(startTime,encodingEndTime)));
    print("Saving...");
    fs::path filepath = filepathstring.toStdString();
    if(filepathstring==""){
        print("File path is empy, it will be chosen automaticlly");
        filepath = folderpath/folderpath.stem().concat(".sc");
    }
    if (!fs::exists(filepath.parent_path())) {
        fs::create_directories(filepath.parent_path());
    }
    swf.stream.init();
    for (sc::pSWFTexture texture : swf.textures) {
        texture->save(&swf, true, false);
    }
    swf.stream.writeTag(0);
    swf.stream.save(filepath, sc::CompressionSignature::ZSTD);
    time_point EndTime = high_resolution_clock::now();
    print(QString::fromStdString("Saving took "+time(encodingEndTime,EndTime)));
    print(QString::fromStdString("All took "+time(startTime,EndTime)));
}
void Decode::encodefolder(QString infolderpathstring, QString outfolderpathstring){
    if(infolderpathstring==""){
        print("ERROR: folder with pngs path is empty");
        return;
    }
    fs::path infolderpath = infolderpathstring.toStdString();
    fs::path outfolderpath = outfolderpathstring.toStdString();
    if(outfolderpathstring==""){
        print("folder with sc path is empty.\nIt will be same as folder with pngs path");
        outfolderpath = infolderpath;
    }

    time_point startTime = high_resolution_clock::now();
    for (auto folder : fs::directory_iterator(infolderpath)) {
        if (fs::is_directory(folder)) {
            encode(QString::fromStdString(folder.path().string()),QString::fromStdString((outfolderpath / folder.path().stem().concat(".sc")).string()));
        }
    }
    time_point EndTime = high_resolution_clock::now();
    print(QString::fromStdString("All took "+time(startTime,EndTime)));
}
