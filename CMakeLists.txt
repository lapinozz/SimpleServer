cmake_minimum_required(VERSION 3.8)
project(SimpleServer LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
message(${CMAKE_MODULE_PATH})
find_package(Networking REQUIRED)

add_library(SimpleServer INTERFACE)
target_link_libraries(SimpleServer INTERFACE Networking)

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

option(BUILD_EXAMPLES "" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

unset(BUILD_EXAMPLES CACHE)