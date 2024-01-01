cmake_minimum_required(VERSION 3.16)

project(sast_music VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/debug)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/release)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

set(CMAKE_AUTORCC ON)

set(FLUENTUI_BUILD_EXAMPLES OFF)
add_subdirectory(libs/FluentUI)

file(GLOB SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    sast_music.qrc
)

file(GLOB_RECURSE SOURCE_IMPORT_PATHS
    ${CMAKE_SOURCE_DIR}/src/controller/*.h
    ${CMAKE_SOURCE_DIR}/src/controller/*.cpp
)

foreach(filepath ${SOURCE_IMPORT_PATHS})
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND SOURCE_IMPORT_FILES ${filename})
endforeach(filepath)

qt_add_executable(sast_music MACOSX_BUNDLE
    ${SOURCE_FILES}
)

qt_add_qml_module(sast_music
    URI sast_music
    VERSION 1.0
    SOURCES ${SOURCE_IMPORT_FILES}
)

set_target_properties(sast_music PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(sast_music
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS sast_music
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
