cmake_minimum_required(VERSION 2.8)
project(pvxmatching)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_SOURCE_DIR}/../fftw/includes)
link_directories(${CMAKE_SOURCE_DIR}/../fftw/libs)

set(SOURCE_FILES main.cpp
        fvc/kjgraph.cpp fvc/kjgraph.hpp fvc/kmpbased.cpp fvc/kmpbased.hpp fvc/kshiftingcondition.cpp fvc/kshiftingcondition.hpp
        fvc/convolutionbased.hpp fvc/convolutionbased.cpp
        utils/alphabet.hpp utils/rbitmap.cpp utils/rbitmap.hpp utils/pair.hpp utils/sbitmap.cpp utils/sbitmap.hpp utils/bitop.hpp
        utils/injective_map.hpp utils/string.hpp utils/tbitmap.cpp utils/tbitmap.hpp
        utils/convolution_base.hpp utils/naive_convolution.hpp utils/fft_convolution.hpp utils/fft_convolution.cpp
        naive/fvcmatching.hpp
        pvc/kmpbased.cpp pvc/kmpbased.hpp pvc/convolutionbased.cpp pvc/convolutionbased.hpp pvc/kshiftingcondition.cpp pvc/kshiftingcondition.hpp
        pvc/kjgraph.cpp pvc/kjgraph.hpp  naive/pvcmatching.hpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
add_library(${PROJECT_NAME} ${SOURCE_FILES})
# add_executable(${PROJECT_NAME} ${SOURCE_FILES})
