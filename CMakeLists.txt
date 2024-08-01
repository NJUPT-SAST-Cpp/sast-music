cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/.cmake/)
include(GetGitRevisionDescription)

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

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Multimedia)

add_subdirectory(libs/mbedtls)

qt_standard_project_setup(REQUIRES 6.5)

set(CMAKE_AUTORCC ON)

set(FLUENTUI_BUILD_EXAMPLES OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND LINUX)
    set(FLUENTUI_QML_PLUGIN_DIRECTORY /usr/lib/qt6/qml/FluentUI)
endif()

add_subdirectory(libs/FluentUI)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp
)

file(GLOB_RECURSE SOURCE_IMPORT_PATHS
    ${CMAKE_SOURCE_DIR}/src/Service/NeteaseCloudMusic/*.h
    ${CMAKE_SOURCE_DIR}/src/Service/NeteaseCloudMusic/*.cpp
    ${CMAKE_SOURCE_DIR}/src/ViewModel/*.h
    ${CMAKE_SOURCE_DIR}/src/ViewModel/*.cpp
)

foreach(filepath ${SOURCE_IMPORT_PATHS})
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND SOURCE_IMPORT_FILES ${filename})
endforeach(filepath)

set(VERSION_RC_PATH "")
set(WIN32_MANIFEST_PATH "")

if(WIN32)
    set(VERSION_RC_PATH ${CMAKE_SOURCE_DIR}/SAST_Evento.rc)
    configure_file(
        ${CMAKE_SOURCE_DIR}/.cmake/version_exe.rc.in
        ${VERSION_RC_PATH}
    )
    configure_file(
        ${CMAKE_SOURCE_DIR}/.cmake/InstallerScript.iss.in
        ${CMAKE_SOURCE_DIR}/action-cli/InstallerScript.iss
    )
    set(WIN32_MANIFEST_PATH ${CMAKE_SOURCE_DIR}/app.manifest)
endif()

if(APPLE)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER})
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT})

    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${spectral_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${spectral_VERSION})

    set(ICON_NAME "app.icns")
    set(${PROJECT_NAME}_MAC_ICON "${CMAKE_SOURCE_DIR}/${ICON_NAME}")
    set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME})
    set_property(SOURCE "${${PROJECT_NAME}_MAC_ICON}" PROPERTY
        MACOSX_PACKAGE_LOCATION Resources)
endif()

qt_add_executable(sast_music MACOSX_BUNDLE
    ${${PROJECT_NAME}_MAC_ICON}
    ${SOURCE_FILES}
    ${VERSION_RC_PATH}
    ${WIN32_MANIFEST_PATH}
    ${SOURCE_FILES}
)

qt_add_qml_module(sast_music
    URI sast_music
    VERSION 1.0
    SOURCES ${SOURCE_IMPORT_FILES}
    RESOURCES res.qrc
    RESOURCES qml.qrc
)

target_include_directories(sast_music
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/ViewModel
)

set_target_properties(sast_music PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/.cmake/Info.plist.in"
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(sast_music
    PRIVATE
    Qt6::Quick
    Qt6::Multimedia
    MbedTLS::mbedcrypto
)

include(GNUInstallDirs)
install(TARGETS sast_music
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
