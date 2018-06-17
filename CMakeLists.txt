cmake_minimum_required(VERSION 3.3)
project(ML_EvaluationMeasures)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ../exe/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(BUILD_1 Evaluation)

set(SOURCE_FILES_1 Point.cpp Cluster.cpp Evaluation.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
add_executable(${BUILD_1} ${SOURCE_FILES_1})
