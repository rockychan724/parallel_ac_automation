cmake_minimum_required(VERSION 2.8)
project(parallel_ac_automation)

set(CMAKE_CXX_STANDARD 11)

add_executable(parallel_ac_automation main.cpp src/KMP.cpp src/KMP.h src/Trie.cpp src/Trie.h src/AcAutomation.cpp src/AcAutomation.h)