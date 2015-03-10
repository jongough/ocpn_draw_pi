##---------------------------------------------------------------------------
## Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(ocpn_draw_pi)

SET(PACKAGE_NAME ocpn_draw_pi)
SET(VERBOSE_NAME ocpn_draw)
SET(TITLE_NAME OCPNDRAW)
SET(CPACK_PACKAGE_CONTACT "Del Elson")

SET(BUNDLE_DATA FALSE)

INCLUDE( VERSION.cmake )
SET(PLUGIN_VERSION "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${PLUGIN_VERSION_PATCH}" )

SET(VERSION_MAJOR ${PLUGIN_VERSION_MAJOR})
SET(VERSION_MINOR ${PLUGIN_VERSION_MINOR})
SET(VERSION_PATCH ${PLUGIN_VERSION_PATCH})
SET(VERSION_DATE ${PLUGIN_VERSION_DATE})

configure_file(cmake/version.h.in ${CMAKE_SOURCE_DIR}/include/version.h)

# this dummy target is necessary to make sure the ADDITIONAL_MAKE_CLEAN_FILES directive is executed.
# apparently, the base CMakeLists.txt file must have "some" target to activate all the clean steps.
ADD_CUSTOM_TARGET(dummy COMMENT "dummy: Done." DEPENDS ${PACKAGE_NAME})

SET_DIRECTORY_PROPERTIES(dummy ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_SOURCE_DIR}/include/version.h)

SET(CMAKE_BUILD_TYPE Debug)

INCLUDE("cmake/PluginConfigure.cmake")

ADD_DEFINITIONS(-DUSE_S57)

SET(SRCS
		src/ocpn_draw_pi.cpp
 		src/Boundary.cpp
		src/BoundaryProp.cpp
 		src/PointMan.cpp
 		src/OCPNSelect.cpp
 		src/PathMan.cpp
 		src/pathmanagerdialog.cpp
 		src/PathProp.cpp
 		src/Path.cpp
 		src/OCPNPoint.cpp
 		src/OCPNDrawConfig.cpp
 		src/OCPNDrawNavObjectChanges.cpp
 		src/OCPNDrawCanvasMenuHandler.cpp
# 		src/RoutePoint.cpp
# 		src/Route.cpp
 		src/OCPNDrawicons.cpp
        	src/jsonreader.cpp
        	src/jsonval.cpp
        	src/jsonwriter.cpp
 	)

SET(HDRS
		include/ocpn_draw_pi.h
 		include/Boundary.h
 		include/BoundaryProp.h
 		include/PointMan.h
 		include/OCPNSelect.h
 		include/PathMan.h
 		include/pathmanagerdialog.h
 		include/PathProp.h
 		include/LinkPropDlg.h
 		include/Layer.h
 		include/Path.h
 		include/OCPNPoint.h
 		include/OCPNDrawConfig.h
 		include/OCPNDrawNavObjectChanges.h
 		include/OCPNDrawCanvasMenuHandler.h
 		include/vector2D.h
# 		include/RoutePoint.h
 		include/ocpn_types.h
# 		include/navutil.h
# 		include/Route.h
# 		include/navutil.h
 		include/OCPNDrawicons.h
# 		include/chcanv.h
		include/version.h
        	include/wx/json_defs.h
       		include/wx/jsonreader.h
        	include/wx/jsonval.h
        	include/wx/jsonwriter.h
)

#IF(WIN32)
#SET(SRC_LTINYXML
#		src/tinyxml/tinyxml.cpp
#		src/tinyxml/tinyxml.h
#		src/tinyxml/tinyxmlerror.cpp
#		src/tinyxml/tinyxmlparser.cpp
#		src/tinyxml/tinystr.cpp
#		src/tinyxml/tinystr.h
#)
#ENDIF(WIN32)

#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mygdal ${CMAKE_SOURCE_DIR}/../../include ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src )
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mygdal  )
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include)
ADD_DEFINITIONS(-DTIXML_USE_STL)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${SRC_LTINYXML})

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")

