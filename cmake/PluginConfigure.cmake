##---------------------------------------------------------------------------
## Author:      Pavel Kalian (Based on the work of Sean D'Epagnier)
## Copyright:   2014
## License:     GPLv3+
##---------------------------------------------------------------------------

SET(PLUGIN_SOURCE_DIR .)

MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

#  Do the version.h & wxWTranslateCatalog configuration into the build output directory,
#  thereby allowing building from a read-only source tree.
# The removes are to get rid of old copies - this can be removed at next release version, i.e. > 1.1.0
FILE(REMOVE ${PROJECT_SOURCE_DIR}/include/version.h)
FILE(REMOVE ${PROJECT_SOURCE_DIR}/include/wxWTranslateCatalog.h)
IF(NOT SKIP_VERSION_CONFIG)
    SET(BUILD_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
    configure_file(cmake/version.h.in ${BUILD_INCLUDE_PATH}/include/version.h)
    configure_file(cmake/wxWTranslateCatalog.h.in ${BUILD_INCLUDE_PATH}/include/wxWTranslateCatalog.h)
    INCLUDE_DIRECTORIES(${BUILD_INCLUDE_PATH}/include)
ENDIF(NOT SKIP_VERSION_CONFIG)

SET(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# SET(PROFILING 1)
IF(CMAKE_BUILD_TYPE STREQUAL Debug)
  ADD_DEFINITIONS( "-DDEBUG_BUILD" )
  MESSAGE (STATUS "DEBUG available")
ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"

# require proper c++
#ADD_DEFINITIONS( "-Wall -ansi -pedantic -Wno-variadic-macros" )
#TODO: Should we use  -fno-stack-protector
#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT WIN32 AND NOT APPLE )
  ADD_DEFINITIONS( "-Wall -Wno-unused -fexceptions -rdynamic -fvisibility=hidden" )
  ADD_DEFINITIONS( " -g -fno-strict-aliasing")
  IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    ADD_DEFINITIONS( " -O0")
  ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
  
  ADD_DEFINITIONS( " -DPREFIX=\\\"${CMAKE_INSTALL_PREFIX}\\\"")
  # profiling with gprof
#    ADD_DEFINITIONS( -pg )
#    SET(CMAKE_EXE_LINKER_FLAGS -pg)
  # profiling with gcov
#    ADD_DEFINITIONS( "-fprofile-arcs -ftest-coverage" )
#    SET(EXTRA_LIBS ${EXTRA_LIBS} "gcov")
ENDIF(NOT WIN32 AND NOT APPLE)

IF(MINGW)
  ADD_DEFINITIONS( "-Wall -Wno-unused -Wno-cpp -fexceptions" )
  ADD_DEFINITIONS( " -g -fno-strict-aliasing")
ENDIF(MINGW)

IF( APPLE )
  ADD_DEFINITIONS( "-Wall -Wno-unused -fexceptions -Wno-overloaded-virtual" )
  ADD_DEFINITIONS( " -g -fno-strict-aliasing")
  ADD_DEFINITIONS( " -Wno-deprecated -Wno-deprecated-declarations -Wno-unknown-pragmas" )
  ADD_DEFINITIONS( " -D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_" )
ENDIF(APPLE)

# Add some definitions to satisfy MS
IF(MSVC)
  ADD_DEFINITIONS(-D__MSVC__)
  ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ELSE(MSVC)
  IF(NOT APPLE)
    SET( CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
  ELSE(NOT APPLE)
    SET( CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
  ENDIF(NOT APPLE) 
ENDIF(MSVC)

SET(wxWidgets_USE_LIBS base core net xml html adv)
SET(BUILD_SHARED_LIBS TRUE)

# Allow multiprocess compile
IF(MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(MSVC)

set (WXWIDGETS_FORCE_VERSION CACHE VERSION "Force usage of a specific wxWidgets version.")
if(WXWIDGETS_FORCE_VERSION)
  set (wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
endif()
FIND_PACKAGE(wxWidgets REQUIRED)

IF(MSYS)
  # this is just a hack. I think the bug is in FindwxWidgets.cmake
  STRING( REGEX REPLACE "/usr/local" "\\\\;C:/MinGW/msys/1.0/usr/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS} )
ENDIF(MSYS)

INCLUDE(${wxWidgets_USE_FILE})

FIND_PACKAGE(OpenGL)
IF(OPENGL_GLU_FOUND)

    SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

    MESSAGE (STATUS "Found OpenGL..." )
    MESSAGE (STATUS "    Lib: " ${OPENGL_LIBRARIES})
    MESSAGE (STATUS "    Include: " ${OPENGL_INCLUDE_DIR})
    ADD_DEFINITIONS(-DocpnUSE_GL)
#    MESSAGE (STATUS "ocpnUSE_GLES defined for workaround")
#    ADD_DEFINITIONS(-DocpnUSE_GLES)
   
   # We need to remove GLU from the OPENGL_LIBRARIES list
    FOREACH (_currentLibFile ${OPENGL_LIBRARIES})
      #MESSAGE (STATUS "    Lib File: " ${_currentLibFile})
      SET(UCNAME ${_currentLibFile})
      string(TOUPPER ${UCNAME} UCNAME)
      IF(NOT ${UCNAME} MATCHES   "(.*)GLU(.*)")
        SET( REVISED_OPENGL_LIBRARIES ${_currentLibFile} ${REVISED_OPENGL_LIBRARIES})
      ENDIF()
    ENDFOREACH (_currentLibFile )

    SET( OPENGL_LIBRARIES ${REVISED_OPENGL_LIBRARIES})
    MESSAGE (STATUS "    Revised GL Lib: " ${OPENGL_LIBRARIES})

ELSE(OPENGL_GLU_FOUND)
    MESSAGE (STATUS "OpenGL not found..." )
ENDIF(OPENGL_GLU_FOUND)

MESSAGE (STATUS "    Adding local GLU" )
add_subdirectory(ocpnsrc/glu)
SET( OPENGL_LIBRARIES "GLU_static" ${OPENGL_LIBRARIES})
MESSAGE (STATUS "    Revised GL Lib (with local): " ${OPENGL_LIBRARIES})


SET(BUILD_SHARED_LIBS TRUE)

FIND_PACKAGE(Gettext REQUIRED)

