cmake_minimum_required(VERSION 3.9)
project(Bunny)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FILE(GLOB SRC_FILES "./*.cpp")
FILE(GLOB HEAD_FILES "./*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES})
target_link_libraries(Bunny glut GLU GL)