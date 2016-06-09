cmake_minimum_required(VERSION 3.5)
project(Slider_Game___Excerise)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES main.cpp SliderGUI.cpp SliderGUI.h GameField.cpp GameField.h IDA_Star.cpp IDA_Star.h Node.h IDAS_Multithread.cpp IDAS_Multithread.h )
add_executable(Slider_Game___Excerise ${SOURCE_FILES} IDAS_Multithread.cpp)

target_link_libraries(Slider_Game___Excerise ${CURSES_LIBRARIES})