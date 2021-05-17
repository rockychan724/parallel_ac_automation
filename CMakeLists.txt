# TODO: optimize CMakeLists.txt
cmake_minimum_required(VERSION 2.8)
project(parallel_ac_automation)

set(CMAKE_CXX_STANDARD 11)

#aux_source_directory(${PROJECT_SOURCE_DIR}/src src_list)
#add_executable(parallel_ac_automation main.cpp ${scr_list})

add_executable(run main.cpp src/KMP.h src/Trie.h src/AcAutomation.h src/ParallelAcAutomation.h test/util.h src/MatchBase.h)
add_executable(test test/test.cpp test/debug.h)

target_link_libraries(run -lpthread)
target_link_libraries(test -lpthread)
