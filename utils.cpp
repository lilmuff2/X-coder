#include "utils.h"
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>
#include <QCoreApplication>
void Utils::getPermission(){
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
}
QString Utils::uri2path(QString path){
    QJniObject text = QJniObject::fromString(path);
    QJniObject context = QJniObject(QNativeInterface::QAndroidApplication::context());
    return QJniObject::callStaticMethod<jstring>
                ("lilmuff1/xcoder/Utils" // class name
                 , "Uri2path" // method name
                 , "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;" // signature
                 , QNativeInterface::QAndroidApplication::context()
                 ,text.object<jstring>()).toString();
}
