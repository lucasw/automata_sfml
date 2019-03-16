cmake_minimum_required(VERSION 2.8.3)
project(usb_cam)

# find_package(SFML REQUIRED)

add_executable(automata src/main.cpp)
# include_directories(${SFML_INCLUDE_DIR})
# target_link_libraries(automata ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
target_link_libraries(automata sfml-graphics sfml-window sfml-system)
