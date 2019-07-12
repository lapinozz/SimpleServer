cmake_minimum_required(VERSION 3.8)
project(example LANGUAGES CXX)

add_executable(example main.cpp)
target_link_libraries(example SimpleServer)
