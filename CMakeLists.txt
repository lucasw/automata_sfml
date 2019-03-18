cmake_minimum_required(VERSION 2.8.3)
project(automata)

find_package(OpenCV REQUIRED)

add_executable(automata src/main.cpp)
# include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(automata ${OpenCV_LIBRARIES})
