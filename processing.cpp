#include "processing.h"
#include "ui_processing.h"
#include "SWFFile.hpp"
#include "processing.h"
#include "logger/time.h"
#include "json.hpp"
#include <QScrollBar>
#if defined Q_OS_ANDROID
#include <Qtcore/private/qandroidextras_p.h>
#include <QJniObject>
#endif
using json = nlohmann::ordered_json;
Processing::Processing(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Processing)
{
    ui->setupUi(this);
    #if defined Q_OS_ANDROID
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    #endif

}
static QString getRealPathFromUri(const QUrl &url)
{
    QString path = "";
#ifdef Q_OS_ANDROID
    QJniObject jUrl = QJniObject::fromString(url.toString());
    QJniObject jContext = QtAndroidPrivate::context();
    QJniObject jContentResolver = jContext.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
    QJniObject jUri = QJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", jUrl.object<jstring>());
    QJniObject jCursor = jContentResolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", jUri.object<jobject>(), nullptr, nullptr, nullptr, nullptr);
    QJniObject jScheme = jUri.callObjectMethod("getScheme", "()Ljava/lang/String;");
    QJniObject authority;
    if(jScheme.isValid())
    {
        authority = jUri.callObjectMethod("getAuthority", "()Ljava/lang/String;");
    }
    if(authority.isValid() && authority.toString() == "com.android.externalstorage.documents")
    {
        QJniObject jPath = jUri.callObjectMethod("getPath", "()Ljava/lang/String;");
        path = jPath.toString();
    }
    else if(jCursor.isValid() && jCursor.callMethod<jboolean>("moveToFirst"))
    {
        QJniObject jColumnIndex = QJniObject::fromString("_data");
        jint columnIndex = jCursor.callMethod<jint>("getColumnIndexOrThrow", "(Ljava/lang/String;)I", jColumnIndex.object<jstring>());
        QJniObject jRealPath = jCursor.callObjectMethod("getString", "(I)Ljava/lang/String;", columnIndex);
        path = jRealPath.toString();
        if(authority.isValid() && authority.toString().startsWith("com.android.providers") && !url.toString().startsWith("content://media/external/"))
        {
            QStringList list = path.split(":");
            if(list.count() == 2)
            {
                QString type = list.at(0);
                QString id = list.at(1);
                if(type == "image")
                    type = type + "s";
                if(type == "document" || type == "documents")
                    type = "file";
                if(type == "msf")
                    type = "downloads";
                if(QList<QString>({"images","video","audio"}).contains(type))
                    type = type + "/media";
                path = "content://media/external/"+type;
                path = path + "/" + id;
                return getRealPathFromUri(path);
            }
        }
    }
    else
    {
        QJniObject jPath = jUri.callObjectMethod("getPath", "()Ljava/lang/String;");
        path = jPath.toString();
        qDebug() << QFile::exists(path) <<path;
    }

    if(path.startsWith("primary:"))
    {
        path = path.remove(0,QString("primary:").length());
        path = "/sdcard/" + path;
    }
    else if(path.startsWith("/document/primary:"))
    {
        path = path.remove(0,QString("/document/primary:").length());
        path = "/sdcard/" + path;
    }
    else if(path.startsWith("/tree/primary:"))
    {
        path = path.remove(0,QString("/tree/primary:").length());
        path = "/sdcard/" + path;
    }
    else if(path.startsWith("/storage/emulated/0/"))
    {
        path = path.remove(0,QString("/storage/emulated/0/").length());
        path = "/sdcard/" + path;
    }
    else if(path.startsWith("/tree//"))
    {
        path = path.remove(0,QString("/tree//").length());
        path = "/" + path;
    }
    if(!QFileInfo(path).isFile() && !QFileInfo(path).isDir() && !path.startsWith("/data"))
        return url.toString();
    return path;
#else
    return url.toString();
#endif

}
Processing::~Processing()
{
    delete ui;
}

void Processing::ChangeTitle(QString newtitle)
{
    if(newtitle.startsWith("Encode")){
        ui->Run->setText(tr("ENCODE"));
        ui->Title->setText(tr("ENCODE"));
        Processing::setWindowTitle(tr("ENCODE"));
        Processing::Encode = true;
        if(!newtitle.endsWith("Folder")){ui->Input2->setPlaceholderText(tr("OUT FILE"));}
        ui->Input1->setPlaceholderText(tr("INPUT FOLDER"));
    }
    if(newtitle.endsWith("Folder")){
        ui->Input1->setPlaceholderText(tr("INPUT FOLDER"));
        Processing::Folder = true;
        Processing::setWindowTitle(Processing::windowTitle()+" FOLDER");
    }
}




