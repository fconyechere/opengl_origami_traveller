IF(NOT EXISTS "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Deliverable1/Origami Crame Render/build/assimp/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Deliverable1/Origami Crame Render/build/assimp/install_manifest.txt\"")
ENDIF(NOT EXISTS "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Deliverable1/Origami Crame Render/build/assimp/install_manifest.txt")

FILE(READ "/Users/faithonyechere/Desktop/TrinityCollegeDublin/ComputerGraphics/Deliverable1/Origami Crame Render/build/assimp/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "/Library/Frameworks/Python.framework/Versions/3.10/lib/python3.10/site-packages/cmake/data/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
