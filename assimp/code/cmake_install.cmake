# Install script for directory: /Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.3.0-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Deliverable1/Origami Crame Render/build/assimp/lib/libassimp.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.a")
    execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/anim.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/aabb.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/ai_assert.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/camera.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/color4.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/color4.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Deliverable1/Origami Crame Render/build/assimp/code/../include/assimp/config.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/ColladaMetaData.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/commonMetaData.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/defs.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/cfileio.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/light.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/material.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/material.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/matrix3x3.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/matrix3x3.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/matrix4x4.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/matrix4x4.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/mesh.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/ObjMaterial.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/pbrmaterial.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/GltfMaterial.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/postprocess.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/quaternion.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/quaternion.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/scene.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/metadata.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/texture.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/types.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/vector2.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/vector2.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/vector3.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/vector3.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/version.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/cimport.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/AssertHandler.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/importerdesc.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Importer.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/DefaultLogger.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/ProgressHandler.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/IOStream.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/IOSystem.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Logger.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/LogStream.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/NullLogger.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/cexport.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Exporter.hpp"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/DefaultIOStream.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/DefaultIOSystem.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/SceneCombiner.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/fast_atof.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/qnan.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/BaseImporter.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Hash.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/ParsingUtils.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/StreamReader.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/StreamWriter.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/StringComparison.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/StringUtils.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/SGSpatialSort.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/GenericProperty.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/SpatialSort.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/SmallVector.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/SmoothingGroups.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/SmoothingGroups.inl"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/StandardShapes.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/RemoveComments.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Subdivision.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Vertex.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/LineSplitter.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/TinyFormatter.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Profiler.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/LogAux.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Bitmap.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/XMLTools.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/IOStreamBuffer.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/CreateAnimMesh.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/XmlParser.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/BlobIOSystem.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/MathFunctions.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Exceptional.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/ByteSwapper.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Compiler/poppack1.h"
    "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Labs/lab4/external/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