void Processing::on_Select1_clicked()
{
    QString text;
    if(Processing::Encode || Processing::Folder){
       text = QFileDialog::getExistingDirectory(this);
    }else{
        #if defined Q_OS_ANDROID
        text = QFileDialog::getOpenFileName(this);
        #else
            text = QFileDialog::getOpenFileName(this,nullptr,nullptr,"SC FILE (*.sc)");
        #endif
    }
    #if defined Q_OS_ANDROID
    text = getRealPathFromUri(text);
    #endif
    ui->Input1->setText(text);
}


void Processing::on_Select2_clicked()
{
    QString text;
    if(Processing::Encode && !Processing::Folder){
        #if defined Q_OS_ANDROID
                text = QFileDialog::getSaveFileName(this);
        #else
                text = QFileDialog::getSaveFileName(this,nullptr,nullptr,"SC FILE (*.sc)");
        #endif
    }else{
        text = QFileDialog::getExistingDirectory(this);
    }
    #if defined Q_OS_ANDROID
    text = getRealPathFromUri(text);
    #endif
    ui->Input2->setText(text);
}
void Processing::print(std::string str)
{
    ui->Output->setText(QString::fromStdString(ui->Output->text().toStdString()+"\n"+str));
    ui->scrollArea->verticalScrollBar()->setValue(10000000);
}
void Processing::print(QString str)
{
    ui->Output->setText((ui->Output->text()+"\n"+str));
    ui->scrollArea->verticalScrollBar()->setValue(10000000);
}
void Processing::on_ExitBtn_clicked()
{
    this->close();
}


void Processing::on_Run_clicked()
{
    ui->Run->setEnabled(false);
    ui->Output->setText(tr("CONSOLE:"));
    if(!Processing::Encode && !Processing::Folder){//decoding
        std::thread th(
            [this](){
                std::chrono::time_point doperation_start = std::chrono::high_resolution_clock::now();
                decode(ui->Input1->toPlainText().toStdString(),ui->Input2->toPlainText().toStdString());
                print(tr("Decoding took: ").toStdString()+sc::time::calculate_time(doperation_start, std::chrono::high_resolution_clock::now()));
                QMetaObject::invokeMethod(ui->Run,"setEnabled", Qt::QueuedConnection,Q_ARG(bool, true));

            }
        );
        th.detach();
    }
    else if(Processing::Encode && !Processing::Folder){//encode
        std::thread th(
            [this](){
                std::chrono::time_point eoperation_start = std::chrono::high_resolution_clock::now();
                encode(ui->Input1->toPlainText().toStdString(),ui->Input2->toPlainText().toStdString());
                print(tr("Encoding took: ").toStdString()+sc::time::calculate_time(eoperation_start, std::chrono::high_resolution_clock::now()));
                QMetaObject::invokeMethod(ui->Run,"setEnabled", Qt::QueuedConnection,Q_ARG(bool, true));
            }
        );
        th.detach();
    }
    else if(!Processing::Encode && Processing::Folder){//decode folder
        std::thread th(
            [this](){
                std::chrono::time_point operation_start = std::chrono::high_resolution_clock::now();
                fs::path input_path =ui->Input1->toPlainText().toStdString(); fs::path output_path = ui->Input2->toPlainText().toStdString();
                if(!fs::exists(input_path)){
                    print(tr("ERROR: Input folder doesnt exist!"));
                    QMetaObject::invokeMethod(ui->Run,"setEnabled", Qt::QueuedConnection,Q_ARG(bool, true));
                    return;
                }
                if(output_path==""){
                    output_path = input_path.string() + "/Decoded";
                    print(tr("INFO: Out path is empty, so it will be:\n").toStdString() + output_path.string());
                }
                if (fs::exists(output_path))
                {
                    fs::remove_all(output_path);
                }
                fs::create_directories(output_path);
                for (auto const& file_descriptor : std::filesystem::directory_iterator(input_path))
                {
                    std::filesystem::path filepath = file_descriptor.path();
                    if (file_descriptor.is_regular_file() && (filepath.stem().string().find("_tex") != std::string::npos || filepath.stem().string().find("_dl") != std::string::npos))
                    {
                        std::chrono::time_point doperation_start = std::chrono::high_resolution_clock::now();
                        decode(filepath,output_path/filepath.stem());
                        print(tr("Decoding took: ").toStdString()+sc::time::calculate_time(doperation_start, std::chrono::high_resolution_clock::now()));
                    }
                }
                print(tr("Decoding all files took: ").toStdString()+sc::time::calculate_time(operation_start, std::chrono::high_resolution_clock::now()));
                QMetaObject::invokeMethod(ui->Run,"setEnabled", Qt::QueuedConnection,Q_ARG(bool, true));
            }
        );
        th.detach();
    }
    else if(Processing::Encode && Processing::Folder){//encode folder
        std::thread th(
            [this](){
                std::chrono::time_point operation_start = std::chrono::high_resolution_clock::now();
                fs::path input_path =ui->Input1->toPlainText().toStdString(); fs::path output_path = ui->Input2->toPlainText().toStdString();
                if(!fs::exists(input_path)){
                    print(tr("ERROR: Input folder doesnt exist!"));
                    QMetaObject::invokeMethod(ui->Run,"setEnabled", Qt::QueuedConnection,Q_ARG(bool, true));
                    return;
                }
                if(output_path==""){
                    output_path = input_path.string() + "/Encoded";
                    print(tr("INFO: Out path is empty, so it will be:\n").toStdString() + output_path.string());
                }
                if (fs::exists(output_path))
                {
                    fs::remove_all(output_path);
                }
                fs::create_directories(output_path);
                for (auto const& file_descriptor : std::filesystem::directory_iterator(input_path))
                {
                    std::filesystem::path filepath = file_descriptor.path();
                    if (file_descriptor.is_directory() && (filepath.stem().string().find("_tex") != std::string::npos || filepath.stem().string().find("_dl") != std::string::npos))
                    {
                        std::chrono::time_point eoperation_start = std::chrono::high_resolution_clock::now();
                        encode(filepath,output_path/filepath.stem().concat(".sc"));
                        print(tr("Encoding took: ").toStdString()+sc::time::calculate_time(eoperation_start, std::chrono::high_resolution_clock::now()));
                    }
                }
                print(tr("Encoding all files took: ").toStdString()+sc::time::calculate_time(operation_start, std::chrono::high_resolution_clock::now()));
                QMetaObject::invokeMethod(ui->Run,"setEnabled", Qt::QueuedConnection,Q_ARG(bool, true));
            }
            );
        th.detach();
    }

}
void Processing::decode(fs::path input_path,fs::path output_path){
    try {
        if(!fs::exists(input_path)){
            print(tr("ERROR: Input file doesnt exist!"));
            return;
        }
        if(input_path.stem().string().find("_tex") == std::string::npos && input_path.stem().string().find("_dl") == std::string::npos){
            print(tr("WARNING: INPUT FILE MAY NOT BE _TEX.SC if so out will be empty. INPUT FILE MUST BE TEXTURE FILE"));
        }
        print(tr("Working on ").toStdString() + input_path.stem().string() + "...");
        print(tr("Loading file..."));
        sc::SWFFile file(input_path);
        if(output_path==""){
            output_path = input_path;
            output_path.replace_extension();
            print(tr("INFO: Out path is empty, so it will be:\n").toStdString() + output_path.string());
        }
        if (fs::exists(output_path))
        {
            fs::remove_all(output_path);
        }
        fs::create_directories(output_path);
        if(file.isdl){
            print(tr("INFO: It is dl file, so it will be coped to out folder"));
            if(fs::exists(output_path/input_path.filename())){
                fs::remove(output_path/input_path.filename());
            }
            copy_file(input_path,output_path/input_path.filename());
        }
        print(tr("Decoding file..."));
        file.save_textures_to_folder(output_path);
    }
    catch (sc::GeneralRuntimeException& e) {
        print(tr("ERROR:"));
        print(tr(e.what()));

    }
}

