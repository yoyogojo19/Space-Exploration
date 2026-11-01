cmake_minimum_required(VERSION 3.10)
project(SpaceExploration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_executable(SpaceExploration scr/main.cpp)
target_link_libraries(SpaceExploration
        PRIVATE
        glfw
        GLEW::GLEW
        OpenGL::GL
)
target_include_directories(SpaceExploration
        PRIVATE
        ${glm_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
)