set(CMAKE_C_COMPILER "/usr/bin/gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")

cmake_minimum_required(VERSION 3.15) 
PROJECT ( Testing )

find_package(OpenCV REQUIRED )
find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -std=c++20 -O3 -save-temps")


# message(${OpenCV_INCLUDE_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../c++/include)


file(GLOB all_SRCS ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/../c++/src/imgUtils.cpp)

add_executable(  ${PROJECT_NAME} ${all_SRCS})
         
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${nlohmann_json_LIBS})