void Processing::encode(std::filesystem::path input_path, std::filesystem::path output_path)
{
    try{
        if(!fs::exists(input_path)){
            print(tr("ERROR: Input folder doesnt exist!"));
            return;
        }
        print(tr("Working on ").toStdString() + input_path.stem().string() + "...");
        json config={{"IsDL",false},{"HasZKTX",true},{"Compression","Zstandard"}};
        if(fs::exists(input_path / input_path.stem().concat(".json"))){std::ifstream cfile(input_path / input_path.stem().concat(".json")); config = json::parse(cfile);}
        sc::SWFFile file;
        if(config["IsDL"]){
            print(tr("INFO: It is dl, so loading sc..."));
            file.load(input_path / input_path.stem().concat(".sc"));
        }
        print(tr("Loading images..."));
        file.load_textures_from_folder(input_path);
        if(output_path==""){
            output_path = input_path/input_path.stem().concat(config["IsDL"]? "_new.sc" : ".sc");
            print(tr("INFO: Out path is empty, so it will be:\n").toStdString() + output_path.string());
        }
        print(tr("Saving file..."));
        file.use_external_texture_files = config["HasZKTX"];
        sc::SWFStream::Signature sig = sc::SWFStream::Signature::Zstandard;
        if(config["Compression"]=="Lzma"){sig=sc::SWFStream::Signature::Lzma;}else if(config["Compression"]=="Lzham"){sig = sc::SWFStream::Signature::Lzham;}
        if (config["IsDL"])
        {
            file.save(output_path,sig);
        }
        else
        {
            file.current_file = output_path;
            file.save_internal(true, false);
            file.stream.save_file(output_path, sig);
            file.stream.clear();
        }
    }
    catch (sc::GeneralRuntimeException& e) {
        print(tr("ERROR:"));
        print(tr(e.what()));
    }
}


