#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>
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
        Config *cfg = new Config();
        QString lan = cfg->lang();
        if(lan=="none"){
            for (const QString &locale : QLocale::system().uiLanguages()) {
                if (translator.load("X-coder_" + QLocale(locale).name()) || translator.load(":/translate/X-coder_" + QLocale(locale).name())) {
                    a.installTranslator(&translator);
                    break;
                }
            }
            cfg->lang(QTranslator::tr("en"));
        }else{
            if (translator.load("X-coder_" + lan) || translator.load(":/translate/X-coder_" +lan)) {
                a.installTranslator(&translator);
            }else if(lan!="en"){
                cfg->lang("en");
            }
        }
        QFontDatabase::addApplicationFont(":/Pusia-Bold.otf");
        a.setFont(QFont("Pusia-Bold", 11, QFont::Normal, false));
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
        #if not defined Q_OS_ANDROID
        if(!currentExitCode){
            if(w.CheckUpdates()){
                return currentExitCode;
            }
        }
        #else
        std::thread th(
            [&currentExitCode, &w, &a](){
                if(!currentExitCode){
                    if(w.CheckUpdates()){
                        a.exit(currentExitCode);
                    }
                }
            });
        th.detach();
        #endif
        currentExitCode = a.exec();
    } while( currentExitCode == 69 );
    return currentExitCode;
}
