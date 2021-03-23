# ---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough Copyright: License:     wxWidgets License
# ---------------------------------------------------------------------------

set(CMLOC "CMakeLists: ")

cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    message(STATUS "${CMLOC}CMP0048 set to New")
    if(POLICY CMP0076)
        cmake_policy(SET CMP0076 NEW)
    endif(POLICY CMP0076)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
    if(POLICY CMP0079)
        cmake_policy(SET CMP0079 NEW)
    endif(POLICY CMP0079)
endif(COMMAND cmake_policy)

set(VERBOSE_NAME "OCPN_Draw")
set(COMMON_NAME "OCPN Draw")
set(TITLE_NAME "OCPNDRAW")
set(PACKAGE_CONTACT "Jon Gough")
set(PACKAGE "ocpn_draw")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "8")
set(VERSION_PATCH "6")
set(VERSION_TWEAK "11")
set(VERSION_DATE "23/03/2021") # DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(OD_API_VERSION_MAJOR "1")
set(OD_API_VERSION_MINOR "1")

message(STATUS "${CMLOC}OD Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")
message(STATUS "${CMLOC}OD API Version: ${OD_API_VERSION_MAJOR}.${OD_API_VERSION_MINOR}")
message(STATUS "${CMLOC}OCPN API Verson: ${OCPN_API_VERSION_MAJOR}.${OCPN_API_VERSION_MINOR}")
message(STATUS "${CMLOC}OCPN Min Verson: ${OCPN_MIN_VERSION}")

set(PARENT "opencpn")

# The next line allows for setting up your own local webserver with git on it for testing purposes, the default is github.com
set(GIT_REPOSITORY_SERVER "github.com")

# Builds repository sufficies
set(PROD "prod") # Standard Repos
set(BETA "beta") # Standard Repos
set(ALPHA "alpha") # Standard Repos

set(CLOUDSMITH_USER "opencpn")
# set this if your cloudsmith repository name does not match your git repository name set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(CLOUDSMITH_BASE_REPOSITORY "ocpn_draw")

set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/draw.html")
set(XML_SUMMARY "Drawing Plugin to allow extra objects to be drawn and used within OCPN")
set(XML_DESCRIPTION "OCPN_DRAW (OD) is a plugin that extends the capability of OCPN to include non-standard object: Boundaries, Boundary Points, Text Points, Dead Reckoning paths, Electronic Bearing Lines (EBL) and Guard Zones. The plugin also has an API to allow other plugins to directly interact with it.")

#
# ----- Modify section above to include all the details for your plugin ----- ##
#

#
# ----- Modify section below if there are special requirements for the plugin ----- ##
#

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Use local version of GLU library
set(USE_LOCAL_GLU TRUE)
# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
option(USE_GL "Enable OpenGL support" ON)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

set(BUNDLE_DATA FALSE)
set(CMAKE_BUILD_TYPE
    "Release"
    CACHE STRING "Choose the type of build")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "None" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

project(${PACKAGE})

include("cmake/PluginSetup.cmake")

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

#
# ----- Modify section above if there are special requirements for the plugin ----- ##
#

##
## ----- do not change next section - needed to configure build process ----- ##
##

include("PluginConfigure")

##
## ----- do not change section above - needed to configure build process ----- ##
##

#
# ----- Change below to match project requirements ----- ##
#

add_definitions(-DUSE_S57)

set(SRCS
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
    src/TextPointCSVImport.cpp)

set(HDRS
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
    include/TextPointCSVImport.h)

set(OCPNSRC
    ocpnsrc/bbox.cpp
    ocpnsrc/cutil.cpp
    ocpnsrc/geodesic.cpp
    ocpnsrc/georef.cpp
    ocpnsrc/Hyperlink.cpp
    ocpnsrc/SelectItem.cpp
    ocpnsrc/TexFont.cpp
)
set(OCPNHDRS
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
)
set(EXTSRC
    extsrc/clipper.cpp
    extsrc/pugixml.cpp
)

set(EXTINCLUDE
    extinclude/clipper.hpp
    extinclude/pugixml.hpp
    extinclude/pugiconfig.hpp
)

set(LIBSRC
)

