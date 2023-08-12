QT += core-private
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    decode.cpp \
    main.cpp \
    mainwindow.cpp \
    utils.cpp
HEADERS += \
    decode.h \
    mainwindow.h \
    utils.h

android: LIBS += -L$$PWD/Deps/SupercellFlash -lASTC \
                 -L$$PWD/Deps/SupercellFlash -lbasisu \
                 -L$$PWD/Deps/SupercellFlash -ldfdutils \
                 -L$$PWD/Deps/SupercellFlash -lETCPACK \
                 -L$$PWD/Deps/SupercellFlash -llibktx \
                 -L$$PWD/Deps/SupercellFlash -lLZHAM \
                 -L$$PWD/Deps/SupercellFlash -lLZMA \
                 -L$$PWD/Deps/SupercellFlash -lSupercellCompression \
                 -L$$PWD/Deps/SupercellFlash -lSupercellFlash \
                 -L$$PWD/Deps/SupercellFlash -lSupercellTextureLoader \
                 -L$$PWD/Deps/SupercellFlash -lZstandard

INCLUDEPATH += $$PWD/Deps/SupercellFlash/Include/ \
               $$PWD/Deps/JSON/
FORMS += \
    decode.ui \
    mainwindow.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
    OTHER_FILES += $$PWD/android/src/lilmuff1.xcoder/*.*
}

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR += \
                $$PWD/android
    OTHER_FILES += $$PWD/android/src/lilmuff1/xcoder/*.*
    isEmpty(OPENCV_ANDROID) {
        error("Let OPENCV_ANDROID point to the opencv-android-sdk, recommended: v4.0")
    }
    INCLUDEPATH += "$$OPENCV_ANDROID/sdk/native/jni/include"
    LIBS += \
        -L"$$OPENCV_ANDROID/sdk/native/libs/arm64-v8a" \
        -L"$$OPENCV_ANDROID/sdk/native/3rdparty/libs/arm64-v8a" \
        -llibtiff \
        -llibjpeg-turbo \
        -llibpng \
        -lIlmImf \
        -ltbb \
        -lopencv_java4 \

    ANDROID_EXTRA_LIBS = $$OPENCV_ANDROID/sdk/native/libs/arm64-v8a/libopencv_java4.so
}
