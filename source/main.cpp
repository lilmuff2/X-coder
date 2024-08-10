#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "json.hpp"
#include <fstream>
#include <io/file_stream.h>
using json = nlohmann::ordered_json;

#if defined Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>
#endif


int main(int argc, char *argv[])
{

    int currentExitCode = 0;
    do {
        QApplication a(argc, argv);
        QTranslator translator;
        std::filesystem::path settings = "settings.json";
        if(!std::filesystem::exists(settings)){
            const QStringList uiLanguages = QLocale::system().uiLanguages();
            for (const QString &locale : uiLanguages) {
                QString baseName = "X-coder_" + QLocale(locale).name();
                if (translator.load(baseName)) {
                    a.installTranslator(&translator);
                    break;
                }else{
                    QString baseName = ":/translate/X-coder_" + QLocale(locale).name();
                    if (translator.load(baseName)) {
                        a.installTranslator(&translator);
                        break;
                    }
                }
            }
            json config = {{"language",QTranslator::tr("en").toStdString()}};
            sc::OutputFileStream file_info(settings);
            file_info.write(config.dump(4).data(), config.dump(4).size());
        }else{
            std::ifstream cfile(settings);
            json config = json::parse(cfile);
            std::string lan = config["language"];
            if (translator.load("X-coder_" + QString().fromStdString(lan))) {
                a.installTranslator(&translator);
            }else if (translator.load(":/translate/X-coder_" + QString().fromStdString(lan))) {
                a.installTranslator(&translator);
            }else if(config["language"]!="en"){
                config["language"]="en";
                sc::OutputFileStream file_info(settings);
                file_info.write(config.dump(4).data(), config.dump(4).size());
            }
        }
        MainWindow w;
        w.show();
    #if defined Q_OS_ANDROID
        if (QtAndroidPrivate::androidSdkVersion() >= 30){
    #define PACKAGE_NAME "package:lilmuff1.xcoder"
            jboolean value = QJniObject::callStaticMethod<jboolean>("android/os/Environment", "isExternalStorageManager");
            if(value == false) {
                QJniObject filepermit = QJniObject::getStaticObjectField( "android/provider/Settings", "ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION","Ljava/lang/String;" );
                QJniObject pkgName = QJniObject::fromString(PACKAGE_NAME);
                QJniObject parsedUri = QJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", pkgName.object<jstring>());
                QJniObject intent("android/content/Intent", "(Ljava/lang/String;Landroid/net/Uri;)V", filepermit.object<jstring>(),parsedUri.object());
                QtAndroidPrivate::startActivity(intent, 0);
            }
        }
        else{
            const QVector<QString> permissions({
                                                "android.permission.WRITE_EXTERNAL_STORAGE",
                                                "android.permission.READ_EXTERNAL_STORAGE"});
            for(const QString &permission : permissions){
                auto result = QtAndroidPrivate::checkPermission(permission).result();
                if(result == QtAndroidPrivate::Denied){
                    QtAndroidPrivate::requestPermission(permission);
                }
            }
        }
    #endif
        currentExitCode = a.exec();
    } while( currentExitCode ==69 );
    return currentExitCode;
}
