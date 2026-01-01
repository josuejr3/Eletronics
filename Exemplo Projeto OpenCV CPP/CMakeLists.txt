cmake_minimum_required(VERSION 3.20)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(TESTE main.cpp)
target_link_libraries(TESTE ${OpenCV_LIBS})
