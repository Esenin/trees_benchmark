cmake_minimum_required(VERSION 3.2)
project(trees_benchmark)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    trees/splayTree/splayTree.h
    trees/vebFunc/vanemdeboas.cpp
    trees/vebFunc/vanemdeboas.h
    trees/advancedAvlTree.cpp
    trees/advancedAvlTree.h
    trees/avlTree.cpp
    trees/avlTree.h
    trees/bTree.cpp
    trees/bTree.h
    trees/iTree.h
    trees/splayTreeWrap.cpp
    trees/splayTreeWrap.h
    trees/stlMap.cpp
    trees/stlMap.h
    trees/testTreeAdapter.cpp
    trees/testTreeAdapter.h
    trees/treeTestLauncher.h
    trees/vebLayout.cpp
    trees/vebLayout.h
    trees/normalDistGenerator.h
    trees/main.cpp
    README.md)

find_package(Threads)

add_subdirectory(AnyBenchmark/TestingFramework)
#add_subdirectory(benchmark)

include_directories(${AnyBenchmark_SOURCE_DIR}/include ${AnyBenchmark_SOURCE_DIR})

add_executable(trees_benchmark ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} AnyBenchmark ${CMAKE_THREAD_LIBS_INIT})
