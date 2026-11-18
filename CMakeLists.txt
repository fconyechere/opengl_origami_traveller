cmake_minimum_required(VERSION 3.16)
project(deliverable2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

get_filename_component(LAB4_ROOT "${CMAKE_CURRENT_LIST_DIR}/../Labs/lab4" ABSOLUTE)
set(GLFW_ROOT "${LAB4_ROOT}/external/glfw-3.1.2")
set(GLAD_ROOT "${LAB4_ROOT}/external/glad-3.3")
set(GLM_ROOT "${LAB4_ROOT}/external/glm-0.9.7.1")
set(ASSIMP_ROOT "${LAB4_ROOT}/external/assimp")

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

if (NOT EXISTS "${ASSIMP_ROOT}/CMakeLists.txt")
    message(FATAL_ERROR "Assimp sources not found at ${ASSIMP_ROOT}")
endif()

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory("${ASSIMP_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/assimp-build")

add_executable(deliverable2
    src/main.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Scene.cpp
    src/Wind.cpp
    src/Bird.cpp
    src/Traveler.cpp
    src/ProceduralTree.cpp
    src/Landscape.cpp
)
target_include_directories(deliverable2 PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src"
    "${GLFW_ROOT}/include"
    "${GLM_ROOT}"
    "${ASSIMP_ROOT}/contrib/stb"
)
target_link_libraries(deliverable2 PRIVATE OpenGL::GL glfw glad assimp::assimp)

if (APPLE)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation REQUIRED)
    find_library(CORE_VIDEO_FRAMEWORK CoreVideo REQUIRED)
    target_link_libraries(deliverable2 PRIVATE
        "${COCOA_FRAMEWORK}"
        "${IOKIT_FRAMEWORK}"
        "${CORE_FOUNDATION_FRAMEWORK}"
        "${CORE_VIDEO_FRAMEWORK}"
    )
endif()
