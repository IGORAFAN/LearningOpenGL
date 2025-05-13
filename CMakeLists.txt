cmake_minimum_required(VERSION 3.10)
project(OpenGLTest)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(OpenGLTest
        main.cpp
        src/shader.cpp
)

include_directories(
        OpenGLTest
        include
)

target_include_directories(OpenGLTest PRIVATE
        ${VCPKG_INCLUDE_DIRS}
)

target_link_libraries(OpenGLTest PRIVATE
        glfw
        glad::glad
        ${STB_INCLUDE_DIRS}
)
