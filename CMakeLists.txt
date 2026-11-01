cmake_minimum_required(VERSION 3.10)
project(MCclone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_executable(MCcolne scr/main.cpp)
target_link_libraries(MCcolne
        PRIVATE
        glfw
        GLEW::GLEW
        OpenGL::GL
)
target_include_directories(MCcolne
        PRIVATE
        ${glm_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
)