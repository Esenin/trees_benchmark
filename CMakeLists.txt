cmake_minimum_required(VERSION 3.2)
project(trees_benchmark)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") # use for clang++
endif()

set(SOURCE_FILES
    trees/splayTree/splayTree.h
    trees/vebFunc/vanemdeboas.cpp
    trees/vebFunc/vanemdeboas.h
    trees/advancedAvlTree.cpp
    trees/advancedAvlTree.h
    trees/avlTree.cpp
    trees/avlTree.h
    trees/bTree.h
    trees/iTree.h
    trees/splayTreeWrap.h
    trees/stlMap.cpp
    trees/stlMap.h
    trees/vebLayout.cpp
    trees/vebLayout.h
    benchmarking/normalDistGenerator.h
    benchmarking/testTreeAdapter.cpp
    benchmarking/testTreeAdapter.h
    benchmarking/treeTestLauncher.h
    benchmarking/main.cpp
    README.md)

find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") # see https://stackoverflow.com/questions/19463602

add_subdirectory(AnyBenchmark)

# use careful: over x4 memory consumption
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer -fPIE -pie")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(trees_benchmark ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} AnyBenchmark ${CMAKE_THREAD_LIBS_INIT})
