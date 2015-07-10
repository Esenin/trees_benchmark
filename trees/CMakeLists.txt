cmake_minimum_required(VERSION 3.2)
project(trees C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") # use for clang++
endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") # see https://stackoverflow.com/questions/19463602


set(SOURCE_FILES
    splayTree/splayTree.h
    vebFunc/vanemdeboas.cpp
    vebFunc/vanemdeboas.h
    advancedAvlTree.cpp
    advancedAvlTree.h
    avlTree.cpp
    avlTree.h
    bTree.h
    iTree.h
    main.cpp
    splayTreeWrap.h
    stlMap.cpp
    stlMap.h
    vebLayout.cpp
    vebLayout.h)

find_package(Threads)
#find_package(Boost  REQUIRED)
#if(Boost_FOUND)
#    set(Boost_USE_STATIC_RUNTIME    ON)
#    include_directories(${Boost_INCLUDE_DIRS})
#endif()

# warning: consumes over x4 memory
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer -fPIE -pie")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

