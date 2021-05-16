cmake_minimum_required(VERSION 2.8)
project(parallel_ac_automation)

set(CMAKE_CXX_STANDARD 11)

#aux_source_directory(${PROJECT_SOURCE_DIR}/src src_list)
#add_executable(parallel_ac_automation main.cpp ${scr_list})

add_executable(run main.cpp src/KMP.cpp src/KMP.h src/Trie.cpp src/Trie.h src/AcAutomation.cpp src/AcAutomation.h src/ParallelAcAutomation.cpp src/ParallelAcAutomation.h src/util.h)
add_executable(test test/test.cpp)

target_link_libraries(
        run
        -lpthread
)