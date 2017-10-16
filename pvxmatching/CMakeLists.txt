cmake_minimum_required(VERSION 2.8)
project(pvxmatching)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp fvc/kjgraph.cpp fvc/kjgraph.hpp fvc/kmpbased.cpp fvc/kmpbased.hpp utils/alphabet.hpp fvc/kshiftingcondition.cpp fvc/kshiftingcondition.hpp utils/rbitmap.cpp utils/rbitmap.hpp utils/pair.hpp utils/sbitmap.cpp utils/sbitmap.hpp utils/bitop.hpp naive/fvcmatching.hpp pvc/kmpbased.cpp pvc/kmpbased.hpp pvc/kshiftingcondition.cpp pvc/kshiftingcondition.hpp pvc/kjgraph.cpp pvc/kjgraph.hpp utils/injective_map.hpp utils/string.hpp utils/tbitmap.cpp utils/tbitmap.hpp naive/pvcmatching.hpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
add_library(${PROJECT_NAME} ${SOURCE_FILES})
# add_executable(${PROJECT_NAME} ${SOURCE_FILES})