set(LIBHDRS
    libs/GL/gl.h
    libs/GL/gl_private.h
    libs/GL/glext.h
    libs/GL/glu.h
)

set (EXTINCLUDE_DIR ${EXTINCLUDE_DIR} extinclude)

if(APPLE OR QT_ANDROID)
    option(OD_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" OFF)
else(APPLE OR QT_ANDROID)
    option(OD_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" ON)
endif(APPLE OR QT_ANDROID)

if(OD_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Using JSON Schema validation - warning this changes the 'flavour' of the executable and it may not be compatible with OpenCPN")
    add_definitions(-DOD_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Adding JSON Schema validation")

    set(EXTSRC ${EXTSRC} extsrc/json-schema-validator/json-schema-draft7.json.cpp extsrc/json-schema-validator/json-uri.cpp extsrc/json-schema-validator/json-validator.cpp extsrc/json-schema-validator/json-patch.cpp)
    set(EXTINCLUDE_DIR ${EXTINCLUDE_DIR} "extinclude/json-schema-validator")
    set(LIBSRC ${LIBSRC} libs/wxJSON/src/jsonreader.cpp libs/wxJSON/src/jsonval.cpp libs/wxJSON/src/jsonwriter.cpp)
    set(LIBHDRS ${LIBHDRS} libs/wx/include/wx/json_defs.h libs/wx/include/wx/jsonreader.h libs/wx/include/wx/jsonval.h libs/wx/include/wx/jsonwriter.h)
else(OD_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Not using JSON Schema validation - warning invalid JSON 'may' cause unexpected crashes")
endif(OD_JSON_SCHEMA_VALIDATOR)

option(OD_USE_SVG "Use SVG graphics" ON)

if(OD_USE_SVG)
    message(STATUS "${CMLOC}SVG support enabled...")
    if(NOT QT_ANDROID)
        add_definitions(-DODraw_USE_SVG)
    endif(NOT QT_ANDROID)
endif(OD_USE_SVG)

if(OCPN_MIN_VERSION)
    message(STATUS "${CMLOC}Using O5 MUI")
    if(NOT QT_ANDROID)
        add_definitions(-DODraw_USE_MUI)
    endif(NOT QT_ANDROID)
endif(OCPN_MIN_VERSION)

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/glu/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/${EXTINCLUDE_DIR})
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include/wx)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)

target_sources(${PACKAGE_NAME} PUBLIC ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${LIBSRC})

if(NOT OCPN_FLATPAK_CONFIG)
    # Build environment not available when flatpak is being configured so following statements will not work
    message(STATUS "${CMLOC}Adding target link libraries to ${PACKAGE_NAME}")

    add_subdirectory(libs/gdal)
    add_subdirectory(libs/tinyxml)

    target_link_libraries(${PACKAGE_NAME} ocpn::gdal)
    target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)
endif(NOT OCPN_FLATPAK_CONFIG)


add_definitions(-DTIXML_USE_STL)

message(STATUS "${CMLOC}Checking to see if JSON Schema needs checking")
if(UNIX AND NOT APPLE AND NOT QT_ANDROID)
    if(OD_JSON_SCHEMA_VALIDATOR)
        # Change OS/ABI to be UNIX - System V to make it run with OCPN
        set(lib_name lib${PACKAGE_NAME}.so)
        message(STATUS "${CMLOC}lib-name: ${lib_name}")
        find_file(elfedit elfedit)
        if(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
            message(STATUS "${CMLOC}Will ensure OS/ABI is set to 0 to match OCPN")
            add_custom_command(
                TARGET ${PACKAGE_NAME}
                POST_BUILD
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                DEPENDS ${PACKAGE_NAME}
                COMMENT " Running post build action on ${lib_name}."
                COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) ')
        else(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
            message(STATUS "${CMLOC}Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
        endif(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
    endif(OD_JSON_SCHEMA_VALIDATOR)
endif(UNIX AND NOT APPLE AND NOT QT_ANDROID)

get_property(
    dirs
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}include dir='${dir}'")
endforeach()

#
# ----- Change above to match project requirements ----- ##
#

#
# ----- do not change - needed to build app ----- ##
#

include("cmake/PluginInstall.cmake")
include("cmake/PluginLocalization.cmake")
include("cmake/PluginPackage.cmake")
