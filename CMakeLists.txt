cmake_minimum_required(VERSION 3.14)
project(ami_v2)

set(CMAKE_CXX_STANDARD 14)

include_directories(SerialPort/include)

add_executable(ami_v2 main.cpp SerialPort/src/SerialPort.cpp)