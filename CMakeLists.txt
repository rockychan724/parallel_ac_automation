cmake_minimum_required(VERSION 2.8)
project(parallel_ac_automation)

set(CMAKE_CXX_STANDARD 11)

#aux_source_directory(${PROJECT_SOURCE_DIR}/src src_list)
#add_executable(parallel_ac_automation main.cpp ${scr_list})

add_executable(run main.cpp src/KMP.h src/Trie.h src/AcAutomation.h src/ParallelAcAutomation.h src/util.h src/MatchBase.h)
add_executable(test test/test.cpp test/TestT.cpp test/TestT.h)

target_link_libraries(
        run
        -lpthread
)