set(CMAKE_C_COMPILER "/usr/bin/gcc-13")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-13")

cmake_minimum_required(VERSION 3.15) 
PROJECT ( Tracker )

find_package(OpenCV REQUIRED )
find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -std=c++23 -O3 -save-temps")

set(STABLE_POINTS_RUST ${PROJECT_SOURCE_DIR}/source/rust/target/release/libstable_points.so)

# message(${OpenCV_INCLUDE_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/source/c++/include)

file(GLOB all_SRCS ${PROJECT_SOURCE_DIR}/source/c++/src/*.cpp)

add_library( ${PROJECT_NAME} SHARED ${all_SRCS})
# add_executable(  ${PROJECT_NAME} ${all_SRCS})
         
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${nlohmann_json_LIBS} ${STABLE_POINTS_RUST})