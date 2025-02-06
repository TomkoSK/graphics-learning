cmake_minimum_required(VERSION 3.30)

add_subdirectory(glfw-3.4)
include_directories(glfw-3.4/include)
include_directories(glad/include/glad glad/include/KHR shader include)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp glad/include/glad/glad.c shader/shader.cpp)

add_executable(graphicsLearning ${SOURCE_FILES})

project(graphicsLearning)

target_link_libraries(graphicsLearning glfw)
