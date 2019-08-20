##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 3.1.1)
IF (COMMAND cmake_policy)
  if (POLICY CMP0043)
    CMAKE_POLICY(SET CMP0043 NEW)
  endif (POLICY CMP0043)
  if (POLICY CMP0077)
    CMAKE_POLICY(SET CMP0077 NEW)
  endif (POLICY CMP0077)
ENDIF (COMMAND cmake_policy)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")

PROJECT(ocpn_draw_pi)
SET(PARENT "opencpn")

SET(PACKAGE_NAME ocpn_draw_pi)
SET(PACKAGE_FILE_NAME "opencpn-plugin-draw")
SET(VERBOSE_NAME ocpn_draw)
SET(TITLE_NAME OCPNDRAW)
SET(CPACK_PACKAGE_CONTACT "Jon Gough")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "6")
SET(VERSION_PATCH "21")
SET(VERSION_DATE "20/08/2019")
SET(OCPN_MIN_VERSION "ov50")
SET(OD_COMMENT "  * Initial release for O5 using CI")
SET(OCPN_API_VERSION_MAJOR "1")
SET(OCPN_API_VERSION_MINOR "16")
SET(OD_API_VERSION_MAJOR "1")
SET(OD_API_VERSION_MINOR "1")

MESSAGE(STATUS "OD Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
MESSAGE(STATUS "OD API Version: ${OD_API_VERSION_MAJOR}.${OD_API_VERSION_MINOR}")
MESSAGE(STATUS "OCPN API Verson: ${OCPN_API_VERSION_MAJOR}.${OCPN_API_VERSION_MINOR}")
MESSAGE(STATUS "OCPN Min Verson: ${OCPN_MIN_VERSION}")

SET(BUNDLE_DATA FALSE)
SET( CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" )
SET_PROPERTY( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "None" "Debug" "Release" "RelWithDebInfo" "MinSizeRel" )

MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

SET(CMAKE_VERBOSE_MAKEFILE ON)

if (CMAKE_VERSION VERSION_LESS "3.1")
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message(STATUS "Setting C++11 standard via CXX flags")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    message(STATUS "Setting C++0x standard via CXX FLAGS")
  else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif()
else ()
  set (CMAKE_CXX_STANDARD 11)
  message(STATUS "Setting C++11 standard via cmake standard mecahnism")
endif ()

OPTION(OCPN_USE_LIBCPP "Use C++11 and libc++ instead of libstdc++ on macOS" ON)

if ( CMAKE_VERSION VERSION_GREATER 3.4 )
  set(ENABLE_CLANG_TIDY OFF CACHE BOOL "Add clang-tidy automatically to builds")
  if (ENABLE_CLANG_TIDY)
    find_program (CLANG_TIDY_EXE NAMES "clang-tidy" PATHS /usr/local/opt/llvm/bin )
    if (CLANG_TIDY_EXE)
      message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
      #For more, see http://clang.llvm.org/extra/clang-tidy/
      #set(CLANG_TIDY_CHECKS "-*,modernize-*")
      set(CLANG_TIDY_CHECKS "-*,performance-*")
      set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/*'"
        CACHE STRING "" FORCE)
    else()
      message(AUTHOR_WARNING "clang-tidy not found!")
      set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # delete it
    endif()
  endif()
endif()

# Add support for address etc sanitizers, part 1/2 (other half after ADD_EXECUTABLE)
if ( CMAKE_VERSION VERSION_GREATER 3.4 )
  set(ENABLE_SANITIZER "none" CACHE STRING "Add clang sanitizer to the build")
  set_property(CACHE ENABLE_SANITIZER PROPERTY STRINGS none address memory thread undefined)
  if (NOT "${ENABLE_SANITIZER}" MATCHES "none")
    add_compile_options(-fsanitize=${ENABLE_SANITIZER})
  endif()
endif()

if ( CMAKE_VERSION VERSION_GREATER 3.9 )
  set(ENABLE_CPPCHECK OFF CACHE BOOL "Add cppcheck automatically to builds")
  if (ENABLE_CPPCHECK)
      find_program (CPPCHECK_EXECUTABLE NAMES "cppcheck" )
      set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXECUTABLE})
  endif()
endif()

#SET_DIRECTORY_PROPERTIES(dummy ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/include/version.h)

INCLUDE("cmake/PluginConfigure.cmake")

ADD_DEFINITIONS(-DUSE_S57)


SET(SRCS
    src/Boundary.cpp
    src/BoundaryCSVImport.cpp
    src/BoundaryMan.cpp
    src/BoundaryProp.cpp
    src/BoundaryPoint.cpp
    src/BoundaryPointCSVImport.cpp
    src/DR.cpp
    src/DRProp.cpp
    src/EBL.cpp
    src/EBLProp.cpp
    src/GZ.cpp
    src/GZMan.cpp
    src/GZProp.cpp
    src/ocpn_draw_pi.cpp
    src/ODAPI.cpp
    src/ODConfig.cpp
    src/ODdc.cpp
    src/ODDRDialogDef.cpp
    src/ODDRDialogImpl.cpp
    src/ODEventHandler.cpp
    src/ODicons.cpp
    src/ODIconCombo.cpp
    src/ODJSON.cpp
    src/ODLayer.cpp
    src/ODLinkPropertiesDialogDef.cpp
    src/ODLinkPropertiesDialogImpl.cpp
    src/ODMarkIcon.cpp
    src/ODNavObjectChanges.cpp
    src/ODNewODPointDialogDef.cpp
    src/ODNewODPointDialogImpl.cpp
    src/ODPath.cpp
    src/ODPathPropertiesDialogDef.cpp
    src/ODPathPropertiesDialogImpl.cpp
    src/ODPoint.cpp
    src/ODPointPropertiesDialog.cpp
    src/ODPointPropertiesImpl.cpp
    src/ODPositionParser.cpp
    src/ODPropertiesDialogDef.cpp
    src/ODPropertiesDialogImpl.cpp
    src/ODRolloverWin.cpp
    src/ODSelect.cpp
    src/ODToolbarDef.cpp
    src/ODToolbarImpl.cpp
    src/ODUtils.cpp
    src/PathMan.cpp
    src/PathAndPointManagerDialogDef.cpp
    src/PathAndPointManagerDialogImpl.cpp
    src/PIL.cpp
    src/PILProp.cpp
    src/PILPropertiesDialogDef.cpp
    src/PILPropertiesDialogImpl.cpp
    src/PointMan.cpp
    src/TextPoint.cpp
    src/TextPointCSVImport.cpp
)

SET(HDRS
    include/ocpn_draw_pi.h
    include/Boundary.h
    include/BoundaryCSVImport.h
    include/BoundaryMan.h
    include/BoundaryProp.h
    include/BoundaryPoint.h
    include/BoundaryPointCSVImport.h
    include/DR.h
    include/DRProp.h
    include/EBL.h
    include/EBLProp.h
    include/globals.h
    include/GZ.h
    include/GZMan.h
    include/GZProp.h
    include/PointMan.h
    include/ODAPI.h
    include/ODConfig.h
    include/ODdc.h
    include/ODDRDialogDef.h
    include/ODDRDialogImpl.h
    include/ODEventHandler.h
    include/ODicons.h
    include/ODIconCombo.h
    include/ODJSON.h
    include/ODJSONSchemas.h
    include/ODLayer.h
    include/ODLinkPropertiesDialogDef.h
    include/ODLinkPropertiesDialogImpl.h
    include/ODMarkIcon.h
    include/ODNavObjectChanges.h
    include/ODNewODPointDialogDef.h
    include/ODNewODPointDialogImpl.h
    include/ODPath.h
    include/ODPathPropertiesDialogDef.h
    include/ODPathPropertiesDialogImpl.h
    include/ODPoint.h
    include/ODPointPropertiesDialog.h
    include/ODPointPropertiesImpl.h
    include/ODPositionParser.h
    include/ODPropertiesDialogDef.h
    include/ODPropertiesDialogImpl.h
    include/ODRolloverWin.h
    include/ODSelect.h
    include/ODToolbarDef.h
    include/ODToolbarImpl.h
    include/ODUtils.h
    include/ODvector2D.h
    include/PathMan.h
    include/PathAndPointManagerDialogDef.h
    include/PathAndPointManagerDialogImpl.h
    include/PIL.h
    include/PILPropertiesDialogDef.h
    include/PILPropertiesDialogImpl.h
    include/PILProp.h
    include/TextPoint.h
    include/TextPointCSVImport.h
)

SET(OCPNSRC 
    ocpnsrc/bbox.cpp
    ocpnsrc/cutil.cpp
    ocpnsrc/geodesic.cpp
    ocpnsrc/georef.cpp
    ocpnsrc/Hyperlink.cpp
    ocpnsrc/SelectItem.cpp
    ocpnsrc/TexFont.cpp
    ocpnsrc/wxJSON/jsonreader.cpp
    ocpnsrc/wxJSON/jsonval.cpp
    ocpnsrc/wxJSON/jsonwriter.cpp

)
SET(OCPNHDRS
    ocpninclude/AISTargetAlertDialog.h
    ocpninclude/AISTargetQueryDialog.h
    ocpninclude/bbox.h
    ocpninclude/cutil.h
    ocpninclude/geodesic.h
    ocpninclude/georef.h
    ocpninclude/Hyperlink.h
    ocpninclude/ocpn_plugin.h
    ocpninclude/SelectItem.h
    ocpninclude/TexFont.h
    ocpninclude/timers.h
    ocpninclude/vector2D.h
    ocpninclude/wx28compat.h
    ocpninclude/GL/gl.h
    ocpninclude/GL/glext.h
    ocpninclude/GL/gl_private.h
    ocpninclude/GL/glu.h
    ocpninclude/wx/json_defs.h
    ocpninclude/wx/jsonreader.h
    ocpninclude/wx/jsonval.h
    ocpninclude/wx/jsonwriter.h
)

SET(EXTSRC
    ${EXTSRC}
    extsrc/clipper.cpp
    extsrc/pugixml.cpp
)

SET(EXTINCLUDE
    ${EXTINCLUDE}
    extinclude/clipper.hpp
    extinclude/pugiconfig.hpp
    extinclude/pugixml.hpp
)

OPTION(OD_USE_SVG "Use SVG graphics" ON)

IF(OD_USE_SVG)
  MESSAGE(STATUS "SVG support enabled...")
  IF(NOT QT_ANDROID)

    ADD_DEFINITIONS(-DODraw_USE_SVG)
  ENDIF(NOT QT_ANDROID)
ENDIF(OD_USE_SVG)

IF(OCPN_MIN_VERSION)
  MESSAGE(STATUS "Using O5 MUI")
  IF(NOT QT_ANDROID)
    ADD_DEFINITIONS(-DODraw_USE_MUI)
  ENDIF(NOT QT_ANDROID)
ENDIF(OCPN_MIN_VERSION)

INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/extsrc/mygdal  )
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/glu/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/GL)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)

IF(UNIX)
 INCLUDE("cmake/FindTinyXML.cmake")
 FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX)

IF(TINYXML_FOUND)
 message (STATUS "Building with system tinyxml")
 INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC})
 TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES})
ELSE(TINYXML_FOUND)
 message (STATUS "Building with embedded tinyxml")
 INCLUDE_DIRECTORIES(extsrc/tinyxml/)
 SET(SRC_LTINYXML
            extsrc/tinyxml/tinyxml.cpp
            extsrc/tinyxml/tinyxmlparser.cpp
            extsrc/tinyxml/tinyxmlerror.cpp
 )
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${SRC_LTINYXML})
ENDIF(TINYXML_FOUND)

ADD_DEFINITIONS(-DTIXML_USE_STL)

IF(UNIX AND NOT APPLE)
  IF(OD_JSON_SCHEMA_VALIDATOR)
    # Change OS/ABI to be UNIX - System V to make it run with OCPN
    SET(lib_name lib${PACKAGE_NAME}.so)
    MESSAGE(STATUS "lib-name: ${lib_name}")
    FIND_FILE(elfedit elfedit) 
    IF(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
      MESSAGE(STATUS "Will ensure OS/ABI is set to 0 to match OCPN")
      ADD_CUSTOM_COMMAND(
        TARGET ${PACKAGE_NAME}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${PACKAGE_NAME}
        COMMENT " Running post build action on ${lib_name}."
        COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) '
      )
    ELSE(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
      MESSAGE(STATUS "Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
    ENDIF(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
  ENDIF(OD_JSON_SCHEMA_VALIDATOR)
ENDIF(UNIX AND NOT APPLE)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
