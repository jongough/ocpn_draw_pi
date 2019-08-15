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
  ADD_DEFINITIONS( " -fno-strict-aliasing")
  MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
  IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_DEFINITIONS(" -O0 -g")
    MESSAGE(STATUS "Optimisation: -O0 -g")
  ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
    ADD_DEFINITIONS(" -O2 -march=native")
    MESSAGE(STATUS "Optimisation: -O2 -march=native")
  ELSEIF(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    ADD_DEFINITIONS(" -O2 -march=native -g")
    MESSAGE(STATUS "Optimisation: -O2 -march=native -g")
  ELSE(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")  
    ADD_DEFINITIONS(" -O2")
    MESSAGE(STATUS "Optimisation: -O2")
  ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    
  
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
  ADD_DEFINITIONS(-D HAVE_SNPRINTF)
  MESSAGE(STATUS "Set SNPRINTF")
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

IF(WIN32)
  IF(MSVC)
    SET(CMAKE_C_FLAGS_DEBUG               "/MP /MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1" )
    SET(CMAKE_C_FLAGS_MINSIZEREL          "/MP /MD  /O1  /Ob1 /D NDEBUG")
    SET(CMAKE_C_FLAGS_RELEASE             "/MP /MD  /O2  /Ob2 /D NDEBUG /Zi")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO      "/MP /MD  /O2  /Ob1 /D NDEBUG /Zi")
    SET(CMAKE_CXX_FLAGS_DEBUG             "/MP /MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1 /EHa")
    SET(CMAKE_CXX_FLAGS_MINSIZEREL        "/MP /MD  /O1  /Ob1 /D NDEBUG /EHa")
    SET(CMAKE_CXX_FLAGS_RELEASE           "/MP /MD  /O2  /Ob2 /D NDEBUG /Zi /EHa")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO    "/MP /MD  /O2  /Ob1 /D NDEBUG /Zi /EHa" )
    SET(CMAKE_EXE_LINKER_FLAGS /DEBUG)
  ENDIF(MSVC)
ENDIF(WIN32)

OPTION (USE_GL "Enable OpenGL support" ON)

# Search for opengles, short of running a program to test the speed
# of acceleration, I simply use gles on "native linux" arm systems
IF (ARCH MATCHES "arm*" AND (NOT QT_ANDROID) )
  find_path(OPENGLESv1_INCLUDE_DIR GLES/gl.h )
  IF (OPENGLESv1_INCLUDE_DIR)
    MESSAGE (STATUS "Found OpenGLESv1")
    ADD_DEFINITIONS(-DocpnUSE_GLES)
    ADD_DEFINITIONS(-DocpnUSE_GL)

    SET(OPENGLES_FOUND "YES")
    SET(OPENGL_FOUND "YES")

    SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl )
    add_subdirectory(src/glshim)

    SET(OPENGL_LIBRARIES "GL_static" "EGL" "X11" "drm"  )
  ENDIF()
ENDIF()

#  Building for QT_ANDROID involves a cross-building environment,
#  So the include directories, flags, etc must be stated explicitly
#  without trying to locate them on the host build system.
IF(QT_ANDROID)
  MESSAGE (STATUS "Using GLESv1 for Android")
  ADD_DEFINITIONS(-DocpnUSE_GLES)
  ADD_DEFINITIONS(-DocpnUSE_GL)
  ADD_DEFINITIONS(-DARMHF)

  SET(OPENGLES_FOUND "YES")
  SET(OPENGL_FOUND "YES")

  SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl )
  add_subdirectory(src/glshim)
ENDIF(QT_ANDROID)

IF ((NOT OPENGLES_FOUND) AND (NOT QT_ANDROID))

  IF(USE_GL)
    FIND_PACKAGE(OpenGL)
  ELSE(USE_GL)
    MESSAGE (STATUS "OpenGL disabled by option..." )
  ENDIF(USE_GL)

    IF(OPENGL_FOUND)

        SET(wxWidgets_USE_LIBS gl ${wxWidgets_USE_LIBS})
        INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

        MESSAGE (STATUS "Found OpenGL..." )
        MESSAGE (STATUS "    Lib: " ${OPENGL_LIBRARIES})
        MESSAGE (STATUS "    Include: " ${OPENGL_INCLUDE_DIR})
        ADD_DEFINITIONS(-DocpnUSE_GL)
    
    # We need to remove GLU from the OPENGL_LIBRARIES list
        FOREACH (_currentLibFile ${OPENGL_LIBRARIES})
            SET(UCNAME ${_currentLibFile})
            string(TOUPPER ${UCNAME} UCNAME)
            IF(NOT ${UCNAME} MATCHES   "(.*)GLU(.*)")
                SET( REVISED_OPENGL_LIBRARIES ${_currentLibFile} ${REVISED_OPENGL_LIBRARIES})
            ENDIF()
        ENDFOREACH (_currentLibFile )

        SET( OPENGL_LIBRARIES ${REVISED_OPENGL_LIBRARIES})
        MESSAGE (STATUS "    Revised GL Lib: " ${OPENGL_LIBRARIES})

    ELSE(OPENGL_FOUND)
        MESSAGE (STATUS "OpenGL not found..." )
    ENDIF(OPENGL_FOUND)
ENDIF()

