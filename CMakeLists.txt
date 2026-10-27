cmake_minimum_required(VERSION 3.16)
project(deliverable2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

get_filename_component(LAB4_ROOT "${CMAKE_CURRENT_LIST_DIR}/../Labs/lab4" ABSOLUTE)
set(GLFW_ROOT "${LAB4_ROOT}/external/glfw-3.1.2")
set(GLAD_ROOT "${LAB4_ROOT}/external/glad-3.3")

if (NOT EXISTS "${GLFW_ROOT}/CMakeLists.txt")
    message(FATAL_ERROR "GLFW sources not found at ${GLFW_ROOT}")
endif()
add_subdirectory("${GLFW_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/glfw")

if (EXISTS "${GLAD_ROOT}/src/gl.c" AND EXISTS "${GLAD_ROOT}/include/glad/gl.h")
    add_library(glad "${GLAD_ROOT}/src/gl.c")
elseif (EXISTS "${GLAD_ROOT}/src/glad.c" AND EXISTS "${GLAD_ROOT}/include/glad/glad.h")
    add_library(glad "${GLAD_ROOT}/src/glad.c")
else()
    message(FATAL_ERROR "GLAD sources not found at ${GLAD_ROOT}")
endif()
target_include_directories(glad PUBLIC "${GLAD_ROOT}/include")

add_executable(deliverable2
    src/main.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Model.cpp
)
target_include_directories(deliverable2 PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src"
    "${GLFW_ROOT}/include"
)
target_link_libraries(deliverable2 PRIVATE OpenGL::GL glfw glad)
