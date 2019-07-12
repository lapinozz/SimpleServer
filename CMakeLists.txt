cmake_minimum_required(VERSION 3.8)
project(SimpleServer LANGUAGES CXX)

include(GNUInstallDirs)

add_library(SimpleServer INTERFACE)

target_include_directories(
    SimpleServer INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(
    TARGETS SimpleServer
    EXPORT SimpleServer-config)

install(
    EXPORT SimpleServer-config
    NAMESPACE lapi::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SimpleServer
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)

add_subdirectory(example)