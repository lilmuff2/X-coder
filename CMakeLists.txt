cmake_minimum_required(VERSION 3.16)

project(X-coder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Qml LinguistTools)


set(TS_FILES res/translate/X-coder_ru.ts)
set(PROJECT_SOURCES
        source/main.cpp
        source/mainwindow.cpp
        source/processing.cpp
        source/json.hpp
        source/SWFFile.hpp
        source/config.hpp
        headers/mainwindow.h
        headers/processing.h
        ui/mainwindow.ui
        ui/processing.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(X-coder
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        android/AndroidManifest.xml
        res/rc.rc
        res/res.qrc
        )
endif()
target_link_libraries(X-coder PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network)
include_directories(headers ui source)
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.X-coder)
endif()
set_target_properties(X-coder PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS X-coder
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
set_property(TARGET X-coder APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)
include(FetchContent)
# Adding SupercellFlash
FetchContent_Declare(
    SupercellFlash
    GIT_REPOSITORY https://github.com/sc-workshop/SupercellFlash.git
    GIT_TAG dev
)
FetchContent_MakeAvailable(SupercellFlash)

# CPP 17
target_compile_features(
    X-coder PRIVATE
    cxx_std_17
)

target_link_libraries(
   X-coder PRIVATE
    SupercellFlash
)
qt6_add_translations(X-coder
    TS_FILES ${TS_FILES}
    QM_FILES_OUTPUT_VARIABLE qm_files)
#install(FILES ${qm_files} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
if (ANDROID)
    include(C:/DEV/ANDROID/SDK/android_openssl/android_openssl.cmake)
    add_android_openssl_libraries(X-coder)
    set_property (TARGET X-coder APPEND PROPERTY
        QT_ANDROID_EXTRA_LIBS "C:/DEV/ANDROID/SDK/android_openssl/ssl_3/arm64-v8a/libcrypto_3.so" "C:/DEV/ANDROID/SDK/android_openssl/ssl_3/arm64-v8a/libssl_3.so" "C:/DEV/ANDROID/SDK/android_openssl/ssl_3/armeabi-v7a/libcrypto_3.so" "C:/DEV/ANDROID/SDK/android_openssl/ssl_3/armeabi-v7a/libssl_3.so"
    )
endif()
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(X-coder)
endif()

