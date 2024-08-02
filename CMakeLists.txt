cmake_minimum_required(VERSION 3.16)

project(X-coder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        SWFFile.hpp
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(X-coder
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        android/AndroidManifest.xml
        rc.rc
        processing.h processing.cpp processing.ui
        json.hpp
        )

else()
    if(ANDROID)
        add_library(X-coder SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(X-coder
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(X-coder PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(X-coder)
endif()

