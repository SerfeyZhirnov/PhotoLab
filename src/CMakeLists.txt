cmake_minimum_required(VERSION 3.5)

project(PhotoLab VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(
    ${CMAKE_SOURCE_DIR}/model
    ${CMAKE_SOURCE_DIR}/view
    ${CMAKE_SOURCE_DIR}/controller
)

set(HEADERS
    ${CMAKE_SOURCE_DIR}/model/model.h
    ${CMAKE_SOURCE_DIR}/view/view.h
    ${CMAKE_SOURCE_DIR}/controller/controller.h
)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/model/model.cc
    ${CMAKE_SOURCE_DIR}/view/view.cc
    ${CMAKE_SOURCE_DIR}/controller/controller.cc
)

set(FORMS 
    ${CMAKE_SOURCE_DIR}/forms/mainwindow.ui
)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

set(RESOURCE_FILE ${CMAKE_SOURCE_DIR}/data/images.qrc)
qt_add_resources(RESOURCE_ADDED ${RESOURCE_FILE})

set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
    ${FORMS}
    ${CMAKE_SOURCE_DIR}/main.cc
)

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE appicon.png)
    set(app_icon_macos "${CMAKE_SOURCE_DIR}/data/icon/appicon.png")
    set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCE_ADDED}
        ${app_icon_macos}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    message(STATUS "cppcheck found: ${CPPCHECK}")
    list(
        APPEND CPPCHECK_ARGS
        "--enable=all"
        "--inconclusive"
        "--quiet"
        "--language=c++"
        "--std=c++17"
        "--suppress=unusedFunction"
        "--suppress=missingInclude"
        "--verbose"
    )
    add_custom_target(
        cppcheck
        COMMAND ${CPPCHECK} ${CPPCHECK_ARGS} ${HEADERS} ${SOURCES}
    )
else()
    message(STATUS "cppcheck not found")
endif()

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    message(STATUS "clang-format found: ${CLANG_FORMAT}")
    list(
        APPEND CLANG_ARGS
        "-n"
        "-style=Google"
        "--verbose"
    )
    add_custom_target(
        clang-format
        COMMAND ${CLANG_FORMAT} ${CLANG_ARGS} ${HEADERS} ${SOURCES}
    )
else()
    message(STATUS "clang-format not found")
endif()
