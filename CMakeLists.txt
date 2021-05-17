# TODO: optimize CMakeLists.txt
cmake_minimum_required(VERSION 2.8)
project(parallel_ac_automation)

set(CMAKE_CXX_STANDARD 11)

add_executable(run main.cpp)
add_executable(test test/test.cpp)

target_link_libraries(run -lpthread)
target_link_libraries(test -lpthread)
