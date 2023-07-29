cmake_minimum_required(VERSION 3.5)

project(PC VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        decode.cpp
        decode.h
        decode.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PC
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PC APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PC SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PC
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PC)
endif()
set_target_properties(PC PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PC)
endif()

target_include_directories(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/include)
target_include_directories(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/JSON)
FIND_PACKAGE( OpenCV REQUIRED )
target_include_directories(PC PRIVATE ${OpenCV_INCLUDE_DIRS} )
target_link_libraries (PC PRIVATE ${OpenCV_LIBS})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/SupercellFlash.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/ASTC.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/ETCPACK.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/LZHAM.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/LZMA.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/SupercellCompression.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/SupercellTextureLoader.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/Zstandard.lib)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/SupercellFlash.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/ASTC.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/ETCPACK.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/LZHAM.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/LZMA.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/SupercellCompression.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/SupercellTextureLoader.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Zstandard.lib)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/SupercellFlash.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/ASTC.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/ETCPACK.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/LZHAM.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/LZMA.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/SupercellCompression.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/SupercellTextureLoader.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Debug/Zstandard.lib)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/SupercellFlash.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/ASTC.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/ETCPACK.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/LZHAM.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/LZMA.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/SupercellCompression.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/SupercellTextureLoader.lib)
    target_link_libraries(PC PRIVATE ${CMAKE_SOURCE_DIR}/Deps/SWF/Zstandard.lib)
endif()


