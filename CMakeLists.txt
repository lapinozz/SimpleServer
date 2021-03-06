cmake_minimum_required(VERSION 3.8)
project(SimpleServer LANGUAGES CXX)

include(GNUInstallDirs)

add_library(SimpleServer INTERFACE)

if (WIN32)
    target_link_libraries(SimpleServer INTERFACE ws2_32 wsock32)
endif ()

target_include_directories(
    SimpleServer INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SimpleServer>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleServer>)

install(
    TARGETS SimpleServer
    EXPORT SimpleServerConfig)

install(
    EXPORT SimpleServerConfig
    NAMESPACE SimpleServer::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SimpleServer
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/SimpleServer
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)

option(BUILD_EXAMPLES "" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

unset(BUILD_EXAMPLES CACHE)
