cmake_minimum_required(VERSION 2.8)
project(pvxperf)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES performance.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/../pvxmatching/)
find_library(libpvx NAMES libpvxmatching.a PATHS ${CMAKE_SOURCE_DIR}/../pvxmatching/cmake-build-debug/ ${CMAKE_SOURCE_DIR}/../pvxmatching/)
target_link_libraries(${PROJECT_NAME} ${libpvx})
