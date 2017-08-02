cmake_minimum_required(VERSION 2.8.12)

project(demo)

SET(GCC_DEBUG_FLAG "-g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_DEBUG_FLAG}")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(sender sender.cpp)
add_executable(receiver receiver.cpp)

target_link_libraries(sender ${OpenCV_LIBS})
target_link_libraries(receiver ${OpenCV_LIBS})
