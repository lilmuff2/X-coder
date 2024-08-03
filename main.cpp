#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#if defined Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>
#endif



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = ":/X-coder_" + QLocale(locale).name();
        qDebug()<<baseName << translator.load(baseName);
        if (translator.load(baseName)) {
            a.installTranslator(&translator);
            break;
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
    return a.exec();
}