MESSAGE (STATUS "    Adding local GLU" )
add_subdirectory(ocpnsrc/glu)
SET( OPENGL_LIBRARIES "GLU_static" ${OPENGL_LIBRARIES})
MESSAGE (STATUS "    Revised GL Lib (with local): " ${OPENGL_LIBRARIES})

IF(APPLE)
    OPTION(OD_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" OFF)
ELSE(APPLE)
    OPTION(OD_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" ON)
ENDIF(APPLE)

IF(OD_JSON_SCHEMA_VALIDATOR)
    MESSAGE(STATUS "Using JSON Schema validation - warning this changes the 'flavour' of the executable and it may not be compatible with OpenCPN")
    ADD_DEFINITIONS(-DOD_JSON_SCHEMA_VALIDATOR)
    MESSAGE(STATUS "Adding JSON Schema validation")

    SET(EXTSRC
        ${EXTSRC}
        extsrc/json-schema-validator/json-schema-draft7.json.cpp
        extsrc/json-schema-validator/json-uri.cpp
        extsrc/json-schema-validator/json-validator.cpp
    )
    SET(EXTINCLUDE
        ${EXTINCLUDE}
        extinclude/nlohmann/json.hpp
        extinclude/json-schema-validator/json-schema.hpp
    )
    INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/extinclude/json-schema-validator)
ELSE(OD_JSON_SCHEMA_VALIDATOR)
    MESSAGE(STATUS "Not using JSON Schema validation - warning invalid JSON 'may' cause unexpected crashes")
ENDIF(OD_JSON_SCHEMA_VALIDATOR)

IF(NOT QT_ANDROID)
  # Find wxWidgets here, and the setting get inherited by all plugins.
  # These options can be used to set the linux widgets build type
  SET( wxWidgets_USE_DEBUG OFF)
  SET( wxWidgets_USE_UNICODE ON)
  SET( wxWidgets_USE_UNIVERSAL OFF)
  SET( wxWidgets_USE_STATIC OFF)

  set (WXWIDGETS_FORCE_VERSION CACHE STRING "Force usage of a specific wxWidgets version.")
  if(WXWIDGETS_FORCE_VERSION)
    set (wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
  endif()
  
  FIND_PACKAGE(wxWidgets REQUIRED COMPONENTS ${wxWidgets_FIND_COMPONENTS})

  IF(MSVC)
    # Exclude wxexpat.lib, since we use our own version.
    # Other things are excluded as well, but we don't need them
    SET(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
  ENDIF(MSVC)

  IF(WIN32 OR APPLE OR QT_ANDROID)
    #FIND_PACKAGE(wxWidgets REQUIRED)
    IF(MSYS)
      # this is to convert msys to windows paths, and handle the missing /usr
      STRING( REGEX REPLACE "/usr/local" ";C:/MinGW/msys/1.0/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS} )
    ENDIF(MSYS)
    INCLUDE(${wxWidgets_USE_FILE})
  ENDIF(WIN32 OR APPLE OR QT_ANDROID)

  MESSAGE (STATUS "Found wxWidgets..." )
  MESSAGE (STATUS " wxWidgets Include: ${wxWidgets_INCLUDE_DIRS}")
  MESSAGE (STATUS " wxWidgets Libraries: ${wxWidgets_LIBRARIES}")

  # We need to remove GLU from the wxWidgets_LIBRARIES list
  # It only appears to get on the list for MSW...
  FOREACH (_currentLibFile ${wxWidgets_LIBRARIES})
    SET(UCNAME ${_currentLibFile})
    string(TOUPPER ${UCNAME} UCNAME)
    IF(NOT ${UCNAME} MATCHES   "(.*)GLU(.*)")
      SET( REVISED_wxWidgets_LIBRARIES  ${REVISED_wxWidgets_LIBRARIES} ${_currentLibFile})
    ENDIF()
  ENDFOREACH (_currentLibFile )
  SET( wxWidgets_LIBRARIES ${REVISED_wxWidgets_LIBRARIES})

  MESSAGE (STATUS " Revised wxWidgets Libraries: ${wxWidgets_LIBRARIES}")
ENDIF(NOT QT_ANDROID)

IF(NOT WIN32 AND NOT APPLE AND NOT QT_ANDROID)
  OPTION(OCPN_FORCE_GTK3 "Force the build to use GTK3" OFF)

  IF(NOT OCPN_FORCE_GTK3)
    FIND_PACKAGE(GTK2)
  ENDIF(NOT OCPN_FORCE_GTK3)

  IF(GTK2_FOUND)
    set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk2)
    INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    SET(GTK_LIBRARIES ${GTK2_LIBRARIES})
    MESSAGE(STATUS "Building against GTK2...")
  ELSE(GTK2_FOUND)
    FIND_PACKAGE(GTK3)
    INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
    SET(GTK_LIBRARIES ${GTK3_LIBRARIES})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXGTK3__")
    set(wxWidgets_CONFIG_OPTIONS ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk3)
    MESSAGE(STATUS "Building against GTK3...")
  ENDIF(GTK2_FOUND)
  FIND_PACKAGE(wxWidgets REQUIRED)
  INCLUDE(${wxWidgets_USE_FILE})
  SET(EXTRA_LIBS ${EXTRA_LIBS} ${GTK_LIBRARIES})
ENDIF(NOT WIN32 AND NOT APPLE AND NOT QT_ANDROID)

SET(BUILD_SHARED_LIBS TRUE)

FIND_PACKAGE(Gettext REQUIRED)

